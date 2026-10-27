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
extern int TOPDOWN_MAP_STRIP_BUFFER;
extern short SCREEN_ORIGIN_X[];

void topdownRefreshMap(TObj *o)
{
    o->d30 = SCREEN_ORIGIN_X[0] + 0xA1;
    o->d34 = (short)SPADP->s17a - 0xEF;
    S16(o->h, 2) = 0x1F - (((unsigned short)SCREEN_ORIGIN_X[0] + 0xA0) & 0x1F);
    o->y.p.whole = 0x1F - ((SPADP->s17a - 0xF0) & 0x1F);
    topdownBuildVisibleTileList(o);
    topdownUploadMapStrip(0, TOPDOWN_MAP_STRIP_BUFFER);
    DrawSync(0);
    topdownUploadMapStrip(1, TOPDOWN_MAP_STRIP_BUFFER);
    topdownUploadMapStrip(2, TOPDOWN_MAP_STRIP_BUFFER);
    topdownUploadMapStrip(3, TOPDOWN_MAP_STRIP_BUFFER);
    topdownUploadMapStrip(4, TOPDOWN_MAP_STRIP_BUFFER);
    if (D_800A60A3 == 0) {
        o->b6b = 0x80;
    } else {
        o->b6b = 0;
    }
}
