#include "pci.hpp"
#include "acpi.hpp"
#include <string.h>

extern kernel_services_t *KS;

namespace ACPI{

    void *FindTable(SDTHeader *sdtHeader, char *signature){
        int enteries = (sdtHeader->length - sizeof(ACPI::SDTHeader)) / 8;

        for (int i = 0; i < enteries; i++){
            ACPI::SDTHeader *newSTDHeader = (ACPI::SDTHeader*)*(u64*)((u64)sdtHeader + sizeof(ACPI::SDTHeader) + (i * 8));
            if (*((u32*)newSTDHeader->signature) == *((u32*)signature)) return newSTDHeader;
        }
        
        return NULL;
    }
}