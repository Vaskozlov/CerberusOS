#ifndef render_h
#define render_h

#include <kernelTypes.h>
#include <render/psf2.h>

__BEGIN_DECLS

/**
 * EdidProtocol_t - edid protocol structure
 * 
 */
typedef struct {
    u32     sizeofEdid;
    u8      *edid;
} EdidProtocol_t;

/**
 * FrameBuffer_t - hold information about frame
 * @width: width of frame
 * @height: height of frame
 * @buffer_size: size of frame buffer
 * @base_address: base address of frame buffer
 * @pixelsPerScanline: number of pixels per scanline
 */
typedef struct {
    u32     width, height;
    u32     *base_address;
    size_t  buffer_size;
    u32     pixelsPerScanline;
} FrameBuffer_t;

__END_DECL

#endif /* render_h */
