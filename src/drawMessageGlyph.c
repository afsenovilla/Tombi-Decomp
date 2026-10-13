// FUNC 8004dc10 380 MAIN0
// MATCHING 8004dc10 380
typedef struct { char p[0xc2]; unsigned short w2, w4; char q[4]; unsigned short idx; char r[2]; unsigned short wce, wd0; } O;
typedef struct { unsigned short id; short pad; short x, y; } E;
typedef struct { short n; short p[3]; } C;
extern C MESSAGE_SLOTS[];
extern E MESSAGE_GLYPHS[][128];
extern unsigned char DAT_800a5de6[];
extern int FUN_8002df70(int, int, int);

void drawMessageGlyph(O *o, unsigned short k)
{
    int n;
    int r;
    n = MESSAGE_SLOTS[o->idx].n++;
    r = FUN_8002df70(o->idx, k & 0xfff, 0);
    if ((k & 0x7000) == 0x4000) {
        MESSAGE_GLYPHS[o->idx][n].id = r | 0x4000;
        MESSAGE_GLYPHS[o->idx][n].x = o->w2 - 0x10;
        MESSAGE_GLYPHS[o->idx][n].y = o->w4 - 0x10;
    } else {
        MESSAGE_GLYPHS[o->idx][n].id = r;
        MESSAGE_GLYPHS[o->idx][n].x = o->wce;
        MESSAGE_GLYPHS[o->idx][n].y = o->wd0;
    }
    if ((k & 0x7000) == 0x5000 || (k & 0x7000) != 0x6000)
        o->wce += DAT_800a5de6[r * 10];
}
