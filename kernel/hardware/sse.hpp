#ifndef sse_hpp
#define sse_hpp

#include <kernel.h>

enum SSE_FLAGS{

};

extern "C" u64 ASM_Scan4SSE();

class SSE{

    static u64 flags;

public:
    strict_inline void Scan4SSE(){
        flags = ASM_Scan4SSE();
    }

    void EnableAVX();
};

#endif /* sse_hpp */
