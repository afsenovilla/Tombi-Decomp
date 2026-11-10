// VERSION PCPX_96084 tomba 8003E6B4 296
// FLAGS -O2 -G0 -fno-strength-reduce
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of JP x001 func_8011D2F0: segment list at +0x9C, drawn and stepped only while a global word +0x4E is below 1100.
#include "TOBJ.H"

extern void ObjListPush_1F800228(TObj *);
extern void func_8001A8C8_pcpx96084(TObj *);
extern short *D_800DE544;
extern unsigned short D_1F8002BA;
extern void func_8003E7DC(TObj *, int);

void func_8003E6B4_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    short n;
    short *p;

    switch (s) {
    case 0:
        p = *(short **)&o->b9c;
        p += 2;
        n = *p;
        p += 2;
        do {
            p[17] = 0;
            n--;
            p += 26;
        } while (n);
        o->b68 = 0;
        o->b6b = 0;
        o->b6a = 0;
        o->velV = 0;
        o->d34 = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        if (D_800DE544[0x4e / 2] < 1100) {
            o->visible = 1;
            ObjListPush_1F800228(o);
            func_8003E7DC(o, (short)(D_1F8002BA - (unsigned short)o->a.p.whole) >> 4);
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_8001A8C8_pcpx96084(o);
        break;
    }
}
#endif
