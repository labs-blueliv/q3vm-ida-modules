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
	Q3_IGNORE, /* No operation */
	Q3_BREAK, /* vm->breakCount++ */
	Q3_ENTER, /* Begin subroutine. */
	Q3_LEAVE, /* End subroutine. */
	Q3_CALL,  /* Call subroutine. */
	Q3_PUSH,  /* Push to stack. */
	Q3_POP,   /* Discard top-of-stack. */
	Q3_CONST, /* Load constant to stack. */
	Q3_LOCAL, /* Get local variable. */
	Q3_JUMP, /* Unconditional jump. */
	Q3_EQ, /* Compare integers, jump if equal. */
	Q3_NE, /* Compare integers, jump if not equal. */
	Q3_LTI, /* Compare integers, jump if less-than. */
	Q3_LEI, /* Compare integers, jump if less-than-or-equal. */
	Q3_GTI, /* Compare integers, jump if greater-than. */
	Q3_GEI, /* Compare integers, jump if greater-than-or-equal. */
	Q3_LTU, /* Compare unsigned integers, jump if less-than */
	Q3_LEU, /* Compare unsigned integers, jump if less-than-or-equal */
	Q3_GTU, /* Compare unsigned integers, jump if greater-than */
	Q3_GEU, /* Compare unsigned integers, jump if greater-than-or-equal */
	Q3_EQF, /* Compare floats, jump if equal */
	Q3_NEF, /* Compare floats, jump if not-equal */
	Q3_LTF, /* Compare floats, jump if less-than */
	Q3_LEF, /* Compare floats, jump if less-than-or-equal */
	Q3_GTF, /* Compare floats, jump if greater-than */
	Q3_GEF, /* Compare floats, jump if greater-than-or-equal */
	Q3_LOAD1,  /* Load 1-byte from memory */
	Q3_LOAD2,  /* Load 2-bytes from memory */
	Q3_LOAD4,  /* Load 4-bytes from memory */
	Q3_STORE1, /* Store 1-byte to memory */
	Q3_STORE2, /* Store 2-byte to memory */
	Q3_STORE4, /* *(stack[top-1]) = stack[top] */
	Q3_ARG,    /* Marshal argument */
	Q3_BLOCK_COPY, /* memcpy */
	Q3_SEX8,  /* Sign-Extend 8-bit */
	Q3_SEX16, /* Sign-Extend 16-bit */
	Q3_NEGI, /* Negate integer. */
	Q3_ADD,  /* Add integers (two's complement). */
	Q3_SUB,  /* Subtract integers (two's complement). */
	Q3_DIVI, /* Divide signed integers. */
	Q3_DIVU, /* Divide unsigned integers. */
	Q3_MODI, /* Modulus (signed). */
	Q3_MODU, /* Modulus (unsigned). */
	Q3_MULI, /* Multiply signed integers. */
	Q3_MULU, /* Multiply unsigned integers. */
	Q3_BAND, /* Bitwise AND */
	Q3_BOR,  /* Bitwise OR */
	Q3_BXOR, /* Bitwise eXclusive-OR */
	Q3_BCOM, /* Bitwise COMplement */
	Q3_LSH,  /* Left-shift */
	Q3_RSHI, /* Right-shift (algebraic; preserve sign) */
	Q3_RSHU, /* Right-shift (bitwise; ignore sign) */
	Q3_NEGF, /* Negate float */
	Q3_ADDF, /* Add floats */
	Q3_SUBF, /* Subtract floats */
	Q3_DIVF, /* Divide floats */
	Q3_MULF, /* Multiply floats */
	Q3_CVIF, /* Convert to integer from float */
	Q3_CVFI, /* Convert to float from integer */
	Q3_MAX, /* Make this the last item */
};

#endif
