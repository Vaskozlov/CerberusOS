#include "pit.hpp"
#include <printf/Printf.h>

u64 PIT::Divisor = 65535;
double PIT::TimeSicneBoot = 0.0;
double PIT::Frequency1Div = 0.0;

void PIT::SetDivisor(u16 divisor){
    Divisor = divisor;

    ARCH::outb((unsigned char)(Divisor & 0x00FF), 0x40);
    ARCH::io_wait();
    ARCH::outb((unsigned char)((divisor & 0xff00) >> 8), 0x40);
}

void PIT::Sleepd(double second){
    double startTime = TimeSicneBoot;
    while (TimeSicneBoot < startTime + second) __asm__ __volatile__("hlt");
}

void PIT::Sleep(i64 milliseconds){
    Sleepd((double)milliseconds / 1000);
}

