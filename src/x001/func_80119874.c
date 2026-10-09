// FUNC 80119874 604 X001
// MATCHING 80119874 604
#include "TOBJ.H"
extern unsigned char D_8009D081;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C975;
extern unsigned char D_1F8001CC;
extern unsigned char D_1F8001CD;
extern short D_1F8001C6;
extern int D_1F8002D4;
extern void *D_8013E548;
extern char LAB_8001d6a4[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_800202b4(TObj *);
extern int func_80119750(TObj *);
extern void MusicStopOrFadeIn(int);
extern void ThreadCreate(int, void *);
extern void FUN_8001eb64(void);
extern void FUN_8005a8a8(int, int, int);
extern void ObjFree(TObj *);

void func_80119874(TObj *o)
{
    switch (o->b04) {
    case 0:
    {
        int d;
        void *a;
        o->b04++;
        *(short *)((char *)o + 0x1e) = 7;
        o->b0d = 0;
        d = D_1F8002D4;
        a = D_8013E548;
        *(signed char *)&o->b0f = -12;
        o->d3c = d;
        o->anim = a;
        AnimLoadDuration(o);
    }
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (D_8009D081 == 2)
                o->step++;
            break;
        case 1:
            if (AnimAdvance(o)) {
                o->timer = 0;
                o->step++;
            }
            FUN_800202b4(o);
            break;
        case 2:
            if (func_80119750(o)) {
                D_8009D081 = 3;
                o->step++;
            }
            FUN_800202b4(o);
            break;
        case 3:
            FUN_800202b4(o);
            o->step++;
            D_8009C93F = 1;
            D_8009C975 = 3;
            break;
        case 4:
            if (D_8009C975 == 1) {
                D_1F8001CC = 1;
                D_1F8001CD = 9;
                D_1F8001C6 = 2;
                MusicStopOrFadeIn(0);
                ThreadCreate(1, LAB_8001d6a4);
                o->step++;
            }
            break;
        case 5:
            D_8009D081 = 4;
            D_8009C975 = 4;
            FUN_8001eb64();
            o->step++;
            break;
        case 6:
            FUN_8005a8a8(0xa4, 0, 0);
            o->timer = 0x1e0;
            o->step++;
            break;
        case 7:
            if (--o->timer == -1)
                o->b04 = 3;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
