// FUNC 8012ee14 640 X004
/* score 87: logic and control flow match. Differs: the D_8009C941 value c stays live in s3 (gcc CSEs the call arg i + 0x2e back to c; game recomputes addiu a0,s2,0x2e) which adds a saved reg and frame 0x28; spawned-object pointer in a1 vs a0 and the h/y/b stores scheduled differently (game loads D_800A6078 then n->h). Tried: c/i int/short/char types, masked compares, separate ifs, -fno-cse-* (diagnostic), temp for h.
   o26: the c/i CSE is -fcse-skip-blocks skipping the `if (n)` body (FLAGS -fno-cse-skip-blocks gives addiu a0,s2,0x2e
   and frame 0x20, but 114 elsewhere); so the game's body was not skippable (label inside or join label used twice).
   Tried: do/while(0), for/while+break, inline spawn() with return, c types x compare forms: no change. */
#include "TOBJ.H"
typedef struct { unsigned short w0, w2, w4, w6, w8, wa, wc; } E;
extern E D_8013145C[];
extern unsigned char D_8009CF24[];
extern unsigned char D_8009C940, D_8009C941;
extern unsigned char D_8009CF29;
extern unsigned char D_8009C93E[], D_8009C93F[], D_8009C942[];
extern short D_800A60EA[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern Fix16 *D_800A6078;
extern short D_800A604E;
extern TObj *FUN_800183b8(void);
extern void FUN_80026e0c(int, int);

void func_8012EE14(TObj *o)
{
    switch (o->state) {
    case 0: {
        short c;
        int i;
        E *p;
        TObj *n;

        if (D_8009C940 == 0) break;
        c = D_8009C941;
        i = c - 0x2e;
        if (D_8009CF24[i]) break;
        if ((c & 0xff) >= 0x33) break;
        if ((c & 0xff) < 0x2e) break;
        p = &D_8013145C[i];
        n = FUN_800183b8();
        if (n) {
            n->active = 4;
            n->type = 0x32;
            n->animFrame = p->w0;
            n->subtype = p->w2;
            n->b0c = p->w4;
            n->b6b = p->wc;
            n->velX = p->w6;
            n->velY = p->w8;
            n->h->p.whole = D_800A6078->p.whole;
            n->y.p.whole = D_800A604E;
            n->velV = 3;
            n->velH = 3;
            n->b.p.whole = p->wa;
        }
        D_8009CF24[i] = 1;
        FUN_80026e0c(i + 0x2e, 1);
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        D_8009CF29 = 1;
        break;
    }
    case 1:
        if (D_8009CF29 == 0) o->state++;
        break;
    case 2:
        D_8009C93E[0] = 1;
        o->w08 = 0x3c;
        o->state++;
        break;
    case 3:
        if (--o->w08 <= 0) o->state = 15;
        break;
    case 15:
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_800A60EA[0] = 0;
        D_8009C93E[0] = 0;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        o->state = 0;
        o->step++;
        break;
    }
}
