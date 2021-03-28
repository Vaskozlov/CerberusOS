#ifndef basicFrameManager_hpp
#define basicFrameManager_hpp

#include <render.h>
#include <kernel.h>

template <typename T>
struct vec2{
    T x, y;

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
    static FrameBuffer_t *FrameBuffer;
    static vec2<u32>     CursorPosition;
    
public:
    static Color_t  ClearColor;
    static Color_t  FontColor;

public:
    static void ClearScreen();
    static int PutChar(int c);

public:
    static inline void SetFrameBuffer(FrameBuffer_t *newFrameBuffer){
        FrameBuffer = newFrameBuffer;
    }

    static inline void SetCursorPosition(vec2<u32> pos){
        CursorPosition = pos;
    }

public:
    BasicRender() = delete;
    ~BasicRender() = delete;

    BasicRender(BasicRender&) = delete;
    BasicRender(BasicRender&&) = delete;
    BasicRender& operator=(BasicRender&&) = delete;
};

#endif /* basicFrameManager_hpp */
