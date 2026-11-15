#ifndef __TM1_INTERACT_H__
#define __TM1_INTERACT_H__

#include "common.h"
#include "tm1/CAR.H"
#include "tm1/GRUTILS.H"
#include "tm1/MATH.H"
#include "tm1/RT.H"

void carSetPowerupDelaysBySkillLevel(s32 level);
s32 getSpecialWeaponCost(s32 id);
void UpdateWeapons(Car* car, u8 isPlayer);
void UpdatePlayerWeaponPadStatus(PlayerCar* car);
void UpdateAIWeaponPadStatus(AICar* car);
void UpdateGuns(Car* car, u8 isPlayer);
void UpdateNonGuns(Car* car, u8 isPlayer);
void FireMissiles(Car* car, u8 isPlayer);
void FireSpecials(Car* car, u8 isPlayer);
s32 find_free_taser(void);
void carInitTasers(void);
s32 fire_taser(Car* car, s32 isPlayer);
void draw_tasers(Db* db, s32 view);
void LaunchDrops(Car* car, u8 isPlayer);
void carAddWeapTex(DbNode* node, s32 id);
void carInitDropWeaps(void);
s32 FindFreeDropWeap(void);
u8 carDropWeapon(u32 kind, s32 idx, VECTOR3* pos, s32 power);
void check_dropweap_impact(s32 i);
void drawOil(POLY_FT4* prim, VECTOR3* pos, s32 view);
void displayDropWeapons(Db* db, s32 view);
void carLockon(u8 who, VECTOR3** out);
void carLockon2(u8 who, VECTOR3** out);
void carInitFlamethrowers(void);
s16 find_free_flamethrower(void);
void fireRearFlameThrower(Car* car, u8 isPlayer);
s16 fireFlameThrower(Car* car, u8 isPlayer, u8 rear, VECTOR3* pos);
void displayFlamethrowers(void);
void carResetGunHeat(PlayerCar* car);
void carInitPickupWeapons(void);
void setPickup(VECTOR3* pos, s32 obj, s32 n);
void regeneratePickupWeapons(void);
void carGetPickup(Car* car, u8 isPlayer, s32 kind, s32 idx);
u8 getBombDamage(void);
u8 health_stand_active(s32 n);
void animate_health_stand(s32 n);
void deactivate_health_stand(s32 n);
void reactivate_healthstand(s32 n);
void init_health_stands(void);
void set_health_stand(s32 n, VECTOR3* pos);
void regen_healthstands(void);
s32 carweapGetMonsterDamage(void);

#endif // __TM1_INTERACT_H__
