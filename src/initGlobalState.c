// FUNC 80016a5c 244 MAIN0
// MATCHING 80016a5c 244
#include "globals.h"
extern unsigned short D_8009f7f2;
extern unsigned short D_8009f814;
extern unsigned char LOADED_SOUND_SET;
extern unsigned short SCREEN_OFFSET_X;
extern unsigned short D_1f8001f0;
extern unsigned short D_1f8001f2;
extern unsigned short D_1f8001ee;
extern unsigned short FRAME_COUNT;
extern unsigned short TICK_COUNT;
extern unsigned char MOVIE_PLAY_STATE;
extern unsigned char D_1f8001d0;
extern unsigned char D_1f8001d1;
extern unsigned char LOAD_COMPLETE_SYM;
extern unsigned char D_1f8001cf;
extern unsigned char MOVIE_SKIP_REQUEST;
extern unsigned short D_1f8001c6;
extern unsigned char D_1f8001c4;
extern unsigned char D_1f8001c5;
extern unsigned char D_1f8003cc;
extern unsigned short D_8009d672;
extern unsigned short D_8009d676;
extern unsigned short D_8009d5d8;
extern unsigned short D_8009f7ea;
extern unsigned short SCREEN_OFFSET_Y;
extern unsigned short D_1f8003d8;
void initGlobalState(void)
{
    D_8009f7f2 = 0xffff;
    D_8009f814 = 0xffff;
    LOADED_SOUND_SET = 0xff;
    LOADED_LIST_FILE_SYM = 0xff;
    SCREEN_OFFSET_X = 4;
    D_1f8001f0 = 0;
    D_1f8001f2 = 0;
    D_1f8001ee = 0;
    FRAME_COUNT = 0;
    TICK_COUNT = 0;
    MOVIE_PLAY_STATE = 0;
    D_1f8001d0 = 0;
    D_1f8001d1 = 0;
    LOAD_COMPLETE_SYM = 0;
    D_1f8001cf = 0;
    MOVIE_SKIP_REQUEST = 0;
    D_1f8001c6 = 0;
    D_1f8001c4 = 0;
    D_1f8001c5 = 0;
    D_1f8003cc = 0;
    D_8009d672 = 0;
    D_8009d676 = 0;
    D_8009d5d8 = 0;
    D_8009f7ea = 0;
    SCREEN_OFFSET_Y = 0xc;
    D_1f8003d8 = D_1f8003d8 & 1;
}
