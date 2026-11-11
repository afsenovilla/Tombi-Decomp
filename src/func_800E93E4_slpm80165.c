// VERSION SLPM_80165 x00 800E93E4 1328
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updateGroundDebris (src/updateGroundDebris.c, JP branch): the spawned piece's anim set and the
// life time do not depend on the frame parity (fixed 0x32), no area 7 case, area 9 pieces use 0xB.
#include "TOBJ.H"
extern unsigned short D_8009A810;
extern int D_1F8002D4[];
extern void *D_8012E204[];
extern void *D_8012F838[];
extern void *D_80119270[];
extern TObj *func_800170D0(void);
extern unsigned func_800200C8(void);
extern int func_8002099C(TObj *);
extern void func_80017464(TObj *);

void func_800E93E4(TObj *o)
{
    TObj *c;
    short v;
    int t;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (--o->w22 == -1) {
                o->w22 = func_800200C8() & 0xf;
                c = func_800170D0();
                if (c) {
                    c->active = 1;
                    c->type = 0xc;
                    c->subtype = 1;
                    c->wac = o->timer & 1;
                    c->b0c = func_800200C8() & 7;
                    *c->h = *o->h;
                    c->h->p.whole += o->timer * 2 - 8;
                    c->y = o->y;
                    *c->d = *o->d;
                }
                if (--o->timer == 0) {
                    o->subtype = 1;
                    o->wac = 0;
                    o->b0c = func_800200C8() & 7;
                }
            }
            break;
        case 1:
            o->b04++;
            *(volatile short *)&o->timer = 0x32;
            o->b0d = 0;
            *(signed char *)&o->b0f = -12;
            if (D_8009A810 == 0) {
                o->w1e = 9;
                o->anim = D_8012E204[o->wac];
            } else if (D_8009A810 == 1) {
                o->w1e = 8;
                o->anim = D_8012F838[o->wac];
            } else if (D_8009A810 == 9) {
                o->w1e = 0xb;
                o->anim = D_80119270[o->wac];
            }
            o->d3c = D_1F8002D4[0];
            switch (o->b0c) {
            case 0:
            case 1:
                o->velV = -0x180;
                o->b0a = 0;
                o->velY = 0x10;
                break;
            case 2:
            case 7:
                o->b0a = 2;
                o->velV = -0x180;
                o->velY = 0xc;
                o->d8c = -0x10;
                o->d84 = 4;
                break;
            case 3:
                o->b0a = 2;
                o->velV = -0x200;
                o->velY = 0x10;
                o->d8c = -0x18;
                o->d84 = 6;
                break;
            case 4:
                o->b0a = 2;
                o->velV = -0x80;
                o->d8c = -0x10;
                o->d84 = 4;
                break;
            case 5:
                o->b0a = 2;
                o->velV = -0x80;
                o->velH = -0x80;
                o->d8c = 0;
                break;
            case 6:
                o->b0a = 2;
                o->velV = -0x80;
                o->velH = 0x80;
                o->d8c = 0;
                break;
            }
            break;
        }
        break;
    case 1:
        if (func_8002099C(o) == 0)
            goto set3;
        switch (o->b0c) {
        case 0:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d->p.whole -= 2;
            break;
        case 1:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d->p.whole += 2;
            break;
        case 2:
        case 7:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d8c += o->d84;
            if (!((unsigned)(o->d8c + 0xf) < 0x1f))
                o->d84 = -o->d84;
            break;
        case 3:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d8c += o->d84;
            if (!((unsigned)(o->d8c + 0xf) < 0x1f))
                o->d84 = -o->d84;
            break;
        case 4:
            o->y.raw += o->velV << 8;
            t = o->d8c + o->d84;
            o->d8c = t;
            if ((unsigned)(t + 0xf) >= 0x1f)
                o->d84 = -o->d84;
            break;
        case 5:
            o->y.raw += o->velV << 8;
            o->h->raw += o->velH << 8;
            o->d8c += 4;
            break;
        case 6:
            o->y.raw += o->velV << 8;
            o->h->raw += o->velH << 8;
            o->d8c -= 4;
            break;
            break;
        }
    dec:
        if (--o->timer == 0) {
        set3:
            o->b04 = 3;
        }
        break;
    case 2:
        break;
    case 3:
        func_80017464(o);
        break;
    }
}
#endif
