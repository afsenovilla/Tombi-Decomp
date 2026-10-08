// FUNC 801363e4 1704 X000
// MATCHING 801363e4 1704
#include "TOBJ.H"
typedef struct { char p0[9]; unsigned char b9; } P801363E4;
typedef struct { short s0, s2; } S801363E4;

extern unsigned char DAT_8009c93f;
extern unsigned char DAT_800a603c[], DAT_800a603d, DAT_800a603e;
extern S801363E4 *DAT_800a6078;
extern unsigned char DAT_800a60a1[], DAT_800a60a2[], DAT_800a60a3[], DAT_800a60d4;
extern short DAT_800a6066, DAT_800a60b4, DAT_800a60b6, DAT_800a60b8, DAT_800a60ba, DAT_800a60ea;
extern P801363E4 *DAT_8009c330;
extern TObj *DAT_800a60c8;
extern unsigned char DAT_8009cebf[];
extern int DAT_8009c984;
extern short DAT_800a604a, DAT_800a604e, DAT_800a6052;
extern int DAT_800a60c4;
extern unsigned short *DAT_800a605c;
extern unsigned char DAT_8009d009;
extern void *D_8013B140[];
extern void *D_8013B130, *D_8013B134, *D_8013B138, *D_8013B13C,  *D_8013B148;
extern char D_801396C4[];

extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern TObj *FUN_8002dc50(int, int, int, int);
extern void FUN_800eb50c(int, int, int, int);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_8003c7c4(char *, Fix16 *, int, int, int);
extern void FUN_8001e4f0(int);
extern void FUN_80026c50(int, int, int);

void FUN_801363e4(TObj *o)
{
    TObj *q = *(TObj **)&o->category;
    Fix16 v[3];

    switch (o->step) {
    case 0:
        DAT_8009c93f = 1;
        if (DAT_800a603c[0] == 5) return;
        if (DAT_800a6078->s2 > 0x130) DAT_800a6078->s2 = 0x130;
        DAT_800a60a3[0] = 1;
        o->w08 = 100;
        DAT_800a6066 = 0;
        DAT_800a603c[0] = 5;
        DAT_800a603d = 0;
        DAT_800a603e = 0;
        o->step++;
        break;
    case 1:
        if (DAT_800a60a1[0] == 0) return;
        if (--o->w08 != 0) return;
        DAT_800a6066 = 0;
        DAT_8009c330->b9 = 5;
        DAT_800a60d4 = 1;
        DAT_800a60a1[0] = 0;
        DAT_800a60b4 = 0x220;
        DAT_800a603c[0] = 6;
        DAT_800a60ea = 0;
        DAT_800a60b8 = 0;
        DAT_800a60ba = 0;
        DAT_800a60b6 = 0;
        DAT_800a603d = 4;
        DAT_800a603e = 0;
        o->step++;
        break;
    case 2:
        switch (o->state) {
        case 0:
            if (DAT_800a60a1[0] == 0) return;
            DAT_800a603c[0] = 5;
            DAT_800a603d = 0;
            DAT_800a603e = 0;
            o->state++;
            break;
        case 1:
            if (DAT_800a6078->s2 < 0x259) return;
            if (DAT_800a60a1[0] == 0) return;
            q->anim = D_8013B148;
            FUN_8001fe6c(q);
            DAT_800a60c8 = FUN_8002dc50(7, DAT_8009cebf[0], 0xd0, 0x6c);
            q->anim = D_8013B134;
            FUN_8001fe6c(q);
            o->w08 = 0x30;
            o->state = 0;
            o->step++;
            break;
        }
        break;
    case 3:
        FUN_8001fec0(q);
        if (DAT_800a60c8->b04 != 2) return;
        switch (DAT_8009cebf[0]) {
        case 0:
            switch (o->state) {
            case 0:
                o->w08 = 0x20;
                q->anim = D_8013B140[0];
                FUN_8001fe6c(q);
                o->state++;
                break;
            case 1:
                q->h->p.whole++;
                if (--o->w08 != 0) return;
                DAT_800a603c[0] = 6;
                DAT_800a603d = 5;
                DAT_800a603e = 0;
                DAT_8009c330->b9 = 10;
                DAT_800a60b4 = -0x128;
                DAT_800a60a1[0] = 0;
                o->state++;
                break;
            case 2:
                if (DAT_800a60a1[0] == 0) return;
                DAT_800a603c[0] = 6;
                DAT_800a603d = 3;
                DAT_800a603e = 0;
                FUN_800eb50c(0, DAT_800a604a, (short)(DAT_800a604e - 8), DAT_800a6052);
                FUN_800eb50c(2, DAT_800a604a, (short)(DAT_800a604e - 8), DAT_800a6052);
                DAT_800a60c4 = 0;
                o->w08 = 400;
                o->state++;
                break;
            case 3:
                if (*DAT_800a605c == 0x3d) FUN_80025f40(0, 0, 0xff, 4);
                if (--o->w08 != 0) return;
                DAT_800a60c8->b04 = 3;
                DAT_8009cebf[0]++;
                DAT_800a60c8 = FUN_8002dc50(7, DAT_8009cebf[0], 0x120, 0x6c);
                q->anim = D_8013B138;
                FUN_8001fe6c(q);
                o->state = 0;
                break;
            }
            break;
        case 1:
            DAT_800a60c8->b04 = 3;
            DAT_800a60a2[0] = 0;
            DAT_8009cebf[0]++;
            o->w08 = 0x78;
            v[0].p.whole = q->a.p.whole;
            v[1].p.whole = q->y.p.whole;
            v[2].p.whole = q->b.p.whole;
            q->anim = D_8013B13C;
            FUN_8001fe6c(q);
            FUN_8003c7c4(D_801396C4, v, 0, 0, 1);
            o->step = 5;
            break;
        case 2:
            DAT_8009c984 |= 0x28;
            DAT_800a60c8->b04 = 3;
            o->w08 = 2;
            q->anim = D_8013B130;
            FUN_8001fe6c(q);
            DAT_8009cebf[0] = 3;
            o->b04 = 3;
            break;
        }
        break;
    case 4:
        FUN_8001fec0(q);
        if (--o->w08 != 0) return;
        DAT_8009cebf[0]++;
        DAT_800a60c8 = FUN_8002dc50(7, DAT_8009cebf[0], 0x120, 0x6c);
        q->anim = D_8013B138;
        FUN_8001fe6c(q);
        o->step = 3;
        break;
    case 5:
        FUN_8001fec0(q);
        if (o->w08 == 0x32) {
            FUN_8001e4f0(9);
            DAT_8009d009 = 1;
            FUN_80026c50(3, 1, 1);
        }
        if (--o->w08 != 0) return;
        DAT_800a60c8 = FUN_8002dc50(7, DAT_8009cebf[0], 0x120, 0x6c);
        q->anim = D_8013B138;
        FUN_8001fe6c(q);
        o->step = 3;
        break;
    }
}
