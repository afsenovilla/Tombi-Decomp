// VERSION PCPX_96084 tomba 80022670 180
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Movie: next frame from the ring (PrePre 8 form of the frame fetch of MovieDecodeFrame.c); near the end of the
// movie sets state 3 and the CD mix; the frame size goes to D_800C8230 / D_800C8234 and the decode rectangles.
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
extern short *D_800DE0B0;
extern short D_800DE084;
extern short D_800A21F2[];
extern char D_800A2204[];
extern int D_800C8230, D_800C8234;
extern u_long StGetNext(u_long **addr, u_long **header);
extern void CdMix(void *vol);

u_long *func_80022670_pcpx96084(DECENV *dec_)
{
    u_long *addr;
    StHEADER *sector;
    register DECENV *dec asm("$16") = dec_;
    asm("");
    if (StGetNext(&addr, (u_long **)&sector) != 0) return 0;
    if (sector->frameCount >= D_800A21F2[D_800DE084 * 2]) {
        D_800DE0B0[0x48 / 2] = 3;
        CdMix(D_800A2204);
    }
    {
        int w = sector->width, h = sector->height;
        D_800C8230 = w;
        D_800C8234 = h;
        dec->rect[0].w = dec->rect[1].w = w;
        dec->rect[0].h = dec->rect[1].h = h;
        dec->slice.h = h;
    }
    return addr;
}
#endif
