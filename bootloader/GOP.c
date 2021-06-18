#include "GOP.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

static int LoadPSF2(EFI_FILE_HANDLE FontHandle){
    size_t glyphBufferSize = KernelServices.psf2.header.charsize * KernelServices.psf2.header.length;
    
    void *glyphBuffer;
    {
        uefi_call_wrapper(
            FontHandle->SetPosition,
            2,
            FontHandle,
            sizeof(Psf2Header_t)
        );

        uefi_call_wrapper(
            BS->AllocatePool,
            3,
            EfiLoaderData,
            glyphBufferSize,
            (void**)&glyphBuffer
        );

        uefi_call_wrapper(
            FontHandle->Read,
            3,
            FontHandle,
            &glyphBufferSize,
            glyphBuffer
        );
    }

    KernelServices.psf2.glyph_buffer = glyphBuffer;

    return EXIT_SUCCESS;    
}

int InitializeGOP(){
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
 
    EFI_STATUS status = uefi_call_wrapper(
        BS->LocateProtocol,
        3,
        &gopGuid,
        NULL,
        (void**)&gop
    );
    
    if(EFI_ERROR(status)){
        Print(L"Unable to locate GOP\n\r");
        return EXIT_FAILURE;
    }

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, numModes;
    UINTN maxWidth = 0, futureMode =  0;
 
    status = uefi_call_wrapper(
        gop->QueryMode,
        4,
        gop,
        gop->Mode==NULL ? 0 : gop->Mode->Mode,
        &SizeOfInfo,
        &info
    );

    if (status == EFI_NOT_STARTED)
        status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
    
    if(EFI_ERROR(status)){
        Print(L"Unable to get native mode\n\r");
        return EXIT_FAILURE;
    }
    else{
        numModes = gop->Mode->MaxMode;
    }

    for (UINTN i = 0; i < numModes; i++) {
        status = uefi_call_wrapper(
            gop->QueryMode,
            4,
            gop,
            i,
            &SizeOfInfo,
            &info
        );

        if (maxWidth < info->PixelsPerScanLine){
            maxWidth = info->PixelsPerScanLine;
            futureMode = i;
        }
    }

    status = uefi_call_wrapper(gop->SetMode, 2, gop, futureMode);

    if (EFI_ERROR(status)){
        return EXIT_FAILURE;
    }

    if (LoadEdid() == 0){
        KernelServices.edidProtocol.sizeofEdid = 0;
        KernelServices.edidProtocol.edid = NULL;
    }
    else{
        KernelServices.edidProtocol.sizeofEdid = Edid->SizeOfEdid;
        KernelServices.edidProtocol.edid = Edid->Edid;
    }

    KernelServices.frameBuffer.base_address = (u32 *)gop->Mode->FrameBufferBase;
    KernelServices.frameBuffer.buffer_size = gop->Mode->FrameBufferSize;
    KernelServices.frameBuffer.height = gop->Mode->Info->VerticalResolution;
    KernelServices.frameBuffer.width = gop->Mode->Info->HorizontalResolution;
    KernelServices.frameBuffer.pixelsPerScanline = gop->Mode->Info->PixelsPerScanLine;

    return EXIT_SUCCESS;
}

int LoadPSF(EFI_FILE_HANDLE Volume, CHAR16 *FileName){
    UINTN size = sizeof(Psf2Header_t);
    EFI_FILE_HANDLE FontHandle = OpenFile(Volume, FileName);
    
    if (FontHandle == NULL)
        return EXIT_FAILURE;

    uefi_call_wrapper(
        FontHandle->Read,
        3,
        FontHandle,
        &size,
        (void*)&KernelServices.psf2.header
    );
  
    if (*((u32*)KernelServices.psf2.header.magic) == PSF2_MAGIC){
        return LoadPSF2(FontHandle);
    }
    else
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
