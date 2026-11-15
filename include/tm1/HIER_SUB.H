#ifndef __TM1_HIER_SUB_H__
#define __TM1_HIER_SUB_H__

#include "common.h"
#include <LIBGPU.H>

#include "tm1/RT.H"
#include "tm1/SCENE.H"

/* A near polygon queued by geomProc for subdivision. */
typedef struct SubPoly {
    /* 0x00 */ DbPoly* poly;
    /* 0x04 */ s32 otIdx;
    /* 0x08 */ void* prim; /* GPU primitive built by geomProc */
    /* 0x0C */ u16 tpage;
    /* 0x0E */ u16 dither;
    /* 0x10 */ u32* texWindow;
} SubPoly; /* 0x14 */

/* Scratchpad values shared by hierProc, geomProc and the HIER_SUB functions. */
#define sdSubPolyCount (*(s32*)0x1F800038)
#define sdSubPolyPtr (*(SubPoly**)0x1F80004C)
#define SPAD_DRMODE ((DR_MODE*)0x1F800060)
#define SPAD_DRMODE_DITHER ((DR_MODE*)0x1F80006C)
#define sdMirrorView (*(s32*)0x1F800088)

void subPoly3(Db* db, SubPoly* f, s32 count);
void subLoadPoly3Words(s32 col, s32 row);
void subPoly4(Db* db, SubPoly* f, s32 count);
void subLoadPoly4Words(s32 col, s32 row);
void subAddPrim(Db* db, SubPoly* f, DR_MODE** cursor, void** extra);

#endif /* __TM1_HIER_SUB_H__ */
