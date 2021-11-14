#pragma once
#include "SysMon.h"

extern "C"
NTSTATUS ObCreateObject(
	_In_opt_ KPROCESSOR_MODE ObjectAttributesAccessMode,
	_In_ POBJECT_TYPE Type,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ KPROCESSOR_MODE AccessMode,
	_Inout_opt_ PVOID ParseContext,
	_In_ ULONG ObjectSize,
	_In_opt_ ULONG PagedPoolCharge,
	_In_opt_ ULONG NonPagedPoolCharge,
	_Out_ PVOID * Object
);

extern "C"
PIMAGE_NT_HEADERS RtlImageNtHeader(
	_In_ PVOID BaseAddress
);

extern "C"
NTSTATUS
PsReferenceProcessFilePointer(
	_In_ PEPROCESS Process,
	_Out_ PFILE_OBJECT* FileObject
);

extern "C"
NTSTATUS ZwFlushInstructionCache(
);

extern "C"
NTSTATUS ObDuplicateObject(
	_In_ PEPROCESS SourceProcess,
	_In_ HANDLE SourceHandle,
	_In_opt_ PEPROCESS TargetProcess,
	_Out_opt_ PHANDLE TargetHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ ULONG HandleAttributes,
	_In_ ULONG Options,
	_In_ KPROCESSOR_MODE PreviousMode
);

PEPROCESS PsGetCurrentProcessByThread(PETHREAD Thread);

// �������Զ���
NTSTATUS NtCreateDebugObject(
	_Out_ PHANDLE DebugObjectHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ ULONG Flags
);

// ��һ���Ѿ����еĽ��̽������ԻỰ
NTSTATUS
NtDebugActiveProcess(
	_In_ HANDLE ProcessHandle,
	_In_ HANDLE DebugObjectHandle
);



// ��һ�����Զ��󸽼ӵ������Խ�����
NTSTATUS
DbgkpSetProcessDebugObject(
	_In_ PEPROCESS Process,
	_In_ PDEBUG_OBJECT DebugObject,
	_In_ NTSTATUS MsgStatus,
	_In_ PETHREAD LastThread
);

/**
* Emulation System
* 
*/
// �������ϵͳ������ٵĽ��̴�����Ϣ
NTSTATUS DbgkpPostFakeProcessCreateMessages(
	_In_ PEPROCESS Process,
	_In_ PDEBUG_OBJECT DebugObject,
	_In_ PETHREAD* pLastThread
);

// �������ϵͳ������ٵ��̴߳�����Ϣ
NTSTATUS DbgkpPostFakeThreadMessages(
	PEPROCESS	Process,
	PETHREAD	StartThread,
	PDEBUG_OBJECT	DebugObject,
	PETHREAD* pFirstThread,
	PETHREAD* pLastThread
);

// �������ϵͳ������ٵ�ģ�������Ϣ
NTSTATUS DbgkpPostModuleMessages(
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread,
	_In_ PDEBUG_OBJECT DebugObject
);

PVOID ObFastReferenceObjectLocked(
	_In_ PEX_FAST_REF FastRef
);

PVOID ObFastDereferenceObject(
	_In_ PEX_FAST_REF FastRef,
	_In_ PVOID Object
);

/**
* Event Collection Routine
*
*/
// �ɼ��̴߳����¼�
VOID DbgkCreateThread(
	PETHREAD Thread
);

// �߳��˳���Ϣ
VOID DbgkExitThread(
	NTSTATUS ExitStatus
);

// �����˳���Ϣ
VOID DbgkExitProcess(
	NTSTATUS ExitStatus
);

// ģ�����
VOID DbgkMapViewOfSection(
	_In_ PVOID SectionObject,
	_In_ PVOID BaseAddress,
	_In_ PEPROCESS Process
);

// ģ��ж��
VOID DbgkUnMapViewOfSection(
	_In_ PEPROCESS Process,
	_In_ PVOID BaseAddress
);

// ��һ�����Զ������Ϣ������׷�ӵ����¼�
NTSTATUS DbgkpQueueMessage(
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread,
	_Inout_ PDBGKM_APIMSG ApiMsg,
	_In_ ULONG Flags,
	_In_ PDEBUG_OBJECT TargetDebugObject
);

/// <summary>
/// �ȴ������¼�
/// </summary>
/// <param name="DebugObjectHandle"></param>
/// <param name="Alertable"></param>
/// <param name="Timeout"></param>
/// <param name="WaitStateChange"></param>
/// <returns></returns>
NTSTATUS NtWaitForDebugEvent(
	_In_ HANDLE DebugObjectHandle,
	_In_ BOOLEAN Alertable,
	_In_opt_ PLARGE_INTEGER Timeout,
	_Out_ PDBGUI_WAIT_STATE_CHANGE WaitStateChange
);

/// <summary>
/// �ظ������¼����ָ������Խ���
/// </summary>
/// <param name="DebugObjectHandle"></param>
/// <param name="ClientId"></param>
/// <param name="ContinueStatus"></param>
/// <returns></returns>
NTSTATUS NtDebugContinue(
	_In_ HANDLE DebugObjectHandle,
	_In_ PCLIENT_ID ClientId,
	_In_ NTSTATUS ContinueStatus
);

// ������ԻỰ
NTSTATUS NtRemoveProcessDebug(
	_In_ HANDLE ProcessHandle,
	_In_ HANDLE DebugObjectHandle
);

// ���͵����¼�
NTSTATUS DbgkpSendApiMessage(
	PDBGKM_APIMSG ApiMsg,
	ULONG	Flag
);

PSYSTEM_DLL_INFO PsQuerySystemDllInfo(
	ULONG index
);

