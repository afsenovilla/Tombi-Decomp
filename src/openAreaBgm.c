// FUNC 8001ec80 888 MAIN0
// MATCHING 8001ec80 888
#include "globals.h"
/* debt: register asm on v; without it CSE/local-alloc make x a block-local copy, global alloc gives v x's s0 (game: v in a1).
   Tried: v/x int/short/ushort combos, x set earlier, x = v in per-case tails with goto, SsSeqSetVol(v, v).
   w: the tail stores written in every case (cross-jumped) give w the refs to win over the switch value (was asm $3).
   c: the call written in both branches lets the byte shift tie to a1 (was asm $5). */
extern unsigned char AREA_BGM_DEFS, D_800784C9;
extern unsigned char D_800784CA[];
extern unsigned short VAB_IDS[];
extern unsigned short BGM_VAB_ID;
extern void *SEQ_DATA[];
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
void SsSeqStop(short a);
void SsSeqClose(short a);
short SsSeqOpen(void *p, short v);
void SsSetMVol(short a, short b);
void SsSeqSetVol(short a, short b, short c);
void SsSeqPlay(short a, int b, int c);
void setReverbMode(int a, short b);

int openAreaBgm(int arg)
{
    unsigned short *p;
    unsigned short t;
    register int v asm("$5");
    int w;
    int x;
    int b;
    if (BGM_SEQ_ID != -1) {
        SsSeqStop(BGM_SEQ_ID);
        SsSeqClose(BGM_SEQ_ID);
        BGM_SEQ_ID = -1;
    }
    BGM_MUTED = 0;
    t = VAB_IDS[D_800784C9];
    BGM_VAB_ID = t;
    BGM_SEQ_ID = SsSeqOpen(SEQ_DATA[AREA_BGM_DEFS], t);
    SsSetMVol(100, 100);
    p = &GAME;
    if (D_800784CA[*p * 4] != 0) {
        SsSeqSetVol(BGM_SEQ_ID, 0, 0);
        BGM_FADE_VOLUME = 0;
        w = AREA_BGM_VOLUME[*p];
        switch (GAME) {
        case 0:
        case 2:
            w += 5;
            BGM_FADE_TARGET = w;
            BGM_FADE_ACTIVE = 1;
            goto done;
        case 6:
            switch (GAME_SELECTED_SECTION) {
            case 0: w -= 5; BGM_FADE_TARGET = w; BGM_FADE_ACTIVE = 1; goto done;
            case 1: w += 12; BGM_FADE_TARGET = w; BGM_FADE_ACTIVE = 1; goto done;
            case 2: w += 12; BGM_FADE_TARGET = w; BGM_FADE_ACTIVE = 1; goto done;
            }
            break;
        case 9:
            switch (GAME_SELECTED_SECTION) {
            case 0: w -= 11; BGM_FADE_TARGET = w; BGM_FADE_ACTIVE = 1; goto done;
            case 1: case 2: case 3: case 4: case 5: case 6: w += 10; BGM_FADE_TARGET = w; BGM_FADE_ACTIVE = 1; goto done;
            }
            break;
        }
        BGM_FADE_TARGET = w;
        BGM_FADE_ACTIVE = 1;
    done:;
    } else {
        v = AREA_BGM_VOLUME[*p];
        switch (GAME) {
        case 0:
        case 2:
            v += 5;
            break;
        case 6:
            switch (GAME_SELECTED_SECTION) {
            case 0: v -= 5; break;
            case 1: v += 12; break;
            case 2: v += 12; break;
            }
            break;
        case 9:
            switch (GAME_SELECTED_SECTION) {
            case 0: v -= 11; break;
            case 1: case 2: case 3: case 4: case 5: case 6: v += 10; break;
            }
            break;
        }
        x = v;
        SsSeqSetVol(BGM_SEQ_ID, x, x);
        BGM_VOLUME = x;
        BGM_FADE_ACTIVE = 0;
    }
    b = (short)arg;
    if (b < 0) {
        unsigned char *t = AREA_REVERB_DEPTH[GAME];
        setReverbMode(AREA_REVERB_MODE[GAME], (short)(t[GAME_SELECTED_SECTION] << 8));
    } else {
        setReverbMode(1, b);
    }
    SsSeqPlay(BGM_SEQ_ID, 1, 1);
    return 0;
}
