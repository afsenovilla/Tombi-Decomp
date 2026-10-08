// FUNC 80118438 512 X000
// MATCHING 80118438 512
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } SV;
typedef struct { short v[8]; } T16;
extern T16 DAT_80138644[];
extern void *DAT_8013b23c[];
extern int DAT_1f8002d4[];
extern short DAT_1f80017e;
short FUN_8005e420(int a, int b);
void FUN_80118638(TObj *o);
void FUN_8011899c(TObj *o);
void FUN_800187e4(TObj *o);

void FUN_80118438(TObj *o)
{
    SV *v;
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 9;
        o->b0d = 0x81;
        o->w08 = FUN_8005e420(0xc0, o->subtype + 0x1f0);
        o->b0a = 0x11;
        o->anim = DAT_8013b23c[o->subtype];
        o->d3c = DAT_1f8002d4[0];
        v = (SV *)((char *)o + 0xb4);
        v[0].x = DAT_80138644[o->subtype].v[0];
        v[0].y = DAT_80138644[o->subtype].v[1];
        v[0].z = 0;
        v[1].x = DAT_80138644[o->subtype].v[2];
        v[1].y = DAT_80138644[o->subtype].v[3];
        v[1].z = 0;
        v[2].x = DAT_80138644[o->subtype].v[4];
        v[2].y = DAT_80138644[o->subtype].v[5];
        v[2].z = 0;
        v[3].x = DAT_80138644[o->subtype].v[6];
        v[3].y = DAT_80138644[o->subtype].v[7];
        v[3].z = 0;
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            FUN_80118638(o);
            break;
        case 1:
            o->d->p.whole = DAT_1f80017e;
            FUN_8011899c(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
