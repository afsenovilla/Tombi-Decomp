// VERSION SLPM_80165 slpm80165 80015340 184
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only (debug build): appends a mark to the CPU-time bar list D_800A36F0 (6-byte entries: root counter 2
// time, then a colour), count D_8009B3F0.
typedef struct { unsigned short t; unsigned char r, g, b; char pad; } Mark;
extern Mark D_800A36F0[];
extern int D_8009B3F0;
extern int GetRCnt(int);

void func_80015340(int r, int g, int b)
{
    D_800A36F0[D_8009B3F0].r = r;
    D_800A36F0[D_8009B3F0].g = g;
    D_800A36F0[D_8009B3F0].b = b;
    {
        unsigned short c = GetRCnt(0xf2000001);
        D_800A36F0[D_8009B3F0].t = c;
        D_8009B3F0++;
    }
}
#endif
