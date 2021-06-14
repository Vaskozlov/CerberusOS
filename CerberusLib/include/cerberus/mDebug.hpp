#ifndef cerbMDebug_hpp
#define cerbMDebug_hpp

#include <cerberus/move.hpp>
#include <cerberus/cerberusc++.hpp>

#define INIT_MDEBUGGER(Printf, Malloc, Free) cerb::MemoryDebug cerb::MemoryDebuger(Printf, Malloc, Free)

namespace cerb{

    class MemoryDebug{
        int (*Printf)(const char *__restrict, ...);
        void *(*Malloc)(size_t);
        void (*Free)(void*);
        size_t memoryBlocks[1000183];
        static constexpr size_t NumberOfBlocks = sizeof(memoryBlocks) / sizeof(size_t);

    public:
        inline void *malloc(size_t size){
            void *addr = Malloc(size);
            Printf("Allocating: %llu bytes\n", size);
            memoryBlocks[(u64)addr % NumberOfBlocks] = size; 
            return addr;
        }

        inline void free(void *addr){
            size_t size = 0;
            cerb::swap(size, memoryBlocks[(u64)addr % NumberOfBlocks]);
            Printf("Freeing: %llu\n", size);
            Free(addr);
        }

    public:
        inline MemoryDebug(int (*printfFun)(const char *, ...), void *(*mallocFun)(size_t), void (*freeFun)(void*)) : Printf(printfFun), Malloc(mallocFun), Free(freeFun) {}
    };

    extern MemoryDebug MemoryDebuger;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-exception-spec-mismatch"

void *operator new(size_t size) { return cerb::MemoryDebuger.malloc(size); }
void operator delete(void *addr) { cerb::MemoryDebuger.free(addr); }

#pragma GCC diagnostic pop

#endif /* cerbMDebug_hpp */
