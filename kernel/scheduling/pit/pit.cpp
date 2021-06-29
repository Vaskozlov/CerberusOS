#include "pit.hpp"
#include <cerberus/printf.h>

u64     PIT::Divisor = 65535;
u64     PIT::TimeSicneBoot = 0;
extern "C" void DO_NOT_OPTIMIZE(i64);

void PIT::Tick(){
    TimeSicneBoot++;
}

void PIT::SetDivisor(u16 divisor){
    Divisor = divisor;

    cerb::outb(PIT_A, (unsigned char)(Divisor & 0x00FF));
    cerb::io_wait();
    cerb::outb(PIT_A, (unsigned char)((divisor & 0xff00) >> 8));
}

void PIT::Sleep(i64 milliseconds){
    i64 startTime = TimeSicneBoot;
    while (TimeSicneBoot < startTime + milliseconds){
        DO_NOT_OPTIMIZE(TimeSicneBoot);
        cerb::hlt();
    }
}

