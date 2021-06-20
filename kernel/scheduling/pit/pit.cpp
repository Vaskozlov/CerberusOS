#include "pit.hpp"

u64     PIT::Divisor = 65535;
u64     PIT::TimeSicneBoot = 0;

void PIT::SetDivisor(u16 divisor){
    Divisor = divisor;

    ARCH::outb((unsigned char)(Divisor & 0x00FF), 0x40);
    ARCH::io_wait();
    ARCH::outb((unsigned char)((divisor & 0xff00) >> 8), 0x40);
}

void PIT::Sleep(i64 milliseconds){
    u64 startTime = TimeSicneBoot;
    while (TimeSicneBoot < startTime + milliseconds) ARCH::hlt();
}

