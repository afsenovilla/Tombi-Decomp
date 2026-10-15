// FUNC 80028b84 92 MAIN0
// MATCHING 80028b84 92
typedef struct O { char p0[0x30]; short h30; short h32; } O;
extern short F2[];

void FUN_80028b84(O *o)
{
    if (F2[0] + F2[-6] < o->h30)
        F2[-6] = o->h30 - F2[0];
    else if (F2[0] + F2[-6] > o->h32)
        F2[-6] = o->h32 - F2[0];
}
