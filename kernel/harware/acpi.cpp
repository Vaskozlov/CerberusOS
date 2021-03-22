#include <acpi.hpp>
#include <string.h>
#include <pci.hpp>

extern kernel_services_t *KS;

namespace ACPI{

    void *FindTable(SDTHeader *sdtHeader, char *signature){
        int enteries = (sdtHeader->length - sizeof(ACPI::SDTHeader)) / 8;

        for (int i = 0; i < enteries; i++){
            ACPI::SDTHeader *newSTDHeader = (ACPI::SDTHeader*)*(u64*)((u64)sdtHeader + sizeof(ACPI::SDTHeader) + (i * 8));

            if (strncmp((const char *)newSTDHeader->signature, signature, 4) == 0)
                return newSTDHeader;
        }
        return NULL;
    }

}