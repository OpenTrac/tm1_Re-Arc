#ifndef __TM1_HIER_SUB_H__
#define __TM1_HIER_SUB_H__

#include "common.h"

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

void subPoly3(Db* db, SubPoly* f, s32 count);
void subLoadPoly3Words(s32 col, s32 row);
void subPoly4(Db* db, SubPoly* f, s32 count);
void subLoadPoly4Words(s32 col, s32 row);
void subAddPrim(Db* db, SubPoly* f, DR_MODE** cursor, void** extra);

#endif /* __TM1_HIER_SUB_H__ */
