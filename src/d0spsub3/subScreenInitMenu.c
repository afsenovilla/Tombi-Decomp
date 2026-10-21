// FUNC 800ec2e8 212 D0SPSUB3
// MATCHING 800ec2e8 212
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p[0x10]; short w10; char p12[0xc]; short w1e; char p20[0x28]; void *p48, *p4c, *p50, *p54, *p58, *p5c, *p60, *p64; } Menu;
extern char D_800EEA00[], D_800EF600[], D_800F5408[], D_800F543C[], D_800F5448[], D_800F54AC[], D_800F54D8[], D_800F57D8[], D_800F5434[], D_800F5400[];
extern void *D_1F8003A0, *D_1F8003A4;
extern void LoadImage(RECT *r, void *p);

void subScreenInitMenu(Menu *m)
{
    RECT r;
    r.x = 0x120;
    r.y = 0x1E0;
    r.w = 0x30;
    r.h = 0x1F;
    LoadImage(&r, D_800EEA00);
    D_1F8003A0 = D_800EF600;
    m->p48 = D_800F5408;
    m->p4c = D_800F5448;
    m->p50 = D_800F543C;
    m->p54 = D_800F5434 + 8; /* == D_800F543C: the game reloads the address (a distinct constant) */
    m->p58 = D_800F5400 + 0x48; /* == D_800F5448 */
    m->p5c = D_800F54AC;
    m->p60 = D_800F57D8;
    m->p64 = D_800F54D8;
    m->w10 = 1;
    m->w1e = -1;
}
