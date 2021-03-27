#ifndef pit_hpp
#define pit_hpp

#include <kernel.h>
#include <hardware/io.hpp>

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SCALE 1193182
#define PIT_SET 0x36

class PIT{
    static u64 Divisor;
    static double TimeSicneBoot;
    static double Frequency1Div;

public:
    static inline u64 GetFrequency() { return PIT_SCALE / Divisor; }
    static inline void Tick() { TimeSicneBoot += 1.0/GetFrequency(); }

public:
    static void Sleep(i64 milliseconds);
    static void Sleepd(double seconds);

public:
    static void SetDivisor(u16 divisor);
    inline static void SetFrequency(u64 frequency){
        Frequency1Div = 1.0/frequency;
        SetDivisor(PIT_SCALE / frequency);
    }

    inline static double GetTimeSicneBoot() { return TimeSicneBoot; }

public:
    PIT() = delete;
    ~PIT() = delete;
};

#endif /* pit_hpp */