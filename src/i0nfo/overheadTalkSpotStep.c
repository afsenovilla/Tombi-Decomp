// FUNC 800ee5e4 440 I0NFO
// MATCHING 800ee5e4 440
#include "TOBJ.H"
extern TObj *func_8002DC50(int a, int b, int x, int y);
extern signed char D_8009C93F, D_8009C942, GAME_PLAYER_MODE;
extern signed char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern unsigned short PLAYER_ANIM_FRAME;
extern unsigned char D_800A60A1, D_800A60A3;
extern signed char D_800A60F8;

void overheadTalkSpotStep(TObj *o)
{
    short r;
    o->visible = D_800A60A3 != 0;
    switch (o->state) {
    case 0:
        r = 0;
        if (o->b68 != 0) {
            int a = D_800A60A1;
            if (a == 2 && (PLAYER_ANIM_FRAME & 7) == a) {
                *(int *)((char *)o + 0x90) = (int)func_8002DC50(1, 1, 0xA0, 0x8C);
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
        if (((TObj *)o->d90)->b04 >= 2) {
            ((TObj *)o->d90)->b04++;
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
}
