// FUNC 80018ebc 172 MAIN0
// MATCHING 80018ebc 172
typedef struct { short a; short b; unsigned char x, y, z, w, v, u; } E;
extern E DAT_800b0bb8[];

void FUN_80018ebc(void)
{
    int a = 0;
    char c = -0x62;
    int i;
    for (i = 0; i < 0x30; i++) {
        DAT_800b0bb8[i].x = a;
        a += 4;
        DAT_800b0bb8[i].a = -1;
        DAT_800b0bb8[i].b = 0;
        DAT_800b0bb8[i].y = c;
        DAT_800b0bb8[i].z = 4;
        DAT_800b0bb8[i].w = 0x18;
        DAT_800b0bb8[i].v = 0;
        DAT_800b0bb8[i].u = 0;
        if (a > 0x3b) {
            a = 0;
            c += 0x18;
        }
    }
}
