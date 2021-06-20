cd bootloader
make -j
cd ../CerberusLib
make -j
cd ../kernel
make -j all
cd ..
