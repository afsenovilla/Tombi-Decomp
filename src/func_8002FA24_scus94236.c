// VERSION SCUS_94236 scus94236 8002FA24 3344
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/game/effect2.c, updateMessageBox); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
#include "game/gte.h"
// INCLUDE_ASM("nonmatchings/main/game/effect2", func_8002EBC4);
extern void func_80023020(GameObject *);
extern s16 fixedMulCos(s32, s32);
extern s16 fixedMulSin(s32, s32);
// INCLUDE_ASM("nonmatchings/main/game/effect2", clearTalkPose);
extern u8 TALK_POSE;
void func_800EDDDC(u8 *p, s32 a, s32 b);
extern u8 D_800A539A; extern u8 D_8009C377; extern u16 D_8009BCC8; extern u8 D_800A5398[]; extern s16 D_800A53C6x asm("D_800A53C6");
static __inline__ void restoreTalkPose(void)
{
    s32 v = D_800A539A;
    D_8009C377 = 0;
    switch (v) {
    case 0:
        if (D_8009BCC8 == 9) func_800EDDDC(D_800A5398, 0, 0);
        break;
    case 1: func_800E7DF8(D_800A5398, 0, D_800A53C6x); break;
    case 2: func_800E8A30(D_800A5398, 0, D_800A53C6x); break;
    case 3: func_800E8758(D_800A5398, 0, D_800A53C6x); break;
    }
}
typedef struct MessageBox {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 visible;
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 subtype;
    /* 0x04 */ u8 state;
    /* 0x05 */ u8 subState;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB[4];
    /* 0x0F */ u8 unkF;
    /* 0x10 */ Fix16 x;
    /* 0x14 */ Fix16 y;
    /* 0x18 */ Fix16 z;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ s16 tpage;
    /* 0x20 */ s16 timer;
    /* 0x22 */ s16 blink;
    /* 0x24 */ u8 pad24[8];
    /* 0x2C */ u16 animTimer;
    /* 0x2E */ u16 anchored;
    /* 0x30 */ s32 worldX;
    /* 0x34 */ s32 worldY;
    /* 0x38 */ s32 worldZ;
    /* 0x3C */ u8 pad3C[0x58];
    /* 0x94 */ u16 *script;
    /* 0x98 */ u8 pad98[0x1C];
    /* 0xB4 */ u16 mode;
    /* 0xB6 */ u16 voiceTone;
    /* 0xB8 */ u16 choice;
    /* 0xBA */ s16 lineX;
    /* 0xBC */ s16 lineY;
    /* 0xBE */ s16 left;
    /* 0xC0 */ s16 top;
    /* 0xC2 */ u16 width;
    /* 0xC4 */ u16 height;
    /* 0xC6 */ s16 voice;
    /* 0xC8 */ u16 color;
    /* 0xCA */ u16 slot;
    /* 0xCC */ u16 voiceTimer;
    /* 0xCE */ u16 cursorX;
    /* 0xD0 */ u16 cursorY;
    /* 0xD2 */ s16 closeTimer;
} MessageBox;
void updateMessageBox(MessageBox *o)
{
    MATRIX *m = &SPAD->objectMatrix;
    SVECTOR *pos = (SVECTOR *)0x1F800040;
    SVECTOR *zero = (SVECTOR *)0x1F800048;
    DVECTOR *sxy = &SPAD->screenXY;
    MATRIX *w = &SPAD->workMatrix;
    s16 n;
    s32 r;
    s32 xy;
    s32 vy;
    s16 yy;
    s16 d;
    u16 half;
    u16 height;

    switch (o->state) {
    case 0:
        o->color = 0;
        o->subState = 0;
        o->state++;
        break;
    case 1:
        if (++o->blink >= 0x3C) {
            o->blink = 0;
        }
        if (o->voice != -1) {
            if (GAME.selectedArea == 6) {
                D_800B0778 = 4;
            } else {
                D_800B0778 = 0x78;
            }
        }
        switch (o->subState) {
        case 0:
            if (SPAD->joypad_state & SPAD->buttonJump) {
                o->timer = 1;
            }
            for (;;) {
                u16 code;

                if (--o->timer != 0) {
                    break;
                }
                o->timer = o->mode;
                code = *o->script++;
                switch ((s16)code) {
                case (s16)MSG_END:
                    o->mode = 0;
                    o->timer = 2;
                    freeMessageGlyphs(o->slot);
                    restoreTalkPose();
                    o->state = 2;
                    break;
                case (s16)MSG_CLOSE:
                    o->mode = 1;
                    o->timer = 2;
                    freeMessageGlyphs(o->slot);
                    restoreTalkPose();
                    o->state = 3;
                    break;
                case (s16)MSG_HALT:
                    o->script--;
                    __asm__ volatile("");
                    restoreTalkPose();
                    o->timer = 2;
                    break;
                case (s16)MSG_POSE:
                    TALK_POSE = *o->script++;
                    S16(o, 0x20) = 1;
                    break;
                case (s16)MSG_NEWLINE:
                    o->cursorX = 8;
                    o->timer = 1;
                    o->cursorY += 0xD;
                    break;
                case (s16)MSG_CHOICE:
                    o->choice = 1;
                    o->timer = 1;
                    break;
                case (s16)MSG_WAIT:
                    if (o->mode == 2) {
                        o->timer = 0xB4;
                    }
                    o->subState = 1;
                    break;
                case (s16)MSG_CLEAR:
                    freeMessageGlyphs(o->slot);
                    MESSAGE_SLOTS[o->slot].count = 0;
                    o->cursorX = 8;
                    o->cursorY = 6;
                    o->timer = 1;
                    break;
                case (s16)MSG_BOX:
                    half = o->width >> 1;
                    height = o->height;
                    o->width = *o->script++;
                    o->height = *o->script++;
                    o->cursorX = 8;
                    o->cursorY = 6;
                    o->timer = 1;
                    o->lineX = -(o->width >> 1);
                    o->lineY = -o->height - 0x30;
                    {
                        s16 t = o->left + half;
                        o->left = t + o->lineX;
                    }
                    {
                        s16 t = o->top + height + 0x30;
                        o->top = t + o->lineY;
                    }
                    break;
                case (s16)MSG_MOVE:
                    {
                        s32 dx = *(s16 *)o->script++;
                        s32 dy = *(s16 *)o->script++;
                        o->timer = 1;
                        o->left += dx;
                        o->top += dy;
                    }
                    break;
                case (s16)MSG_DELAY:
                    o->timer = *o->script++;
                    break;
                case (s16)MSG_VOICE:
                    o->voice = *o->script++;
                    o->voiceTone = *o->script++;
                    if (o->voiceTone == 8) {
                        o->voiceTone = 0xFFFF;
                    }
                    TALK_POSE = 1;
                    o->timer = 1;
                    break;
                case (s16)MSG_COLOR:
                    o->color = *o->script++;
                    o->timer = 1;
                    break;
                default:
                    n = MESSAGE_SLOTS[o->slot].count++;
                    r = loadMessageGlyph(o->slot, code & 0xFFF, 0);
                    if ((code & 0x7000) == 0x4000) {
                        MESSAGE_GLYPHS[o->slot][n].id = r | 0xC000;
                        MESSAGE_GLYPHS[o->slot][n].x = o->width - 0x10;
                        MESSAGE_GLYPHS[o->slot][n].y = o->height - 0x10;
                        MESSAGE_GLYPHS[o->slot][n].clut = GetClut(0x150, o->color + 0x1F0);
                    } else {
                        MESSAGE_GLYPHS[o->slot][n].id = r | 0x8000;
                        MESSAGE_GLYPHS[o->slot][n].x = o->cursorX;
                        MESSAGE_GLYPHS[o->slot][n].y = o->cursorY;
                        MESSAGE_GLYPHS[o->slot][n].clut = GetClut(0x150, o->color + 0x1F0);
                    }
                    if ((code & 0x7000) == 0x5000) {
                        o->cursorX += GLYPH_CACHE[(s16)r].w;
                        o->timer = 1;
                    } else if ((code & 0x7000) != 0x6000) {
                        o->cursorX += GLYPH_CACHE[(s16)r].w;
                    }
                    if (SPAD->joypad_state & SPAD->buttonJump) {
                        o->timer = 1;
                    }
                    if (o->voice != -1) {
                        if (--o->voiceTimer == 0) {
                            playSFXWithNote(0xB, (u16)o->voice);
                            o->voiceTimer = 3;
                        }
                    }
                    break;
                }
            }
            break;
        case 1:
            if (o->choice == 0) {
                switch (o->mode) {
                case 3:
                case 6:
                    if (SPAD->joypad_state & SPAD->buttonJump) {
                        o->subState = 0;
                    }
                    break;
                case 2:
                    if (--o->timer == 0 || (SPAD->joypad_state & SPAD->buttonJump)) {
                        o->timer = 1;
                        o->subState = 0;
                    }
                    break;
                }
            } else {
                if (SPAD->debugNoSaveScreen == 0 && (SPAD->joypad_state & 0x4000)) {
                    playSFXWithNote(0xA, 0xA);
                    openMenuScreen(MENU_SCREEN_SAVE);
                    o->mode = 0;
                    o->timer = 2;
                    freeMessageGlyphs(o->slot);
                    o->state = 2;
                    o->subState = 0;
                }
                if (SPAD->joypad_state & 0x2000) {
                    playSFXWithNote(0x1B, 0x1C);
                    o->timer = 1;
                    o->subState = 0;
                }
            }
            break;
        }
        if (o->anchored != 0) {
            getBaseMatrix(m);
            pos->vx = o->worldX;
            pos->vy = o->worldY;
            pos->vz = o->worldZ;
            MulMatrix0((MATRIX *)D_1F8000C0, m, w);
            ApplyRotMatrix(pos, w->t);
            w->t[0] += SPAD->viewMatrix.t[0];
            w->t[1] += SPAD->viewMatrix.t[1];
            w->t[2] += SPAD->viewMatrix.t[2];
            SetRotMatrix(w);
            SetTransMatrix(w);
            zero->vx = 0;
            zero->vy = 0;
            zero->vz = 0;
            gte_ldv0(zero);
            gte_rtps();
            gte_stsxy(sxy);
            xy = *(s32 *)sxy;
            o->x.p.whole = xy;
            vy = *(s32 *)sxy >> 16;
            o->left = o->x.p.whole + o->lineX;
            o->z.p.whole = 0;
            yy = xy >> 16;
            o->top = yy + o->lineY;
            o->y.p.whole = vy;
        }
        if (o->left < 8) {
            o->left = 8;
        }
        if (o->top < 8) {
            o->top = 8;
        }
        d = 0x138 - (o->left + o->width);
        if (d < 0) {
            o->left += d;
        }
        d = 0xD8 - (o->top + o->height);
        if (d < 0) {
            o->top += d;
        }
        MESSAGE_SLOTS[o->slot].x = o->left;
        MESSAGE_SLOTS[o->slot].y = o->top;
        if (MESSAGE_SLOTS[o->slot].count != -1) {
            o->visible = 1;
            pushEffectDrawList(o);
        }
        if (o->closeTimer != -1 && !(--o->closeTimer & 0xFFFF)) {
            freeMessageGlyphs(o->slot);
            restoreTalkPose();
            o->state = 3;
        }
        break;
    case 2:
        break;
    case 3:
        freeEffectObject(o);
        break;
    }
}
#endif
