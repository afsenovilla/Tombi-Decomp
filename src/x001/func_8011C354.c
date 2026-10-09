// FUNC 8011c354 544 X001
// MATCHING 8011c354 544
#include "TOBJ.H"

extern Fix16 *D_800A6078, *D_800A607C[];
extern int FUN_800201ac(TObj *, int);
extern void playSFX(int);
extern short MulCosDup(int, int);
extern short MulNegSin(int, int);
extern void ObjFreeDup(TObj *);

void func_8011C354(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 10;
        o->box2 = 10;
        o->box1 = 0x14;
        o->box3 = 0x14;
        break;
    case 1:
        switch (o->step) {
        case 0:
            o->step++;
            o->wb4 = 0;
            o->d8c = 0;
            o->b69 = 0;
        case 1:
            if (FUN_800201ac(o, 0x60)) {
                if (o->b69) {
                    if ((unsigned short)o->wb4 == 0) playSFX(0x4b);
                    if (o->d8c >= -0x6f) {
                        o->d8c -= (unsigned short)o->wb4 >> 8;
                        o->wb4 += 0x10;
                    }
                }
                { int t = MulCosDup(((o->d8c >> 4) + 0x40) & 0xff, 0x19a) + 0x10; o->d30 = o->h->p.whole + t; }
                o->d34 = o->y.p.whole + MulNegSin(((o->d8c >> 4) + 0x40) & 0xff, 0x19a);
                o->d38 = D_800A607C[0]->p.whole;
            }
            if (D_800A6078->p.whole < 0x53c) o->step++;
            break;
        case 2:
            if (FUN_800201ac(o, 0x24)) o->step = 0;
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
