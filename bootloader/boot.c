#include <elf.h>
#include <string.h>

#include "GOP.h"
#include "efiFiles.h"

EFI_FILE_HANDLE RootVolume;
EFI_FILE_HANDLE FontVolume;
kernel_services_t KernelServices;

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
    InitializeLib(ImageHandle, SystemTable);

    RootVolume = GetVolume(ImageHandle);
    FontVolume = OpenFile(RootVolume, L"font");
    EFI_FILE_HANDLE KernelFile = OpenFile(RootVolume, L"kernel.elf");

    if (KernelFile == NULL){
        Print(L"Unable to open kernel file\n\r");
        return EFI_LOAD_ERROR;
    }

    void *rspd = NULL;
    EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;
    EFI_CONFIGURATION_TABLE *configTable = SystemTable->ConfigurationTable;

    for (UINTN index = 0; index < SystemTable->NumberOfTableEntries; index++, configTable++){
        if (CompareGuid(&configTable[index].VendorGuid, &Acpi2TableGuid)){
            if (strncmp("RSD PTR ", configTable->VendorTable, 8) == 0){
                rspd = (void*) configTable->VendorTable;
            }
        }
    }
    
    Elf64_Ehdr eHeader;
    {
        EFI_FILE_INFO *elfFileInfo;
        UINTN sizeOfHeader = sizeof(Elf64_Ehdr);
        UINTN elfFileInfoSize = GetFileSize(KernelFile, NULL);

        uefi_call_wrapper(
            BS->AllocatePool,
            3,
            EfiLoaderData,
            elfFileInfoSize,
            (void**)&elfFileInfo
        );

        elfFileInfoSize = GetFileSize(KernelFile, (void**)elfFileInfo);

        uefi_call_wrapper(
            KernelFile->Read,
            3,
            KernelFile,
            &sizeOfHeader,
            &eHeader
        );
    }

    if (
        *((u32*) eHeader.e_ident) != *((u32*) ELFMAG) ||
        eHeader.e_ident[EI_CLASS] != ELFCLASS64 ||
		eHeader.e_ident[EI_DATA] != ELFDATA2LSB ||
		eHeader.e_type != ET_EXEC ||
		eHeader.e_machine != EM_X86_64 ||
		eHeader.e_version != EV_CURRENT
    ){
        Print(L"Bad ELF format\n\r");
        return EFI_LOAD_ERROR;
    }

    Elf64_Phdr *pHeaders;
    {
        UINTN pHeadersSize;

        uefi_call_wrapper(
            KernelFile->SetPosition,
            2,
            KernelFile,
            eHeader.e_phoff  
        );

        pHeadersSize = eHeader.e_phnum * eHeader.e_phentsize;

        uefi_call_wrapper(
            BS->AllocatePages,
            3,
            EfiLoaderData,
            pHeadersSize,
            (void**)&pHeaders
        );

        uefi_call_wrapper(
            KernelFile->Read,
            3,
            KernelFile,
            &pHeadersSize,
            pHeaders
        );
    }

    for(
        Elf64_Phdr *pHeader = pHeaders;
        (u8*) pHeader < (u8*) pHeaders + eHeader.e_phnum * eHeader.e_phentsize;
        pHeader = (Elf64_Phdr*)((u8*)pHeader + eHeader.e_phentsize)
    ){
        switch (pHeader->p_type){

            case PT_LOAD:
            {
                UINTN pHeaderSize;
                Elf64_Addr segment = pHeader->p_paddr;
                int pages = (pHeader->p_memsz + 0x1000 - 1) / 0x1000;

                uefi_call_wrapper(
                    BS->AllocatePages,
                    4,
                    AllocateAddress,
                    EfiLoaderData,
                    pages,
                    &segment
                );

                uefi_call_wrapper(
                    KernelFile->SetPosition,
                    2,
                    KernelFile,
                    pHeader->p_offset
                );

                pHeaderSize = pHeader->p_filesz;

                uefi_call_wrapper(
                    KernelFile->Read,
                    3,
                    KernelFile,
                    &pHeaderSize,
                    (void*)segment
                );
            }

            break;
        }
    }
    
    if (InitializeGOP() != EXIT_SUCCESS){
        Print(L"Error in GOP initialization\n\r");
        return EFI_LOAD_ERROR;
    }

    if (LoadPSF(FontVolume, L"zap-light24.psf") != EXIT_SUCCESS){
        Print(L"Unable to open font file\n\r");
        return EFI_LOAD_ERROR;
    }

    EFI_MEMORY_DESCRIPTOR *Map = NULL;
    UINTN MapSize, MapKey;
    UINTN DescriptorSize;

    UINT32 DescriptorVersion;
    {
        uefi_call_wrapper(
            BS->GetMemoryMap,
            5,
            &MapSize,
            Map,
            &MapKey,
            &DescriptorSize,
            &DescriptorVersion
        );

        uefi_call_wrapper(
            BS->AllocatePool,
            3,
            EfiLoaderData,
            MapSize,
            (void**) &Map
        );

        uefi_call_wrapper(
            BS->GetMemoryMap,
            5,
            &MapSize,
            Map,
            &MapKey,
            &DescriptorSize,
            &DescriptorVersion
        );
    }

    KernelServices.mMap = (MemoryDescriptor_t*)Map;
    KernelServices.mMapSize = MapSize;
    KernelServices.mMapDescriptorSize = DescriptorSize;
    KernelServices.rsdp = rspd;


    uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, MapKey);

    int (*KernelStart)() = ((__attribute__((sysv_abi)) int (*)(kernel_services_t *services)) eHeader.e_entry);
    
    Print(L"Success %d\n\r", KernelStart(&KernelServices));
    
    return EFI_SUCCESS;
}
