// FUNC 800eb394 376 X016
// MATCHING 800eb394 376
#include "TOBJ.H"
extern void FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001e560(int, int);
extern void FUN_800187e4(TObj *);

void FUN_800eb394(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        FUN_800202b4(o);
        if (o->visible) {
            switch (o->subtype) {
            case 0:
                switch (o->state) {
                case 0:
                    o->state++;
                    break;
                case 1:
                    FUN_8001fec0(o);
                    break;
                }
                break;
            case 1:
                switch (o->state) {
                case 0:
                    FUN_8001e560(0xe, 4);
                    o->state++;
                    break;
                case 1:
                    if (FUN_8001fec0(o))
                        o->b04 = 2;
                    break;
                }
                break;
            case 2:
                switch (o->state) {
                case 0:
                    FUN_8001e560(0xe, 0);
                    o->state++;
                    break;
                case 1:
                    if (FUN_8001fec0(o))
                        o->b04 = 2;
                    break;
                }
                break;
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
