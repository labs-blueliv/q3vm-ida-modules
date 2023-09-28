#include <ida.hpp>
#include <idp.hpp>
#include "ins.hpp"

const instruc_t Instructions[] =
{
	{"UNDEF",       0},
	{"IGNORE",      0},
	{"BREAK",       0},
	{"ENTER",       CF_USE1},
	{"LEAVE",       CF_STOP|CF_USE1},
	{"CALL",        CF_CALL},
	{"PUSH",        0},
	{"POP",         0},
	{"CONST",       CF_USE1},
	{"LOCAL",       CF_USE1},
	{"JUMP",        CF_STOP|CF_JUMP},
	{"EQ",          CF_USE1|CF_JUMP},
	{"NE",          CF_USE1|CF_JUMP},
	{"LTI",         CF_USE1|CF_JUMP},
	{"LEI",         CF_USE1|CF_JUMP},
	{"GTI",         CF_USE1|CF_JUMP},
	{"GEI",         CF_USE1|CF_JUMP},
	{"LTU",         CF_USE1|CF_JUMP},
	{"LEU",         CF_USE1|CF_JUMP},
	{"GTU",         CF_USE1|CF_JUMP},
	{"GEU",         CF_USE1|CF_JUMP},
	{"EQF",         CF_USE1|CF_JUMP},
	{"NEF",         CF_USE1|CF_JUMP},
	{"LTF",         CF_USE1|CF_JUMP},
	{"LEF",         CF_USE1|CF_JUMP},
	{"GTF",         CF_USE1|CF_JUMP},
	{"GEF",         CF_USE1|CF_JUMP},
	{"LOAD1",       0},
	{"LOAD2",       0},
	{"LOAD4",       0},
	{"STORE1",      0},
	{"STORE2",      0},
	{"STORE4",      0},
	{"ARG",         CF_USE1},
	{"BLOCK_COPY",	CF_USE1},
	{"SEX8",        0},
	{"SEX16",       0},
	{"NEGI",        0},
	{"ADD",         0},
	{"SUB",         0},
	{"DIVI",        0},
	{"DIVU",        0},
	{"MODI",        0},
	{"MODU",        0},
	{"MULI",        0},
	{"MULU",        0},
	{"BAND",        0},
	{"BOR",         0},
	{"BXOR",        0},
	{"BCOM",        0},
	{"LSH",         0},
	{"RSHI",        0},
	{"RSHU",        0},
	{"NEGF",        0},
	{"ADDF",        0},
	{"SUBF",        0},
	{"DIVF",        0},
	{"MULF",        0},
	{"CVIF",        0},
	{"CVFI",        0},
};

CASSERT(qnumber(Instructions) == Q3_MAX);