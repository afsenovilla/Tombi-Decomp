// VERSION SCUS_94236 scus94236 800163B0 1424
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/main.c, main); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
#include "psyq/libcd.h"
extern u_long _ramsize;
extern u_long _stacksize;
// INCLUDE_ASM("nonmatchings/main/main", main);
void main(void)
{
    u8 cdMode;
    u16 toggled;
    u16 flags;
    u16 targetFrame;
    u16 joypad_state;
    u16 newFlags;
    u16 curFlags;
    s32 dbgMode;
    u_short* joypad_state_ptr;

    SetDispMask(0);
    SetVideoMode(MODE_NTSC);
    ResetCallback();
    initGlobalState();
    ResetGraph(0);
    SetGraphDebug(0);
    InitGeom();
    CdInit();
    cdMode = CdlModeSpeed;
    while (CdControl(CdlSetmode, &cdMode, 0) == 0);
    DecDCTReset(0);
    CD_QUEUE_TAIL = 0;
    CD_QUEUE_HEAD = 0;
    initGraphics();
    setDrawSyncParams(1, 1);
    shutdownSound();
    SetDefDispEnv(&D_8009AFE8, 0, 0, 512, 240);
    memCardInit();
    joypadInit();
    initTasks();
    startTask(0, bootSequenceTask);
    EnterCriticalSection();
    SPAD->vblankEvent = OpenEvent(RCntCNT3, EvSpINT, RCntMdINTR, &vblankHandler);
    ExitCriticalSection();
    joypad_state_ptr = &D_8009EB5A;
    EnableEvent(*(u32*)(&D_1F8000C0[0]+0x118));
    SetDispMask(1);
    while (true) {
        *(u16*)((byte*)D_1F8001A0+0x48) = 0;
        if ((*(u16 *)&SCRATCHPAD.pauseFlags) < 0x4001U) {
            D_8009C8A8 = (FRAME_BUFFER_INDEX * 0x780) + &D_800A1890;
            dispatchTasks();
        }
        if ((*(u16 *)&SPAD->useDrawSync) != 0) {
            DrawSync(0);
        }
        targetFrame = *(u16*)(&D_1F8000C0[0]+0x12A);
        while (((Task*)0x1F8001D4)->unk14 < targetFrame) {
        }
        if ((*(u16 *)&SCRATCHPAD.useDrawSync) == 0) {
            ResetGraph(1);
        }
        if ((SPAD->debugMoveDisplay != 0) && ((SPAD->inTitle | SPAD->inGame) != 0)) {
            while (~*joypad_state_ptr & JOY_R2) {
                if (~*joypad_state_ptr & JOY_LEFT) {
                    D_8009AFE8.disp.x -= 4;
                }
                if (~*joypad_state_ptr & JOY_RIGHT) {
                    D_8009AFE8.disp.x += 4;
                }
                if (~*joypad_state_ptr & JOY_UP) {
                    D_8009AFE8.disp.y -= 4;
                }
                if (~*joypad_state_ptr & JOY_DOWN) {
                    D_8009AFE8.disp.y += 4;
                }
                VSync(0);
                PutDispEnv(&D_8009AFE8);
                do {} while (0);
            }
        }

        switch (MOVIE_PLAY_STATE) {
            case MOVIE_STATE_PLAYING:
                break;
            case MOVIE_STATE_IDLE:
            case MOVIE_STATE_STARTING:
                if (PAUSE_FLAGS <= 0x4000) {
                    flipFrameBuffer();
                    tickTaskTimers();
                }
                break;
            case MOVIE_STATE_ENDING:
                SCRATCHPAD.moviePlayState = 2;
                if (PAUSE_FLAGS <= 0x4000) {
                    flipFrameBuffer();
                    tickTaskTimers();
                }
                break;
        }

        if (SPAD->debugFreeze != 0) {
            if ((MOVIE_PLAY_STATE == MOVIE_STATE_IDLE) && ((SPAD->inTitle | SPAD->inGame) != 0) && (LOAD_COMPLETE == 1) && (JOYPAD_STATE & JOY_L1)) {
                (*(u16 *)&SCRATCHPAD.pauseFlags) = (u16)(0x8000 - PAUSE_FLAGS);
            }
        } else if (SPAD->debugPauseStep != 0 && MOVIE_PLAY_STATE == MOVIE_STATE_IDLE && ((SPAD->inTitle | SPAD->inGame) != 0)) {
            dbgMode = LOAD_COMPLETE;
            if (dbgMode == 1) {
                joypad_state = JOYPAD_STATE;
                if (joypad_state & JOY_L1) {
                    toggled = 1 - PAUSE_TOGGLE;
                    (*(u16 *)&SCRATCHPAD.pauseToggle) = toggled;
                    PAUSE_FLAGS = (u16)(toggled << 0xF);
                }
                if (PAUSE_TOGGLE != 0) {
                    PAUSE_FLAGS = 0x8000U;
                    if (joypad_state & JOY_L2) {
                        (*(u16 *)&SCRATCHPAD.pauseFlags) = 0U;
                    }
                } else if (joypad_state & JOY_L2) {
                    flags = PAUSE_FLAGS;
                    newFlags = (~flags & 0x4000) | (flags & 0xBFFF);
                    *(u16*)(&D_1F800118[0]+0xD8) = newFlags;
                    if (!(newFlags & 0x4000)) {
                        (*(u16 *)&SCRATCHPAD.pauseFlags) = (u16)(newFlags & 0xFFF0);
                    }
                } else {
                    curFlags = PAUSE_FLAGS;
                    if (curFlags & 0x4000) {
                        (*(u16 *)&SCRATCHPAD.pauseFlags) = (u16)((curFlags + 1) & 0xFFF3);
                    }
                }
            }
        }   
    };
}
#endif
