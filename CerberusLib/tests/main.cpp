#include <iostream>
#include <cmath>
#include <cerberus/vector.hpp>
#include <cerberus/mDebug.hpp>
#include <cerberus/printf.h>
#include <cerberus/cerbMath.h>
#include <cerberus/doubleBitmap.hpp>
#include <tuple>

INIT_MDEBUGGER(printf, malloc, free);

void vector_test(){
    cerb::vector<std::string> a = {"hello world!"};
    std::string abc = "Hello world!";
    
    a.push_back("Hello world!");
    a.push_back("Hello world!");
    a.push_back("Hello world!");
    a.push_back("Hello world!");
    a.push_back("Hello world!");
    a.push_back("Hello world!");
    a.push_back("Hello world!");
    a.push_back("Hello world!");

    cerb::vector<int> b;
    cerb::vector<int> c;
    
    b.push_back(200);
    c.push_back(200);
   
    a.reserve(100);

    for (auto elem: a)
        std::cout << elem << std::endl;
}

int main(){
    Putchar = putchar;

    static u64 buffer1[100], buffer2[100];
    
    cerb::DoubleBitmapFree<u64> bits(buffer1, buffer2, 100 * cerb::sizeofbits<u64>());
    bits.set1(10, 1);

    return EXIT_SUCCESS;
}