// near-twin of PAL komaPigLeap (tools/jp_autofix.py)
// VERSION SLPS_01144 x03 80122250 1852
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
extern unsigned char D_8012F47C[];
extern A *D_8012FC1C[], *D_8012FBFC[], *D_8012FC00[], *D_8012FC20[], *D_8012FC24[], *D_8012FC28[];
extern char MOVE_TABLE_2[], MOVE_TABLE_1[], MOVE_TABLE_0[];
extern void func_8001E2D4(int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern short probeWallAt(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int func_8011E8DC_slps01144(TObj *);
static __inline__ void setanimbox(TObj *o, A *a)
{
    unsigned char *p;
    o->anim = a;
    p = D_8012F47C + a->w2 * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;
}
static __inline__ void wall(TObj *o)
{
    unsigned char *q = (unsigned char *)&o->wb4;
    if (o->movetab) applyAnimVelocityX(o, 1 - o->animFrame);
    if ((o->b9d & 2) && o->animFrame != (o->b9d & 1)) o->movetab = 0;
    if (o->animFrame == 1)
        *(short *)(q + 4) = 0x10;
    else
        *(short *)(q + 4) = -0x10;
    if (probeWallAt(o, o->h->p.whole + *(short *)(q + 4), o->y.p.whole, 1 - o->animFrame))
        o->movetab = 0;
}
void func_80122250(TObj *o)
{
    unsigned char *q = (unsigned char *)&o->wb4;
    switch (o->state) {
    case 0:
        func_8001E2D4(15);
        o->velV = -0x400;
        o->movetab = MOVE_TABLE_2;
        o->b9c = 1;
        o->wac = 0x21;
        o->b69 = 0;
        o->d8c = 0;
        {
            int st = o->state + 1;
            asm("" : "=r"(st) : "0"(st));
            o->animFrame = 1 - *(unsigned short *)((char *)o + 0x7a);
            o->state = st;
        }
        setanimbox(o, D_8012FC1C[0]);
    case 1:
        wall(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        wall(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (!func_8011E8DC_slps01144(o)) break;
        if (o->movetab == 0) {
            unsigned short w = *(unsigned short *)(q + 2);
            o->state = 4;
            o->timer = 360;
            o->wac = 0x1b;
            o->d8c = w;
            setanimbox(o, D_8012FC00[0]);
        } else {
            o->b9c = 1;
            o->active = 3;
            o->velV = -0x280;
            o->d8c = 0;
            o->movetab = MOVE_TABLE_1;
            o->state++;
            q[1] = 1;
            o->b69 = 0;
            o->wac = 0x1a;
            setanimbox(o, D_8012FBFC[0]);
        }
        break;
    case 3:
        if (o->animFrame)
            o->d8c = (o->d8c - 0x14) & 0xff;
        else
            o->d8c = (o->d8c + 0x14) & 0xff;
        wall(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (!func_8011E8DC_slps01144(o)) break;
        if (o->movetab == 0) {
            unsigned short w = *(unsigned short *)(q + 2);
            o->state = 4;
            o->timer = 360;
            o->wac = 0x1b;
            o->d8c = w;
            setanimbox(o, D_8012FC00[0]);
        } else {
            unsigned char c = q[1];
            unsigned short w;
            q[1] = c + 0xff;
            if (c) {
                o->velV = ~(o->velV - 0x80) + 1;
                if (o->velV < 0) o->b9c = 1;
                if (o->velV < -0x300) o->velV = -0x300;
                o->movetab = MOVE_TABLE_0;
                return;
            }
            w = *(unsigned short *)(q + 2);
            o->timer = 360;
            o->wac = 0x1b;
            o->d8c = w;
            o->state++;
            setanimbox(o, D_8012FC00[0]);
        }
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x22;
            o->state++;
            setanimbox(o, D_8012FC20[0]);
        }
        goto tail;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->active = 1;
            o->timer = 120;
            o->wac = 0x23;
            o->state++;
            setanimbox(o, D_8012FC24[0]);
        }
        goto tail;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x24;
            o->state++;
            setanimbox(o, D_8012FC28[0]);
        }
        goto tail;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            *(signed char *)&o->b0f = -9;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 0;
            o->substep = 6;
            o->b69 = 0;
            o->b68 = 0;
            o->b9c = 0;
        }
    tail:
        o->y.p.whole += 2;
        func_8011E8DC_slps01144(o);
        break;
    }
}
#endif
