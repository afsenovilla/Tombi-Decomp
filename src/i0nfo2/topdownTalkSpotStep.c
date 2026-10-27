// FUNC 800ee8b0 536 I0NFO2
// MATCHING 800ee8b0 536
#include "TOBJ.H"
#include "raw7.h"
extern int func_8002DC50(int a, int b, int x, int y);
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char GAME_PLAYER_MODE;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned short D_800A6066;
extern unsigned char D_800A60A1;
extern unsigned char D_800A60A3;
extern unsigned char D_800A60F8;
extern short D_800FED54[][4];

void topdownTalkSpotStep(TObj *o)
{
    TObj *p;
    short r;
    short pad[4];
    o->visible = D_800A60A3 == 0;
    switch (o->state) {
    case 0:
        r = 0;
        if (o->b68 != 0) {
            int a = D_800A60A1;
            if (a == 2 && (D_800A6066 & 7) == a) {
                S32(o, 0x90) = func_8002DC50(D_800FED54[o->subtype][0], D_800FED54[o->subtype][1], 0xA0, 0x8C);
                GAME_PLAYER_MODE = 2;
                D_8009C942 = 1;
                D_8009C93F = 1;
                PLAYER_STATE = 4;
                *(short *)0x1F8001C6 = 0;
                D_800A60F8 = 0;
                PLAYER_SUBSTATE = 6;
                PLAYER_STEP = 0;
                o->state++;
                r = 1;
            }
            if (!r) {
                o->state = 2;
            }
        }
        o->b69 = 0;
        o->b68 = 0;
        break;
    case 1:
        p = (TObj *)S32(o, 0x90);
        if (p->b04 >= 2) {
            p->b04++;
            o->state++;
        }
        break;
    case 2:
        D_8009C942 = 0;
        D_8009C93F = 0;
        o->b68 = 0;
        o->b69 = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        *(short *)0x1F8001C6 = 0;
        D_800A60F8 = 0;
        o->state = 0;
        break;
    }
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == U8(o, 0xB6)) {
            o->b6b = 0x80;
        } else {
            o->b6b = 0;
        }
    } else if (U16(o, 0xB6) != 0) {
        o->b6b = 0;
    } else {
        o->b6b = 0x80;
    }
}
