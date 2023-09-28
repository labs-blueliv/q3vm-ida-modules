#include "q3.hpp"


//--------------------------------------------------------------------------
void q3_t::q3_header(outctx_t &ctx)
{
  ctx.gen_header(GH_PRINT_PROC_ASM_AND_BYTESEX);
}

//--------------------------------------------------------------------------
void q3_t::q3_footer(outctx_t &ctx)
{
  ctx.gen_empty_line();

  ctx.out_line(ash.end, COLOR_ASMDIR);
  ctx.flush_outbuf(DEFAULT_INDENT);

  ctx.gen_cmt_line("end of file");
}

//--------------------------------------------------------------------------
void q3_t::q3_segstart(outctx_t &ctx, segment_t *Srange)
{
  qstring sname;
  get_visible_segm_name(&sname, Srange);

  ctx.gen_cmt_line(COLSTR("segment %s", SCOLOR_AUTOCMT), sname.c_str());
}

//--------------------------------------------------------------------------
void q3_t::q3_segend(outctx_t &ctx, segment_t *seg)
{
  qstring sname;
  get_visible_segm_name(&sname, seg);
  ctx.gen_cmt_line("end of '%s'", sname.c_str());
}

//----------------------------------------------------------------------
void idaapi out_insn(outctx_t &ctx)
{
	ctx.out_mnemonic();

	if (ctx.insn.get_canon_feature(ctx.ph) & CF_USE1) {
		ctx.out_one_operand(0);
	}
  
	ctx.flush_outbuf();
}

//----------------------------------------------------------------------
bool q3_t::out_opnd(outctx_t &ctx, const op_t &x)
{
	ctx.out_value(x, OOFW_IMM | OOFW_32);
  	return 1;
}