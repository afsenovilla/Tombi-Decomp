/* score 25: two spots: case 6 substep 0 (game: lhu D_80125C9C; nop; sh w74 before lbu substep) and substep 1/2 tails (game cross-jumps the final sb substep of case 1 into case 2, with move a0 after the sra). Tried orders, raw stores, [0] arrays, goto set/n forms. */
// FUNC 8011a5fc 996 X014
#include "TOBJ.H"

extern unsigned short D_8009C962[];
extern short D_80125C80[];
extern unsigned short D_80125C70[];
extern short D_80125C9C, D_80125C9E;
extern void *D_80129F78;
extern int D_1F8002DCx[];
extern unsigned char D_8009C938;
extern TObj *D_8009C954;
extern short D_8007A5F0[], D_8007A1F0[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void func_8011A498(TObj *);
extern void func_80119B58(TObj *);

void func_8011A5FC(TObj *o)
{
    TObj *p;
    int v;
    int c, sn;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        v = D_80125C80[D_8009C962[0]];
        o->b0a = 9;
        o->w1e = 8;
        o->d8c = v;
        o->ba6 = 0;
        o->ba5 = 0;
        o->ba7 = 0;
        o->b6a = 0;
        o->b0d = 1;
        o->d38 = v;
        o->d30 = o->d8c;
        o->w08 = (D_80125C70[D_8009C962[0]] << 6) | 0xf;
        o->d3c = D_1F8002DCx[0];
        o->anim = D_80129F78;
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (D_8009C938) break;
        switch (D_8009C962[0]) {
        case 0:
            p = D_8009C954;
            if (!o->b6a && p) {
                o->d8c = ((0x1000 - p->d8c) >> 4) & 0xff;
                o->h->p.whole = p->a.p.whole;
                o->y.p.whole = p->y.p.whole;
            }
            break;
        case 2:
            p = D_8009C954;
            if (!o->b6a && p) {
                {
                    int i = (o->d30 + 1) & 0xff;
                    o->d30 = i;
                    c = D_8007A5F0[i];
                    sn = D_8007A1F0[i];
                }
                o->h->raw = p->h->raw + c * 0x700;
                o->y.raw = p->y.raw + sn * 0x700;
                o->d8c = (o->d30 + 0x80) & 0xff;
            }
            break;
        case 3:
            if (!o->b6a) o->d8c = (o->d8c - 1) & 0xff;
            break;
        case 4:
            if (!o->b6a) o->d8c = (o->d8c + 1) & 0xff;
            break;
        case 5:
            if (!o->b6a) {
                o->d8c = (o->d8c + 1) & 0xff;
                func_8011A498(o);
            }
            break;
        case 6:
            if (!o->b6a) {
                o->d8c = (o->d8c + 1) & 0xff;
                switch (o->substep) {
                case 0:
                    o->w74 = D_80125C9C;
                    o->substep++;
                    o->w76 = D_80125C9E;
                    break;
                case 1:
                    if (o->w74 < ++o->a.p.whole) {
                        o->a.p.whole = o->w74;
                        o->substep++;
                    }
                    break;
                case 2:
                    if (--o->a.p.whole < o->w76) {
                        o->a.p.whole = o->w76;
                        o->substep = 1;
                    }
                    break;
                }
            }
            break;
        case 1:
        case 7:
            if (!o->b6a) func_8011A498(o);
            break;
        default:
            return;
        }
        func_80119B58(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
