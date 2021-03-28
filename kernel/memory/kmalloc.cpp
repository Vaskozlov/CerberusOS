#include "kmalloc.hpp"
#include <printf/Printf.h>

MallocHeader kMallocHeader;

template<class T>
const inline T align(T number, size_t power)
{
    auto middle = (number & ((1UL << power) - 1));
    middle = middle == 0 ? (1UL << power) : middle;
    return number + (1UL << power) - middle;
}

void InitKMalloc(){
    kMallocHeader.MallocBegin = align(PhisicalAllocator::GetTotalMemory(), 30);
    kMallocHeader.MallocHead = kMallocHeader.MallocBegin;
    kMallocHeader.MappedPages = 0;
    kMallocHeader.firstElem = NULL;
}

static void SplitRegion(kMallocElem *elem, size_t firstRegSize){
    if (elem->size - firstRegSize < sizeof(kMallocElem) * 2) return;

    kMallocElem *anotherReg = (kMallocElem *)((u64)elem + firstRegSize + sizeof(kMallocElem));
    anotherReg->free = true;
    anotherReg->next = NULL;
    anotherReg->previous = elem;
    anotherReg->size = elem->size - (firstRegSize + sizeof(kMallocElem) * 2);
    elem->size = firstRegSize;
    elem->next = anotherReg;
}

void *kmalloc(u64 size){
    if (size == 0) return NULL;
    if (size >= PhisicalAllocator::GetAvailableMemory()) return NULL;
    u64 smallestSize = 0xFFFFFFFFFFFFFFFF;
    kMallocElem *bestElem = NULL;

    kMallocElem *elem = kMallocHeader.firstElem;
    
    while(elem != NULL){
        if (elem->free == true && elem->size >= size && elem->size < smallestSize){
            smallestSize = elem->size;
            bestElem = elem;
        }
        elem = elem->next;
    }

    if (bestElem == NULL){
        kMallocElem *newElem = (kMallocElem*) kMallocHeader.MallocHead;
        kMallocHeader.MallocHead += ((((size) >> 21UL) + 1) << 21UL) + sizeof(kMallocElem);
        
        newElem->size = ((((size) >> 21UL) + 1) << 21UL) - sizeof(kMallocElem);
        newElem->next = NULL;
        newElem->free = true;
        newElem->previous = NULL;

        bestElem = newElem;
        smallestSize = newElem->size;

        if (kMallocHeader.firstElem == NULL) kMallocHeader.firstElem = newElem;

        kMallocHeader.lastElem = newElem;
    }

    SplitRegion(bestElem, size);
    bestElem->free = false;
    return (void*)((u64)bestElem + sizeof(kMallocElem));
}

void kfree(void *address){
    kMallocElem *elem2Clear = (kMallocElem *)((u64) address - sizeof(kMallocElem));
    
    if (elem2Clear->free == 1){
        Printf("Double free on pointer %p\n", address);
        return;
    }

    elem2Clear->free = true;
    
    if (elem2Clear == kMallocHeader.firstElem)
        kMallocHeader.firstElem = elem2Clear;

    if (elem2Clear->next != NULL && elem2Clear->next->free == true){
        kMallocElem *nextNext = elem2Clear->next->next;
        elem2Clear->size += sizeof(kMallocElem) + elem2Clear->next->size;

        if (nextNext != NULL)
            nextNext->previous = elem2Clear;

        elem2Clear->next = nextNext;
    }

    if (elem2Clear->previous != NULL && elem2Clear->previous->free == true){
        elem2Clear->previous->size += sizeof(kMallocElem) + elem2Clear->size;
        elem2Clear->previous->next = elem2Clear->next;

        if (elem2Clear->next != NULL)
            elem2Clear->next->previous = elem2Clear->previous;
    }
}
