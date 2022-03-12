#include <ntddk.h>
#include <wdf.h>
#include <wdm.h>

extern void inline AssemblyFunc1(void);
extern void inline AssemblyFunc2(void);

VOID DrvUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);

//#pragma alloc_text(INIT, DriverEntry)
//#pragma alloc_text(PAGE, Example_Unload)

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	UNREFERENCED_PARAMETER(pRegistryPath);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	//UINT64 uiIndex = 0;
	PDEVICE_OBJECT pDeviceObject = NULL;
	UNICODE_STRING usDriverName, usDosDeviceName;

	DbgPrint("DriverEntry Called.");

	RtlInitUnicodeString(&usDriverName, L"\\Device\\SimpleHV");
	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\SimpleHV");

	NtStatus = IoCreateDevice(pDriverObject, 0, &usDriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);

	if (NtStatus == STATUS_SUCCESS)
	{
		pDriverObject->DriverUnload = DrvUnload;
		pDeviceObject->Flags |= IO_TYPE_DEVICE;
		pDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);
		IoCreateSymbolicLink(&usDosDeviceName, &usDriverName);
	}
	return NtStatus;
}

VOID DrvUnload(PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING usDosDeviceName;
	DbgPrint("DrvUnload Called rn");
	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\MyHypervisor");
	IoDeleteSymbolicLink(&usDosDeviceName);
	IoDeleteDevice(DriverObject->DeviceObject);
}