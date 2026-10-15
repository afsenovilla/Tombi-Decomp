// FUNC 8012f274 220 X004
// MATCHING 8012f274 220
#include "gcd.h"
#include "TOBJ.H"
extern void bigKeyPaintingCheckPiece(TObj *);
extern void paintingBigKeyReward(TObj *);

void stepBigKeyPainting(TObj *o)
{
    short i;
    switch (o->step) {
    case 0:
        bigKeyPaintingCheckPiece(o);
        break;
    case 1:
        if (o->state == 0) {
            o->step = 0;
            o->state = 0;
            if (GCD.bcddc != 0xff) {
                for (i = 0; i < 5; i++) {
                    if (((unsigned char*)&GCD.bcf24)[i] == 0) return;
                }
                o->step = 2;
            }
        }
        break;
    case 2:
        paintingBigKeyReward(o);
        break;
    }
}
