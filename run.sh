qemu-system-x86_64 -S -cpu EPYC-v2 -M q35 -m 4G -drive file=./cerberusOS.img -drive if=pflash,format=raw,unit=0,file="ovmf/OVMF.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="ovmf/OVMF_VARS.fd" -net none -rtc clock=host,base=localtime