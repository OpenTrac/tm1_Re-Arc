#include "common.h"

#include "tm1/MATH.H"

void mathMulVec(MATRIX* lhs, VECTOR3* rhs, VECTOR3* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[0][1]) + (rhs->vz * lhs->m[0][2]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[1][0]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[1][2]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[2][0]) + (rhs->vy * lhs->m[2][1]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}

void mathMulTransVec(MATRIX* lhs, VECTOR3* rhs, VECTOR3* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[1][0]) + (rhs->vz * lhs->m[2][0]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[0][1]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[2][1]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[0][2]) + (rhs->vy * lhs->m[1][2]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}

// Identical to `mathMulVec`.
void mathMulVecLong(MATRIX* lhs, VECTOR3* rhs, VECTOR3* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[0][1]) + (rhs->vz * lhs->m[0][2]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[1][0]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[1][2]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[2][0]) + (rhs->vy * lhs->m[2][1]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}

void mathMulTransVecShort(MATRIX* lhs, SVECTOR* rhs, VECTOR3* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[1][0]) + (rhs->vz * lhs->m[2][0]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[0][1]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[2][1]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[0][2]) + (rhs->vy * lhs->m[1][2]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}

void mathNormalizeVec(VECTOR3* in, VECTOR3* out)
{
    s32 norm = SquareRoot0((in->vx * in->vx) + (in->vy * in->vy) + (in->vz * in->vz));
    if (norm > 0) {
        out->vx = (in->vx * FXP_ONE) / norm;
        out->vy = (in->vy * FXP_ONE) / norm;
        out->vz = (in->vz * FXP_ONE) / norm;
    } else {
        out->vx = 0;
        out->vy = 0;
        out->vz = 0;
    }
}
