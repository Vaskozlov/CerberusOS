#ifndef pit_hpp
#define pit_hpp

#include <kernel.h>
#include <arch.hpp>

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SCALE 1193182
#define PIT_SET 0x36

class PIT{
    static u64 Divisor;
    static u64 TimeSicneBoot;
    static double Frequency1Div;

public:
    static strict_inline u64 GetFrequency() { return PIT_SCALE / Divisor; }
    static strict_inline void Tick()        { TimeSicneBoot++; }

public:
    static void Sleep(i64 milliseconds);
    
    strict_inline static void Sleepd(double seconds){
        Sleep((i64)(seconds * 1000));
    }

public:
    static void SetDivisor(u16 divisor);

    strict_inline static void SetFrequency(u64 frequency){
        SetDivisor(PIT_SCALE / frequency);
    }

    strict_inline static u64 GetTimeSicneBoot() { return TimeSicneBoot; }

public:
    PIT() = delete;
    ~PIT() = delete;
};

#endif /* pit_hpp */