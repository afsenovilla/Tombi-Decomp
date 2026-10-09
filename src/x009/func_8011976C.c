// FUNC 8011976c 368 X009
// MATCHING 8011976c 368
#include "TOBJ.H"
extern int D_1F8002D4[];
extern unsigned short D_8012ABB4[];
extern unsigned short D_8012ABAC[];
extern void **D_8012ABA0[];
extern int FUN_800201ac(TObj *, int);
extern void ObjFree(TObj *);

void func_8011976C(TObj *o)
{
    unsigned int v;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 10;
        o->b0d = 1;
        o->w08 = ((D_8012ABB4[o->subtype] + o->b0c) << 6) | ((D_8012ABAC[o->subtype] >> 4) & 0x3f);
        o->d3c = D_1F8002D4[0];
        o->anim = *D_8012ABA0[o->subtype];
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
        FUN_800201ac(o, 200);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
