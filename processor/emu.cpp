/*
 *  Interactive disassembler (IDA).
 *  Q3 Processor Module Emulator Code.
 *
 */

#include "q3.hpp"
#include <frame.hpp>

//----------------------------------------------------------------------
// Analyze an instruction
static ea_t next_insn(insn_t *insn, ea_t ea)
{
	if (decode_insn(insn, ea) == 0)
		return 0;
	ea += insn->size;
	return ea;
}

//----------------------------------------------------------------------
// Create function stack frame
bool q3_t::create_func_frame(func_t *pfn) const
{
	ea_t ea = pfn->start_ea;
	insn_t insn;

	ea = next_insn(&insn, ea);
	
	if (!ea) return 0;
	if (insn.itype != Q3_ENTER) return 0;

	int lv_size = insn.Op1.value;

	//pfn->flags |= FUNC_FRAME;
	return add_frame(pfn, lv_size, 0, 0);
}

//----------------------------------------------------------------------
void q3_t::handle_jump(const insn_t &insn)
{
	segment_t* insn_pointers = get_segm_by_name("INSN_PTRS");
	segment_t* code = get_segm_by_name("CODE");
	
	int32_t operand;
	ea_t next_insn;

	bool is_cond_jump = has_insn_feature(insn.itype, CF_USE1);
	
	// Handle conditional jumps using their operand.
	if (is_cond_jump) {
		operand = insn.Op1.value;
		next_insn = (operand * 4) + code->start_ea;
	}
	else {	// Handle JUMP getting operand from previous instruction.
		operand = get_dword(insn.ea - 4);
		next_insn = (get_dword((operand * 4) + insn_pointers->start_ea) * 4) + code->start_ea;
	}

	insn.add_cref(next_insn, is_cond_jump ? insn.Op1.offb : 0, fl_JN);
}

//----------------------------------------------------------------------
void q3_t::handle_call(const insn_t &insn)
{
	// CALL calculates its destination using previous CONST operand.
	// Assuming before every CALL there is a CONST instruction untill it fails because prev_head
	// does not work at this point.

	segment_t* insn_pointers;
	segment_t* code;
	ea_t next_insn;
	int32_t operand = get_dword(insn.ea - 4);
	
	if (operand >= 0) {
		insn_pointers = get_segm_by_name("INSN_PTRS");
		code = get_segm_by_name("CODE");
		next_insn = (get_dword((operand * 4) + insn_pointers->start_ea) * 4) + code->start_ea;
		
		insn.add_cref(next_insn, 0, fl_CN);
		flow = func_does_return(next_insn);
	}
	else {
		// TODO MARK SYSCALL 
		// If syscall continue normal flow.
		add_cref(insn.ea, insn.ea + insn.size, fl_F);
	}
}


//----------------------------------------------------------------------
static bool add_stkpnt(const insn_t &insn, sval_t delta)
{
	func_t *pfn = get_func(insn.ea);
	if (pfn == NULL)
		return false;

	return add_auto_stkpnt(pfn, insn.ea + insn.size, delta);
}


//----------------------------------------------------------------------
static void trace_sp(const insn_t &insn)
{
	switch (insn.itype)
	{
	case Q3_ENTER:
		add_stkpnt(insn, -insn.Op1.value);
		break;
	}
}

//----------------------------------------------------------------------
int q3_t::q3_emu(const insn_t &insn)
{	
	uint32 feature = insn.get_canon_feature(ph);
	
	flow = (feature & CF_STOP) == 0;

	if (feature & CF_CALL) handle_call(insn);
	if (feature & CF_JUMP) handle_jump(insn);
	if (insn.itype == Q3_LOCAL & may_create_stkvars()) {
		insn.create_stkvar(insn.Op1, insn.Op1.value, STKVAR_VALID_SIZE);
		op_stkvar(insn.ea, insn.Op1.n);
	}
	if (flow)
		add_cref(insn.ea, insn.ea + insn.size, fl_F); // Basic control flow contruction.

	if (may_trace_sp())
	{
		if (flow)
			trace_sp(insn);
		else
			recalc_spd(insn.ea);
	}
	return 1;
}
