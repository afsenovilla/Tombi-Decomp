// FUNC 800ee37c 516 D0SPSUB
typedef struct { unsigned short x, y, z, pad; } Pt;
typedef struct { unsigned char a, b, c, d; short pad; } Link;
typedef struct { int x, y, z; } Vec;
extern char D_8010A2D4[];
extern short D_8010A2DC, D_8010A2F2, D_8010A302, D_8010A388, D_8010A38A, D_8010A38C, D_8010A38E;
extern unsigned char D_8010A2DE, D_8010A2E1;
extern signed char D_8010A2E3;
extern int D_8010A2E4, D_8010A2E8, D_8010A2EC;
extern Vec D_8010A304;
extern int D_8010A2F8, D_8010A310;
extern int D_800FECE4;
extern int D_1F8002EC;
extern unsigned short D_8009C960, D_8009C962;
extern short *D_800FE064[];
extern Link D_800FDAB4[];
extern Pt D_800FD418[];
extern int D_80111124;

char *func_800EE37C(void)
{
    short *e;
    Link *l;
    int a, b;
    D_8010A2DC = 0x78D2;
    D_8010A2E3 = -0x1E;
    D_8010A2F2 = 1;
    D_8010A2E1 = 1;
    D_8010A2DE = 1;
    D_8010A302 = 1;
    D_8010A2F8 = D_800FECE4;
    D_8010A310 = D_1F8002EC;
    e = &D_800FE064[D_8009C960][D_8009C962];
    l = &D_800FDAB4[*e];
    a = l->a;
    b = l->b;
    D_8010A2E4 = ((short)(D_800FD418[a].x + 8) + (short)(D_800FD418[b].x + 8)) / 2 << 16;
    D_8010A2E8 = ((short)(D_800FD418[a].y + 2) + (short)(D_800FD418[b].y + 2)) / 2 << 16;
    D_8010A2EC = ((short)D_800FD418[a].z + (short)D_800FD418[b].z) / 2 << 16;
    D_8010A304 = *(Vec *)&D_8010A2E4;
    D_8010A388 = 0;
    D_80111124 = *e;
    D_8010A38A = l->c;
    D_8010A38C = l->d;
    D_8010A38E = 1;
    return D_8010A2D4;
}
