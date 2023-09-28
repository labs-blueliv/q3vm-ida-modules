#include "q3.hpp"
#include <diskio.hpp>
int data_id;

//--------------------------------------------------------------------------
static const char *const RegNames[] =
{
	"PC", "TOS", "VCS", "VDS"
};


//----------------------------------------------------------------------
void q3_t::load_from_idb()
{
  ioh.restore_device();
}

//----------------------------------------------------------------------
// This old-style callback only returns the processor module object.
static ssize_t idaapi notify(void *, int msgid, va_list)
{
  if ( msgid == processor_t::ev_get_procmod )
    return size_t(SET_MODULE_DATA(q3_t));
  return 0;
}

//--------------------------------------------------------------------------
ssize_t idaapi q3_t::on_event(ssize_t msgid, va_list va)
{
  int code = 0;
  switch ( msgid )
  {
    case processor_t::ev_init:
      helper.create(PROCMOD_NODE_NAME); // Creates named netnode.
      break;

    case processor_t::ev_term:
      ioh.ports.clear();
      clr_module_data(data_id);
      break;

    case processor_t::ev_newfile:
      {
        segment_t *sptr = get_first_seg();
        if ( sptr != NULL )
        {
          if ( sptr->start_ea - get_segm_base(sptr) == 0 )
          {
            inf_set_start_ea(sptr->start_ea);
            inf_set_start_ip(sptr->start_ea);
          }
		}
      }
      break;

    case processor_t::ev_oldfile:
      load_from_idb();
      break;

    case processor_t::ev_creating_segm:
      {                 // default DS is equal to CS
        segment_t *sptr = va_arg(va, segment_t *);
        sptr->defsr[rVDS-ph.reg_first_sreg] = sptr->sel;
      }
      break;

    case processor_t::ev_out_header:
      {
        outctx_t *ctx = va_arg(va, outctx_t *);
        q3_header(*ctx);
        return 1;
      }

    case processor_t::ev_out_footer:
      {
        outctx_t *ctx = va_arg(va, outctx_t *);
        q3_footer(*ctx);
        return 1;
      }

    case processor_t::ev_out_segstart:
      {
        outctx_t *ctx = va_arg(va, outctx_t *);
        segment_t *seg = va_arg(va, segment_t *);
        q3_segstart(*ctx, seg);
        return 1;
      }

    case processor_t::ev_out_segend:
      {
        outctx_t *ctx = va_arg(va, outctx_t *);
        segment_t *seg = va_arg(va, segment_t *);
        q3_segend(*ctx, seg);
        return 1;
      }

    case processor_t::ev_ana_insn:
      {
        insn_t *out = va_arg(va, insn_t *);
        return q3_ana(out);
      }

    case processor_t::ev_emu_insn:
      {
        const insn_t *insn = va_arg(va, const insn_t *);
        return q3_emu(*insn) ? 1 : -1;
      }

    case processor_t::ev_out_insn:
      {
        outctx_t *ctx = va_arg(va, outctx_t *);
        out_insn(*ctx);
        return 1;
      }

    case processor_t::ev_out_operand:
      {
        outctx_t *ctx = va_arg(va, outctx_t *);
        const op_t *op = va_arg(va, const op_t *);
        return out_opnd(*ctx, *op) ? 1 : -1;
      }

	  case processor_t::ev_create_func_frame:
	  {
		func_t *pfn = va_arg(va, func_t *);
		create_func_frame(pfn);
		return 1;
	  }


    default:
      break;
  }
  return code;
}

//--------------------------------------------------------------------------
static const asm_t Q3asm =
{
	0, // flag
	0, // uflag
	"Quake 3 VM Assembler.", // name
	0, // help
	NULL, // header
	NULL, // origin directive
	NULL, // end directive

	";",          // comment string
	'\'',         // string delimiter
	'\0',         // char delimiter (no char consts)
	"\\\"'",      // special symbols in char and string constants

	".ascii",     // ascii string directive
	".byte",      // byte directive
	".word",      // word directive
	".dword",         // dword  (4 bytes)
	NULL,         // qword  (8 bytes)
	NULL,         // oword  (16 bytes)
	NULL,         // float  (4 bytes)
	NULL,         // double (8 bytes)
	NULL,         // tbyte  (10/12 bytes)
	NULL,         // packed decimal real
	NULL,         // arrays (#h,#d,#v,#s(...)
	".block %s",  // uninited arrays
	".equ",       // Equ
	NULL,         // seg prefix
	"$",
	NULL,         // func_header
	NULL,         // func_footer
	NULL,         // public
	NULL,         // weak
	NULL,         // extrn
	NULL,         // comm
	NULL,         // get_type_name
	NULL,         // align
	'(', ')',     // lbrace, rbrace
	NULL,    // mod
	NULL,    // and
	NULL,    // or
	NULL,    // xor
	NULL,    // not
	NULL,    // shl
	NULL,    // shr
	NULL,    // sizeof
};

static const asm_t *const asms[] = { &Q3asm, NULL };

//--------------------------------------------------------------------------

#define FAMILY "Quake 3 Virtual Machine"
static const char *const shnames[] = { "Q3", NULL };
static const char *const lnames[]  = { FAMILY, NULL };

//--------------------------------------------------------------------------

static const uchar enterseq[] = { 0x03, 0x00, 0x00, 0x00 };
static const uchar leaveseq[]  = { 0x04, 0x00, 0x00, 0x00 };   // ret

static const bytes_t codestart[] =
{
	{ sizeof(enterseq), enterseq },
	{ 0, NULL }
};
static const bytes_t retcodes[] =
{
  { sizeof(leaveseq),  leaveseq },
  { 0, NULL }
};

processor_t LPH =
{
  IDP_INTERFACE_VERSION,  // version
  0x8513,                // id
                          // flag
    PRN_HEX
  | PR_RNAMESOK           // can use register names for byte names
  | PR_SEGTRANS           // segment translation is supported (map_code_ea)
  | PR_SEGS               // has segment registers?
  | PR_SGROTHER,           // the segment registers don't contain
                          // the segment selectors, something else
                          // flag2
  PR2_IDP_OPTS,         // the module has processor-specific configuration options
  8,                      // 8 bits in a byte for code segments,
  8,                      // 8 bits in a byte for other segments

  shnames,    // short processor names (null term)
  lnames,     // long processor names (null term)

  asms,       // array of enabled assemblers

  notify,     // Various messages:

  RegNames,             // Register names
  qnumber(RegNames),    // Number of registers

  rPC,rVDS,
  1,                    // size of a segment register
  rVCS,rVDS,

  codestart,                 // No known code start sequences
  retcodes,

  0, Q3_MAX,
  Instructions,         // instruc
  0,                    // int tbyte_size;  -- doesn't exist
  { 0, 0, 0, 0 },       // char real_width[4];
                        // number of symbols after decimal point
                        // 2byte float (0-does not exist)
                        // normal float
                        // normal double
                        // long double
  Q3_LEAVE,            
};
