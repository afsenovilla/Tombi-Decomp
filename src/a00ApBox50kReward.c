// FUNC 8012171c 340 X000
// MATCHING 8012171c 340
#include "TOBJ.H"
extern char A00_AP_BOX_50K_ITEM_ENTRY[];
extern void spawnItemFromEntry(void *, void *, int, int, TObj *);

void a00ApBox50kReward(TObj *o)
{
    switch (o->step) {
    case 0:
        o->wb4 = 0x3c;
        o->step++;
        break;
    case 1:
        if (--*(unsigned short *)&o->wb4 == 0xffff)
            o->step++;
        break;
    case 2:
        o->wb4 = 0x78;
        o->step++;
        break;
    case 3:
        if (o->b0c == 0) {
            if (*(unsigned short *)&o->wb4 == 0x78)
                spawnItemFromEntry(A00_AP_BOX_50K_ITEM_ENTRY, &o->a, 0x20, -0x400, o);
            else if (*(unsigned short *)&o->wb4 == 0x5a)
                spawnItemFromEntry(A00_AP_BOX_50K_ITEM_ENTRY, &o->a, -0x20, -0x400, o);
        }
        if (--*(unsigned short *)&o->wb4 == 0xffff) {
            o->wb4 = 0x5a;
            o->step++;
        }
        break;
    case 4:
        if (!((*(unsigned short *)&o->wb4 >> 1) & 1))
            o->visible = 0;
        if (--*(unsigned short *)&o->wb4 == 0xffff)
            o->b04 = 3;
        break;
    }
}
