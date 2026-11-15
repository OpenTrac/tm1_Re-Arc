#ifndef __TM1_HUD_H__
#define __TM1_HUD_H__

#include "common.h"

#include "tm1/GRUTILS.H"

void hudResetRadar(void);
void hudDisplayRadar(u_long* ot);
void hudInitRadar(void);
void hudAddRadarSig(s32 id, VECTOR3* pos, s32 ang);
void hudradarActivate(void);
void hudRadarToggle(void);
void hudStoreArrowIcon(s32 id, DbNode* node);

#endif /* __TM1_HUD_H__ */
