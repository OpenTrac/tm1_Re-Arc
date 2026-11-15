#include "common.h"

#include "tm1/UA.H"
#include "tm1/UA_SW.H"

static DbSwitch* hoverCopSwitch;
static DbSwitch* bazCopSwitch;
static DbSwitch* FallingBoxSwitch;
static DbSwitch* DropBoxSwitch;
static DbSwitch* BreakingWindowSwitch;
static s32 numDestroySwitches;
static s32 numDestroyGroups;

void uaswInitDbSwitch(DbNode* node, s32 type)
{
    s32 idx;

    switch (type) {
    case UA_UNK_600:
    case UA_UNK_601:
    case UA_UNK_610:
    case UA_UNK_611:
    case UA_UNK_612:
    case UA_UNK_613:
    case UA_UNK_770:
    case UA_UNK_902:
    case UA_UNK_1000:
    case UA_UNK_1001:
    case UA_UNK_1002:
    case UA_UNK_1009:
    case UA_UNK_1010:
    case UA_UNK_1017:
        if (node->h.op == DB_OP_SWITCH) {
            idx = numDestroySwitches;
            if (idx < 30) {
                destroySwitch[idx] = &node->sw;
                node->sw.sub = idx;
                numDestroySwitches = idx + 1;
            }
        }
        break;
    case UA_BREAKING_WINDOW:
        if (node->h.op == DB_OP_SWITCH) {
            BreakingWindowSwitch = &node->sw;
        }
        break;
    case UA_BAZ_COP:
        if (node->h.op == DB_OP_SWITCH) {
            bazCopSwitch = &node->sw;
        }
        break;
    case UA_HOVER_COP:
        if (node->h.op == DB_OP_SWITCH) {
            hoverCopSwitch = &node->sw;
        }
        break;
    case UA_HEALTH_STAND:
        if (node->h.op == DB_OP_SWITCH) {
            if (node->sw.sub < 11) {
                healthstandSwitch[node->sw.sub - 1] = &node->sw;
            }
        }
        break;
    case UA_BARRICADE:
        if (node->h.op == DB_OP_SWITCH) {
            barricadeSwitch[node->sw.sub - 1] = &node->sw;
        }
        break;
    case UA_FALLING_BOX:
        FallingBoxSwitch = &node->sw;
        break;
    case UA_DROP_BOX:
        DropBoxSwitch = &node->sw;
        break;
    case UA_ICE_CREAM_TRUCK_HEADLIGHTS:
    case UA_TAXI_CAB_HEADLIGHTS:
    case UA_SEMI_TRUCK_HEADLIGHTS:
    case UA_MONSTER_TRUCK_HEADLIGHTS:
    case UA_POLICE_CAR_HEADLIGHTS:
    case UA_LAMBORGHINI_HEADLIGHTS:
    case UA_HUMVEE_HEADLIGHTS:
    case UA_HARLEY_HEADLIGHTS:
    case UA_DUNE_BUGGY_HEADLIGHTS:
    case UA_LOW_RIDER_HEADLIGHTS:
    case UA_CORVETTE_HEADLIGHTS:
    case UA_MAD_MAX_HEADLIGHTS:
    case UA_BOSS_CAR_HEADLIGHTS:
        if (node->h.op == DB_OP_GROUP) {
            if (node->grp.sub < 4) {
                if (node->grp.sub != 0) {
                    carHeadlightSwitch[UA_CAR_SLOT(type)][node->grp.sub - 1] = &node->grp;
                }
            }
        }
        break;
    case UA_UNK_930:
    case UA_UNK_931:
    default:
        break;
    }
}

void uaswSetState(s32 id, s32 num, u32 state)
{
    switch (id) {
    case UA_UNK_600:
    case UA_UNK_601:
    case UA_UNK_610:
    case UA_UNK_611:
    case UA_UNK_612:
    case UA_UNK_613:
    case UA_UNK_770:
    case UA_UNK_902:
    case UA_UNK_1000:
    case UA_UNK_1001:
    case UA_UNK_1002:
    case UA_UNK_1009:
    case UA_UNK_1010:
    case UA_UNK_1017:
        uaswSetSwitch(destroySwitch[num], state);
        break;
    case UA_BREAKING_WINDOW:
        uaswSetSwitch(BreakingWindowSwitch, state);
        break;
    case UA_BAZ_COP:
        uaswSetSwitch(bazCopSwitch, state);
        break;
    case UA_HOVER_COP:
        uaswSetSwitch(hoverCopSwitch, state);
        break;
    case UA_HEALTH_STAND:
        uaswSetSwitch(healthstandSwitch[num - 1], state);
        break;
    case UA_BARRICADE:
        uaswSetSwitch(barricadeSwitch[num - 1], state);
        break;
    case UA_FALLING_BOX:
        uaswSetSwitch(FallingBoxSwitch, state);
        break;
    case UA_DROP_BOX:
        uaswSetSwitch(DropBoxSwitch, state);
        break;
    case UA_UNK_930:
    case UA_UNK_931:
    default:
        break;
    }
}

