// FUNC 801169c0 2528 X002
// MATCHING 801169c0 2528
#include "spad.h"
#include "gcd.h"
#include "gv2.h"
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short idx;
    void **tab;
} Ent;
extern Ent D_8011C6D8[];
extern int SPR_DATA[];
extern short FUN_8005e420(int, int);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

static __inline__ void sa(TObj *o, int n)
{
    if (o->subtype == 1) {
        o->w1e = 2;
        o->anim = D_8011C6D8[1].tab[n];
        FUN_8001fe94(o, 0);
    } else {
        o->anim = D_8011C6D8[o->subtype].tab[n];
        FUN_8001fe94(o, 0);
    }
}

#define PICK(c) if (c) sa(o, 0); else sa(o, 2); break

void updateSevenFriendsBackdropSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->animFrame = 1;
        o->b0a = 7;
        o->b0d = 1;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->w08 = FUN_8005e420(0x80, 0x1ff);
        o->w1e = D_8011C6D8[o->subtype].w1e;
        o->d3c = SPR_DATA[D_8011C6D8[o->subtype].idx];
        if (o->subtype == 1) {
            if (GCD.bce41 == 0) {
                o->w1e = 5;
                o->anim = D_8011C6D8[9].tab[0];
                FUN_8001fe94(o, 0);
            } else if (GCD.bce41 == 0xff) {
                sa(o, 2);
            } else {
                sa(o, 0);
            }
        } else if (GCD.bce41 == 0) {
            switch (o->subtype) {
            case 2: PICK(GV2.bc3 & 2);
            case 3: PICK(GV2.bc3 & 0x10);
            case 4: PICK(GV2.bc3 & 0x40);
            case 5: PICK(GV2.bc3 & 4);
            case 6: PICK(GV2.bc3 & 1);
            case 7: PICK(GV2.bc3 & 0x20);
            case 8: PICK(GV2.bc3 & 8);
            default: goto skip;
            }
        } else {
            switch (o->subtype) {
            case 2: PICK(GCD.bcfdb);
            case 3: PICK(GCD.bcfdc);
            case 4: PICK(GCD.bcfdd);
            case 5: PICK(GCD.bcfde);
            case 6: PICK(GCD.bcfdf);
            case 7: PICK(GCD.bcfe0);
            case 8: PICK(GCD.bcfe1);
            default: goto skip;
            }
        }
    skip:
        o->b04++;
        break;
    case 1:
        o->a.p.whole = o->d30 - (*(unsigned short*)((char*)&SPAD.i174 + 2));
        o->y.p.whole = o->d34 - ((unsigned short*)&SPAD.s186)[0];
        o->visible = 1;
        FUN_80018ca4(o);
        FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
