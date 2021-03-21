#include "loadEdid.h"

EFI_EDID_ACTIVE_PROTOCOL *Edid;

int LoadEdid(void){
    EFI_STATUS Status;
    UINTN  NumHandles = 0, Size, Counter = 0;
    EFI_HANDLE *HandleList = NULL;
    EFI_GUID gEfiGopProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    Status = uefi_call_wrapper(BS->LocateHandle, 5, ByProtocol, 
                               &gEfiGopProtocolGuid, NULL, &Size, HandleList);

    if (Status == EFI_BUFFER_TOO_SMALL){
        HandleList = AllocateZeroPool(Size + sizeof(EFI_HANDLE));
         
        if (HandleList == NULL){
            //Print(L"ERROR: Unable to allocate memory\n");
            return 0;
        }

        Status = uefi_call_wrapper(BS->LocateHandle, 5, ByProtocol, 
                                    &gEfiGopProtocolGuid, NULL, &Size, HandleList);
        HandleList[Size / sizeof(EFI_HANDLE)] = NULL;
    }
 
    if (EFI_ERROR(Status)){
        if (HandleList != NULL){
            FreePool(HandleList);
            //Print(L"ERROR: No handles\n");
            return 0;
        }
    } 

    NumHandles = Size / sizeof(EFI_HANDLE);

    if (NumHandles == 0 || NumHandles > 2000) {
        //Print(L"No handles found\n");
        return 0;
    } 
    
    for (unsigned int i = 0; i < NumHandles; i++){
        EFI_HANDLE Handle = HandleList[i];
    
        Status = uefi_call_wrapper(BS->HandleProtocol, 3, Handle,
                          &gEfiEdidActiveProtocolGuid, (void**)&Edid);
        
        Counter += (Status == EFI_SUCCESS);
    }

    return Counter;
}
