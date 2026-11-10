// VERSION SLPM_80165 x00 80109CB4 356
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only player step dispatcher (by +0x05); step 3 is func_80052160_pcpx96084 (PrePre 8) inline: clear the
// motion state, anim 0x1E (PlayerSetAnimIfChanged inline), then advance it. Steps 3-5 keep only the facing bit
// of +0x2E; steps 3 and 5 clear +0x8C / +0x84 afterwards.
#include "TOBJ.H"
typedef struct { char p0[8]; unsigned char b8; char p1[0x2c - 9]; unsigned short cur, prev; } PX;
extern PX *D_800893C8;
extern void func_800F64C4(TObj *), func_800EDA04(TObj *), func_80020554(TObj *), func_800205A8(TObj *);
extern void func_80105FD4(TObj *), func_80106544(TObj *), func_800F4A54(TObj *);

void func_80109CB4(TObj *o)
{
    switch (o->step) {
    case 0:
        func_800F64C4(o);
        break;
    case 1:
    case 2:
        break;
    case 3:
        o->animFrame &= 1;
        switch (o->state) {
        case 0:
            o->d8c = 0;
            o->wb0 = 0;
            D_800893C8->b8 = 0;
            o->b69 = 0;
            *(unsigned char *)&o->wac = 0;
            o->velY = 0;
            o->velV = 0;
            o->animFrame &= 1;
            D_800893C8->cur = 0x1e;
            if (D_800893C8->prev != 0x1e) {
                func_800EDA04(o);
                func_80020554(o);
                D_800893C8->prev = D_800893C8->cur;
            }
            o->state++;
        case 1:
            func_800205A8(o);
            break;
        }
        o->d8c = 0;
        o->d84 = 0;
        break;
    case 4:
        o->animFrame &= 1;
        func_80105FD4(o);
        break;
    case 5:
        o->animFrame &= 1;
        func_80106544(o);
        o->d8c = 0;
        o->d84 = 0;
        break;
    case 6:
        func_800F4A54(o);
        break;
    }
}
#endif
