#ifndef basicFrameManager_hpp
#define basicFrameManager_hpp

#include <render.h>
#include <kernel.h>

extern kernel_services_t *KS;

template <typename _Tp>
struct vec2{
    _Tp x, y;

public:
    always_inline vec2(_Tp X, _Tp Y) : x(X), y(Y) {}

public:
    vec2() = default;
    ~vec2() = default;
};

enum PixelColor{
    COLOR_BLACK = 0x00000000UL,
    COLOR_RED   = 0xFF000000UL,
    COLOR_GREEN = 0x0000FF00UL,
    COLOR_BLUE  = 0x000000FFUL,
    COLOR_WHITE = 0x00FFFFFFUL
};

class BasicRender{
    static FrameBuffer_t    *FrameBuffer;
    static vec2<u32>        CursorPosition;
    static vec2<u32>        PrintingSize;
    
public:
    static Color_t  ClearColor;
    static Color_t  FontColor;

public:
    static void ClearScreen();
    static int PutChar(int c);
    static void SetColor(unsigned char r, unsigned char g, unsigned char b);

public:
    static inline void SetFrameBuffer(FrameBuffer_t *newFrameBuffer){
        FrameBuffer = newFrameBuffer;
        PrintingSize.x = newFrameBuffer->pixelsPerScanline / KS->psf2.header.width;
        PrintingSize.y = newFrameBuffer->height / KS->psf2.header.height;
    }

    static inline void SetCursorPosition(vec2<u32> pos){
        CursorPosition = pos;
    }

    static inline vec2<u32> GetCursorPosition() { return CursorPosition; }

public:
    BasicRender() = delete;
    ~BasicRender() = delete;

    BasicRender(BasicRender&) = delete;
    BasicRender(BasicRender&&) = delete;
    BasicRender& operator=(BasicRender&&) = delete;
};

#endif /* basicFrameManager_hpp */
