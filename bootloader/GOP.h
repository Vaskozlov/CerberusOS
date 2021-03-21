#ifndef GOP_h
#define GOP_h

#include <efi.h>
#include <efilib.h>
#include <kernel.h>

#include "efiFiles.h"
#include "loadEdid.h"

extern EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
extern kernel_services_t KernelServices;

/**
 * InitializeGOP - setup Framebuffer object
 * Return: EXIT_SUCCESS on success and EXIT_FAILURE on failure
 */
int InitializeGOP();

/**
 * LoadPSF - load PSF file into kernel_args
 * @Volume: folder where file is located
 * @FileName: name of file 
 * Return: EXIT_SUCCESS on success and EXIT_FAILURE
 */
int LoadPSF(EFI_FILE_HANDLE Volume, CHAR16 *FileName);

#endif /* GOP_h */
