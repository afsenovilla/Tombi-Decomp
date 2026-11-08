// VERSION SLPS_01144 optsub00 800EA418 284
#if defined(VERSION_SLPS_01144)
// JP optionsMenuDrawCursor (PAL o0ptsub0 800e8e44): one cursor table, no actuator branch
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { short v, p1, p2, p3; } RECTF;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
asm(".set SPT_,0x1f800000");
asm(".set NP_,0x1f800164");
asm(".set OT_,0x1f8001e0");
extern TILE SPT_;
extern char *NP_;
extern unsigned long *OT_;
/* the cursor rects {x, y, w, h}: retail names each field */
extern RECTF D_800EA948[], D_800EA94A[], D_800EA94C[], D_800EA94E[];

void func_800EA418(int sel)
{
    TILE *p;
    TILE *sp = (TILE *)0x1F800000;
    SPT_.len = 3;
    SPT_.code = 0x60;
    SPT_.g0 = 0xFF;
    SPT_.r0 = 0;
    SPT_.b0 = 0;
    SPT_.code |= 2;
    SPT_.x0 = D_800EA948[sel].v;
    SPT_.y0 = D_800EA94A[sel].v;
    SPT_.w = D_800EA94C[sel].v;
    SPT_.h = D_800EA94E[sel].v;
    p = (TILE *)NP_;
    *p = *sp;
    addPrim(OT_ + 6, p);
    NP_ += sizeof(TILE);
}
#endif
