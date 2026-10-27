// FUNC 8002f16c 536 MAIN0
// MATCHING 8002f16c 536
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void startPadVibration(int, int, int, int);
extern char D_80012228[];
extern char D_80012240[];
extern char D_80012260[];
static char *PTR_80079d58[7] = {
    (char *)D_80012228, (char *)19, (char *)4, (char *)D_80012240,
    (char *)19, (char *)4, (char *)D_80012260,
};
extern unsigned short DAT_1f8001f8;
extern unsigned char DAT_800a6039, DAT_800a6047;
extern char PLAYER, DAT_800a6104;
extern short DAT_800a6118;
extern unsigned short DAT_800a6066;
extern short *PLAYER_H;
extern short *PLAYER_D;
extern short PLAYER_Y;

void playerEffect4EStepA(TObj *o)
{
    unsigned char b;
    int s;
    Fix16 *f;
    short t;
    unsigned char v;
    switch (o->state) {
    case 0:
        o->anim = *(void **)(PTR_80079d58[o->subtype * 3] + o->b0c * 4);
        AnimLoadDuration(o);
        o->timer = 0xb4;
        o->state = o->state + 1;
    case 1:
        break;
    default:
        return;
    }
    if (o->b0c == 0 && (DAT_1f8001f8 & 3) == 0)
        startPadVibration(0, 0, 0xff, 2);
    AnimAdvance(o);
    o->timer = o->timer - 1;
    o->visible = DAT_800a6039;
    if (o->timer >= 0x3d) {
        if (o->b6b < 0x7f)
            b = o->b6b + 8;
        else
            b = o->b6b;
    } else {
        if (o->timer < 0) {
            if (DAT_800a6118 > 0)
                PLAYER = 3;
            else
                PLAYER = 1;
            DAT_800a6104 = 3;
            o->b6b = 0;
            o->b04 = 2;
            goto L;
        }
        if (o->b6b != 0)
            b = o->b6b - 2;
        else
            b = 0;
    }
    o->b6b = b;
L:
    s = PLAYER_H[1];
    f = o->h;
    if (DAT_800a6066 & 1)
        f->p.whole = s + 4;
    else
        f->p.whole = s - 4;
    o->y.p.whole = PLAYER_Y - 8;
    o->d->p.whole = PLAYER_D[1];
    switch (o->b0c) {
    case 0:
        b = DAT_800a6047 + 1;
        break;
    case 1:
        b = DAT_800a6047 - 1;
        break;
    default:
        return;
    }
    o->b0f = b;
}
