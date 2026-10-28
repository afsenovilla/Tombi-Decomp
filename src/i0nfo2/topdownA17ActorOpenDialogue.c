// FUNC 800f4110 812 I0NFO2
// MATCHING 800f4110 812
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern int topdownA17ActorDialogueBank3(TObj *o);
extern unsigned char D_8009CE05[], D_8009CE06, D_8009CE07;
extern unsigned char D_8009CFA5;

static __inline__ int inl_800F3EA0(TObj *o)
{
    Pos6 p;
    p = *(Pos6 *)&o->a;
    if (D_8009CE05[0] != 0) {
        if (D_8009CE05[0] == 0xFF) {
            o->d90 = func_8002DCC8(2, 10, &p);
        } else if (D_8009CE06 == 0) {
            o->d90 = func_8002DCC8(2, 5, &p);
        } else if (D_8009CE07 != 0) {
            o->d90 = func_8002DCC8(2, 9, &p);
        } else {
            o->d90 = func_8002DCC8(2, 6, &p);
            o->state = 2;
            o->wac = 0x63;
        }
    } else {
        o->d90 = func_8002DCC8(2, 5, &p);
    }
    return 1;
}

#define CALL(a, b) { o->d90 = func_8002DCC8(a, b, &p); r++; }

short topdownA17ActorOpenDialogue(TObj *arg)
{
    register TObj *o asm("$16") = arg;
    Pos6 p;
    short r;
    p = *(Pos6 *)&o->a;
    r = 0;
    o->wac = 0;
    switch ((unsigned short)o->wba) {
    case 0: CALL(2, 0xA); break;
    case 1: CALL(2, 7); break;
    case 2: r = inl_800F3EA0(o); break;
    case 3: r = topdownA17ActorDialogueBank3(o); break;
    case 5: CALL(5, 0); break;
    case 6:
        CALL(5, D_8009CFA5 == 2 ? 0xF : 1)
        break;
    case 7:
        switch (D_8009CFA5) {
        case 0: CALL(5, 2); break;
        case 1: CALL(5, 0xB); break;
        case 2: CALL(5, 0x10); break;
        default: r++; break;
        }
        break;
    case 8: CALL(5, 3); break;
    case 9:
        switch (D_8009CFA5) {
        case 0: CALL(5, 4); break;
        case 1: CALL(5, 0xC); break;
        case 2: CALL(5, 0x11); break;
        default: r++; break;
        }
        break;
    case 10: CALL(5, 5); break;
    case 11: CALL(5, 6); break;
    case 12:
        CALL(5, D_8009CFA5 == 2 ? 0x12 : 7)
        break;
    case 13:
        switch (D_8009CFA5) {
        case 0: CALL(5, 8); break;
        case 1: CALL(5, 0xD); break;
        case 2: CALL(5, 0x13); break;
        default: r++; break;
        }
        break;
    case 14: CALL(5, 9); break;
    case 15:
        CALL(5, D_8009CFA5 != 0 ? 0xE : 0xA)
        break;
    }
    return r;
}
