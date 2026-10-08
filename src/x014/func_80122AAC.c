// FUNC 80122aac 828 X014
// MATCHING 80122aac 828
#include "TOBJ.H"
extern unsigned char D_8009C942;
extern unsigned short D_8009C962;
extern unsigned short D_1F8001F8;
extern TObj *ObjAlloc(void);
extern int func_800202B4(TObj *);
extern void playSFX(int);
extern void func_80118314(int, int, int);
extern void func_80116900(int, int);

void func_80122AAC(TObj *o)
{
    int i;
    TObj *n;

    if (D_8009C942 == 1) {
        if (o->visible) func_800202B4(o);
        return;
    }
    {
        switch (o->state) {
        case 0:
            o->timer = 0x3c;
            o->state++;
            break;
        case 1:
            if (--o->timer == -1) {
                o->visible = 1;
                o->timer = 8;
                o->velV = ((o->velY << 16) - o->y.raw) >> 11;
                o->state++;
            }
            break;
        case 2:
            o->d88 = (o->d88 + 0x80) & 0xfff;
            o->y.raw += o->velV << 8;
            if (--o->timer == 0) {
                o->state++;
                if (D_8009C962 == 7) o->timer = 0x3c;
                else o->timer = 0x1e;
                for (i = 0; i < 2; i++) {
                    n = ObjAlloc();
                    if (n != 0) {
                        n->active = 2;
                        n->type = 0x25;
                        n->animFrame = i & 1;
                        n->a.raw = o->a.raw;
                        n->y.raw = o->y.raw;
                        n->b.raw = o->b.raw;
                    }
                }
                func_80118314(o->a.p.whole, o->y.p.whole, o->b.p.whole);
                if (D_8009C962 == 7) playSFX(0xf6);
                else playSFX(0xea);
            }
            break;
        case 3:
            o->d88 = (o->d88 + 0x80) & 0xfff;
            if (D_1F8001F8 & 1) o->visible = 1;
            else o->visible = 0;
            if (--o->timer == 0) {
                if (D_8009C962 == 7) {
                    o->state = 5;
                } else {
                    o->timer = 0x3c;
                    o->state++;
                }
            }
            break;
        case 4:
            o->subtype = 1;
            o->visible = 1;
            o->da0 = 0;
            if (D_1F8001F8 & 1) func_80116900(o->wac, 0);
            else func_80116900(o->wac, 1);
            if (--o->timer == 0) o->state++;
            break;
        case 5:
            o->visible = 0;
            if (D_8009C962 != 7) func_80116900(o->wac, 0);
            o->active = 2;
            o->step = 0;
            o->b04++;
            break;
        }
    }
    if (o->visible) func_800202B4(o);
}
