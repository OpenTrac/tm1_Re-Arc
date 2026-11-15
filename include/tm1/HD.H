#ifndef __TM1_HD_H__
#define __TM1_HD_H__

#include "common.h"
#include <LIBGTE.H>

#include "tm1/CS.H"
#include "tm1/SCENE.H"

typedef struct HdEnt {
    /* 0x00 */ DbNode* node;
    /* 0x04 */ s32 v[3];
    /* 0x10 */ s32 matIdx;
    /* 0x14 */ u16 id0;
    /* 0x16 */ u16 id1;
} HdEnt; /* 0x18 */

typedef struct HdCsHit {
    /* 0x00 */ s32 hit;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 histIdx;
    /* 0x0A */ s16 tag0;
    /* 0x0C */ s16 tag1;
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ s32 depth;
    /* 0x14 */ VECTOR3 normal;
    /* 0x20 */ Cs* obj;
} HdCsHit; /* 0x24 */

void hdPush(DbNode* node, s32* v, s32 matIdx, s32 id0, s16 id1);
s32 hdPop(HdEnt** out);
void hdSetNodeIds(s16* out0, s16* out1, HdEnt* e, s16 id0, s16 id1);
s32 hdIsPtInSphere(VECTOR* pt, s32 radius, VECTOR* centre, s32 slack);
void hdTraverse(Cs* cs, s32* pt, HdCsHit* res, s32 slack);
void hdLod(DbLod* node, HdEnt* e);
s32 hdPntBox(HdBox* b, VECTOR* pos, HdCsHit* res, s32 matIdx);
s32 hdPntVolume(DbVolume* n, VECTOR* pos, HdCsHit* res, s32 matIdx);
void hdCtrlNode(HdEnt* e, s16 id0, s16 id1);
s32 hdInCtrlBox(HdBox* box, s32* pt);
void hdSetHitTag(HdCsHit* hit, s16 id0, s16 id1, HdEnt* e);
void HdCsTest(Cs* cs, HdCsHit* hit, s32 doWorld, MATRIX* otherMat, s32* otherPos);
void hdInit(HdEnt* stack);

#endif /* __TM1_HD_H__ */
