// FUNC 80118bfc 1304 X004
// MATCHING 80118bfc 1304
#include "spad.h"
typedef struct { short m[3][3]; short pad; int t[3]; } MAT;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { unsigned char b0, b1, b2, n; int list[1]; } L;
typedef struct { unsigned char x, f, z; } E3;

extern MAT VIEW_CAMERA;
typedef struct { unsigned short g; char pad[0x43e]; unsigned char f; } GS;
extern GS GAME_SELECTED_SECTION;
extern E3 D_8013062C[];
extern E3 D_801308FC[];
extern void SetRotMatrix(MAT *);
extern void SetTransMatrix(MAT *);
extern void area4ScriptOpSetQuad(short, int);
extern void getBaseMatrix(MAT *);
extern void ApplyRotMatrix(SVEC *, int *);
extern int area4ScriptOpPointInside(int *);

void area4ScriptOp(L *o, unsigned char *a)
{
    GS *gs = &GAME_SELECTED_SECTION;
    unsigned char cnt = a[0];
    int *tab = (int *)(a + 4);
    short i, k, m, d;
    short x, y, z;
    short n;
    E3 *e;
    SVEC v;
    MAT mt;

    if (gs->g < 4) {
    SetRotMatrix(&VIEW_CAMERA);
    SetTransMatrix(&VIEW_CAMERA);
    switch (gs->g) {
    case 0:
        d = 0x21c - SPAD.s17e;
        if (d > 0) i = d + 0x1c2;
        else i = 0x1c2;
        m = -0x10e;
        break;
    case 1:
        i = 0x168;
        m = -0x10e;
        break;
    case 2:
        if ((unsigned short)((*(unsigned short*)((char*)&SPAD.i174 + 2)) - 0x2b3) < 0x199) i = 0x168;
        else i = 0x1c2;
        m = -0xb4;
        break;
    case 3:
        if ((short)(*(unsigned short*)((char*)&SPAD.i174 + 2)) >= 0x105) i = 0x168;
        else i = 0x1c2;
        m = -0xb4;
        break;
    }
    if (GAME_SELECTED_SECTION.f) i = 0x21c;
    area4ScriptOpSetQuad(m, 0);
    area4ScriptOpSetQuad(i, 1);
    for (i = 0; i < cnt; i++) {
        if (o->n >= 0x58) return;
        switch (GAME_SELECTED_SECTION.g) {
        case 0:
            k = i;
            if (D_8013062C[i].f & 8) continue;
            if (GAME_SELECTED_SECTION.f == 0 && (D_8013062C[i].f & 1)) continue;
            x = D_8013062C[i].x * 0x5a + 0x1c2;
            y = (*(unsigned short*)((char*)&SPAD.if0 + 2));
            z = D_8013062C[i].z * 0x5a + 0x5a;
            break;
        case 1:
            k = cnt - 1 - i;
            if (D_8013062C[k].f & 8) continue;
            if (GAME_SELECTED_SECTION.f == 0 && (D_8013062C[k].f & 2)) continue;
            x = D_8013062C[k].x * 0x5a + 0x1c2;
            y = (*(unsigned short*)((char*)&SPAD.if0 + 2));
            z = D_8013062C[k].z * 0x5a + 0x14;
            break;
        case 2:
            k = i;
            if (D_801308FC[i].f & 8) continue;
            if (GAME_SELECTED_SECTION.f == 0 && (D_801308FC[i].f & 1)) continue;
            x = D_801308FC[i].x * 0x5a + 0x21c;
            y = (*(unsigned short*)((char*)&SPAD.if0 + 2));
            z = D_801308FC[i].z * 0x5a + 0x108;
            break;
        case 3:
            k = cnt - 1 - i;
            if (D_801308FC[k].f & 8) continue;
            if (GAME_SELECTED_SECTION.f == 0 && (D_801308FC[k].f & 2)) continue;
            x = D_801308FC[k].x * 0x5a + 0x21c;
            y = (*(unsigned short*)((char*)&SPAD.if0 + 2));
            z = D_801308FC[k].z * 0x5a + 0xdb;
            break;
        }
        getBaseMatrix(&mt);
        v.vx = x;
        v.vy = y;
        v.vz = z;
        ApplyRotMatrix(&v, mt.t);
        if (area4ScriptOpPointInside(mt.t)) {
            o->list[o->n] = (int)(a + tab[k]);
            o->n++;
        }
    }
    } else {
        o->list[0] = (int)(a + *(int *)(a + 4));
        o->n = 1;
    }
}