void uaswSetSwitch(DbSwitch* s, u32 state)
{
    if (s != 0) {
        if (state < s->nChild) {
            s->sel = state;
        }
    }
}

void uaswInitCarSwitch(Cs* cs, s32 id)
{
    DbNode* p;
    DbNode* q;
    s32 kind;
    s32 idx;

    p = cs->epNode;
    if (p == 0) {
        return;
    }
    kind = p->h.op;
    if (kind != DB_OP_GROUP) {
        return;
    }
    p = p->grp.child[0];
    if (p == 0) {
        return;
    }
    if (p->h.op != kind) {
        return;
    }
    q = p->grp.child[0];
    if (q == 0) {
        return;
    }
    if (q->h.op != DB_OP_SWITCH) {
        return;
    }
    idx = UA_CAR_SLOT(id);
    if (idx < 16) {
        carSwitch[idx] = &q->sw;
        q->sw.sel = 0;
    }
}

void uaswSetCarState(s32 id, u32 state, s8 unused)
{
    s32 idx;

    idx = UA_CAR_SLOT(id);
    if (idx < 16) {
        if (carSwitch[idx] != 0) {
            if (state < carSwitch[idx]->nChild) {
                carSwitch[idx]->sel = state;
            }
        }
    }
}

void uaswInitCarTire(DbNode* n)
{
    s32 idx;
    s32 sub;

    if (n == 0) {
        return;
    }
    if (n->h.op != DB_OP_SWITCH) {
        return;
    }
    idx = UA_CAR_SLOT(n->h.kind);
    if (idx < 16) {
        sub = n->sw.sub - 1;
        if (sub < 3) {
            carTireSwitch[idx][sub] = &n->sw;
        }
    }
}

void uaswToggleCarTireState(s32 idx, s32 dir)
{
    s32 j;
    DbSwitch* n;

    if (idx < 16) {
        for (j = 0; j < 3; j++) {
            n = carTireSwitch[idx][j];
            if (n != 0) {
                if (dir < 0) {
                    n->sel--;
                    if (n->sel >= n->nChild) {
                        n->sel = n->nChild - 1;
                    }
                } else {
                    n->sel++;
                    if (n->sel >= n->nChild) {
                        n->sel = 0;
                    }
                }
            }
        }
    }
}

void uaswInitCarShadow(DbNode* n)
{
    s32 idx;
    s32 sub;

    if (n == 0) {
        return;
    }
    if (n->h.op != DB_OP_GROUP) {
        return;
    }
    idx = UA_CAR_SLOT(n->h.kind);
    if (idx < 16) {
        sub = n->grp.sub - 1;
        if (sub < 3) {
            carShadowSwitch[idx][sub] = &n->grp;
        }
    }
}

void uaswSetCarShadow(s32 id, s32 on)
{
    s32 idx;
    s32 j;
    DbGroup* n;

    idx = UA_CAR_SLOT(id);
    if (idx < 16) {
        for (j = 0; j < 3; j++) {
            n = carShadowSwitch[idx][j];
            if (n != 0) {
                if (on) {
                    n->nChild = 1;
                } else {
                    n->nChild = 0;
                }
            }
        }
    }
}

