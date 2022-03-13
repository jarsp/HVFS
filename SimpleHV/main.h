#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <wdm.h>

#define DPRINT(fmt, ...) DbgPrint("[HVFS] " fmt, __VA_ARGS__)

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject, _In_ PUNICODE_STRING pRegistryPath);

VOID DrvUnload(_In_ PDRIVER_OBJECT DriverObject);
NTSTATUS DrvUnsupported(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
