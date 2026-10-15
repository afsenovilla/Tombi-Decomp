// FUNC 800ed4ec 700 I0NFO3
// MATCHING 800ed4ec 700
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned int D_8009C96C;
extern unsigned char D_8009CE21;
extern unsigned char D_8009CE35;
extern unsigned char D_8009CE36;
extern unsigned char D_8009CE45;
extern unsigned char D_8009D074[];

#define CALL(x, y) { a = x; b = y; goto call; }
#define CALLS(x, y, s, w) { o->d90 = func_8002DCC8(x, y, &p); o->state = s; o->wac = w; goto inc; }

short func_800ED4EC(TObj *arg)
{
    register TObj *o asm("$16") = arg;
    Pos6 p;
    register short r asm("$17");
    int a, b;
    p = *(Pos6 *)&o->a;
    r = 0;
    o->wac = 0;
    switch ((unsigned short)o->wba) {
    case 0: CALL(6, 0);
    case 1: CALL(6, 1);
    case 2: CALL(6, 2);
    case 3:
        if (D_8009CE21 == 0) CALLS(6, 3, 2, 0x7D)
        if (D_8009CE21 == 0xFF) CALL(6, 5)
        CALL(6, 4)
    case 4:
        if (D_8009CE35 == 0) CALLS(6, 6, 2, 0x91)
        CALL(6, 7)
    case 5: CALL(6, 8);
    case 6: CALL(6, 3);
    case 7:
        if (D_8009D074[0] == 0) {
            o->d90 = func_8002DCC8(6, 9, &p);
            D_8009D074[0] = 1;
            goto inc;
        }
        CALLS(6, 0xA, 2, 0x80)
    case 8:
        if (D_8009CE45 == 0) CALLS(6, 0xF, 2, 0xA1)
        CALL(6, 0x11)
    case 9:
        if (D_8009CE36 == 0) CALLS(6, 0xC, 2, 0x92)
        CALL(6, 0xC)
    case 10:
        if (D_8009CE36 != 0xFF) {
            o->d90 = func_8002DCC8(5, 0, &p);
            o->state = 4;
            goto inc;
        }
        if (D_8009C96C > 0x7A120) {
            o->d90 = func_8002DCC8(5, 2, &p);
            o->state = 5;
            goto inc;
        }
        CALL(5, 1)
    call:
        o->d90 = func_8002DCC8(a, b, &p);
    inc:
        r++;
    }
    return r;
}
