#include "efiFiles.h"

EFI_FILE_HANDLE GetVolume(EFI_FILE_HANDLE ImageHandle){
    EFI_FILE_HANDLE Volume;
	EFI_FILE_IO_INTERFACE *IOVolume;
    EFI_LOADED_IMAGE *loaded_image = NULL;

	uefi_call_wrapper(
		BS->HandleProtocol,
		3,
		ImageHandle,
		&gEfiLoadedImageProtocolGuid,
		(void**)&loaded_image
	);

	uefi_call_wrapper(
		BS->HandleProtocol,
		3,
		loaded_image->DeviceHandle,
		&gEfiSimpleFileSystemProtocolGuid,
		(void*)&IOVolume
	);
    
	uefi_call_wrapper(
		IOVolume->OpenVolume,
		2,
		IOVolume,
		&Volume
	);

	return Volume;
}


EFI_FILE_HANDLE OpenFile(EFI_FILE_HANDLE Volume, CHAR16 *FileName){
    EFI_FILE_HANDLE FIleHandle;

	if (uefi_call_wrapper(
		Volume->Open,
		5,
		Volume,
		&FIleHandle,
		FileName,
		EFI_FILE_MODE_READ,
		EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM
	) != EFI_SUCCESS){
		return NULL;
	}

	return FIleHandle;
}


UINTN GetFileSize(EFI_FILE_HANDLE FileHandle, OUT VOID *Buffer){
	UINTN file_size;
	
    uefi_call_wrapper(
        FileHandle->GetInfo,
        4,
        FileHandle,
        &gEfiFileInfoGuid,
        &file_size,
        Buffer
    );

	return file_size;
}
