// VERSION SLPS_01144 info2 800EC5A8 1280
// JP version of PAL i0nfo2 800eaea8 (topdownPlayerMainStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void func_80020E38(TObj *o);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void func_800EBCAC(TObj *o);
extern void func_800EBE18(TObj *o);
typedef struct { char p[0x2C]; unsigned short w2c; } P330;
extern P330 *D_8009DEE0;
extern unsigned char D_8009E4E2;
extern unsigned char D_8009E51D;
extern unsigned char D_8009EE58[];
extern volatile unsigned short D_8009F200[];
extern void overheadCheckTileTrigger(TObj *o, short x, short y);
extern short (*D_800FFDE0[])[4];
extern unsigned short D_8009E508;
extern short D_800A7B8E;
extern struct { short pad; short whole; } *D_800A7BB8;

static __inline__ void setDir(TObj *o)
{
    D_8009EE58[0] = 1;
    o->animFrame &= 7;
    if (D_8009F200[0] & 0x1000) {
        if (D_8009F200[0] & 0x2000) o->animFrame = 1;
        else if (D_8009F200[0] & 0x8000) o->animFrame = 3;
        else o->animFrame = 2;
    } else if (D_8009F200[0] & 0x4000) {
        if (D_8009F200[0] & 0x2000) o->animFrame = 7;
        else if (D_8009F200[0] & 0x8000) o->animFrame = 5;
        else o->animFrame = 6;
    } else if (D_8009F200[0] & 0x8000) {
        o->animFrame = 4;
    } else if (D_8009F200[0] & 0x2000) {
        o->animFrame = 0;
    } else {
        o->animFrame |= 8;
    }
}

void func_800EC5A8(TObj *o)
{
    short i;
    short *e;
    short (*tb)[4];
    short *py;
    short px;
    unsigned short d;
    short l;
    int k = 0;
    *((unsigned char *)o + 0xD2) = 0;
    switch (o->step) {
    case 0:
        setDir(o);
        if (D_8009E4E2 != 0) {
            *(unsigned char *)&o->wac = 0;
            switch (o->state) {
            case 0:
                o->state++;
            case 1:
                overheadPlayerSetMoveAnim(o);
                func_80020E38(o);
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
                break;
            }
            if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C8) {
                *(unsigned char *)&o->wac = 1;
                o->animFrame |= 8;
                break;
            }
        }
        break;
    case 1:
        setDir(o);
        func_800EBCAC(o);
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
            break;
        }
        break;
    case 0x30:
        func_800EBE18(o);
        break;
    case 0x63:
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            D_8009DEE0->w2c = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
        }
        if (D_8009E51D == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            D_8009E4E2 = 1;
        }
        break;
    }
    overheadCheckTileTrigger(o, o->h->p.whole, o->y.p.whole);
    if (o->b68 != 0) {
        switch (D_8009E508) {
        case 5: k = 0; break;
        case 0xB: k = 1; break;
        case 0x10: k = 2; break;
        case 0x11: k = 3; break;
        case 0x13: k = 4; break;
        }
        i = 0;
        e = D_800FFDE0[k][0];
        if (e[0] != 0xFF) {
            py = &D_800A7B8E;
            px = D_800A7BB8->whole;
            tb = (void *)e;
        loop:
            d = px - e[0];
            l = e[2] - e[0];
            if ((unsigned short)d <= (short)l
                && (d = *py - e[1], l = e[3] - e[1], (unsigned short)d <= (short)l)) {
                goto found;
            }
            i++;
            e = tb[i];
            if (e[0] == 0xFF) return;
            goto loop;
        }
        return;
    found:
        o->b6b = i + 1;
        return;
    } else {
        o->b6b = 0;
    }
}
#endif
