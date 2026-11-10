// VERSION PCPX_96084 tomba 80023E88 324
// FLAGS -O2 -G0 -fno-strength-reduce
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of JP func_8001FE6C (CD file load into VRAM): 0x18-byte file table entries, destination / sector count at 0x1F8003DC / 0x1F8003EC.
typedef struct { short x, y, w, h; } RECT_;
extern unsigned short D_801FC0E0;
typedef struct { int a, b, c, d, e, f; } E8;
extern E8 D_800E5D40[];
extern int D_800E5D44[];
extern int D_1F8003DC;
extern volatile unsigned D_1F8003EC;
#define DSTV D_1F8003DC
#define NSEC D_1F8003EC
int CdControl(int, void *, int);
int CdRead(int, void *, int);
int CdReadSync(int, void *);
int LoadImage(RECT_ *, void *);

int func_80023E88_pcpx96084(void *dst, short *hdr)
{
    RECT_ r;
    if (D_801FC0E0 != 0) return -1;
    {
        E8 *p = &D_800E5D40[hdr[0]];
        DSTV = (int)dst;
        if (CdControl(2, p, 0)) {
            NSEC = (unsigned)(D_800E5D44[hdr[0] * 6] + 2047) >> 11;
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
            return D_800E5D44[hdr[0] * 6];
        }
    }
}
#endif
