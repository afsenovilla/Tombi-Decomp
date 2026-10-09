// FUNC 80118c1c 396 X001
// MATCHING 80118c1c 396
#include "TOBJ.H"
extern int D_1F8002D4[];
extern short D_8013C428[];
extern short D_8013C404[];
extern void **D_8013C3C4[];
extern short FUN_8005e420(int, int);
extern int FUN_80020078(TObj *, int);
extern void ObjFree(TObj *);

void func_80118C1C(TObj *o)
{
    unsigned char v;

    switch (o->b04) {
    case 0:
        o->w1e = 9;
        o->b0d = 0;
        if (D_8013C428[o->subtype]) {
            o->b0d = 1;
            o->w08 = FUN_8005e420(D_8013C404[o->subtype], D_8013C428[o->subtype] + o->b0c);
        }
        o->d3c = D_1F8002D4[0];
        o->anim = *D_8013C3C4[o->subtype];
        o->b04++;
        v = *(unsigned char *)&o->box0 >> 4;
        if (v < 12)
            o->d64 = (v << 10) + 0x1000;
        else
            o->d64 = 0x1000 - ((v - 11) << 9);
        v = *(unsigned char *)&o->box0 & 0xf;
        if (v < 8)
            o->d8c = v << 2;
        else
            o->d8c = (unsigned char)(-((int)(v - 7) << 5) / 8);
        break;
    case 1:
        FUN_80020078(o, 0xf0);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
