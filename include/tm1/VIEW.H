#ifndef __TM1_VIEW_H__
#define __TM1_VIEW_H__

#include "common.h"
#include <LIBGTE.H>

#include "tm1/CS.H"
#include "tm1/FOV_DATA.H"
#include "tm1/MATH.H"
#include "tm1/RT.H"

s32 viewGetCurrentFOVH(s32 which);
s32 viewGetCurrentHorzFOVH(s32 which);
s32 viewGetCurrentVertFOVH(s32 which);
s32 viewGetCurrentFOVDegrees(s32 which);
s32 viewGetCurrentFOVDegreesHorz(s32 which);
s32 viewGetCurrentFOVDegreesVert(s32 which);
void viewGetScreenSize(s32* w, s32* h);
void viewCreate(Db* db, s32 entry, s32 which);
void viewToggleView();
void viewInit();
void viewSetFovDat(s32 entry, s32 which);
s32 viewGetFov();
void viewSetFov(s32 fov);
void viewUpdateFov();
void viewTweekInit();
void viewProc(s32 which);
s32 viewGetCurrentFovSet(s32 which);
void viewSetParent(Cs* node, s32 which);
void viewSetSky(Cs* node);
Cs* viewGetSky(s32 which);
void viewSetSkyPosition(s32* pos, s32 which);
void viewGetDeltaRot(void* dst, s32 which);
void viewGetDeltaTrans(void* dst, s32 which);
void viewSetDeltaRot(void* src, s32 which);
void viewSetDeltaTrans(void* src, s32 which);
Cs* viewGetOwnship(s32 which);
void viewSetBgColor();
void viewGetCenter(s32 which, s32* cx, s32* cy);
VECTOR3* viewGetEyeTrans(s32 which);
MATRIX* viewGetEyeMat(s32 which);
void wordCp(s32* dst, s32* src, s32 n);

#endif
