#include <basicFrameManager.hpp>

#define Psf2Glyph KS->psf2.header
#define PixelsPerScalLine FrameBuffer->pixelsPerScanline
#define FrameAddress FrameBuffer->base_address

extern kernel_services_t *KS;

// seting up static variables
FrameBuffer_t   *BasicRender::FrameBuffer;
vec2<u32>       BasicRender::CursorPosition   = {10U, 10U};
Color_t         BasicRender::ClearColor       = {PixelColor::COLOR_BLACK};
Color_t         BasicRender::FontColor        = {PixelColor::COLOR_WHITE};

void BasicRender::ClearScreen(){
    u32 *pixels = FrameAddress;

    for(
        ;
        pixels < FrameAddress + FrameBuffer->buffer_size / sizeof(u32);
        pixels++
    ){
        *pixels = ClearColor.value;
    }

    CursorPosition.x = 10;
    CursorPosition.y = 10;
}

__attribute__((optnone)) 
int BasicRender::PutChar(int c){
    u16 value;
    u32 *pixels = (u32*) FrameAddress;
    u8 *font_ptr = (u8 *) KS->psf2.glyph_buffer + (c * Psf2Glyph.charsize);

    if (CursorPosition.x + Psf2Glyph.width >= PixelsPerScalLine){
        CursorPosition.x = 10;
        CursorPosition.y += Psf2Glyph.height;
    }

    if (CursorPosition.y + Psf2Glyph.height >= FrameBuffer->height) return -1;

    if (c == '\n'){
        CursorPosition.x = 10;
        CursorPosition.y += Psf2Glyph.height;
        return c;
    }

    for (u32 y = CursorPosition.y; y < CursorPosition.y + Psf2Glyph.height; y++){
        value = *((u16*)font_ptr);

    #if defined(__x86_64__) || defined(__AMD64__)
        __asm__ __volatile__(
            "rolw $1, %0;\n"
            : "=r" (value)
            : "r" (value)
        );
    #else
        // add later
    #endif

        font_ptr += sizeof(value);
        value <<= 1;

        for (u32 x = CursorPosition.x + Psf2Glyph.width - 1; x >= CursorPosition.x; x--){
            if ((value & 0b1) > 0)
                *(pixels + x + (y * PixelsPerScalLine)) = FontColor.value;
            else
                *(pixels + x + (y * PixelsPerScalLine)) = ClearColor.value;
            
           value >>= 1;
        }
    }

    CursorPosition.x += Psf2Glyph.width + 1;
    return c;
}
