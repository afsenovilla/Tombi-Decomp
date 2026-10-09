// FUNC 8011a85c 1696 X001
/* score 170: control flow decoded. Left: w74 re-read (volatile gives lhu, game lh), BURST tails merged by
   cross-jumping where the game keeps them apart (w98 store scheduled between the arg loads), case 2 register use,
   case 3 b0c switch tree. */
#include "TOBJ.H"

extern short D_800A60B4[];
extern unsigned short D_8009C962[];
extern short D_8007A5F0[];
extern char D_8013C474[];
extern void playSFX(int);
extern int func_8011A72C(TObj *o);
extern void func_801191E4(int x, int y, int z);
extern TObj *func_8013675C(short k, short d, short n, short x, short y, short z);
extern TObj *func_80020D98(void *);

#define BURST() \
    func_801191E4(o->h->p.whole, o->y.p.whole, o->d->p.whole); \
    o->b6b = 1; \
    playSFX(D_8009C962[0] ? 0x49 : 0x44);

void func_8011A85C(TObj *o)
{
    TObj *e;

    switch (o->state) {
    case 0:
        o->y.raw = o->d34;
        o->h->raw = o->d30;
        o->w74 = 0;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->velV = 0x340;
        if (*(volatile short *)&o->w74 == 0) {
            o->d84 = 0;
            o->y.raw = o->d34;
            if (D_800A60B4[0] == 0) {
                o->animFrame = 4;
            } else {
                if ((o->animFrame >> 1) & 1) {
                    o->w74 = 0x200;
                    o->velH = 0x200;
                    o->velX = -0x10;
                } else {
                    o->w74 = -0x200;
                    o->velH = -0x200;
                    o->velX = 0x10;
                }
                o->ba6 = 0x20;
                o->timer = 2;
            }
        }
        o->state++;
        o->b6b = 0;
        if (D_8009C962[0] == 0) playSFX(0x42);
        else playSFX(0x47);
        break;
    case 1:
        if (!o->b6a) goto next;
        if ((o->animFrame >> 1) != 2 && func_8011A72C(o)) o->animFrame = 4;
        {
            unsigned char v = ((unsigned char *)&o->d88)[1];
            if (v > 0x80) o->y.raw += (D_8007A5F0[v] * (o->velV - 0x100)) >> 4;
            else o->y.raw += (D_8007A5F0[v] * o->velV) >> 4;
        }
        o->d88 += 0x280;
        if ((((unsigned)o->d88 >> 8) & 0xff) == 0) {
            o->y.raw = o->d34;
            if (D_8009C962[0] == 0) playSFX(0x43);
            else playSFX(0x48);
        }
        break;
    case 2:
        e = (TObj *)o->d94;
        o->d88 = 0;
        o->velV = ((o->d34 - e->y.raw) / 15) >> 8;
        o->velX = ((o->d30 - o->h->raw) >> 8) / 64;
        o->w74 = 0;
        o->active = 2;
        goto next;
    case 3:
        o->y.raw -= (D_8007A5F0[((unsigned char *)&o->d88)[1]] * o->velV) >> 4;
        if (o->b6a) o->y.raw -= (D_8007A5F0[((unsigned char *)&o->d88)[1]] * o->velV) >> 4;
        o->h->raw += o->velX << 8;
        o->d88 += 0x200;
        if (o->w76 < ((o->d88 >> 8) & 0xff)) {
            switch (o->b0c) {
            case 2:
                break;
            case 1:
                if (o->w98 == 0) {
                    e = func_80020D98(D_8013C474);
                    if (!e) {
                        o->w98 = 1;
                        break;
                    }
                    e->active = 5;
                    e->b0f = 3;
                    e->animFrame = 0;
                    e->h->raw = o->h->raw;
                    e->y.raw = o->y.raw;
                    e->d->raw = o->d->raw;
                    o->w98 = 1;
                    BURST();
                }
                break;
            case 3:
            case 4:
                if (o->w98 == 0) {
                    o->d90 = (int)func_8013675C(0, 0, o->b0c, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    o->w98 = 1;
                    if (o->d90) {
                        BURST();
                    }
                }
                break;
            }
        }
        if (((unsigned char *)&o->d88)[1] >= 0x41) {
            if (!o->b6b) {
                BURST();
            }
            o->active = 1;
            o->velH = 0;
            o->velX = 0;
            o->w74 = 0;
            o->d84 = 0;
            o->h->raw = o->d30;
            if (o->y.raw >= o->d34) {
                o->state++;
                o->y.raw = o->d34;
                o->h->raw = o->d30;
                o->velH = 0;
                o->velV = 0;
                o->velX = 0;
                o->velY = 0;
                o->d84 = 0;
                o->d88 = 0;
                o->w74 = 0;
                o->velV = 0x200;
            }
        }
        break;
    case 4:
        o->d88 += 0x400;
        {
            int v = o->d88 >> 8;
            if (v & 0x80) goto next;
            o->y.raw -= (D_8007A5F0[v & 0xff] * o->velV) >> 4;
        }
        if (o->b69) o->b69 = 0;
        if (o->b6a) o->state = 0;
        break;
    next:
        o->state++;
        break;
    case 5:
        o->y.raw = o->d34;
        o->h->raw = o->d30;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->w74 = 0;
        o->step = 0;
        o->state = 0;
        o->b68 = 0;
        break;
    }
}
