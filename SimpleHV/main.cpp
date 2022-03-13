#include "main.h"
#include "support.h"

extern "C" NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT pDriverObject, _In_ PUNICODE_STRING pRegistryPath)
{
	UNREFERENCED_PARAMETER(pRegistryPath);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObject = NULL;
	UNICODE_STRING usDriverName, usDosDeviceName;

	DPRINT("DriverEntry called.\n");

	RtlInitUnicodeString(&usDriverName, L"\\Device\\SimpleHV");
	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\SimpleHV");

	NtStatus = IoCreateDevice(
		pDriverObject,
		0,
		&usDriverName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&pDeviceObject
	);
	if (!NT_SUCCESS(NtStatus)) goto cleanup;

	IoCreateSymbolicLink(&usDosDeviceName, &usDriverName);
	if (!NT_SUCCESS(NtStatus)) goto cleanup;

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = DrvUnsupported;
	}

	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DrvUnsupported;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DrvUnsupported;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DrvUnsupported;
	pDriverObject->MajorFunction[IRP_MJ_READ] = DrvUnsupported;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DrvUnsupported;

	pDriverObject->DriverUnload = DrvUnload;

	pDeviceObject->Flags |= IO_TYPE_DEVICE;
	pDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

	return NtStatus;

cleanup:
	IoDeleteSymbolicLink(&usDosDeviceName);
	if (pDeviceObject) IoDeleteDevice(pDeviceObject);
	return NtStatus;
}

VOID
DrvUnload(_In_ PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING usDosDeviceName;
	DPRINT("DrvUnload called.\n");

	RtlInitUnicodeString(&usDosDeviceName, L"\\DosDevices\\SimpleHV");
	IoDeleteSymbolicLink(&usDosDeviceName);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS
DrvUnsupported(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	DPRINT("Unsupported function.\n");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = NULL;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}
