#ifndef efiFiles_h
#define efiFiles_h

#include <efi.h>
#include <efilib.h>

/**
 * GetVolume - open root volume 
 * @ImageHandle: image handle from efi_main
 * Return: volume handle of root dir
 */
EFI_FILE_HANDLE GetVolume(EFI_FILE_HANDLE ImageHandle);

/**
 * OpenFile - open file or directory
 * @Volume: volume where file is located
 * @FileName: name of file or directory
 * Return: handle of file or directory
 */
EFI_FILE_HANDLE OpenFile(EFI_FILE_HANDLE Volume, CHAR16 *FileName);

/**
 * GetFileSize - return size of file
 * @FileHandle: file handle
 * @Buffer: out buffer
 * Return: size of file in bytes
 */
UINTN GetFileSize(EFI_FILE_HANDLE FileHandle, OUT VOID *Buffer);

#endif /* efiFiles_h */
