// FUNC 80043260 516 MAIN0
// MATCHING 80043260 516
typedef struct P { short x; unsigned short y; } P;
typedef struct S {
    char p0[0x14]; short w14; unsigned short z; char p1[0x40 - 0x18];
    P *pos; P *pos2; char p2[0x69 - 0x48]; unsigned char b69; char p3[0x6c - 0x6a];
    unsigned short wx; short rx; unsigned short wz; short rz; char p4[0x7e - 0x74]; short velY;
    char p5[0x9c - 0x80]; unsigned char b9c; char p6[0xa6 - 0x9d]; unsigned char ba6; char p7[0xb0 - 0xa7]; short wb0;
} S;
int func_80043260(S *a, S *b)
{
    short dx, dz, adx, px, pz, ex, ez;
    unsigned short sx;
    if ((unsigned short)(a->pos2->y - b->pos2->y + 0x2d) >= 0x5b)
        return 0;
    sx = b->wx + a->wx;
    px = sx;
    dx = a->pos->y - b->pos->y;
    adx = dx;
    if ((unsigned short)(dx + sx) > b->rx + a->rx)
        return 0;
    dz = a->z - b->z;
    pz = b->wz + a->wz;
    if ((unsigned short)(dz + pz) > a->rz + b->rz)
        return 0;
    ex = px;
    if (dx < 0) {
        adx = -dx;
        px = -sx;
    } else {
        px = (b->rx - b->wx) + (a->rx - a->wx);
        ex = px;
    }
    ez = pz;
    if (dz < 0) {
        dz = -dz;
        pz = -pz;
    } else {
        pz = (b->rz - b->wz) + (a->rz - a->wz);
        ez = pz;
    }
    if (ex - adx < ez - dz) {
        a->pos->y = b->pos->y + px;
        if (px < 0)
            a->ba6 = 2;
        else
            a->ba6 = 3;
        return 2;
    }
    if (pz <= 0) {
        unsigned short bz;
        if (a->b9c & 1)
            return 0;
        bz = b->z;
        a->wb0 = 0;
        a->w14 = 0;
        a->b69 = 1;
        a->velY = 0;
        a->z = bz + pz;
        b->b69 = 1;
        return 1;
    }
    a->z = b->z + pz;
    if (a->velY < 0)
        a->velY = 0;
    return 3;
}
