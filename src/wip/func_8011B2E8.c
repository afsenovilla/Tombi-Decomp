// FUNC 8011b2e8 456 X000
// wip score 2 (ncheck=matchcheck): only diff: game ends case-0 branch 1 with 'j; andi v0,v1,0xff' and a shared
// 'sh v0,0x74' tail (cross-jump); ours stores u (v1) directly. w-temp versions put lbu 0x6c above the sh (41).
#include "TOBJ.H"
extern short DAT_80138690[];
extern short DAT_80138698[];
extern unsigned short DAT_801386a0[];
extern void **PTR_80138680[];
extern int DAT_1f8002e4[];
extern short FUN_8005e420(int, int);
extern void FUN_80020078(TObj *, int);
extern void FUN_800187e4(TObj *);

void func_8011B2E8(TObj *o)
{
    unsigned char t = o->b04;
    unsigned char u;
    unsigned int v;
    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->w1e = 7;
        o->b0d = 0;
        if (DAT_80138698[o->subtype] != 0) {
            o->b0d = 1;
            o->w08 = FUN_8005e420(DAT_80138690[o->subtype], DAT_80138698[o->subtype] + o->b0c);
        }
        o->d3c = DAT_1f8002e4[0];
        u = *(unsigned char *)&o->box0 >> 4;
        o->anim = *PTR_80138680[o->subtype];
        if (u < 0xc) {
            o->d64 = u * 0x400 + 0x1000;
            o->w74 = u;
        } else {
            o->d64 = 0x1000 - (u - 0xb) * 0x200;
            o->w74 = -((u - 0xb) >> 1);
        }
        v = *(unsigned char *)&o->box0 & 0xf;
        if (v < 8)
            o->d8c = v << 2;
        else
            o->d8c = (unsigned char)(((int)(v - 7) * -0x20) / 8);
        break;
    case 1:
        FUN_80020078(o, (short)(DAT_801386a0[o->subtype] + o->w74 * 0x20));
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
