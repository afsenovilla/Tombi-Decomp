// FUNC 800eda90 504 I0NFO2
// MATCHING 800eda90 504
#include "TOBJ.H"
#include "raw7.h"
#include "spad.h"
typedef struct { char pad[0x176]; short s176; short pad178; unsigned short s17a; } SpadXY;
#define SPXY ((SpadXY *)0x1F800000)
extern void DrawSync(int);
extern int GetGraphType(void);
extern void func_80018DA4(TObj *o);
extern void func_80018934(TObj *o);
extern void func_800ECA74(void);
extern void func_800ED380(int, int);
extern void func_800ED7EC(TObj *);
extern unsigned char D_800A60A3;
extern int D_800FECB4;

void func_800EDA90(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        func_800ECA74();
        o->b0a = 3;
        o->w1e = (GetGraphType() == 1 || GetGraphType() == 2) ? 0x20C : 0x8C;
        o->w08 = 0x7F88;
        o->d30 = 0x20;
        o->d34 = 0x20;
        o->b0f = 0;
        o->h->p.whole = 0;
        o->y.p.whole = 0;
        func_800ED7EC(o);
        o->b6b = 0x80;
        o->b04++;
        break;
    case 1:
        o->d30 = SPXY->s176 + 0xA1;
        o->d34 = (short)SPXY->s17a - 0xEF;
        S16(o->h, 2) = 0x1F - (((unsigned short)SPXY->s176 + 0xA0) & 0x1F);
        o->y.p.whole = 0x1F - ((SPXY->s17a - 0xF0) & 0x1F);
        func_800ED7EC(o);
        func_800ED380(0, D_800FECB4);
        DrawSync(0);
        func_800ED380(1, D_800FECB4);
        func_800ED380(2, D_800FECB4);
        func_800ED380(3, D_800FECB4);
        func_800ED380(4, D_800FECB4);
        if (D_800A60A3 == 0) {
            o->b6b = 0x80;
        } else {
            o->b6b = 0;
        }
        o->visible = 1;
        func_80018DA4(o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_80018934(o);
        break;
    }
}