PVOID ObFastReferenceObject(
	_In_ PEX_FAST_REF FastRef
);

LOGICAL ExFastRefDereference(
	_Inout_ PEX_FAST_REF FastRef,
	_In_ PVOID Object
);

EX_FAST_REF
ExFastReference(
	_Inout_ PEX_FAST_REF FastRef
);

LOGICAL
ExFastRefAddAdditionalReferenceCounts(
	_Inout_ PEX_FAST_REF FastRef,
	_In_ PVOID Object,
	_In_ ULONG RefsToAdd
);

PETHREAD PsGetNextProcessThread(
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread
);

// ȡ��Section��Ӧ���ļ����
HANDLE DbgkpSectionToFileHandle(
	_In_ PVOID SectionObject
);

NTSTATUS PsSuspendThread(
	_In_ PETHREAD Thread,
	_Out_opt_ PULONG PreviousSuspendCount
);

NTSTATUS KeResumeThread(
	_Inout_ PETHREAD Thread
);

VOID DbgkSendSystemDllMessages(
	PETHREAD Thread,
	PDEBUG_OBJECT DebugObject,
	PDBGKM_APIMSG ApiMsg
);



BOOLEAN DbgkpSuppressDbgMsg(
	_In_ PTEB Teb
);

PVOID PsCaptureExceptionPort(
	_In_ PEPROCESS Process
);

// ��ǰ���̵��쳣�˿ڷ����쳣�ĵڶ��ִ�������
NTSTATUS DbgkpSendApiMessageLpc(
	_Inout_ PDBGKM_APIMSG ApiMsg,
	_In_ PVOID Port,
	_In_ BOOLEAN SuspendProcess
);

NTSTATUS DbgkpSendErrorMessage(
	_In_ PEXCEPTION_RECORD ExceptionRecord,
	_In_ PDBGKM_APIMSG DbgApiMsg
);

// ���𱻵��Խ���
BOOLEAN
DbgkpSuspendProcess(
);

// �ָ������Խ��̵�ִ��
VOID DbgkpResumeProcess(
	_In_ PEPROCESS Process
);

// �������ϵͳ�����쳣��Ϣ
BOOLEAN DbgkForwardException(
	_In_ PEXCEPTION_RECORD ExceptionRecord,
	_In_ BOOLEAN DebugException,
	_In_ BOOLEAN SecondChance
);

// ���ѵȴ��������ظ����߳�
VOID DbgkpWakeTarget(
	_In_ PDEBUG_EVENT DebugEvent
);

// �ͷŵ����¼�
VOID DbgkpFreeDebugEvent(
	_In_ PDEBUG_EVENT DebugEvent
);

// ���ý��̻������BeingDebugged
VOID DbgkpMarkProcessPeb(
	_In_ PEPROCESS Process
);

// ���ں�ģʽ�Ľṹ��ת��Ϊ�û�ģʽ�Ľṹ��
VOID DbgkpConvertKernelToUserStateChange(
	PDBGUI_WAIT_STATE_CHANGE WaitStateChange,
	PDEBUG_EVENT DebugEvent
);

NTSTATUS MmGetFileNameForAddress(
	_In_ PVOID ProcessVa,
	_Out_ PUNICODE_STRING FileName
);

NTSTATUS DbgkClearProcessDebugObject(
	_In_ PEPROCESS Process,
	_In_ PDEBUG_OBJECT SourceDebugObject
);

/// <summary>
/// ���õ��Զ��������
/// </summary>
/// <param name="DebugObjectHandle"></param>
/// <param name="DebugObjectInformationClass"></param>
/// <param name="DebugInformation"></param>
/// <param name="DebugInformationLength"></param>
/// <param name="ReturnLength"></param>
/// <returns></returns>
NTSTATUS NtSetInformationDebugObject(
	_In_ HANDLE DebugObjectHandle,
	_In_ DEBUG_OBJECT_INFORMATION_CLASS DebugObjectInformationClass,
	_In_ PVOID DebugInformation,
	_In_ ULONG DebugInformationLength,
	_Out_opt_ PULONG ReturnLength
);

// ��ѯ������Ϣ����Ĺ��˼���
NTSTATUS NtQueryDebugFilterState(
	_In_ ULONG ComponentId,
	_In_ ULONG Level
);

// ���õ�����Ϣ����Ĺ��˼���
NTSTATUS NtSetDebugFilterState(
	_In_ ULONG ComponentId,
	_In_ ULONG Level,
	_In_ BOOLEAN State
);

// ����ָ�������еĵ��Զ���
NTSTATUS DbgkOpenProcessObject(
	_In_ PEPROCESS Process,
	_In_ PACCESS_STATE AccessState,
	_In_ ACCESS_MASK DesiredAccess
);

// �򿪽��̣��̶߳����������ü���
VOID DbgkpOpenHandles(
	_In_ PDBGUI_WAIT_STATE_CHANGE WaitStateChange,
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread
);

// �رյ��Զ���
VOID DbgkpCloseObject(
	_In_ PEPROCESS Process,
	_In_ PVOID Object,
	_In_ ACCESS_MASK GrantedAccess,
	_In_ ULONG_PTR ProcessHandleCount,
	_In_ ULONG_PTR SystemHandleCount
);

// �������Զ���
NTSTATUS DbgkCopyProcessDebugPort(
	_In_ PEPROCESS TargetProcess,
	_In_ PEPROCESS SourceProcess,
	_In_ PDEBUG_OBJECT DebugObject,
	_Out_ PBOOLEAN bFlag
);

NTSTATUS PsTerminateProcess(
	PEPROCESS Process,
	NTSTATUS Status
);

PEPROCESS PsGetNextProcess(
	_In_ PEPROCESS Process
);