#include <float.h>
#include <limits.h>
#include <iso646.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdalign.h>
#include <stdnoreturn.h>

#include "uefi/efi.h"
#include "uefi/protocol/efi-acpitp.h"
#include "uefi/protocol/efi-fp.h"
#include "uefi/protocol/efi-lidpp.h"
#include "uefi/protocol/efi-pbcp.h"
#include "uefi/protocol/efi-stip.h"
#include "uefi/protocol/efi-vmpp.h"
#include "uefi/protocol/efi-dpp.h"
#include "uefi/protocol/efi-gop.h"
#include "uefi/protocol/efi-lip.h"
#include "uefi/protocol/efi-sfsp.h"
#include "uefi/protocol/efi-stop.h"


//Checks error and exits if there is one
void BootCheckError(EFI_HANDLE* inImageHandle, EFI_SYSTEM_TABLE* inSystemTable, EFI_STATUS inStatus) {
	if(inStatus == EFI_SUCCESS) { return; }
	else { 
		inSystemTable->BootServices->Exit(inImageHandle, inStatus, 0, NULL); //we have no data to send
	}
	
}

//Main entry point for Ustka
EFI_STATUS EFIAPI EfiMain (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	UINTN Index = 0;
	UINTN Status = EFI_NOT_READY;
	EFI_STATUS FunctionStatus = EFI_SUCCESS;
	
	SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
    SystemTable->ConOut->OutputString(
		SystemTable->ConOut, 
		(CHAR16*)L"SimpleUEFI boot example\r\n"
		L"by Martin/MegapolisPlayer, 2023\r\n"
		L"Press any key to boot.\r\n"
		//\n moves down at same line, \r moves to first char
	);
	
	EFI_EVENT KeyEvent;
	EFI_INPUT_KEY InputKey;
	KeyEvent = SystemTable->ConIn->WaitForKey;
	SystemTable->BootServices->WaitForEvent(1, &KeyEvent, &Index);
	
	while(Status == EFI_NOT_READY) {
		Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &InputKey);
	}
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16*)L"Key recieved: ");
	SystemTable->ConOut->OutputString(SystemTable->ConOut, &InputKey.UnicodeChar);
	
	UINTN MapSize = 0, MapKey = 0, DescriptorSize = 0; UINT32 DescriptorVersion = 0;
    EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
	
	//first call, we get size required
	FunctionStatus = SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, NULL, &DescriptorSize, NULL);
	//no error check - will return EFI_BUFFER_TOO_SMALL because we are just querying the size
	MapSize += 2 * DescriptorSize; //make space for 2 more descriptors, basically current descriptors + 1 for alloc + 1 we need
	
	//allocate enough memory for memory map
	FunctionStatus = SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&MemoryMap);
	BootCheckError(ImageHandle, SystemTable, FunctionStatus);
	
	//second call, we get everything, no error checking because then the first would also fail (same func)
	FunctionStatus = SystemTable->BootServices->GetMemoryMap(&MapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
	BootCheckError(ImageHandle, SystemTable, FunctionStatus);
	
	SystemTable->BootServices->ExitBootServices(ImageHandle, 0);
	
	//runtime services
	
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_BACKGROUND_BLUE | EFI_WHITE);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16*)L"Activated runtime services.\r\n");
	
	while(1) {};
	
    return EFI_SUCCESS;
}