#ifndef LoadEdid_h
#define LoadEdid_h

#include <efi.h>
#include <efilib.h>
#include <kernel.h>

extern EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
extern EFI_EDID_ACTIVE_PROTOCOL *Edid;

/**
 * LoadEdid - loads edid
 * Return: EXIT_SUCCESS on success or EXIT_FAILURE on failure
 */
int LoadEdid(void);

#endif /* LoadEdid_h */