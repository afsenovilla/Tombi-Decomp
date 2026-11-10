// VERSION SLPM_80165 slpm80165 8001F6E0 324
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of func_8001FE6C_slps01144 (src/func_8001FE6C_slps01144.c): 24-byte entries of the named file table.
typedef struct { short x, y, w, h; } RECT_;
extern unsigned short D_801FD8E0;
typedef struct { int a, b; char name[16]; } E8; /* 24-byte entries (the demo file table has names) */
extern E8 D_800725F4[];
extern E8 D_800725F8[];
extern int D_1F800290;
extern volatile unsigned D_1F800294;
#define DSTV D_1F800290
#define NSEC D_1F800294
int CdControl(int, void *, int);
int CdRead(int, void *, int);
int CdReadSync(int, void *);
int LoadImage(RECT_ *, void *);

int func_8001FE6C_slps01144(void *dst, short *hdr)
{
    RECT_ r;
    if (D_801FD8E0 != 0) return -1;
    {
        E8 *p = &D_800725F4[hdr[0]];
        DSTV = (int)dst;
        if (CdControl(2, p, 0)) {
            NSEC = (unsigned)(D_800725F8[hdr[0]].a + 2047) >> 11;
            CdRead(NSEC, (void *)DSTV, 0x80);
        }
        if (CdReadSync(0, 0) == -1) return -1;
        {
            if ((((unsigned char *)hdr)[3] & 0xf0) == 0x10) {
                r.x = hdr[4];
                r.y = hdr[5];
                r.w = hdr[6];
                r.h = hdr[7];
                LoadImage(&r, dst);
            }
            return D_800725F8[hdr[0]].a;
        }
    }
}
#endif
