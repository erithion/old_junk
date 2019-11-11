/*
 *      Interactive disassembler (IDA).
 *      Version 3.06
 *      Copyright (c) 1990-96 by Ilfak Guilfanov.
 *      ALL RIGHTS RESERVED.
 *                              FIDO:   2:5020/209
 *                              E-mail: ig@estar.msk.su
 *
 */

#ifndef __MOVES_HPP
#define __MOVES_HPP
#pragma pack(push, 1)           // IDA uses 1 byte alignments!

#include <sistack.hpp>

// Helper functions. Should not be called directly!
class curloc;
#define DEFINE_CURLOC_HELPERS(decl) \
decl void  ida_export curloc_linkTo   (curloc *, const char *stackName);\
decl void  ida_export curloc_jump_push(curloc *, bool try_to_unhide, ea_t ea, int lnnum, int x, int y);\
decl bool  ida_export curloc_pop      (curloc *, bool try_tohide);\
decl bool  ida_export curloc_get      (curloc *, size_t depth);\
decl void  ida_export curloc_mark     (curloc *, int marker,const char *title, const char *desc);\
decl ea_t  ida_export curloc_markedpos(curloc *, int *marker);\
decl bool  ida_export curloc_jump     (curloc *, int marker);\
decl char *ida_export curloc_markdesc (curloc *, int marker);

DEFINE_CURLOC_HELPERS(idaman)

#define CURLOC_SISTACK_ITEMS 4

class curloc : public sistack_t
{
public:
  DEFINE_CURLOC_HELPERS(friend)
  ea_t ea;                // Address
  ushort x,y;             // coords on the screen
  ushort lnnum;           // number of line for the current address
#define DEFAULT_LNNUM 0xFFFF
  ushort flags;           // unhid something?
#define UNHID_SEGM 0x0001 // unhid a segment at 'target'
#define UNHID_FUNC 0x0002 // unhid a function at 'target'
#define UNHID_AREA 0x0004 // unhid an area at 'target'
  ea_t target;

  curloc(void)                   { ea = target = BADADDR; flags = 0; x = 0; y = 0; }
  curloc(const char *stackName)  { linkTo(stackName); }
  void linkTo(const char *stackName)
        { curloc_linkTo(this, stackName); }
  void setx(int xx)              { x  = ushort(xx); }
  void jump_push(bool try_to_unhide, ea_t ea=BADADDR, int lnnum=0, int x=0, int y=0)
        { curloc_jump_push(this, try_to_unhide, ea, lnnum, x, y); }
  void push(void);
  bool pop(bool try_tohide)
        { return curloc_pop(this, try_tohide); }
  bool get(size_t depth)
        { return curloc_get(this, depth); }
  size_t size(void)               { return sistack_t::size()/CURLOC_SISTACK_ITEMS; }
  void mark(int marker,const char *title, const char *desc) // Mark position
                                        // marker == -1: ask the user to select the
                                        //               mark slot. title is used
                                        //               as the window caption.
                                        //               if title==NULL then
                                        //               title="please select a mark slot"
                                        // desc == NULL: ask the user to enter
                                        //               the mark description
                                        // desc == "":   destroy the mark
        { curloc_mark(this, marker, title, desc); }
  ea_t markedpos(int *marker)           // get address of marked location
        { return curloc_markedpos(this, marker); }
  bool jump(int marker)                 // Jump to marker
        { return curloc_jump(this, marker); }
  char *markdesc(int marker)
        { return curloc_markdesc(this, marker); }
private:
  void toup(ea_t _ea) { ea = _ea; lnnum = 0; x = 0; y = 0; flags = 0; target = BADADDR; };
  void unhide_if_necessary(ea_t ea);
  void hide_if_necessary(void);
};

#define MAX_MARK_SLOT   1024     // Max number of marked locations

void init_marks(void);
void term_marks(void);
void change_jumps_stack_format(void);
void move_marks(ea_t from, ea_t to, asize_t size);

#pragma pack(pop)
#endif // __MOVES_HPP

