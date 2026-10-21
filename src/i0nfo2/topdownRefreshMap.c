// FUNC 800ed994 252 I0NFO2
// MATCHING 800ed994 252
#include "TOBJ.H"
#include "raw7.h"
#include "spad.h"
#define SPADP ((SPADBlock *)0x1F800000)
extern void DrawSync(int);
extern void topdownUploadMapStrip(int, int);
extern void topdownBuildVisibleTileList(TObj *);
extern unsigned char D_800A60A3;
extern int D_800FECB4;
extern short D_1F800176[];

void topdownRefreshMap(TObj *o)
{
    o->d30 = D_1F800176[0] + 0xA1;
    o->d34 = (short)SPADP->s17a - 0xEF;
    S16(o->h, 2) = 0x1F - (((unsigned short)D_1F800176[0] + 0xA0) & 0x1F);
    o->y.p.whole = 0x1F - ((SPADP->s17a - 0xF0) & 0x1F);
    topdownBuildVisibleTileList(o);
    topdownUploadMapStrip(0, D_800FECB4);
    DrawSync(0);
    topdownUploadMapStrip(1, D_800FECB4);
    topdownUploadMapStrip(2, D_800FECB4);
    topdownUploadMapStrip(3, D_800FECB4);
    topdownUploadMapStrip(4, D_800FECB4);
    if (D_800A60A3 == 0) {
        o->b6b = 0x80;
    } else {
        o->b6b = 0;
    }
}
