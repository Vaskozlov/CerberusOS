#include "kmalloc.h"
#include <cerberus/printf.h>
#include <memory/VMManager.hpp>

MallocHeader_t MallocMainHeder;
extern VMManager KernelVMM;

void InitKMalloc(){
    MallocMainHeder.MallocBegin = cerb::align(PhisicalAllocator::GetTotalMemory(), 30);
    MallocMainHeder.MallocHead = MallocMainHeder.MallocBegin;
}

static void SplitRegion(kMallocElem_t *region, size_t size){

    if ((i64)region->size - (i64)size - (i64)sizeof(kMallocElem_t) < 1) [[unlikely]] return;

    kMallocElem_t *newRegion = (kMallocElem_t *) ((u64)region + sizeof(kMallocElem_t) + size);
    newRegion->size = region->size - size - sizeof(kMallocElem_t);
    newRegion->free = 1;
    newRegion->next = NULL;
    newRegion->previous = region;
    region->next = newRegion;
    region->size = size;
}

always_inline void *FillElem(kMallocElem_t *suitableElem, u64 size){
    if (suitableElem == NULL) [[unlikely]]{
        suitableElem = (kMallocElem_t*) MallocMainHeder.MallocHead;
        MallocMainHeder.MallocHead += cerb::align(size + sizeof(kMallocElem_t) * 2, 21);
        suitableElem->size = cerb::align(size + sizeof(kMallocElem_t), 21) - sizeof(kMallocElem_t);  // creating new malloc page with size of 2MB (or more, but it will be aligned to 2MB)
        suitableElem->free = 1;
        suitableElem->next = NULL;
        suitableElem->previous = MallocMainHeder.lastElem;

        if (MallocMainHeder.lastElem == NULL) MallocMainHeder.firstElem = suitableElem;
        else MallocMainHeder.lastElem->next = suitableElem;
    
        SplitRegion(suitableElem, size);

        MallocMainHeder.lastElem = suitableElem->next == NULL ? suitableElem : suitableElem->next;
    }
    else [[likely]] {
        kMallocElem_t *suited = suitableElem->next;
        SplitRegion(suitableElem, size);
        kMallocElem_t *mayNew = suitableElem->next == NULL ? suitableElem : suitableElem->next;

        if (suited == NULL) [[unlikely]]
            MallocMainHeder.lastElem = mayNew;
        else  [[likely]]{
            suited->previous = mayNew;
            mayNew->next = suited;
        }
    }

    suitableElem->free = 0;
    return (void*)((u64)suitableElem + sizeof(kMallocElem_t));
}

void *kmalloc_smallest(u64 size){
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

    return FillElem(suitableElem, size);
}

void *kmalloc_fast(u64 size){
    kMallocElem_t *suitableElem = NULL;
    kMallocElem_t *elem = MallocMainHeder.firstElem;

    while (elem != NULL){

        if (elem->free == 1 && elem->size >= size){
            suitableElem = elem;
           break;
        }

        elem = elem->next;
    }

    return FillElem(suitableElem, size);
}

void *kmalloc_biggest(u64 size){
    u64 minimumSize = 0;
    kMallocElem_t *suitableElem = NULL;
    kMallocElem_t *elem = MallocMainHeder.firstElem;

    while (elem != NULL){

        if (elem->free == 1 && elem->size >= size && elem->size > minimumSize){
            suitableElem = elem;
            minimumSize = elem->size;
        }

        elem = elem->next;
    }

    return FillElem(suitableElem, size);
}

void kfree(void *address){

    kMallocElem_t *elem2Clear = (kMallocElem_t *)((u64) address - sizeof(kMallocElem_t));

    if (elem2Clear->free == 1){
        cerbPrintf("Double free on pointer %p\n", address);
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

#if OS_DEBUG == 0

void *operator new  (size_t size)  { return kmalloc(size); }
void *operator new[](size_t size)  { return kmalloc(size); }
void operator delete  (void *p)    { kfree(p);              }
void operator delete[](void *p)    { kfree(p);              }

#endif /* OS_DEBUG */
