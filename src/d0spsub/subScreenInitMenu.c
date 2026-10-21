// FUNC 800ec390 228 D0SPSUB
// MATCHING 800ec390 228
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p[0x10]; short w10; char p12[0xc]; short w1e; char p20[0x28]; void *p48, *p4c, *p50, *p54, *p58, *p5c, *p60, *p64; } Menu;
extern char D_800F18E0[], D_800F24E0[], D_800F7B78[], D_800FE8A4[], D_800FE8D8[], D_800FE8E4[], D_800FE948[], D_800FE974[], D_800FEC74[], D_800FE8D0[], D_800FE89C[];
extern void *D_1F8003A0, *D_1F8003A4;
extern void LoadImage(RECT *r, void *p);

void subScreenInitMenu(Menu *m)
{
    RECT r;
    r.x = 0x120;
    r.y = 0x1E0;
    r.w = 0x30;
    r.h = 0x1F;
    LoadImage(&r, D_800F18E0);
    D_1F8003A0 = D_800F24E0;
    D_1F8003A4 = D_800F7B78;
    m->p48 = D_800FE8A4;
    m->p4c = D_800FE8E4;
    m->p50 = D_800FE8D8;
    m->p54 = D_800FE8D0 + 8; /* == D_800FE8D8: the game reloads the address (a distinct constant) */
    m->p58 = D_800FE89C + 0x48; /* == D_800FE8E4 */
    m->p5c = D_800FE948;
    m->p60 = D_800FEC74;
    m->p64 = D_800FE974;
    m->w10 = 1;
    m->w1e = -1;
}
