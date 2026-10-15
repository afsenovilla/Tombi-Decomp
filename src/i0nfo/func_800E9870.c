// FUNC 800e9870 1128 I0NFO
// MATCHING 800e9870 1128
#include "TOBJ.H"
extern void func_8001FEC0(TObj *o);
extern void func_800E88A0(TObj *o);
extern void func_800E8D60(TObj *o);
extern void func_800E9024(TObj *o);
extern void func_800E9188(TObj *o);
typedef struct { char p[0x2C]; unsigned short w2c; } P330;
extern P330 *D_8009C330;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C975;
extern unsigned char D_8009D00F;
extern unsigned char D_8009D2B0[];
extern volatile unsigned short D_8009D670[];

static __inline__ void setDir(TObj *o)
{
    D_8009D2B0[0] = 1;
    o->animFrame &= 7;
    if (D_8009D670[0] & 0x10) {
        if (D_8009D670[0] & 0x20) o->animFrame = 1;
        else if (D_8009D670[0] & 0x80) o->animFrame = 3;
        else o->animFrame = 2;
    } else if (D_8009D670[0] & 0x40) {
        if (D_8009D670[0] & 0x20) o->animFrame = 7;
        else if (D_8009D670[0] & 0x80) o->animFrame = 5;
        else o->animFrame = 6;
    } else if (D_8009D670[0] & 0x80) {
        o->animFrame = 4;
    } else if (D_8009D670[0] & 0x20) {
        o->animFrame = 0;
    } else {
        o->animFrame |= 8;
    }
}

void func_800E9870(TObj *o)
{
    *((unsigned char *)o + 0xE2) = 0;
    switch (o->step) {
    case 0:
        setDir(o);
        if (D_8009C93A != 0) {
            switch (o->state) {
            case 0:
                o->state++;
            case 1:
                if (D_8009D00F != 0) {
                    func_800E88A0(o);
                } else {
                    func_800E8D60(o);
                    func_8001FEC0(o);
                }
                if (!(o->animFrame & 8)) {
                    o->step = 1;
                    o->state = 0;
                }
                break;
            }
            if (*(unsigned char *)&o->wac >= 2) {
                *(unsigned char *)&o->wac = 0;
                o->step = 2;
                o->state = 0;
                return;
            }
            if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C4) {
                *(unsigned char *)&o->wac = 1;
                o->animFrame |= 8;
                return;
            }
            *(unsigned char *)&o->wac = 0;
        }
        break;
    case 1:
        setDir(o);
        func_800E9024(o);
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->animFrame |= 8;
            func_800E8D60(o);
            o->state++;
            break;
        case 1:
            if (D_8009D00F != 0) {
                func_800E88A0(o);
            } else {
                func_800E8D60(o);
                func_8001FEC0(o);
            }
            break;
        }
        break;
    case 0x30:
        func_800E9188(o);
        break;
    case 0x63:
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            D_8009C330->w2c = o->animFrame |= 8;
            func_800E8D60(o);
            o->state++;
        case 1:
            if (D_8009D00F != 0) {
                func_800E88A0(o);
            } else {
                func_800E8D60(o);
            }
            break;
        }
        if (D_8009C975 == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            D_8009C93A = 1;
        }
        break;
    }
}
