// FUNC 80113484 720 X000
/* score 6: only the DAT_80115a18 index block: game loads o->b0c (lbu) before DAT_8009c960 (lhu); ours the reverse.
   Tried operand order, temps (all int/short/uchar combos, both orders), 2D array, scalar/[0]/volatile alias, ternary.
   b23: sched order follows RTL luid (and-insn before sll-insn => lbu first) and `S*4 + idx` gives the game's ORDER
   but then the lbu chain gets $2 (score 14); `int k = S*4+idx` / `<< 2` give the game's REGS but lhu first (6).
   The same pattern on the anim table (pointer table, sum shifted again) matches. Also tried static __inline__
   get(s,i) with all param/return types, bitfield b0c, casts on every operand: none gives both. */
#include "TOBJ.H"
typedef void (*ObjFn)(TObj *);
typedef union { int w; struct { unsigned short a, b; } h; } Stage;
extern Stage DAT_8009c960;
extern Stage DAT_8009c960b;
extern Stage DAT_8009c960c;
extern unsigned char DAT_8009cf9d;
extern int DAT_1f8002e0[];
extern int DAT_1f8002d4[];
extern unsigned char DAT_80115a54[];
extern unsigned char DAT_80115a18[];
extern void **DAT_80115a08[];
extern void **DAT_80115948[];
extern ObjFn DAT_80115d90[];
extern ObjFn DAT_80115dac[];
extern ObjFn DAT_80115dc8[];
extern void FUN_8001fe6c(TObj *);
extern void FUN_80018790(TObj *);

void FUN_80113484(TObj *o)
{
    int ok;
    switch (o->b04) {
    case 0:
        o->w9a = 0;
        if (o->animFrame) {
            o->w9a = o->animFrame;
            o->animFrame = 0;
            ok = o->w9a == 1 && DAT_8009cf9d == 2;
            if (!ok) {
                o->b04 = 3;
                break;
            }
        }
        if (DAT_8009c960.w == 0x30009) {
            o->w1e = 10;
            o->d3c = DAT_1f8002e0[0];
        } else {
            if (DAT_8009c960.h.a == 1 && DAT_8009c960.h.b < 2)
                o->w1e = DAT_80115a54[o->b0c & 0x7f];
            else
                o->w1e = DAT_80115a18[(DAT_8009c960c.h.a << 2) + (o->b0c & 0x7f)];
            o->d3c = DAT_1f8002d4[0];
        }
        o->b04++;
        o->b0d = 0;
        o->b6a = o->b0f;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->box0 = 0x10;
        o->box1 = 0x20;
        if (o->b0c == 1) {
            o->box2 = 0x11;
            o->box3 = 0x21;
        } else {
            o->box2 = 0xd;
            o->box3 = 0x1d;
        }
        *(signed char *)&o->b0f = -3;
        o->wac = 0;
        if (DAT_8009c960b.w == 0x30009)
            o->anim = *DAT_80115a08[o->b0c & 0x7f];
        else
            o->anim = *DAT_80115948[DAT_8009c960c.h.a * 4 + (o->b0c & 0x7f)];
        FUN_8001fe6c(o);
        DAT_80115d90[o->subtype](o);
        break;
    case 1:
        DAT_80115dac[o->subtype](o);
        break;
    case 2:
        DAT_80115dc8[o->subtype](o);
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
