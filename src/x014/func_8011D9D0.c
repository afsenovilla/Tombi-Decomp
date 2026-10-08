// FUNC 8011d9d0 320 X014
// MATCHING 8011d9d0 320
#include "TOBJ.H"
typedef struct {
    short wb4, wb6, wb8, wba, wbc;
    unsigned char bbe, bbf;
    short wc0, wc2, wc4, wc6;
} SB;
extern int D_1F8002D0;
extern int D_80125CC4[], D_80125CE4[], D_80125D04[];
extern unsigned short D_8009C962;
extern unsigned char D_8009CDA4[];
#define S16(o, x) (*(short *)((char *)(o) + (x)))
#define S32(o, x) (*(int *)((char *)(o) + (x)))
#define U8(o, x) (*(unsigned char *)((char *)(o) + (x)))

static __inline__ void setRank(SB *s)
{
    int i, n;

    i = 0;
    n = 0;
    for (; i < 0x100; i++) {
        if (D_8009CDA4[i] == 0xff) n++;
    }
    if (n < 0x33) i = 7;
    else if (n < 0x3d) i = 6;
    else if (n < 0x47) i = 5;
    else if (n < 0x51) i = 4;
    else if (n < 0x5b) i = 3;
    else if (n < 0x65) i = 2;
    else i = n < 0x6f;
    s->wc4 = i;
}

void func_8011D9D0(TObj *t)
{
    S16(t, 0xc0) = 0;
    S16(t, 0xc2) = 0;
    S16(t, 0xc4) = 0;
    S16(t, 0xc6) = 0;
    S16(t, 0x1e) = 4;
    S32(t, 0x3c) = D_1F8002D0;
    S16(t, 0xb4) = 0;
    S16(t, 0xae) = 0;
    S32(t, 0x84) = 0;
    S32(t, 0x88) = 0;
    S32(t, 0x8c) = 0;
    U8(t, 0xd) = 0;
    U8(t, 0x6a) = 0;
    S32(t, 0xa8) = D_80125CC4[U8(t, 2)];
    S32(t, 0x90) = D_80125CE4[U8(t, 2)];
    S32(t, 0x94) = D_80125D04[U8(t, 2)];
    S32(t, 0x24) = **(int **)((char *)t + 0xa8);
    if (D_8009C962 != 7) return;
    setRank((SB *)&t->wb4);
}
