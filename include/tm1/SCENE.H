#ifndef __TM1_SCENE_H__
#define __TM1_SCENE_H__

#include "common.h"
#include <LIBGTE.H>

#include "tm1/MATH.H"

typedef union DbNode DbNode;

typedef struct HdBox {
    /* 0x00 */ MATRIX mat;
    /* 0x20 */ SVECTOR lo;
    /* 0x28 */ SVECTOR hi;
} HdBox; /* 0x30 */

/* UV block of a textured polygon record (same layout as in POLY_FT4). */
typedef struct DbPolyUV {
    /* 0x00 */ u8 u0;
    /* 0x01 */ u8 v0;
    /* 0x02 */ u16 clut;
    /* 0x04 */ u8 u1;
    /* 0x05 */ u8 v1;
    /* 0x06 */ u16 tpage;
    /* 0x08 */ u8 u2;
    /* 0x09 */ u8 v2;
    /* 0x0A */ u16 pad0A;
    /* 0x0C */ u8 u3;
    /* 0x0D */ u8 v3;
    /* 0x0E */ u16 pad0E;
} DbPolyUV; /* 0x10 */

/* The {u1, v1, tpage} word of a textured quad primitive. */
typedef struct UVWord {
    /* 0x00 */ u8 u;
    /* 0x01 */ u8 v;
    /* 0x02 */ u16 page;
} UVWord; /* 0x04 */

/*
 * Header of a polygon record in a DbObject's polygon list.
 * The header is followed by:
 * - `numColor` colour words
 * - the normal indices (at word `normalOffset`)
 * - a DbPolyUV (at word `uvOffset`).
 */
typedef struct DbPoly {
    /* 0x00 */ u8 numUV; /* bits 0-6: UV words; bit 7: DB_POLY_DRAW_MODE */
    /* 0x01 */ u8 numColor;
    /* 0x02 */ u8 len; /* total number of words in the record */
    /* 0x03 */ u8 primLen; /* GPU primitive length in words */
    /* 0x04 */ s16 vi[4];
    /* 0x0C */ u32 unk0C : 16;
    u32 numVert : 3; /* 0x3 = triangle, 0x4 = quad; others indicate a sprite */
    u32 numNormal : 3; /* 0x0 = unlit, 0x1 = flat; others indicate per-vertex */
    u32 vertStride : 2; /* words between vertices in the GPU primitive */
    u32 normalOffset : 4;
    u32 uvOffset : 3;
    u32 doubleSided : 1; /* skips back-face culling */
} DbPoly; /* 0x10 */

#define DB_POLY_NEXT(p) ((DbPoly*)((u8*)(p) + (p)->len * 4))

/* Set this polygon's texture page and window with a draw-mode packet.
 * The texture window word follows the DbPolyUV. */
#define DB_POLY_DRAW_MODE 0x80

/* DbNodeHdr.op: selects the node layout. */
#define DB_OP_OBJECT 0
#define DB_OP_GROUP 1
#define DB_OP_LOD 2
#define DB_OP_ANIM 3
#define DB_OP_TRANS 4
#define DB_OP_ROT 5
#define DB_OP_UNK_6 6
#define DB_OP_BOX 7
#define DB_OP_VOLUME 8
#define DB_OP_SWITCH 9
#define DB_OP_UNK_10 10
#define DB_OP_CTRL 11

typedef struct DbNodeHdr {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
} DbNodeHdr;

/* DbObject.flags. Bits 19-30 hold a fixed OT index (0 = sort by depth). */
#define DB_OBJ_NO_FOV_TEST 0x2000
#define DB_OBJ_DITHER 0x20000
#define DB_OBJ_SUBDIVIDE 0x40000 /* subdivide near polygons (see HIER_SUB) */

typedef struct DbObject {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ SVECTOR* verts;
    /* 0x08 */ SVECTOR* normals;
    /* 0x0C */ DbPoly* polys;
    /* 0x10 */ s32 nPoly;
    /* 0x14 */ u16 sub;
    /* 0x16 */ u16 skip;
    /* 0x18 */ u32 flags;
    /* 0x1C */ VECTOR3 center;
    /* 0x28 */ s32 radius;
    /* 0x2C */ HdBox box;
} DbObject;

/* DbGroup, DbTrans, DbRot and DbCtrl flags. */
#define DB_NODE_NO_COLLIDE 0x1
#define DB_NODE_HIDDEN 0x2
#define DB_NODE_NO_BOUNDS_TEST 0x4

typedef struct DbGroup {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ VECTOR3 center;
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

typedef struct DbLod {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ VECTOR3 center;
    /* 0x10 */ s32 nRec;
    /* 0x14 */ DbLodRec* rec[1];
} DbLod;

typedef struct DbAnim {
    /* 0x00 */ DbNodeHdr h;
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

typedef struct DbTrans {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ VECTOR3 ofs;
    /* 0x10 */ u16 sub;
    /* 0x12 */ u8 nChild;
    /* 0x13 */ u8 flags;
    /* 0x14 */ DbNode* child[1];
} DbTrans;

typedef struct DbRot {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ MATRIX mat;
    /* 0x24 */ u16 sub;
    /* 0x26 */ u8 nChild;
    /* 0x27 */ u8 flags;
    /* 0x28 */ DbNode* child[1];
} DbRot;

typedef struct DbBox {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ s32 code;
    /* 0x08 */ HdBox box;
} DbBox;

typedef struct DbVolume {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ u8 nplane;
    /* 0x05 */ s8 code;
    /* 0x06 */ u16 sub;
    /* 0x08 */ MATRIX mat0;
    /* 0x28 */ MATRIX mat1;
    /* 0x48 */ SVECTOR d0;
    /* 0x50 */ SVECTOR d1;
} DbVolume;

typedef struct DbSwitch {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ u16 sub;
    /* 0x06 */ u8 nChild;
    /* 0x07 */ u8 sel;
    /* 0x08 */ DbNode* child[1];
} DbSwitch;

typedef struct DbCtrl {
    /* 0x00 */ DbNodeHdr h;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u16 sub;
    /* 0x0A */ u8 nBox;
    /* 0x0B */ u8 pad0B;
    /* 0x0C */ DbNode* childOut; /* traversed when outside every box */
    /* 0x10 */ DbNode* childIn; /* traversed when inside any box */
    /* 0x14 */ HdBox* box[1];
} DbCtrl;

/**
 * Implementation note: At first glance, this could be implemented more cleanly
 * by defining `DbNode` as a structure with a shared `DbNodeHdr`:
 *
 * ```c
 * struct DbNode {
 *   DbNodeHdr h;
 *   union {
 *     // the various subtypes, but with their `DbNodeHdr` removed
 *   } d;
 * };
 * ```
 *
 * However, there are various places in the code which explicitly check the
 * opcode type, then create a temp variable converting a `DbNode` pointer
 * directly to that opcode's struct.
 * Thus, it's more likely that the header field is actually duplicated.
 */
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
