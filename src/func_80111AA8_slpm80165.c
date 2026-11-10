// VERSION SLPM_80165 x00 80111AA8 556
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updateA00SporeCloud (src/updateA00SporeCloud.c, the beta / JP branch): a fixed +0x08 value
// 0x780E instead of a random one, no pig stun, and a culled cloud just waits (no jump to the free step).
#include "TOBJ.H"
extern int func_8005453C(void);
extern void func_80020554(TObj *);
extern int func_8002099C(TObj *);
extern int func_800205A8(TObj *);
extern void func_80017464(TObj *);
extern int D_1F8002D4[];
extern void *D_8012E194[];

void func_80111AA8(TObj *o)
{
    short q;
    unsigned char k;

    k = o->b04;
    switch (k) {
    case 0:
        o->wb4 = 1;
        o->wb6 = 0x80;
        o->wb8 = 0;
        o->wba = 0;
        if (o->subtype == 0)
            o->w1e = (func_8005453C() == 1 || func_8005453C() == 2) ? 0xa : 0xa;
        else
            o->w1e = (func_8005453C() == 1 || func_8005453C() == 2) ? 0x8a : 0x2a;
        o->w08 = 0x780e;
        o->b0d = 0x80;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8012E194[o->subtype];
        func_80020554(o);
        o->timer = 0x38;
        o->w22 = 0;
        o->b04++;
        break;
    case 1:
        if (func_8002099C(o) == 0)
            break;
        if (o->subtype == k) {
            if (--o->timer == 0) {
                o->timer++;
                if (*(unsigned short *)&o->wb6 != 0) o->wb6--;
                o->w22 += 2;
                q = o->w22 / 10;
                if (q > 0) {
                    o->w22 %= 10;
                    o->wb8++;
                    o->wba++;
                }
            }
        }
        if (func_800205A8(o)) o->b04++;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80017464(o);
        break;
    }
}
#endif
