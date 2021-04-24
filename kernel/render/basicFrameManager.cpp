#include <basicFrameManager.hpp>
#include <printf/Printf.h>
#include <arch.hpp>

#define Psf2Glyph KS->psf2.header
#define PixelsPerScalLine FrameBuffer->pixelsPerScanline
#define FrameAddress FrameBuffer->base_address

FrameBuffer_t   *BasicRender::FrameBuffer;
vec2<u32>       BasicRender::PrintingSize;
vec2<u32>       BasicRender::CursorPosition  = {0, 0};
Color_t         BasicRender::ClearColor      = {PixelColor::COLOR_BLACK};
Color_t         BasicRender::FontColor       = {PixelColor::COLOR_WHITE};

void BasicRender::ClearScreen(){    
    u64 times = FrameBuffer->buffer_size / sizeof(u32) / 2;

    ARCH::memset64(FrameAddress, (u64)ClearColor.value | ((u64)ClearColor.value << 32UL), times);
    CursorPosition.x = 0;
    CursorPosition.y = 0;
}


int BasicRender::PutChar(int c){
    u16 value;
    u8  *font_ptr = (u8 *) KS->psf2.glyph_buffer + (c * Psf2Glyph.charsize);

    if (CursorPosition.x >= PrintingSize.x){
        CursorPosition.x = 0;
        CursorPosition.y ++;
    }

    if (CursorPosition.y > PrintingSize.y) return -1;

    if (c == '\n'){
        CursorPosition.x = 0;
        CursorPosition.y++;
        return c;
    }

    for (u32 y = 0; y < Psf2Glyph.height; y++){
        
        auto lineAddress = FrameAddress + (y + Psf2Glyph.height * CursorPosition.y) * PixelsPerScalLine + CursorPosition.x * Psf2Glyph.width + Psf2Glyph.width;
    
        value = ARCH::rotate_left16(*((u16*)font_ptr),1);
        font_ptr += sizeof(value);

        for (u32 x = Psf2Glyph.width; x > 0; x--){
            *lineAddress = (value & 0b1) > 0 ? FontColor.value : ClearColor.value;
            lineAddress--;
            value >>= 1;
        }
    }

    CursorPosition.x++;
    return c;
}

void BasicRender::SetColor(unsigned char r, unsigned char g, unsigned char b){
    FontColor.color.r = b;
    FontColor.color.g = g;
    FontColor.color.b = r;
}
