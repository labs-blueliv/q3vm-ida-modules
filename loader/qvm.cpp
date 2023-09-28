#include "qvm.hpp"

int idaapi accept_qvm_file(qstring *fileformatname, qstring *processor, linput_t *li, const char *filename) {
	uint32 magic;

	if (lread4bytes(li, &magic, false)) return 0;
	if (magic != QVM_HEADER_MAGIC & magic != RHADAVM_HEADER_MAGIC) return 0;
	
	*fileformatname = "Quake 3 VM binary.";
	*processor = "Q3";

	return 1;
}

int read_le_int(uint8_t bytes[4])
{
	return (bytes[0] << 0 | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24);
}

int qvm_expand_code(int *code_base, int *insn_ptrs, int insn_count, int code_length)
{
	/*
		Replicates .qvm bytecode DWORD expansion.
	*/

	int int_pc;
	int byte_pc;
	int instruction;
	uint8_t* code; 
	int op;
	
	// Init unexpanded code.
	code = (uint8_t*)malloc(code_length);
	memcpy(code, code_base, code_length);
	
	// Operand expansion loop.
	int_pc = byte_pc = instruction = 0;
	while (instruction < insn_count)
	{
		insn_ptrs[instruction] = int_pc; // INSN_PTRS is an artificial segment used to translate jump insntruction arguments.
		instruction++;

		op = (int)code[byte_pc]; // Expand byte to DWORD sized opcode.
		code_base[int_pc] = op;

		if (byte_pc > code_length)
		{
			free(code);
			return -1;
		}

		byte_pc++;
		int_pc++;
		
		// Expand params of opcodes that have.
		switch (op)
		{
		case Q3_ENTER:
		case Q3_CONST:
		case Q3_LOCAL:
		case Q3_LEAVE:
		case Q3_EQ:
		case Q3_NE:
		case Q3_LTI:
		case Q3_LEI:
		case Q3_GTI:
		case Q3_GEI:
		case Q3_LTU:
		case Q3_LEU:
		case Q3_GTU:
		case Q3_GEU:
		case Q3_EQF:
		case Q3_NEF:
		case Q3_LTF:
		case Q3_LEF:
		case Q3_GTF:
		case Q3_GEF:
		case Q3_BLOCK_COPY:
			code_base[int_pc] = read_le_int(&code[byte_pc]);
			byte_pc += 4;
			int_pc++;
			break;
		case Q3_ARG:
			code_base[int_pc] = (int)code[byte_pc];
			byte_pc++;
			int_pc++;
			break;
		}
	}

	int_pc = 0;
	instruction = 0;

	while (instruction < insn_count)
	{
		op = code_base[int_pc];
		instruction++;
		int_pc++;

		switch (op) {
		// Ops with a destination operand that needs to be translated.
		case Q3_EQ:
		case Q3_NE:
		case Q3_LTI:
		case Q3_LEI:
		case Q3_GTI:
		case Q3_GEI:
		case Q3_LTU:
		case Q3_LEU:
		case Q3_GTU:
		case Q3_GEU:
		case Q3_EQF:
		case Q3_NEF:
		case Q3_LTF:
		case Q3_LEF:
		case Q3_GTF:
		case Q3_GEF:
			code_base[int_pc] = insn_ptrs[code_base[int_pc]];
			int_pc++;
			break;
		
		// Instructions with operand that doesn't need translation.
		case Q3_ENTER:
		case Q3_CONST:
		case Q3_LOCAL:
		case Q3_LEAVE:
		case Q3_BLOCK_COPY:
		case Q3_ARG:
			int_pc++;
			break;
		}
	}

	free(code);
	return instruction;
}

void idaapi load_qvm_file(linput_t *li, ushort neflags, const char *) {
	qvmHeader_t header;
	int32_t data_size;
	int i;
	void* instruction_pointers;
	void* code;
	void* data;
	int expanded_code_size;
	int code_end;
	int data_segm_start;
	int insn_ptrs_segm_start;
	int insn_ptrs_segm_end;

	set_processor_type("Q3", SETPROC_LOADER);

	// Read header.
	lread(li, &header, sizeof(qvmHeader_t));

	// CODE
	// Alloc memory to expand code.
	expanded_code_size = header.codeLength * 4;
	code_end = header.codeOffset + expanded_code_size;

	code = malloc(expanded_code_size);
	memset(code, 0, expanded_code_size);
	
	qlseek(li, qoff64_t(header.codeOffset));
	lread(li, code, header.codeLength);	

	instruction_pointers = malloc(header.instructionCount * 4);
	memset(instruction_pointers, 0, header.instructionCount * 4);

	qvm_expand_code((int*)code, (int*)instruction_pointers, header.instructionCount, header.codeLength);

	insn_ptrs_segm_start = code_end + (32 - (code_end % 32));
	insn_ptrs_segm_end = insn_ptrs_segm_start + (header.instructionCount * 4);

	
	// Fill IDA database from memory.
	mem2base(code, header.codeOffset, code_end, -1);
	mem2base(instruction_pointers, insn_ptrs_segm_start, insn_ptrs_segm_end, -1);
	
	if (!add_segm(0, header.codeOffset, code_end, "CODE",
		CLASS_CODE)) {
		loader_failure();
	}

	if (!add_segm(0, insn_ptrs_segm_start, insn_ptrs_segm_end, "INSN_PTRS",
		CLASS_DATA)) {
		loader_failure();
	}

	free(code);
	free(instruction_pointers);
	
	// DATA
	// Getting a 2's power where all data fits and adding 4. Replicated from QVM loader source.
	data_size = header.dataLength + header.litLength + header.bssLength;

	for (i = 0; data_size > (1 << i); i++)
	{
	}
	data_size = 1 << i;
	data_size += 4;

	data = malloc(data_size);
	memset(data, 0, data_size);

	qlseek(li, qoff64_t(header.dataOffset));
	lread(li, data, header.dataLength + header.litLength);
	
	// Generate a base addr for .DATA.
	data_segm_start = insn_ptrs_segm_end + (32 - (insn_ptrs_segm_end % 32));

	mem2base(data, data_segm_start, data_segm_start + data_size, -1);
	
	free(data);

	if (!add_segm(0, data_segm_start, data_segm_start + data_size, "DATA",
		CLASS_DATA)) {
		loader_failure();
	}

	// Setting segment properties.
	segment_t *s = getseg(header.codeOffset);
	set_segm_addressing(s, 1);

	// IDA STUFF
	create_filename_cmt();
	
	// Add entry point.
	add_entry(header.codeOffset, header.codeOffset, "_start", true);
}

loader_t LDSC = {
	IDP_INTERFACE_VERSION,
	0,
	accept_qvm_file,
	load_qvm_file,
	NULL,
	NULL,
	NULL
};

