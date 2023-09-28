#include "q3.hpp"

int q3_t::q3_ana(insn_t *_insn)
{
	insn_t &insn = *_insn;
	uint32 code = insn.get_next_dword();

	insn.itype = code;
	if (code > Q3_MAX) return 0;

	if (Instructions[code].feature & CF_USE1) {
		insn.Op1.offb = 4; // Opperand offset
		insn.Op1.type = o_imm;
		insn.Op1.dtype = dt_dword;
		insn.Op1.value = insn.get_next_dword();
	}

	return insn.size;
}
