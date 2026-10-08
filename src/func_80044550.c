// FUNC 80044550 204 MAIN0
// MATCHING 80044550 204
typedef struct P { short x; unsigned short y; } P;
typedef struct S {
    char p0[0x16]; unsigned short z; char p1[0x40 - 0x18];
    P *pos; P *pos2; char p2[0x6c - 0x48];
    unsigned short wx; short rx; unsigned short wz; short rz;
} S;
int func_80044550(S *a, S *b)
{
    short dx;
    if ((unsigned short)(a->pos2->y - b->pos2->y + 0x2d) >= 0x5b)
        return -1;
    dx = a->pos->y - b->pos->y;
    if ((unsigned short)(dx + (b->wx + a->wx)) > b->rx + a->rx)
        return -1;
    if ((unsigned short)((a->z - b->z) + (a->wz + b->wz)) <= a->rz + b->rz) {
        *(short *)0x1F80019E = 0;
        return dx >= 0;
    }
    return -1;
}
