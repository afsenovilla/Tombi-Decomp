// FUNC 8001d4b0 396 MAIN0
// MATCHING 8001d4b0 396
#include "TOBJ.H"
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    u_long *vlcbuf[2];
    int vlcid;
    u_short *imgbuf[2];
    int imgid;
    RECT rect[2];
    int rectid;
    RECT slice;
    int isdone;
} DECENV;
typedef struct {
    u_short id;
    u_short type;
    u_short secCount;
    u_short nSectors;
    u_long frameCount;
    u_long frameSize;
    u_short width;
    u_short height;
} StHEADER;
extern u_long StGetNext(u_long **addr, u_long **header);
extern u_long StFreeRing(u_long *base);
extern void DecDCTvlc(u_long *bs, u_long *buf);
extern void CdMix(void *vol);
extern void FUN_8001eff8(int);
extern unsigned char DAT_1f8001cd;
extern TObj *DAT_1f8001d4;
extern short DAT_80077f10[];
extern char DAT_80077f0c[];

static __inline__ u_long *strNext(DECENV *dec)
{
    u_long *addr;
    StHEADER *sector;
    int cnt = 2000;

    while (StGetNext(&addr, (u_long **)&sector)) {
        if (--cnt == 0) return 0;
    }
    if (sector->frameCount >= DAT_80077f10[DAT_1f8001cd] - 3) {
        DAT_1f8001d4->w48 = 3;
        CdMix(DAT_80077f0c);
    }
    if (DAT_1f8001cd == 0x15 && DAT_1f8001d4->b68 == 0 && sector->frameCount > 14) {
        FUN_8001eff8(0);
        DAT_1f8001d4->b68 = 1;
    }
    dec->rect[0].w = dec->rect[1].w = sector->width;
    dec->rect[0].h = dec->rect[1].h = sector->height;
    dec->slice.h = sector->height;
    return addr;
}

u_long MovieDecodeFrame(DECENV *dec)
{
    u_long *next;
    int cnt = 2000;

    while ((next = strNext(dec)) == 0) {
        if (--cnt == 0) return -1;
    }
    dec->vlcid = 1 - dec->vlcid;
    DecDCTvlc(next, dec->vlcbuf[dec->vlcid]);
    return StFreeRing(next);
}
