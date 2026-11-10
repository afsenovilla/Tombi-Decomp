// VERSION PCPX_96084 tomba 800926A0 108
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
typedef struct { unsigned char minute, second, sector, track; } CdlLOC;
extern void CdIntToPos(int, CdlLOC *);
extern int CdControl(int, void *, void *);
extern int CdRead(int, void *, int);
extern int CdReadSync(int, void *);

int func_800926A0_pcpx96084(int n, int sector, void *buf)
{
    CdlLOC loc;
    CdIntToPos(sector, &loc);
    CdControl(2, &loc, 0);
    CdRead(n, buf, 128);
    return CdReadSync(0, 0) == 0;
}
#endif
