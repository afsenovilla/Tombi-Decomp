/* v reused for D_8009C941 and the constant 8 puts 8 in v1 (game); store order found by permutation. */
// MATCHING 80123c20 484
// FUNC 80123c20 484 X000
#include "TOBJ.H"
extern unsigned char DAT_8009c940[];
extern void *PTR_8013b0d8; extern unsigned char D_8009C941;
extern int DAT_1f8002d4[];
extern unsigned char DAT_8009cd98, DAT_8009cd9c, DAT_800b146c, DAT_800b1474, DAT_800b1470;
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern int FUN_80063548(int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8004d620(int, int);
extern void FUN_800188e0(TObj *);

void func_80123C20(TObj *o)
{
    int v;
    switch (o->b04) {
    case 0:
        if (DAT_8009c940[0] != 0) {
            if ((v = D_8009C941) == 0x7e) {
                DAT_8009c940[0] = 0;
                v = 8;
                o->box0 = v;
                o->box2 = v;
                o->box1 = 0x10;
                o->active = 1;
                o->b69 = 0;
                o->box3 = 0x10;
                o->b0a = 0;
                o->b0d = 1;
                o->w1e = 6;
                o->w08 = 0x7c0f;
                o->anim = PTR_8013b0d8;
                o->d3c = DAT_1f8002d4[0];
                o->d30 = o->a.p.whole;
                o->w22 = 0;
                FUN_8001fe6c(o);
                o->b04++;
            }
        } else {
            o->active = 2;
        }
        break;
    case 1:
        if (FUN_800202b4(o))
            FUN_8001fec0(o);
        o->a.p.whole = o->d30 + (FUN_80063548(o->w22 & 0xfff) >> 7);
        if ((o->w22 & 0xfff) > 0x800)
            o->animFrame = 0;
        else
            o->animFrame = 1;
        o->w22 += 0x10;
        break;
    case 2:
        FUN_8005a9a4(0x77, 0);
        FUN_8004d620(0x32, 2);
        DAT_8009cd98 = 9;
        DAT_8009cd9c = 0x3c;
        DAT_800b146c = 0x3c;
        DAT_800b1474 = 1;
        DAT_800b1470 = 0x1e;
        o->b04++;
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
