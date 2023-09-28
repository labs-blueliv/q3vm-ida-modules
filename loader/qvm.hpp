#include "../idaldr.h"

#define QVM_HEADER_MAGIC 0x12721444
#define RHADAVM_HEADER_MAGIC 0x14744214
#define MAX_FILE_FORMAT_NAME 256

enum opcodes ENUM_SIZE(uint32)
{
	Q3_UNDEF,
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

typedef struct
{
	int32_t vmMagic;          /**< Bytecode image shall start with VM_MAGIC */
	int32_t instructionCount; /**< Number of instructions in .qvm */
	int32_t codeOffset;       /**< Byte offset in .qvm file of .code segment */
	int32_t codeLength;       /**< Bytes in code segment */
	int32_t dataOffset;       /**< Byte offset in .qvm file of .data segment */
	int32_t dataLength;       /**< Bytes in .data segment */
	int32_t litLength; /**< Bytes in strings segment (after .data segment) */
	int32_t bssLength; /**< How many bytes should be used for .bss segment */
} qvmHeader_t;

int idaapi accept_qvm_file(qstring *, qstring *, linput_t *, const char *);
void idaapi load_qvm_file(linput_t *, ushort, const char *);