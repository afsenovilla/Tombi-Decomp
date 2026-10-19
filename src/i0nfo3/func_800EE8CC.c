// FUNC 800ee8cc 320 I0NFO3
// MATCHING 800ee8cc 320
typedef struct { short x0, y0, x1, y1, a, b; } Zone;
extern signed char D_8009D2B0;
extern unsigned short D_800A604E;
extern short *D_800A6078;
extern short D_800A60B4[], D_800A60B6[];
extern unsigned short D_800A6126[], D_800A612A[];
extern Zone D_800EEDB0[];

short func_800EE8CC(void)
{
    unsigned short y, x;
    int k, r;
    Zone *z;
    if (D_8009D2B0 == 3) return 0;
    y = D_800A604E;
    x = D_800A6078[1];
    if ((unsigned short)(y + 0x4B0) >= 0x12D) return 0;
    k = 0;
    if ((short)x >= 0x1C2) {
        if ((short)x < 0x280) {
            k = 1;
        } else {
            k = 3;
            if ((short)x < 0x398) k = 2;
        }
    }
    z = &D_800EEDB0[k];
    if ((short)x > z->x0 && (short)x < z->x1 && z->y0 < (short)y && (short)y < z->y1) {
        D_800A6126[0] = z->a;
        D_800A612A[0] = z->b;
        r = D_800A6078[1];
        D_800A60B6[0] = (short)D_800A612A[0] < (short)D_800A604E;
        D_800A60B4[0] = (short)D_800A6126[0] < r;
        return 1;
    }
}
