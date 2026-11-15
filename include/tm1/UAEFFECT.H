#ifndef __TM1_UAEFFECT_H__
#define __TM1_UAEFFECT_H__

#include "common.h"
#include "tm1/CS.H"
#include "tm1/SCENE.H"

typedef struct HitLight {
    /* 0x00 */ s32 count;
    /* 0x04 */ Cs* node;
} HitLight; /* 0x08 */

/* The {u1, v1, tpage} word of a textured quad primitive. */
typedef struct UVWord {
    /* 0x00 */ u8 u;
    /* 0x01 */ u8 v;
    /* 0x02 */ u16 page;
} UVWord; /* 0x04 */

void UAeffectInit(void);
void UAeffectUpdateMonsterTruckWheels(s32 speed);
void UAeffectUpdateTVs(void);
void UAeffectInitTV(DbObject* node);
void UAeffectInitMonsterTruckWheel(DbRot* node);
void UAeffectUpdateSpecialEffects(void);
void UAeffectUpdateCarSpeed(s32 carId, s32 speed);
void UAeffectActHitLights(Cs* node, s32 count, u8* unused_rgb);
void UAeffectUpdateHitLights(void);
void UAeffectBarricade(s32 a0, s32 a1, s32 state);
void effectResetEffects(void);
void UAeffectSetFreezeLight(Cs* node);
void UAeffectClearFreezeLight(Cs* node);
void UAeffectInitHealthStandLightning(DbObject* node);
void UAeffectUpdateHealthStandLightning(void);

#endif /* __TM1_UAEFFECT_H__ */
