#include "kmalloc.h"
#include <printf/Printf.h>
#include <memory/VMManager.hpp>

MallocHeader_t MallocMainHeder;
extern VMManager KernelVMM;

void InitKMalloc(){
    MallocMainHeder.MallocBegin = align(PhisicalAllocator::GetTotalMemory(), 30);
    MallocMainHeder.MallocHead = MallocMainHeder.MallocBegin;
}

static void SplitRegion(kMallocElem_t *region, size_t size){

    if ((i64)region->size - (i64)size - (i64)sizeof(kMallocElem_t) < 1) return;

    kMallocElem_t *newRegion = (kMallocElem_t *) ((u64)region + sizeof(kMallocElem_t) + size);
    newRegion->size = region->size - size - sizeof(kMallocElem_t);
    newRegion->free = 1;
    newRegion->next = NULL;
    newRegion->previous = region;
    region->next = newRegion;
    region->size = size;
}

void *kmalloc(u64 size){
    u64 minimumSize = UINT64_MAX;
    kMallocElem_t *suitableElem = NULL;
    kMallocElem_t *elem = MallocMainHeder.firstElem;

    while (elem != NULL){

        if (elem->free == 1 && elem->size >= size && elem->size < minimumSize){
            suitableElem = elem;
            minimumSize = elem->size;
        }

        elem = elem->next;
    }

    if (suitableElem == NULL){
        suitableElem = (kMallocElem_t*) MallocMainHeder.MallocHead;
        MallocMainHeder.MallocHead += align(size + sizeof(kMallocElem_t) * 2, 21);
        suitableElem->size = align(size + sizeof(kMallocElem_t), 21) - sizeof(kMallocElem_t);
        suitableElem->free = 1;
        suitableElem->next = NULL;
        suitableElem->previous = MallocMainHeder.lastElem;

        if (MallocMainHeder.lastElem == NULL) MallocMainHeder.firstElem = suitableElem;
        else MallocMainHeder.lastElem->next = suitableElem;
    
        SplitRegion(suitableElem, size);

        MallocMainHeder.lastElem = suitableElem->next == NULL ? suitableElem : suitableElem->next;
    }
    else{
        kMallocElem_t *suited = suitableElem->next;
        SplitRegion(suitableElem, size);
        kMallocElem_t *mayNew = suitableElem->next == NULL ? suitableElem : suitableElem->next;

        if (suited == NULL){
            MallocMainHeder.lastElem = mayNew;
        }
        else{
            suited->previous = mayNew;
            mayNew->next = suited;
        }
    }
    suitableElem->free = 0;

    elem = MallocMainHeder.firstElem;
    return (void*)((u64)suitableElem + sizeof(kMallocElem_t));
}

void kfree(void *address){

    kMallocElem_t *elem2Clear = (kMallocElem_t *)((u64) address - sizeof(kMallocElem_t));

    if (elem2Clear->free == 1){
        Printf("Double free on pointer %p\n", address);
        return;
    }

    elem2Clear->free = 1;

    if (elem2Clear->previous != NULL && elem2Clear->previous->free == 1){
        kMallocElem_t *elemBefore = elem2Clear->previous;

        elemBefore->next = elem2Clear->next;
        elemBefore->size += elem2Clear->size + sizeof(kMallocElem_t);
        
        if (elem2Clear->next != NULL) elem2Clear->next->previous = elemBefore;
        elem2Clear = elemBefore;
    }

    if (elem2Clear->next != NULL && elem2Clear->next->free == 1){
        kMallocElem_t *elemNext = elem2Clear->next;

        if (elemNext == MallocMainHeder.lastElem) MallocMainHeder.lastElem = elem2Clear;

        elem2Clear->size += elemNext->size + sizeof(kMallocElem_t);
        elem2Clear->next = elemNext->next;
        if (elemNext->next != NULL) elemNext->next->previous = elem2Clear;
    }
}
