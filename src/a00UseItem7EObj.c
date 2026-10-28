// FUNC 80123c20 484 X000
// MATCHING 80123c20 484
/* v reused for GVARS_USE_ITEM_ID and the constant 8 puts 8 in v1 (game); store order found by permutation. */
// probably: dispatchAreaItemDraw area 0: an object that activates when GVARS_USE_ITEM_PENDING is set and GVARS_USE_ITEM_ID == 0x7E (anim PTR_8013b0d8)
#include "TOBJ.H"
extern unsigned char GVARS_USE_ITEM_PENDING[];
extern void *PTR_8013b0d8; extern unsigned char GVARS_USE_ITEM_ID;
extern int DAT_1f8002d4[];
extern unsigned char DAT_8009cd98, DAT_8009cd9c, DAT_800b146c, DAT_800b1474, DAT_800b1470;
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern int FUN_80063548(int);
extern void setEventComplete(int, int);
extern void printInfoMessage(int, int);
extern void freeObjectLayer5(TObj *);

void a00UseItem7EObj(TObj *o)
{
    int v;
    switch (o->b04) {
    case 0:
        if (GVARS_USE_ITEM_PENDING[0] != 0) {
            if ((v = GVARS_USE_ITEM_ID) == 0x7e) {
                GVARS_USE_ITEM_PENDING[0] = 0;
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
        setEventComplete(0x77, 0);
        printInfoMessage(0x32, 2);
        DAT_8009cd98 = 9;
        DAT_8009cd9c = 0x3c;
        DAT_800b146c = 0x3c;
        DAT_800b1474 = 1;
        DAT_800b1470 = 0x1e;
        o->b04++;
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
