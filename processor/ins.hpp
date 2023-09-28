/*
 *  Interactive disassembler (IDA).
 *  Q3ASM
 *
 */

#ifndef __INSTRS_HPP
#define __INSTRS_HPP

extern const instruc_t Instructions[];


enum nameNum ENUM_SIZE(uint32)
{
	Q3_UNDEF = 0, /* Error: VM halt */
	Q3_IGNORE,
	Q3_BREAK,
	Q3_PUSH,
	Q3_ENTER,
	Q3_CALL,
	Q3_LEAVE,
	Q3_POP,
	Q3_LTI,
	Q3_LEI,
	Q3_GTI,
	Q3_GEI,
	Q3_LTU,
	Q3_LEU,
	Q3_GTU,
	Q3_GEU,
	Q3_CONST,
	Q3_LOCAL,
	Q3_JUMP,
	Q3_EQ,
	Q3_NE,
	Q3_STORE1,
	Q3_STORE2,
	Q3_STORE4,
	Q3_LOAD1,
	Q3_LOAD2,
	Q3_LOAD4,
	Q3_ARG,
	Q3_BLOCK_COPY,
	Q3_SEX8,
	Q3_SEX16,
	Q3_NEGI,
	Q3_ADD,
	Q3_SUB,
	Q3_DIVI,
	Q3_DIVU,
	Q3_MODI,
	Q3_MODU,
	Q3_MULI,
	Q3_MULU,
	Q3_BAND,
	Q3_BOR,
	Q3_BXOR,
	Q3_BCOM,
	Q3_LSH,
	Q3_RSHI,
	Q3_RSHU,
	Q3_EQF,
	Q3_NEF,
	Q3_LTF,
	Q3_LEF,
	Q3_GTF,
	Q3_GEF,
	Q3_MAX, /* Make this the last item */
};
#endif
