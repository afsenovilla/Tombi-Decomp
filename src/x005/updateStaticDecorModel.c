// FUNC 800ee18c 208 X005
// MATCHING 800ee18c 208
// VERSION BETA_980203 x00 800F2C98 204
#if defined(VERSION_BETA_980203)
// Beta version of PAL x005 800EE18C (updateStaticDecorModel.c): PAL C of src/x005/updateStaticDecorModel.c (updateStaticDecorModel, twin score 0.905), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "raw7.h"
extern unsigned short GAME;
extern void f1(), f2(), f3();
void updateStaticDecorModel(char *o)
{
    unsigned char s = U8(o, 4);
    switch (s) {
    case 0:
        U8(o, 0) = 2;
        U8(o, 0xa) = 0x10;
        U8(o, 4)++;
        break;
    case 1:
        if (GAME == 4 || GAME == 0xc) {
            f1();
        } else {
            f2();
        }
        break;
            U8(o, 1) = 1;
    case 2:
        U8(o, 4) = s + 1;
        break;
    case 3:
        f3();
        break;
    }
}
#else
#include "raw7.h"
extern unsigned short GAME;
extern void f1(), f2(), f3();
void updateStaticDecorModel(char *o)
{
    unsigned char s = U8(o, 4);
    switch (s) {
    case 0:
        U8(o, 0) = 2;
        U8(o, 0xa) = 0x10;
        U8(o, 4)++;
        break;
    case 1:
        if (GAME == 4 || GAME == 0xc) {
            U8(o, 1) = 1;
            f1();
        } else {
            f2();
        }
        break;
    case 2:
        U8(o, 4) = s + 1;
        break;
    case 3:
        f3();
        break;
    }
}
#endif
