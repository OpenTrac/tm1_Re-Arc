#ifndef __TM1_GRUTILS_H__
#define __TM1_GRUTILS_H__

#include "common.h"
#include <LIBGPU.H>

#include "tm1/SCENE.H"

/* Parsed DMD3D sprite. */
typedef struct GrSprite {
    u8 u0, v0, w, h;
    u16 tpage, clut;
} GrSprite;

/* 2D screen sprite record built by grutilsParse2DScreenSprite (16 bytes). */
typedef struct {
    s16 u0; /* +0 */
    s16 v0; /* +2 */
    s16 w; /* +4 */
    s16 h; /* +6 */
    s16 tpage; /* +8 */
    s16 clut; /* +10 */
    s16 x; /* +12 */
    s16 y; /* +14 */
} GrScreenSprite;

u16 Load4BitClut(u32* data, int x, int y);
u32 grutilsParse2DScreenSprite(DbNode* node, GrScreenSprite* out, u32 max);
u32 grutilsParse3DSprite(DbNode* node, GrSprite* out, u32 max);
s32 grutilsParse2DSprite(DbNode* node, GrSprite* out, u32 max);
void grutilsDrawNumberUsingSprites(s32 num, SPRT* prim, GrSprite* digits, u32* ot);

#endif
