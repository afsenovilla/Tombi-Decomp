// FUNC 8011b238 808 X006
// MATCHING 8011b238 808
#include "gvars.h"
/* Sibling of motocrossBikeModelStepB / X000 l4Type1BDeformModel. Debt: register asm("$3") on q (as in the siblings). */
#include "gamestate.h"
typedef struct {
    char p0[7];
    unsigned char substep;
    char p8[2];
    unsigned char b0a;
    char pb[0x74 - 0xb];
    short velH;
    short velV;
    char p78[2];
    short w7a;
    char p7c[0xa0 - 0x7c];
    int da0;
    char pa4[4];
    int da8;
    short wac;
    char pae[0xc4 - 0xae];
    int dc4;
    int dc8;
    short wcc;
} S;
extern int *DAT_1f800334p;
extern int DAT_1f800334;
extern int *DAT_1f800318p;
extern int DAT_1f800318;
extern int *DAT_1f800314p;
extern int DAT_1f800314;
extern int *DAT_1f800310p;
extern int DAT_1f800310;
extern char MODEL_PRIM_BUFFERS[];
extern void copyModelPrimGroups(int, int);
extern void subtractScaledVertices(int, int, int);

typedef struct { char p[0x34]; int d34; } TObj_;

#define RET(w, n, r) { o->velV = -0x1000; o->wac = w; o->da0 = DAT_1f800334 + DAT_1f800334p[n]; o->b0a = 0x15; o->substep = 0; return r; }
#define SETUP(P, Q) { a = o->da0; o->velH = 0x80; o->substep++; o->velV = 0; o->b0a = 0x16; p = P; q = Q; }
#define COMMON { o->wcc = o->velV; copyModelPrimGroups(o->da0, o->dc4); subtractScaledVertices(o->dc4, o->dc8, o->wcc); return 0; }
#define FADE { if (((unsigned short*)&GAME.selectedSection)[0] == 0) { int v; o->w7a += 0x10; v = g->d34 + ((short)o->w7a >> 8); g->d34 = v; if (v > 0) g->d34 = 0; } }

int motocrossBikeModelStepA(S *o)
{
    int *p;
    register int q asm("$3");
    int a;
    TObj_ *g = (*(TObj_ **)&GVARS.p50);

    switch (o->substep) {
    case 0:
        if (o->wac == 0) {
            o->substep = 1;
        } else if (o->wac == 1) {
            o->substep = 3;
        } else if (o->wac == 2) {
            o->substep = 5;
        }
        o->w7a = 0;
        break;
    case 1:
        SETUP(DAT_1f800310p, DAT_1f800310);
        goto tail;
    case 2:
        o->velV -= o->velH;
        if (o->velV < -0x1000) RET(1, 2, 1);
        COMMON;
    case 3:
        SETUP(DAT_1f800314p, DAT_1f800314);
        goto tail;
    case 4:
        o->velV -= o->velH;
        FADE;
        if (o->velV < -0x1000) RET(2, 3, 1);
        COMMON;
    case 5:
        SETUP(DAT_1f800318p, DAT_1f800318);
    tail:
        q += p[1];
        o->dc4 = (int)MODEL_PRIM_BUFFERS;
        o->da8 = (int)MODEL_PRIM_BUFFERS;
        o->dc8 = q;
        copyModelPrimGroups(a, o->dc4);
        return 0;
    case 6:
        o->velV -= o->velH;
        FADE;
        if (o->velV < -0x1000) RET(3, 4, 2);
        COMMON;
    }
    return 0;
}
