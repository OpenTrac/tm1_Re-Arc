#ifndef __TM1_UA_SW_H__
#define __TM1_UA_SW_H__

#include "common.h"
#include "tm1/CS.H"
#include "tm1/SCENE.H"

extern DbSwitch* carSwitch[15];
extern DbSwitch* carTireSwitch[15][3];
extern DbTrans* gWeaponPickup[50];
extern DbSwitch* destroySwitch[30];
extern DbGroup* destroyGroup[30];
extern DbSwitch* barricadeSwitch[4];
extern DbSwitch* healthstandSwitch[10];
extern DbGroup* carHeadlightSwitch[15][3];
extern DbGroup* carShadowSwitch[15][3];

void uaswInitDbSwitch(DbNode* node, s32 type);
void uaswSetState(s32 id, s32 num, u32 state);
void uaswSetSwitch(DbSwitch* s, u32 state);
void uaswInitCarSwitch(Cs* cs, s32 id);
void uaswSetCarState(s32 id, u32 state, s8 unused);
void uaswInitCarTire(DbNode* n);
void uaswToggleCarTireState(s32 idx, s32 dir);
void uaswInitCarShadow(DbNode* n);
void uaswSetCarShadow(s32 id, s32 on);
void uaswInitDbEditNumChildren(DbNode* node, s32 type);
void uaswSetNumChildren(s32 id, s32 num, s32 n);
void uaswSetNumGroupsChildren(DbGroup* s, s32 n);
void uaswSetNumTransChildren(DbTrans* s, s32 n);
void uaswInit(void);
void uaswCarHeadlightsOnOff(u32 id, s32 mode);

#endif /* __TM1_UA_SW_H__ */
