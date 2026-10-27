// FUNC 8001f1c0 300 MAIN0
// MATCHING 8001f1c0 300
#include "globals.h"
extern unsigned short DAT_8009c960;
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern void SsSeqSetVol(short, short, short);

void restoreAreaBgmVolume(void)
{
    int v;
    short sv;
    short d;

    if (BGM_SEQ_ID == -1)
        return;
    v = AREA_BGM_VOLUME[DAT_8009c960];
    switch (GAME) {
    case 0:
    case 2:
        v = v + 5;
        break;
    case 6:
        switch (GAME_SELECTED_SECTION) {
        case 0:
            v = v - 5;
            break;
        case 1:
            v = v + 12;
            break;
        case 2:
            v = v + 12;
            break;
        }
        break;
    case 9:
        switch (GAME_SELECTED_SECTION) {
        case 0:
            v = v - 11;
            break;
        case 1: case 2: case 3: case 4: case 5: case 6:
            v = v + 10;
            break;
        }
        break;
    }

    sv = v;
    d = BGM_SEQ_ID;
    SsSeqSetVol(d, sv, sv);
    BGM_MUTED = 0;
}
