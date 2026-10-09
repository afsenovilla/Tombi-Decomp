// FUNC 8012c9f0 1960 X004
// MATCHING 8012c9f0 1960
#include "TOBJ.H"

extern unsigned char D_8009CE62, D_8009CE63, D_8009D086[], D_8009C940, D_8009C941;
extern unsigned char D_8009C93F[], D_8009C942[];
extern unsigned char D_800A60E2, D_800A60D6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned short D_800A6066;
extern short D_1F80016E;
extern void *D_8013577C[], *D_80135784[];
extern short D_8007A5F0[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_8003e474(int, int, void *);

void func_8012C9F0(TObj *o)
{
    short y;

    switch (o->state) {
    case 0:
        if (D_8009CE62 != 0xff) {
            o->b04 = 3;
            break;
        }
        o->active = 2;
        o->box0 = 0x20;
        o->box1 = 0x40;
        o->b68 = 0;
        if (D_8009D086[0] == 0) {
            o->state = 1;
        } else if (D_8009D086[0] == 1) {
            o->state = 4;
        } else {
            o->b04 = 3;
        }
        break;
    case 1:
        if (D_8009C940 != 0 && D_8009C941 != 0x90) break;
        y = D_1F80016E;
        if (!((y < -0x40b && (D_800A60E2 || D_800A60D6)) || y == -0x450)) break;
        o->state++;
        D_8009D086[0] = 1;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->d90 = FUN_8002dcc8(3, 0, &o->a);
        o->wac = 2;
        o->animFrame = ~D_800A6066 & 1;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 2:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        if (D_8009CE63 == 0) {
            o->timer = 0x140;
            o->state++;
            FUN_8005a8a8(0xbf, 0, 0);
        } else {
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            o->wac = 0;
            o->anim = D_8013577C[0];
            o->state = 0;
        }
        break;
        }
    case 3:
        if (--o->timer != -1) break;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        o->wac = 0;
        o->anim = D_8013577C[0];
        o->state = 0;
        break;
    case 4:
        if (D_8009C940 != 0 && D_8009C941 == 0x90) {
            o->state = 6;
            break;
        }
        if (o->b68 == 0) break;
        o->state++;
        D_8009D086[0] = 1;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->d90 = FUN_8002dcc8(3, 0, &o->a);
        o->wac = 2;
        o->animFrame = o->b68 & 1;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 5:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wac = 0;
        o->anim = D_8013577C[0];
        o->state = 0;
        break;
        }
    case 6:
        o->state++;
        D_8009C940 = 0;
        FUN_80026e0c(0x90, 1);
        o->d90 = FUN_8002dcc8(3, 1, &o->a);
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wac = 2;
        o->animFrame = ~D_800A6066 & 1;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 7:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->anim = D_8013577C[0];
        break;
        }
    case 8:
        if (o->animFrame == 0) o->velH = -0x40;
        else o->velH = 0x40;
        o->d88 = 0;
        o->velV = 0x200;
        o->animFrame = 1 - o->animFrame;
        o->state++;
        break;
    case 9:
        o->h->raw += o->velH << 8;
        o->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
        o->d88 += 2;
        if (o->d88 < 0x60) break;
        o->wac = 2;
        o->state++;
        o->animFrame = 1 - o->animFrame;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 10:
        o->state++;
        o->d90 = FUN_8002dcc8(3, 2, &o->a);
        o->wac = 2;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 11:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->anim = D_8013577C[0];
        o->timer = 0x140;
        FUN_8005a9a4(0xbf, 0);
        break;
        }
    case 12:
        if (--o->timer == -1) o->state++;
        break;
    case 13:
        o->state++;
        o->d90 = FUN_8002dcc8(3, 3, &o->a);
        o->wac = 2;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 14:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->anim = D_8013577C[0];
        o->timer = 0x78;
        break;
        }
    case 15:
        if (--o->timer == -1) o->state++;
        if (o->timer == 0x64 || o->timer == 0x50) FUN_8003e474(0x37, 0, &o->a);
        break;
    case 16:
        o->state++;
        o->d90 = FUN_8002dcc8(3, 4, &o->a);
        o->wac = 2;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 17:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->animFrame = 1 - o->animFrame;
        o->anim = D_8013577C[0];
        break;
        }
    case 18:
        if (o->animFrame == 0) o->velH = 0x280;
        else o->velH = -0x280;
        o->velV = -0x380;
        o->velY = 0x40;
        o->state++;
        break;
    case 19:
        if (!o->visible) {
            o->state++;
            break;
        }
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        if (o->velV < 0x400) o->velV += o->velY;
        break;
    case 20:
        D_800A603C[0] = 1;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_8009D086[0] = 2;
        o->state = 0;
        break;
    }
}
