// FUNC 80046d58 1836 MAIN0
// MATCHING 80046d58 1836
#include "TOBJ.H"

extern unsigned short D_8009C960;
extern unsigned char D_8009D2C3;
extern short D_1F80024C;
extern TObj **D_1F800224;
extern short D_1F80019E;
void func_8004306C(TObj *o, TObj *e);
void func_80124EBC(TObj *o, TObj *e);
void func_80124F3C(TObj *o, TObj *e);
void func_80043EBC(TObj *o, TObj *e);
void func_80124FA4(TObj *o, TObj *e);
void func_8011FE08(TObj *o, TObj *e);
void func_80125044(TObj *o, TObj *e);
void func_801252B8(TObj *o, TObj *e);
void func_8011FBB8(TObj *o, TObj *e);
void func_801249E0(TObj *o, TObj *e);
void func_8011EEC0(TObj *o, TObj *e);
void func_8012030C(TObj *o, TObj *e);
void func_80124D10(TObj *o, TObj *e);
void func_80124FD4(TObj *o, TObj *e);
void func_80043D80(TObj *o, TObj *e);
void func_80125074(TObj *o, TObj *e);
void func_80124BA8(TObj *o, TObj *e);
void func_801250A4(TObj *o, TObj *e);
void func_801251B0(TObj *o, TObj *e);
void func_8012538C(TObj *o, TObj *e);
void func_80124EFC(TObj *o, TObj *e);
void func_80124F1C(TObj *o, TObj *e);
void func_80124058(TObj *o, TObj *e);
void func_8012047C(TObj *o, TObj *e);
void func_8011DF1C(TObj *o, TObj *e);
void func_80126CC4(TObj *o, TObj *e);
void func_8011F644(TObj *o, TObj *e);
void func_8011F5B8(TObj *o, TObj *e);
void func_8011F4DC(TObj *o, TObj *e);
void func_8011F310(TObj *o, TObj *e);
void func_8011F3F8(TObj *o, TObj *e);
void func_80120C88(TObj *o, TObj *e);
void func_8011E8E4(TObj *o, TObj *e);
void func_8011DEE8(TObj *o, TObj *e);
void func_801210E4(TObj *o, TObj *e);
void func_8011D370(TObj *o, TObj *e);
void func_8011C99C(TObj *o, TObj *e);
short func_80043260(TObj *o, TObj *e);
void func_80045BAC(TObj *o, TObj *e);
void func_80043C74(TObj *o, TObj *e);
void func_8011F3CC(TObj *o, TObj *e);

static __inline__ short hit(TObj *o, TObj *e)
{
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)(o->h->p.whole - e->h->p.whole + (o->box0 + e->box0)) > o->box1 + e->box1)
        return 0;
    if ((unsigned short)(o->y.p.whole - e->y.p.whole + (o->box2 + e->box2)) > o->box3 + e->box3)
        return 0;
    return 1;
}

void func_80046D58(TObj *o)
{
    TObj **pp;
    TObj *e;

    pp = D_1F800224;
    for (D_1F80019E = D_1F80024C; D_1F80019E != 0; ) {
        e = *pp++;
        D_1F80019E--;
        if (!(e->active & 1))
            continue;
        switch (e->type) {
        case 0: func_8004306C(o, e); break;
        case 1: func_80124EBC(o, e); break;
        case 2: func_80124F3C(o, e); break;
        case 3: func_80043EBC(o, e); break;
        case 4:
            if (D_8009C960 == 0)
                func_80124FA4(o, e);
            else
                func_8011FE08(o, e);
            break;
        case 5: func_80125044(o, e); break;
        case 6: func_801252B8(o, e); break;
        case 7:
            switch (D_8009C960) {
            case 0: func_801249E0(o, e); break;
            case 3: func_8011FBB8(o, e); break;
            case 4: func_8011EEC0(o, e); break;
            case 9: func_8012030C(o, e); break;
            }
            break;
        case 9: func_80124D10(o, e); break;
        case 10: func_80124FD4(o, e); break;
        case 0xb: func_80043D80(o, e); break;
        case 0xc: func_80125074(o, e); break;
        case 0x14: func_80124BA8(o, e); break;
        case 0x15: func_801250A4(o, e); break;
        case 0xe: func_801251B0(o, e); break;
        case 0xf: func_8012538C(o, e); break;
        case 0x17: func_80124EFC(o, e); break;
        case 0x18: func_80124F1C(o, e); break;
        case 0x12: func_80124058(o, e); break;
        case 0x1a:
            e->b69 = 0;
            if (hit(o, e)) {
                if (e->type != 0x26)
                    *(unsigned char *)&o->da0 = 4;
                e->b69 = 1;
            }
            break;
        case 0x1c: func_8012047C(o, e); break;
        case 0x1d: func_8011DF1C(o, e); break;
        case 0x23: func_80126CC4(o, e); break;
        case 0x28: func_8011F644(o, e); break;
        case 0x25: func_8011F5B8(o, e); break;
        case 0x26:
            e->b69 = 0;
            if (hit(o, e)) {
                if (e->type != 0x26)
                    *(unsigned char *)&o->da0 = 4;
                e->b69 = 1;
            }
            break;
        case 0x1f: func_8011F4DC(o, e); break;
        case 0x38:
            e->b69 = 0;
            if (hit(o, e)) {
                *(unsigned char *)&o->wa8 = 3;
                *(unsigned char *)&o->da0 = 1;
                e->b69 = 1;
            }
            break;
        case 0x24: func_8011F310(o, e); break;
        case 0x22: func_8011F3F8(o, e); break;
        case 0x33: func_80120C88(o, e); break;
        case 0x34: func_8011E8E4(o, e); break;
        case 0x3b: func_8011DEE8(o, e); break;
        case 0x31: func_801210E4(o, e); break;
        case 0x42: func_8011D370(o, e); break;
        case 0x43: func_8011C99C(o, e); break;
        case 0x32:
        case 0x44:
            e->b69 = 0;
            if (func_80043260(o, e) == 1)
                o->h->raw += e->velH << 8;
            break;
        case 0x45:
            switch (e->b0c) {
            case 3:
                if (D_8009C960 == 1 || (D_8009D2C3 & 0x40))
                    func_80045BAC(o, e);
                break;
            case 4:
                func_80043C74(o, e);
                break;
            default:
                func_80043260(o, e);
                break;
            }
            break;
        case 0x1e: func_8011F3CC(o, e); break;
        }
    }
}
