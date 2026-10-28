// FUNC 800f802c 112 X003
// MATCHING 800f802c 112
typedef struct O { char p0[6]; char st; char p1[0x24 - 7]; int anim; char p2[0x88 - 0x28]; int a; int b; char p3[0x9c - 0x90]; char c; char p4[0xaa - 0x9d]; char d; char p5[0xb0 - 0xab]; short idx; } O;
extern unsigned char SLOPE_TILT_TABLE[];
extern char DAT_80010c50[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(O *, int);
void playerStepEntrySfxAnim1State5(O *o)
{
    FUN_8001e5f4(0x1c, 0x7f);
    o->c = 0;
    o->d = 0;
    o->anim = (int)DAT_80010c50;
    FUN_8001fe94(o, 1);
    o->a = 0;
    {
        int v = SLOPE_TILT_TABLE[o->idx];
        o->st = 5;
        o->b = v;
    }
}
