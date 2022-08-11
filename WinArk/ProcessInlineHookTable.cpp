#include "stdafx.h"
#include "ProcessInlineHookTable.h"
#include "DriverHelper.h"
#include "SymbolManager.h"
#include "Helpers.h"

#ifdef _WIN64
#pragma comment(lib,"x64/capstone.lib")
#else
#pragma comment(lib,"x86/capstone.lib")
#endif // _WIN64



CProcessInlineHookTable::CProcessInlineHookTable(BarInfo& bars, TableInfo& table,DWORD pid)
	:CTable(bars, table), m_Pid(pid),m_ModuleTracker(pid) {
	SetTableWindowInfo(bars.nbar);
}

CString CProcessInlineHookTable::TypeToString(HookType type) {
	switch (type)
	{
		case HookType::x64HookType1:
			return L"x64HookType1";
		case HookType::x64HookType2:
			return L"x64HookType2";
		case HookType::x64HookType3:
			return L"x64HookType3";
		case HookType::x64HookType4:
			return L"x64HookType4";
		default:
			return L"Unknown Type";
	}
}

int CProcessInlineHookTable::ParseTableEntry(CString& s, char& mask, int& select, InlineHookInfo& info, int column){
	switch (static_cast<Column>(column))
	{
		case Column::HookObject :
			s = info.Name.c_str();
			break;

		case Column::HookType:
			s = TypeToString(info.Type);
			break;

		case Column::Address:
		{
			auto& symbols = SymbolManager::Get();
			DWORD64 offset = 0;
			auto symbol = symbols.GetSymbolFromAddress(m_Pid, info.Address, &offset);
			CStringA text;
			if (symbol) {
				auto sym = symbol->GetSymbolInfo();
				text.Format("%s!%s+0x%X", symbol->ModuleInfo.ModuleName, sym->Name, (DWORD)offset);
				std::string details = text.GetString();
				std::wstring wdetails = Helpers::StringToWstring(details);
				s.Format(L"0x%p (%s)", info.Address, wdetails.c_str());
			}
			else
				s.Format(L"0x%p", info.Address);
			break;
		}

		case Column::Module:
			s = info.TargetModule.c_str();
			break;

		case Column::TargetAddress:
			s.Format(L"0x%p", info.TargetAddress);
			break;

		default:
			break;
	}
	return s.GetLength();
}

bool CProcessInlineHookTable::CompareItems(const InlineHookInfo& s1, const InlineHookInfo& s2, int col, bool asc){
	return false;
}

LRESULT CProcessInlineHookTable::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	m_hProcess = DriverHelper::OpenProcess(m_Pid, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
	if (m_hProcess == nullptr)
		return -1;
	m_VMTracker.reset(new WinSys::ProcessVMTracker(m_hProcess));
	if (m_VMTracker == nullptr)
		return -1;
	if (cs_open(CS_ARCH_X86, CS_MODE_64, &_x64handle) != CS_ERR_OK)
		return -1;
	cs_option(_x64handle, CS_OPT_DETAIL, CS_OPT_ON);
	cs_option(_x64handle, CS_OPT_UNSIGNED, CS_OPT_ON);
	cs_option(_x64handle, CS_OPT_SKIPDATA, CS_OPT_ON);

	Refresh();

	return 0;
}

LRESULT CProcessInlineHookTable::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lparam, BOOL&){
	if (m_hProcess != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hProcess);
	return 0;
}

LRESULT CProcessInlineHookTable::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	PaintTable(m_hWnd);
	return 0;
}

LRESULT CProcessInlineHookTable::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	Tablefunction(m_hWnd, uMsg, wParam, lParam);
	return 0;
}

LRESULT CProcessInlineHookTable::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	Tablefunction(m_hWnd, uMsg, wParam, lParam);
	return 0;
}

LRESULT CProcessInlineHookTable::OnUserVabs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	Tablefunction(m_hWnd, uMsg, wParam, lParam);
	return 0;
}

LRESULT CProcessInlineHookTable::OnUserVrel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	Tablefunction(m_hWnd, uMsg, wParam, lParam);
	return 0;
}

LRESULT CProcessInlineHookTable::OnUserChgs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	Tablefunction(m_hWnd, uMsg, wParam, lParam);
	return 0;
}

LRESULT CProcessInlineHookTable::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	return Tablefunction(m_hWnd, WM_VSCROLL, zDelta >= 0 ? 0 : 1, wParam);
}

