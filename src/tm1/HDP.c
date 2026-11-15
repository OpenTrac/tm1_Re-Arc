#include "common.h"

#include <LIBGTE.H>

#include "tm1/CS.H"
#include "tm1/MATH.H"

#include "tm1/HD.H"
#include "tm1/HDP.H"

HdpEnt* fastHdpStack = (HdpEnt*)0x1F800000;

static HdpEnt* hdpStack;
static s32 hdp_stackIdx;

void hdpPush(DbNode* node, VECTOR3* pos, s32 id0, s32 id1);
#ifdef NON_MATCHING
void hdpPush(DbNode* node, VECTOR3* pos, s32 id0, s32 id1)
{
    HdpEnt* base;
    s32 i = hdp_stackIdx;

    do {
        if ((u32)i < 50) {
            base = fastHdpStack;
        } else {
            base = hdpStack;
        }
    } while (0);
    base[i].node = node;
    base[i].pos.vx = pos->vx;
    base[i].pos.vy = pos->vy;
    base[i].pos.vz = pos->vz;
    base[i].id0 = id0;
    base[i].id1 = id1;
    if (hdp_stackIdx < 200) {
        hdp_stackIdx++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/HDP", hdpPush);
#endif

s32 hdpPop(HdpEnt** out)
{
    HdpEnt* base;
    s32 i = hdp_stackIdx - 1;

    hdp_stackIdx = i;

    if (i >= 0) {
        if ((u32)i < 50) {
            base = fastHdpStack;
        } else {
            base = hdpStack;
        }
        base += i;
        *out = base;
    }
    return hdp_stackIdx;
}

void hdpSetNodeIds(s16* p0, s16* p1, HdpEnt* n, s16 id0, s16 id1)
{
    if (id0 == 0) {
        *p0 = n->id0;
        *p1 = n->id1;
    } else {
        *p0 = id0;
        *p1 = id1;
    }
}

s32 hdpIsPtInSphere(VECTOR3* p, s32 rad, VECTOR3* c)
{
    VECTOR3 d;
    s32 dist;

    if (rad <= 0xB504) {

        d.vx = (s32)((u32)p->vx - (u32)c->vx);
        d.vy = (s32)((u32)p->vy - (u32)c->vy);
        d.vz = (s32)((u32)p->vz - (u32)c->vz);
        dist = (s32)((u32)d.vx * (u32)d.vx + (u32)d.vy * (u32)d.vy + (u32)d.vz * (u32)d.vz);
        if (dist < 0 || dist >= (s32)((u32)rad * (u32)rad)) {
            return 0;
        }
        return 1;
    }
    return 1;
}

#ifdef NON_MATCHING
void hdpTraverse(Cs* cs, VECTOR3* pt, HdCsHit* res)
{
    HdpEnt* e;
    s16 id0;
    s16 id1;
    DbNode* node;
    s32* pos;
    s32 t0;
    s32 t1;
    u8 kind;

    node = cs->epNode;
    pos = pt;
    t0 = 0;
    t1 = 0;
    e = NULL;
    hdp_stackIdx = 0;
push:
    hdpPush(node, pos, t0, t1);
    while (res->depth == 0 && hdpPop(&e) >= 0) {
        kind = e->node->h.op;
        switch (kind) {
        case DB_OP_OBJECT: {
            DbObject* n = &e->node->obj;

            if (n->skip == 1) {
                break;
            }
            if (hdpIsPtInSphere(&n->center, n->radius, &e->pos) == 0) {
                break;
            }
            res->depth = hdpPntBox(&n->box, &e->pos, &res->hit, 0);
            if (res->hit == 0) {
                break;
            }
            hdpSetHitTag(res, n->h.kind, n->sub, e);
            break;
        }
        case DB_OP_BOX: {
            DbBox* n = &e->node->box;

            res->depth = hdpPntBox(&n->box, &e->pos, &res->hit, n->code == -2);
            if (res->hit == 0) {
                break;
            }
            hdpSetHitTag(res, n->h.kind, n->code, e);
            break;
        }
        case DB_OP_VOLUME: {
            DbVolume* n = &e->node->vol;

            res->depth = hdpPntVolume(n, &e->pos, res, n->code == -2);
            if (res->hit == 0) {
                break;
            }
            hdpSetHitTag(res, n->h.kind, n->sub, e);
            break;
        }
        case DB_OP_GROUP: {
            DbGroup* n = &e->node->grp;
            s32 i;
            s32 cnt;

            if (n->flags & DB_NODE_NO_COLLIDE) {
                break;
            }
            if (hdpIsPtInSphere(&n->center, n->radius, &e->pos) == 0) {
                break;
            }
            hdpSetNodeIds(&id0, &id1, e, n->h.kind, n->sub);
            cnt = n->nChild;
            i = 0;
            if (cnt != 0) {
                do {
                    hdpPush(n->child[i], &e->pos, id0, id1);
                    i++;
                } while (i < cnt);
            }
            break;
        }
        case DB_OP_LOD:
            hdpLod(&e->node->lod, e);
            break;
        case DB_OP_TRANS: {
            DbTrans* n = &e->node->trans;
            s32 i;

            if (n->flags & DB_NODE_NO_COLLIDE) {
                break;
            }
            e->pos.vx = e->pos.vx - n->ofs.vx;
            e->pos.vy = e->pos.vy - n->ofs.vy;
            e->pos.vz = e->pos.vz - n->ofs.vz;
            hdpSetNodeIds(&id0, &id1, e, n->h.kind, n->sub);
            for (i = 0; i < n->nChild; i++) {
                hdpPush(n->child[i], &e->pos, id0, id1);
            }
            break;
        }
        case DB_OP_ROT: {
            DbRot* n = &e->node->rot;
            VECTOR v;
            s32 dead[2];
            s32 i;

            if (n->flags & DB_NODE_NO_COLLIDE) {
                break;
            }
            e->pos.vx = e->pos.vx - n->mat.t[0];
            e->pos.vy = e->pos.vy - n->mat.t[1];
            e->pos.vz = e->pos.vz - n->mat.t[2];
            v.vx = e->pos.vx;
            v.vy = e->pos.vy;
            v.vz = e->pos.vz;
            mathMulVec(&n->mat, &v, &e->pos);
            hdpSetNodeIds(&id0, &id1, e, n->h.kind, n->sub);
            for (i = 0; i < n->nChild; i++) {
                hdpPush(n->child[i], &e->pos, id0, id1);
            }
            break;
        }
        case DB_OP_SWITCH: {
            DbSwitch* n = &e->node->sw;

            hdpSetNodeIds(&id0, &id1, e, n->h.kind, n->sub);
            t0 = id0;
            t1 = id1;
            node = n->child[n->sel];
            pos = &e->pos;
            goto push;
        }
        case DB_OP_ANIM: {
            DbAnim* n = &e->node->anim;

            hdpSetNodeIds(&id0, &id1, e, n->h.kind, n->sub);
            t0 = id0;
            t1 = id1;
            node = n->child[n->cur];
            pos = &e->pos;
            goto push;
        }
        case DB_OP_CTRL: {
            DbCtrl* n = &e->node->ctrl;

            if (n->flags & DB_NODE_NO_COLLIDE) {
                break;
            }
            hdpSetNodeIds(&id0, &id1, e, n->h.kind, n->sub);
            hdpCtrlNode(e, id0, id1);
            break;
        }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/HDP", hdpTraverse);
#endif

void hdpLod(DbLod* n, HdpEnt* e)
{
    VECTOR3 d;
    u32 dist;
    s32 i;
    s32 go;

    d.vx = n->center.vx - e->pos.vx;
    d.vy = n->center.vy - e->pos.vy;
    d.vz = n->center.vz - e->pos.vz;
    d.vx = d.vx >> 3;
    d.vy = d.vy >> 3;
    d.vz = d.vz >> 3;
    dist = (d.vx * d.vx) + (d.vy * d.vy) + (d.vz * d.vz);
    go = 1;
    for (i = 0; i < n->nRec; i++) {
        if (go == 0) {
            break;
        }
        if (dist >= n->rec[i]->near && dist < n->rec[i]->far) {
            hdpPush(n->rec[i]->child, &e->pos, e->id0, e->id1);
            go = 0;
        }
    }
}

s32 hdpPntBox(HdBox* b, VECTOR3* pos, s32* hit, s32 mode)
{
    VECTOR3 p;
    VECTOR3 d;

    mathMulVec(&b->mat, pos, &p);
    d.vx = b->lo.vx + p.vx;
    if (d.vx > 0) {
        d.vy = b->lo.vy + p.vy;
        if (d.vy > 0) {
            d.vz = b->lo.vz + p.vz;
            if (d.vz > 0 && b->hi.vx - d.vx > 0 && b->hi.vy - d.vy > 0 && b->hi.vz - d.vz > 0) {
                *hit = 1;
            }
        }
    }
    if (*hit == 1 && mode != 0) {
        *hit = 0;
        return -2;
    }
    return *hit == 1;
}

s32 hdpPntVolume(DbVolume* n, VECTOR3* pos, HdCsHit* res, s32 mode)
{
    VECTOR3 p;

    mathMulVec(&n->mat0, pos, &p);
    if (n->d0.vx + p.vx > 0 && n->d0.vy + p.vy > 0 && n->d0.vz + p.vz > 0) {
        if (n->nplane < 4) {
            res->hit = 1;
        } else {
            mathMulVec(&n->mat1, pos, &p);
            if (n->d1.vx + p.vx > 0
                && (n->nplane < 5
                    || (n->d1.vy + p.vy > 0 && (n->nplane < 6 || n->d1.vz + p.vz > 0)))) {
                res->hit = 1;
            }
        }
    }
    if (res->hit == 1 && mode != 0) {
        res->depth = -2;
        res->hit = 0;
        return res->depth;
    }
    return res->hit == 1;
}

#ifdef NON_MATCHING
void hdpCtrlNode(HdpEnt* e, s16 id0, s16 id1)
{
    DbCtrl* n;
    s32 i;
    s32 ok;

    n = &e->node->ctrl;
    i = 0;
    ok = 1;
    while (i < n->nBox) {
        if (hdpInCtrlBox(n->box[i], &e->pos) != 0) {
            ok = 0;
        }
        i++;
        if (ok == 0) {
            break;
        }
    }
    if (ok != 0) {
        if (n->childOut != 0) {
            hdpPush(n->childOut, &e->pos, id0, id1);
        }
    } else {
        if (n->childIn != 0) {
            hdpPush(n->childIn, &e->pos, id0, id1);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/HDP", hdpCtrlNode);
#endif

#ifdef NON_MATCHING
s32 hdpInCtrlBox(HdBox* b, VECTOR3* pos)
{
    VECTOR p;
    VECTOR d;
    s32 r;

    mathMulVec(&b->mat, pos, &p);
    r = 0;
    d.vz = b->lo.vz + p.vz;
    if (d.vz >= 0) {
        d.vy = b->lo.vy + p.vy;
        if (d.vy >= 0) {
            d.vx = b->lo.vx + p.vx;
            if (d.vx >= 0 && b->hi.vx - d.vx >= 0 && b->hi.vy - d.vy >= 0) {
                r = b->hi.vz - d.vz >= 0;
            }
        }
    }
    return r;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/HDP", hdpInCtrlBox);
#endif

void hdpSetHitTag(HdCsHit* r, s16 id0, s16 id1, HdpEnt* n)
{
    if (id0 == 0) {
        r->tag0 = n->id0;
        r->tag1 = n->id1;
    } else {
        r->tag0 = id0;
        r->tag1 = id1;
    }
}

#ifdef NON_MATCHING
HdCsHit* HdPntTest(s32 skip0, s32 skip1, VECTOR3* pt, s32* hitId)
{
    static HdCsHit tmpResult;
    Cs* cs;

    tmpResult.hit = 0;
    tmpResult.histIdx = -1;
    tmpResult.tag0 = -1;
    tmpResult.depth = 0;
    *hitId = 0;
    cs = csGetWorldCs();
    if (cs->epNode != 0) {
        hdpTraverse(cs, pt, &tmpResult);
        if (tmpResult.hit == 1) {
            *hitId = cs->unkC0;
        }
    }
    if (*hitId == 0) {
        for (cs = csGetCsList(); cs != NULL && *hitId == 0; cs = cs->next) {
            if (cs->drawMode != 0 && cs->unkC0 != skip0 && cs->unkC0 != skip1 && cs->unk0C != 0
                && cs->unkC0 > 0 && cs->unkC0 < 80
                && hdpIsPtInSphere(&cs->pos, cs->epNode->grp.radius, pt) != 0) {
                *hitId = cs->unkC0;
            }
        }
    }
    return &tmpResult;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/HDP", HdPntTest);
#endif

void hdpInit(HdpEnt* stack)
{
    hdpStack = stack;
}
