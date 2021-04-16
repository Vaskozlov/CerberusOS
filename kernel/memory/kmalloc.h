#ifndef kmalloc_h
#define kmalloc_h

#include <kernel.h>

__BEGIN_DECLS

typedef struct kMallocElem{
    uint8_t free;
    u64 size;
    struct kMallocElem *next;
    struct kMallocElem *previous;
} kMallocElem_t;

typedef struct MallocHeader{
    u64 MappedPages;
    u64 MallocHead;
    u64 MallocBegin;
    struct kMallocElem *firstElem;
    struct kMallocElem *lastElem;
} MallocHeader_t;

extern MallocHeader_t MallocMainHeder;

void InitKMalloc();
void *kmalloc_smallest(u64 size);
void *kmalloc_fast(u64 size);
void *kmalloc_biggest(u64 size);
void kfree(void *address);

__END_DECLS

#if (defined(__cplusplus) || defined(c_plusplus))

strict_inline void *kmalloc(u64 size) { return kmalloc_smallest(size); }

#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Winline-new-delete"
    strict_inline void *operator new  (size_t size)  { return kmalloc(size); }
    strict_inline void *operator new[](size_t size)  { return kmalloc(size); }
    strict_inline void operator delete  (void *p)    { return kfree(p);      }
    strict_inline void operator delete[](void *p)    { return kfree(p);      }
#  pragma GCC diagnostic pop

#else

#define kmalloc(size) kmalloc_smallest(size)

#endif

#endif /* kmalloc_hpp */
