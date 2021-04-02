#ifndef kernel_h
#define kernel_h

#include <kernelTypes.h>
#include <render/render.h>

__BEGIN_DECLS

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef struct{
    u32     type;
    u32     pad;
    void    *physicalStart;
    void    *virtualStart;
    u64     numberOfPages;
    u64     attribute;
} MemoryDescriptor_t;

/**
 * kernel_services_t - basic kernel services structure
 * 
 * 
 */
typedef struct {
    u64                 mMapSize;
    u64                 mMapDescriptorSize;
    MemoryDescriptor_t  *mMap;
    EdidProtocol_t      edidProtocol;
    FrameBuffer_t       frameBuffer;
    Psf2Font_t          psf2;
    void                *rsdp;
} kernel_services_t;

void *memset (void *__s, int __c, size_t __n) __THROW __nonnull ((1));

#define kprintf(...) Printf(0, __VA_ARGS__)

__END_DECLS

#endif /* kernel_h */
