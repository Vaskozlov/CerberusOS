#ifndef psf2_h
#define psf2_h

#include <kernelTypes.h>

__BEGIN_DECLS

#define PSF2_MAGIC0             0x72U
#define PSF2_MAGIC1             0xb5U
#define PSF2_MAGIC2             0x4aU
#define PSF2_MAGIC3             0x86U
#define PSF2_MAGIC              0x864ab572UL

#define PSF2_HAS_UNICODE_TABLE  0x1
#define PSF2_MAXVERSION         0x0

#define PSF2_SEPARATOR          0xFF
#define PSF2_STARTSEQ           0xFE

/**
 * INIT_PSF2 - clear PSF2Header struct
 */
#define INIT_PSF2 {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U}

/**
 * Psf2Header - header for PSF font version 2
 * @magic: magic number of font
 * @version: vesrion of font
 * @headersize: offset of bitmap in file
 * @flags: flags of font
 * @length: number of glyphs
 * @charsize: number of bytes for char
 * @height: max height of glyphs
 * @width: max width of glyphs (with width they show ma dimensions of glyphs)
 */
typedef struct {
    u8 magic[4];
    u32 version;
    u32 headersize;
    u32 flags;
    u32 length;
    u32 charsize;
    u32 height, width;
} Psf2Header_t;

/**
 * Psf2Font - structure of psf font (version 2)
 * @glyph_buffer: buffer with chars
 * @Psf2Header_t: psf2 header
 */
typedef struct {
    void            *glyph_buffer;
    Psf2Header_t    header;
} Psf2Font_t;

/**
 * ColorMask_t - color mask of pixel
 * @r: red value of pixel (0-255)
 * @g: green value of pixel (0-255)
 * @b: blue value of pixel (0-255)
 * @empty: empty value (not used, but can be used for some data)
 */
typedef struct {
    u8      r;
    u8      g;
    u8      b;
    u8      empty;
} ColorMask_t;

/**
 * Color_t - ColorMask_t with an opportunity to get value of pixels as u32
 * @value: value of pixel
 * @color: ColorMask of pixel
 */
typedef union{
    u32         value;
    ColorMask_t color;
} Color_t;

__END_DECLS

#endif /* psf2_h */
