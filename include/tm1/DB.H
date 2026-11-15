#ifndef __TM1_DB_H__
#define __TM1_DB_H__

#include "common.h"
#include <LIBGTE.H>

#include "tm1/CS.H"
#include "tm1/SCENE.H"

void dbInit(u32 tmsVersion);
Cs* dbInitCsForAModel(void* node);
void dbScanForInteractiveStuff(DbNode* group);
void dbProcessInteractives(s32 type, DbNode* node, s32* loc, s32 parent, s32* parentLoc);
void dbReset3DEnvironmentTrap(void);

#endif /* __TM1_DB_H__ */
