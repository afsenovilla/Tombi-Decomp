// FUNC 80118bfc 1304 X004
// MATCHING 80118bfc 1304
typedef struct { short m[3][3]; short pad; int t[3]; } MAT;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { unsigned char b0, b1, b2, n; int list[1]; } L;
typedef struct { unsigned char x, f, z; } E3;

extern MAT D_1F8000C0;
typedef struct { unsigned short g; char pad[0x43e]; unsigned char f; } GS;
extern GS D_8009C962;
extern unsigned short D_1F80017E;
extern unsigned short D_1F800176;
extern unsigned short D_1F8000F2;
extern E3 D_8013062C[];
extern E3 D_801308FC[];
extern void SetRotMatrix(MAT *);
extern void SetTransMatrix(MAT *);
extern void func_80118720(short, int);
extern void FUN_80021f5c(MAT *);
extern void ApplyRotMatrix(SVEC *, int *);
extern int func_801188F0(int *);

void func_80118BFC(L *o, unsigned char *a)
{
    GS *gs = &D_8009C962;
    unsigned char cnt = a[0];
    int *tab = (int *)(a + 4);
    short i, k, m, d;
    short x, y, z;
    short n;
    E3 *e;
    SVEC v;
    MAT mt;

    if (gs->g < 4) {
    SetRotMatrix(&D_1F8000C0);
    SetTransMatrix(&D_1F8000C0);
    switch (gs->g) {
    case 0:
        d = 0x21c - D_1F80017E;
        if (d > 0) i = d + 0x1c2;
        else i = 0x1c2;
        m = -0x10e;
        break;
    case 1:
        i = 0x168;
        m = -0x10e;
        break;
    case 2:
        if ((unsigned short)(D_1F800176 - 0x2b3) < 0x199) i = 0x168;
        else i = 0x1c2;
        m = -0xb4;
        break;
    case 3:
        if ((short)D_1F800176 >= 0x105) i = 0x168;
        else i = 0x1c2;
        m = -0xb4;
        break;
    }
    if (D_8009C962.f) i = 0x21c;
    func_80118720(m, 0);
    func_80118720(i, 1);
    for (i = 0; i < cnt; i++) {
        if (o->n >= 0x58) return;
        switch (D_8009C962.g) {
        case 0:
            k = i;
            if (D_8013062C[i].f & 8) continue;
            if (D_8009C962.f == 0 && (D_8013062C[i].f & 1)) continue;
            x = D_8013062C[i].x * 0x5a + 0x1c2;
            y = D_1F8000F2;
            z = D_8013062C[i].z * 0x5a + 0x5a;
            break;
        case 1:
            k = cnt - 1 - i;
            if (D_8013062C[k].f & 8) continue;
            if (D_8009C962.f == 0 && (D_8013062C[k].f & 2)) continue;
            x = D_8013062C[k].x * 0x5a + 0x1c2;
            y = D_1F8000F2;
            z = D_8013062C[k].z * 0x5a + 0x14;
            break;
        case 2:
            k = i;
            if (D_801308FC[i].f & 8) continue;
            if (D_8009C962.f == 0 && (D_801308FC[i].f & 1)) continue;
            x = D_801308FC[i].x * 0x5a + 0x21c;
            y = D_1F8000F2;
            z = D_801308FC[i].z * 0x5a + 0x108;
            break;
        case 3:
            k = cnt - 1 - i;
            if (D_801308FC[k].f & 8) continue;
            if (D_8009C962.f == 0 && (D_801308FC[k].f & 2)) continue;
            x = D_801308FC[k].x * 0x5a + 0x21c;
            y = D_1F8000F2;
            z = D_801308FC[k].z * 0x5a + 0xdb;
            break;
        }
        FUN_80021f5c(&mt);
        v.vx = x;
        v.vy = y;
        v.vz = z;
        ApplyRotMatrix(&v, mt.t);
        if (func_801188F0(mt.t)) {
            o->list[o->n] = (int)(a + tab[k]);
            o->n++;
        }
    }
    } else {
        o->list[0] = (int)(a + *(int *)(a + 4));
        o->n = 1;
    }
}
