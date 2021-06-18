cd gnu-efi
make -j
cd ../musl
CXX="clang++" CXXFLAGS="-O3" CC="clang" CFLAGS="-O3" ./configure 
make -j
cd ../CerberusLib
make -j
cd ..
