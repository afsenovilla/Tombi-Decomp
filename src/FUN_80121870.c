// FUNC 80121870 492 X000
// MATCHING 80121870 492
#include "TOBJ.H"
extern unsigned int DAT_8009c96c;
extern int FUN_800202b4(TObj *);
extern void FUN_80121124(TObj *);
extern void FUN_8005a8a8(int, int, int);
extern int FUN_8005a9a4(int, int);
extern void FUN_80121ae0(TObj *);
extern void FUN_8012160c(TObj *);
extern void FUN_8012171c(TObj *);
extern void FUN_80018838(TObj *);

void FUN_80121870(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->b0a = 0x13;
        *(int *)((char *)o + 0x84) = 0;
        *(int *)((char *)o + 0x88) = 0;
        o->d8c = 0;
        o->timer = 0;
        o->w22 = 0;
        *(short *)((char *)o + 0x74) = 0xa00;
        *(short *)((char *)o + 0x76) = 0xa00;
        *(short *)((char *)o + 0x78) = 0xa00;
        o->b69 = 0;
        if (o->b0c == 0) {
            o->box0 = 12;
            o->box1 = 24;
            o->box2 = 12;
            o->box3 = 24;
        } else {
            o->active = 2;
        }
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b0c == 0) {
            FUN_80121124(o);
            switch (o->step) {
            case 0:
                if (o->b69 == 1)
                    o->step++;
                break;
            case 1:
                if (DAT_8009c96c <= 0xc34f) {
                    FUN_8005a8a8(0x10, 0, 0);
                    o->step--;
                } else {
                    FUN_8005a9a4(0x10, 0);
                    o->b04 = 2;
                    o->step = 0;
                    if (o->b0c == 0)
                        FUN_80121ae0(o);
                }
                break;
            }
            o->b69 = 0;
        } else {
            FUN_8012160c(o);
            o->b04 = ((TObj *)o->d90)->b04;
        }
        break;
    case 2:
        FUN_800202b4(o);
        if (o->b0c != 0)
            FUN_8012160c(o);
        FUN_8012171c(o);
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
