#ifndef __TM1_SCENE_H__
#define __TM1_SCENE_H__

#include "common.h"
#include <LIBGTE.H>

typedef union DbNode DbNode;

typedef struct HdBox {
    /* 0x00 */ MATRIX mat;
    /* 0x20 */ SVECTOR lo;
    /* 0x28 */ SVECTOR hi;
} HdBox; /* 0x30 */

typedef struct DbNodeHdr {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
} DbNodeHdr;

/* op 0 */
typedef struct DbObject {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ SVECTOR* verts;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ u8* polys;
    /* 0x10 */ s32 nPoly;
    /* 0x14 */ u16 sub;
    /* 0x16 */ u16 skip;
    /* 0x18 */ u32 flags;
    /* 0x1C */ s32 center[3];
    /* 0x28 */ s32 radius;
    /* 0x2C */ HdBox box;
} DbObject;

/* op 1 */
typedef struct DbGroup {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ s32 center[3];
    /* 0x10 */ s32 radius;
    /* 0x14 */ u8 nChild;
    /* 0x15 */ u8 flags;
    /* 0x16 */ u16 sub;
    /* 0x18 */ DbNode* child[1];
} DbGroup;

typedef struct DbLodRec {
    /* 0x00 */ u32 far;
    /* 0x04 */ u32 near;
    /* 0x08 */ DbNode* child;
} DbLodRec;

/* op 2 */
typedef struct DbLod {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ s32 center[3];
    /* 0x10 */ s32 nRec;
    /* 0x14 */ DbLodRec* rec[1];
} DbLod;

/* op 3 */
typedef struct DbAnim {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u16 sub;
    /* 0x06 */ u16 rate;
    /* 0x08 */ u8 lo;
    /* 0x09 */ u8 hi;
    /* 0x0A */ u8 mode;
    /* 0x0B */ u8 nChild;
    /* 0x0C */ u8 cur;
    /* 0x0D */ u8 dir;
    /* 0x0E */ u16 timer;
    /* 0x10 */ DbNode* child[1];
} DbAnim;

/* op 4 */
typedef struct DbTrans {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ s32 ofs[3];
    /* 0x10 */ u16 sub;
    /* 0x12 */ u8 nChild;
    /* 0x13 */ u8 flags;
    /* 0x14 */ DbNode* child[1];
} DbTrans;

/* op 5 */
typedef struct DbRot {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ MATRIX mat;
    /* 0x24 */ u16 sub;
    /* 0x26 */ u8 nChild;
    /* 0x27 */ u8 flags;
    /* 0x28 */ DbNode* child[1];
} DbRot;

/* op 7 */
typedef struct DbBox {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ s32 code;
    /* 0x08 */ HdBox box;
} DbBox;

/* op 8 */
typedef struct DbVolume {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u8 nplane;
    /* 0x05 */ s8 code;
    /* 0x06 */ u16 sub;
    /* 0x08 */ MATRIX mat0;
    /* 0x28 */ MATRIX mat1;
    /* 0x48 */ SVECTOR d0;
    /* 0x50 */ SVECTOR d1;
} DbVolume;

/* op 9 */
typedef struct DbSwitch {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u16 sub;
    /* 0x06 */ u8 nChild;
    /* 0x07 */ u8 sel;
    /* 0x08 */ DbNode* child[1];
} DbSwitch;

/* op 11 */
typedef struct DbCtrl {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u16 sub;
    /* 0x0A */ u8 nBox;
    /* 0x0B */ u8 pad0B;
    /* 0x0C */ DbNode* childOut; /* traversed when outside every box */
    /* 0x10 */ DbNode* childIn; /* traversed when inside any box */
    /* 0x14 */ HdBox* box[1];
} DbCtrl;

union DbNode {
    DbNodeHdr h;
    DbObject obj;
    DbGroup grp;
    DbLod lod;
    DbAnim anim;
    DbTrans trans;
    DbRot rot;
    DbBox box;
    DbVolume vol;
    DbSwitch sw;
    DbCtrl ctrl;
};

#endif /* __TM1_SCENE_H__ */
