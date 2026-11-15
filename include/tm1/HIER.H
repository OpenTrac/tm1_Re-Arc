#ifndef __TM1_HIER_H__
#define __TM1_HIER_H__

#include "common.h"
#include <LIBGPU.H>
#include <LIBGTE.H>

#include "tm1/CS.H"
#include "tm1/HD.H"
#include "tm1/HIER_SUB.H"
#include "tm1/RT.H"
#include "tm1/SCENE.H"

typedef struct HierEnt {
    /* 0x00 */ DbNode* node;
    /* 0x04 */ s32 matIdx;
    /* 0x08 */ VECTOR3 pos;
    /* 0x14 */ s32 eoCnt;
} HierEnt; /* 0x18 */

typedef struct MatInfo {
    /* 0x00 */ MATRIX* mat;
    /* 0x04 */ MATRIX* light;
    /* 0x08 */ MATRIX* fov0;
    /* 0x0C */ MATRIX* fov1;
} MatInfo; /* 0x10 */

typedef struct ObjRec {
    /* 0x00 */ DbObject* node;
    /* 0x04 */ VECTOR3 pos;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ Cs* cs;
    /* 0x18 */ s32 matIdx;
    /* 0x1C */ s32 eoCnt;
} ObjRec; /* 0x20 */

extern HierEnt hierStack[150];
extern MatInfo matInfo[90];
extern SubPoly subPolyList[];
extern ObjRec objList[220];
extern MATRIX matList[];
extern MATRIX lightList[];
extern MATRIX fovNorms1[];
extern MATRIX fovNorms2[];

void hierPush(DbNode* node, s32 mat, VECTOR3* pos, s32 eo);
s32 hierPop(HierEnt** out);
void hierTraverse(Cs* cs, DbNode* node);
s32 hierIsInFov(VECTOR3* ofs, s32 dist, VECTOR3* pos, MATRIX* fov0, MATRIX* fov1);
void hierLod(DbLod* n, HierEnt* e);
void hierCtrlNode(DbNode* unused, HierEnt* e);
s32 hierInBox(HdBox* b, VECTOR3* pos);
void hierAnimate(DbAnim* a);
void hierAnimateBiDirect(DbAnim* a);
void hierSetWorldEP(s32 ep);
void hierProc(Db* db, s32 which);
void hierProcOne(Db* db, Cs* cs);
void hierInit(void);
void wordCopy(s32* dst, s32* src, s32 n);
void hierSetLodScale(s32 v);
void hierSetLodShift(s32 v);
void hierSetGroundFlag(s32 v);

#endif /* __TM1_HIER_H__ */