void uaswInitDbEditNumChildren(DbNode* node, s32 type)
{
    s32 idx;

    switch (type) {
    case UA_UNK_400:
    case UA_PICKUP_FIRE_MISSILE:
    case UA_PICKUP_FREEZE_MISSILE:
    case UA_PICKUP_HOMING_MISSILE:
    case UA_PICKUP_POWER_MISSILE:
    case UA_PICKUP_REAR_MISSILE:
    case UA_PICKUP_SINGING_MISSILE:
    case UA_PICKUP_CATAPULT:
    case UA_PICKUP_FLAMETHROWER:
    case UA_PICKUP_BOMB:
    case UA_PICKUP_OIL:
    case UA_PICKUP_SPIKE:
    case UA_UNK_420:
    case UA_UNK_421:
    case UA_UNK_422:
    case UA_UNK_423:
    case UA_UNK_430:
    case UA_UNK_431:
    case UA_PICKUP_SPECIAL:
        if (node->h.op == DB_OP_TRANS) {
            idx = node->trans.sub - 1;
            if (idx < 50) {
                gWeaponPickup[idx] = &node->trans;
            }
        }
        break;
    case UA_UNK_715:
    case UA_UNK_716:
    case UA_UNK_720:
    case UA_UNK_721:
    case UA_UNK_730:
    case UA_UNK_1003:
    case UA_UNK_1004:
    case UA_UNK_1005:
    case UA_UNK_1008:
    case UA_UNK_1015:
        if (node->h.op == DB_OP_GROUP) {
            idx = numDestroyGroups;
            if (idx < 30) {
                destroyGroup[idx] = &node->grp;
                node->grp.sub = idx;
                numDestroyGroups = idx + 1;
            }
        }
        break;
    }
}

void uaswSetNumChildren(s32 id, s32 num, s32 n)
{
    s32 idx;

    idx = num - 1;
    switch (id) {
    case UA_UNK_400:
    case UA_PICKUP_FIRE_MISSILE:
    case UA_PICKUP_FREEZE_MISSILE:
    case UA_PICKUP_HOMING_MISSILE:
    case UA_PICKUP_POWER_MISSILE:
    case UA_PICKUP_REAR_MISSILE:
    case UA_PICKUP_SINGING_MISSILE:
    case UA_PICKUP_CATAPULT:
    case UA_PICKUP_FLAMETHROWER:
    case UA_PICKUP_BOMB:
    case UA_PICKUP_OIL:
    case UA_PICKUP_SPIKE:
    case UA_UNK_420:
    case UA_UNK_421:
    case UA_UNK_422:
    case UA_UNK_423:
    case UA_UNK_430:
    case UA_UNK_431:
    case UA_PICKUP_SPECIAL:
        if (idx < 50) {
            uaswSetNumTransChildren(gWeaponPickup[idx], n);
        }
        break;
    case UA_UNK_715:
    case UA_UNK_716:
    case UA_UNK_720:
    case UA_UNK_721:
    case UA_UNK_730:
    case UA_UNK_1003:
    case UA_UNK_1004:
    case UA_UNK_1005:
    case UA_UNK_1008:
    case UA_UNK_1015:
        uaswSetNumGroupsChildren(destroyGroup[idx + 1], n);
        break;
    }
}

void uaswSetNumGroupsChildren(DbGroup* s, s32 n)
{
    if (s != 0) {
        s->nChild = n;
    }
}

void uaswSetNumTransChildren(DbTrans* s, s32 n)
{
    if (s != 0) {
        s->nChild = n;
    }
}

void uaswInit(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 15; i++) {
        carSwitch[i] = 0;
        for (j = 0; j < 3; j++) {
            carTireSwitch[i][j] = 0;
            carHeadlightSwitch[i][j] = 0;
            carShadowSwitch[i][j] = 0;
        }
    }
    for (i = 0; i < 50; i++) {
        gWeaponPickup[i] = 0;
    }
    for (i = 0; i < 30; i++) {
        destroySwitch[i] = 0;
        destroyGroup[i] = 0;
    }
    hoverCopSwitch = 0;
    bazCopSwitch = 0;
    for (i = 0; i < 4; i++) {
        barricadeSwitch[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        healthstandSwitch[i] = 0;
    }
    FallingBoxSwitch = 0;
    DropBoxSwitch = 0;
    numDestroySwitches = 0;
    numDestroyGroups = 0;
}

void uaswCarHeadlightsOnOff(u32 id, s32 mode)
{
    u32 idx;
    s32 j;

    idx = UA_CAR_SLOT(id);
    for (j = 0; j < 3; j++) {
        if (idx < 15) {
            if (carHeadlightSwitch[idx][j] != 0) {
                if (mode == 1) {
                    carHeadlightSwitch[idx][j]->nChild = 2;
                } else {
                    carHeadlightSwitch[idx][j]->nChild = 0;
                }
            }
        }
    }
}
