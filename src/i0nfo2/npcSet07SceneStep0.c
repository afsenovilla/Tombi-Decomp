// FUNC 800f9974 616 I0NFO2
// MATCHING 800f9974 616
#include "TOBJ.H"
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CDD9;
extern unsigned char D_8009CEE7;
extern unsigned char D_8009CEE9[];
extern unsigned char D_8009CEEB;
extern unsigned char D_8009CEEC[];
extern unsigned char D_8009CEF2[];
extern unsigned char D_8009CEF3;
extern unsigned char D_8009CF00;
extern unsigned char D_8009CF04;
extern unsigned char D_8009CF2B;
extern unsigned char D_8009CF9D;
extern unsigned char D_8009CFA4[];
extern unsigned char D_8009CFC6[];
extern unsigned char D_8009CFC7;
extern unsigned char D_800A60A3;

void npcSet07SceneStep0(TObj *o)
{
    if (D_8009CDD9 == 0) {
        o->step = 9;
        o->state = 0;
    }
    if (D_800A60A3 == 1) {
        if (D_8009CEE7 != 0) {
            D_8009CEE7 = 0;
            o->step = 1;
            o->state = 0;
        }
        if (D_8009CF2B != 0) {
            D_8009CF2B = 0;
            o->step = 8;
            o->state = 0;
        }
    }
    if (D_800A60A3 == 3) {
        if (D_8009CEE9[0] == 0) {
            D_8009CEE9[0] = 1;
            o->step = 2;
            o->state = 0;
        }
        if (D_8009CEEB != 0) {
            D_8009CEEB = 0;
            o->step = 2;
            o->state = 0;
        }
    }
    if (D_8009CFA4[0] != 0) {
        unsigned char c = D_8009CF9D;
        D_8009CFA4[0] = 0;
        if (c == 1) {
            D_8009CF9D = c + 1;
            D_8009CDD9 = 2;
        }
        o->step = 0xA;
        o->state = 0;
    }
    if (D_800A60A3 == 0) {
        if (D_8009CEEC[0] != 0) {
            D_8009CEEC[0] = 0;
            if (D_8009CDD5 == 2) {
                o->step = 6;
                o->state = 1;
            } else {
                o->step = 3;
                o->state = 0;
            }
        }
        if (D_8009CEF2[0] != 0) {
            D_8009CEF2[0] = 0;
            o->step = 5;
            o->state = 0;
        }
        if (D_8009CEF3 != 0) {
            D_8009CEF3 = 0;
            o->step = 4;
            o->state = 0;
        }
        if (D_8009CDD5 == 1 && D_8009CF00 != 0 && D_8009CF04 != 0) {
            o->step = 6;
            o->state = 0;
        }
        if (D_8009CFC6[0] != 0) {
            D_8009CFC6[0] = 0;
            o->step = 0xB;
            o->state = 0;
        }
        if (D_8009CFC7 != 0) {
            D_8009CFC7 = 0;
            o->step = 0xC;
            o->state = 0;
        }
    }
}
