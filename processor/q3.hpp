#ifndef _Q3_HPP
#define _Q3_HPP

#include "../idaidp.hpp"
#include "ins.hpp"
#include <segregs.hpp>
#include <diskio.hpp>
#include "../iohandler.hpp"

#define PROCMOD_NAME            q3
#define PROCMOD_NODE_NAME       "$ Quake III VM"

//------------------------------------------------------------------
struct q3_iohandler_t : public iohandler_t
{
  q3_iohandler_t(netnode &nn) : iohandler_t(nn) {}
};


struct q3_t : public procmod_t
{
  netnode helper;
  q3_iohandler_t ioh = q3_iohandler_t(helper);
  ea_t intmem = BADADDR; // linear EA of the internal memory/registers segment
  bool flow = false;

  //reg.cpp
  virtual ssize_t idaapi on_event(ssize_t msgid, va_list va) override;
  void load_from_idb();

  // ana.cpp
  int  q3_ana(insn_t *insn);

  // emu.cpp
  int  q3_emu(const insn_t &insn);
  void handle_call(const insn_t &insn);
  void handle_jump(const insn_t &insn);
  bool create_func_frame(func_t *pfn) const;
  
  // out.cpp
  bool out_opnd(outctx_t &ctx, const op_t &x);
  void q3_header(outctx_t &ctx);
  void q3_footer(outctx_t &ctx);
  void q3_segstart(outctx_t &ctx, segment_t *seg);
  void q3_segend(outctx_t &ctx, segment_t *seg);
};
extern int data_id;

enum q3_registers
{
  rPC, rTOS, rVCS, rVDS
};

#endif
