#ifndef kmalloc_hpp
#define kmalloc_hpp

#include <memory/VMManager.hpp>

struct kMallocElem{
    bool free;
    u64 size;
    kMallocElem *next;
    kMallocElem *previous;
};

struct MallocHeader{
    u64 MappedPages;
    u64 MallocHead;
    u64 MallocBegin;
    kMallocElem *firstElem;
    kMallocElem *lastElem;
};

extern VMManager KernelVMM;
extern MallocHeader kMallocHeader;

void InitKMalloc();
void *kmalloc(u64 size);
void kfree(void *address);

#endif /* kmalloc_hpp */