LRESULT CProcessInlineHookTable::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CProcessInlineHookTable::OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CProcessInlineHookTable::OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CProcessInlineHookTable::OnRBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CProcessInlineHookTable::OnUserSts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CProcessInlineHookTable::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CProcessInlineHookTable::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CProcessInlineHookTable::OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&){
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

const std::vector<std::shared_ptr<WinSys::MemoryRegionItem>>& CProcessInlineHookTable::GetItems() {
	std::vector<std::shared_ptr<WinSys::MemoryRegionItem>> regions = m_VMTracker->GetRegions();
	DWORD value;
	for (auto region : regions) {
		value = region->Protect & 0xFF;
		if (value & PAGE_EXECUTE || value & PAGE_EXECUTE_READ || 
			value & PAGE_EXECUTE_WRITECOPY || value & PAGE_EXECUTE_READWRITE) {
			m_Items.push_back(region);
		}
	}
	return m_Items;
}

void CProcessInlineHookTable::CheckInlineHook(uint8_t* code, size_t codeSize, uint64_t address, ULONG_PTR moduleBase, SIZE_T moduleSize) {
	// �����ʱ��ϳ�������£��ῨUI
	size_t count;
	cs_insn* insn;

	count = cs_disasm(_x64handle, code, codeSize, address, 0, &insn);
	if (count > 0) {
		for (size_t j = 0; j < count; j++) {
			CheckX64HookType1(insn, j, count, moduleBase, moduleSize, address, codeSize);
			CheckX64HookType2(insn, j, count);
			CheckX64HookType4(insn, j, count, moduleBase, moduleSize, address, codeSize);
		}
		cs_free(insn, count);
	}
}

void CProcessInlineHookTable::Refresh() {
	m_VMTracker->EnumRegions();

	m_Items = GetItems();
	m_ModuleTracker.EnumModules();
	m_Modules = m_ModuleTracker.GetModules();

	m_Sys64Modules.clear();
	m_Table.data.info.clear();

	for (const auto& m : m_Modules) {
		if (m->Path.find(L"System32") != std::wstring::npos) {
			m_Sys64Modules.push_back(m);
		}
	}

	uint8_t* code = nullptr;
	SIZE_T size = 0;
	ULONG_PTR moduleBase;
	SIZE_T moduleSize;
	ULONG_PTR address;
	for (const auto& block : m_Items) {
		code = static_cast<uint8_t*>(malloc(block->RegionSize));
		if (code != nullptr) {
			memset(code, 0, block->RegionSize);
			bool success = ::ReadProcessMemory(m_hProcess, block->BaseAddress, code, block->RegionSize, &size);
			if (!success) {
				free(code);
				continue;
			}
			address = (ULONG_PTR)block->BaseAddress;
			auto m = GetModuleByAddress(address);
			if (m != nullptr) {
				moduleSize = m->ModuleSize;
				moduleBase = (ULONG_PTR)m->Base;
			}
			else {
				moduleBase = 0;
				moduleSize = 0;
			}
			CheckInlineHook(code, size, address, moduleBase, moduleSize);
			free(code);
			code = nullptr;
		}
	}
	m_Table.data.n = m_Table.data.info.size();
}

std::shared_ptr<WinSys::ModuleInfo> CProcessInlineHookTable::GetModuleByAddress(ULONG_PTR address) {
	for (auto m : m_Modules) {
		ULONG_PTR base = (ULONG_PTR)m->Base;
		if (address >= base && address < base + m->ModuleSize)
			return m;
	}
	return nullptr;
}

void CProcessInlineHookTable::CheckX64HookType1(cs_insn* insn, size_t j, size_t count, 
	ULONG_PTR moduleBase, size_t moduleSize,
	ULONG_PTR base, size_t size) {
	cs_detail* d1, *d2;

	d1 = insn[j].detail;
	if (d1 == nullptr)
		return;

	if (j + 1 >= count)
		return;
	d2 = insn[j + 1].detail;
	if (d2 == nullptr)
		return;

	if (d1->x86.op_count != 2)
		return;

	cs_x86_op* op1, * op2;
	op1 = &(d1->x86.operands[0]);
	op2 = &(d2->x86.operands[1]);

	if (op1->type != x86_op_type::X86_OP_REG)
		return;

	if (op1->access != CS_AC_WRITE)
		return;

	if (op2->type != X86_OP_IMM)
		return;

	if (op1->size != 8)
		return;

	if (op2->size != 8)
		return;

	if (strcmp(insn[j + 1].mnemonic, "jmp"))
		return;

	if (d2->x86.op_count != 1)
		return;

	if (d2->x86.operands[0].type != X86_OP_REG)
		return;

	if (d2->x86.operands[0].access != CS_AC_READ)
		return;

	ULONG_PTR targetAddress = d1->x86.operands[1].imm;

	if (targetAddress >= moduleBase && targetAddress <= moduleBase + moduleSize)
		return;

	InlineHookInfo info;
	info.TargetAddress = targetAddress;
	info.TargetModule = L"Unknown";
	auto m = GetModuleByAddress(targetAddress);
	if (m != nullptr) {
		info.TargetModule = m->Path;
	}
	info.Type = HookType::x64HookType1;
	info.Address = insn[j].address;
	m = GetModuleByAddress(info.Address);
	info.Name = L"Unknown";
	if(m!=nullptr)
		info.Name = m->Name;
	m_Table.data.info.push_back(info);
}


void CProcessInlineHookTable::CheckX64HookType2(cs_insn* insn, size_t j, size_t count) {
	cs_detail* d1, * d2;
	d1 = insn[j].detail;
	if (j + 1 >= count) {
		return;
	}
	d2 = insn[j + 1].detail;
	if (d2 == nullptr)
		return;

	if (strcmp(insn[j].mnemonic, "push")) {
		return;
	}
	if (j + 2 >= count) {
		return;
	}

	if (strcmp(insn[j + 2].mnemonic, "ret")) {
		return;
	}

	if (d1->regs_read_count != 1)
		return;

	if (strcmp("rsp", cs_reg_name(_x64handle, d1->regs_read[0])))
		return;

	if (d1->x86.op_count != 1 || d1->x86.operands[0].size != 8)
		return;

	if (d2->x86.op_count != 2)
		return;

	if (d2->x86.operands[0].access != CS_AC_WRITE)
		return;


	if (d2->x86.operands[1].type != CS_OP_IMM)
		return;

	if (d2->x86.operands[1].size != 4 || d2->x86.operands[0].size != 4)
		return;

	if (d2->x86.operands[0].mem.base != X86_REG_RSP)
		return;

	InlineHookInfo info;
	ULONG lowAddr = d1->x86.operands[0].imm;
	ULONG highAddr = d2->x86.operands[1].imm;
	info.TargetAddress = ((ULONG_PTR)highAddr << 32) | lowAddr;
	info.TargetModule = L"Unknown";
	auto m = GetModuleByAddress(info.TargetAddress);
	if (m != nullptr) {
		info.TargetModule = m->Path;
	}
	info.Type = HookType::x64HookType2;
	info.Address = insn[j].address;
	m = GetModuleByAddress(info.Address);
	info.Name = L"Unknown";
	if (m != nullptr)
		info.Name = m->Name;
	m_Table.data.info.push_back(info);
}

void CProcessInlineHookTable::CheckX64HookType4(cs_insn* insn, size_t j, size_t count, ULONG_PTR moduleBase, size_t moduleSize,
	ULONG_PTR base, size_t size) {
	cs_detail* d1, * d2;
	d1 = insn[j].detail;
	if (d1 == nullptr)
		return;
	if (j + 1 >= count)
		return;

	if (d1->x86.opcode[0] == 0xEB)
		return;
	if (d1->x86.op_count != 1)
		return;
	if (d1->x86.operands[0].type != CS_OP_IMM)
		return;
	if (d1->x86.operands[0].size != 8)
		return;

	if (strcmp(insn[j].mnemonic, "jmp"))
		return;

	bool flag = true;
	ULONG_PTR targetAddress = d1->x86.operands[0].imm;
	ULONG_PTR address = base + size;
	if (targetAddress >= base && targetAddress <= base + size) {
		// �ų�ѭ����ת
		return;
	}

	// ���費���κδ����
	flag = true;
	for (const auto& info : m_Items) {
		if (targetAddress >= (ULONG_PTR)info->BaseAddress && targetAddress <= (ULONG_PTR)info->BaseAddress + info->RegionSize) {
			flag = false;
		}
	}
	if (flag)
		return;
	
	flag = false;
	for (const auto& m : m_Sys64Modules) {
		// �ų�����API
		if (targetAddress >= (ULONG_PTR)m->Base && targetAddress <= (ULONG_PTR)m->Base + m->ModuleSize) {
			flag = true;
		}
	}
	if (flag) {
		return;
	}

	// �ų��Ƿ���ַ
	uint8_t code[7];
	SIZE_T dummy;
	bool success = ::ReadProcessMemory(m_hProcess, (LPVOID)targetAddress, code, 6, &dummy);
	if (!success)
		return;

	cs_insn* jmpCode;
	count = cs_disasm(_x64handle, code, sizeof(code) - 1, targetAddress, 0, &jmpCode);
	if (0==count) {
		return;
	}
	ULONG_PTR codeAddress;
	cs_detail* d = jmpCode[0].detail;
	if (d != nullptr) {
		ULONG_PTR memAddress = targetAddress + jmpCode[0].size + d->x86.operands[0].mem.disp;
		::ReadProcessMemory(m_hProcess, (LPVOID)memAddress, &codeAddress, sizeof(codeAddress), &dummy);
	}
	
	success = ::ReadProcessMemory(m_hProcess, (LPVOID)codeAddress, &dummy, sizeof(dummy), &dummy);
	InlineHookInfo info;
	if (success) {
		info.TargetAddress = codeAddress;
	}
	else {
		info.TargetAddress = targetAddress;
	}
	info.TargetModule = L"Unknown";
	auto m = GetModuleByAddress(info.TargetAddress);
	if (m != nullptr) {
		info.TargetModule = m->Path;
	}
	info.Type = HookType::x64HookType4;
	info.Address = insn[j].address;
	m = GetModuleByAddress(info.Address);
	info.Name = L"Unknown";
	if (m != nullptr)
		info.Name = m->Name;
	m_Table.data.info.push_back(info);
}