/* MAIN0 functions the decomp has only as asm (no unit in src/): C translations for the port, written from
 * build/full/asm/main0/nonmatchings/main0/<name>.s (m2c draft, checked by hand against the asm).  They are found
 * through port_extra_funcs (port_lookup falls back to it), since the game reaches them through tables in retail
 * data. */
#include "port_runtime.h"

typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;

#define SPAD(T, a) (*(volatile T *)(0x1F800000u + (a)))
#define F8(o, off)  (*(u8 *)((u8 *)(o) + (off)))
#define F16(o, off) (*(u16 *)((u8 *)(o) + (off)))
#define S16(o, off) (*(s16 *)((u8 *)(o) + (off)))
#define F32(o, off) (*(u32 *)((u8 *)(o) + (off)))

extern void ApplyRotMatrix(void *, void *);
extern u16 GetClut(int, int);
extern void MulMatrix0(void *, void *, void *);
extern void ObjFree(void *);
extern void ObjListPush_1F800220(void *);
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern int SfxPlay2(int, int);
extern void freeMessageGlyphs(unsigned int);
extern void getBaseMatrix(void *);
extern short loadMessageGlyph(int, int, int);
extern void openMenuScreen(int);
extern void func_800E8A98(void *, int, int);       /* overlay functions: dispatched by the call thunks */
extern void func_800E940C(void *, int, int);
extern void func_800E96E8(void *, int, int);
extern void func_800EEA7C(void *, int, int);
extern u8 D_1F8000C0[];
extern u16 D_8009C960;
extern s8 D_8009D00F;
extern u8 D_800A4648[];      /* message slots, 8 bytes: s16 glyph count, -, s16 x, s16 y */
extern u8 D_800A4DE0[];      /* glyph records: 0x400 bytes per slot, 8 per glyph */
extern u8 D_800A5DE6[];      /* glyph widths, 10 bytes per glyph */
extern u8 D_800A6038[];
extern u8 D_800A603A;
extern s16 D_800A6066;
extern s16 D_800B1418;
void port_gte_op(unsigned long op);
void port_gte_mtc2(int reg, unsigned long v);
unsigned long port_gte_mfc2(int reg);

static void msg_notify_close(void) {
    D_8009D00F = 0;
    switch (D_800A603A) {
    case 0: if (D_8009C960 == 9) func_800EEA7C(D_800A6038, 0, 0); break;
    case 1: func_800E8A98(D_800A6038, 0, D_800A6066); break;
    case 2: func_800E96E8(D_800A6038, 0, D_800A6066); break;
    case 3: func_800E940C(D_800A6038, 0, D_800A6066); break;
    }
}
#define PAD_HIT() (SPAD(u16, 0x3C6) & SPAD(u16, 0x1FC))

/* func_8002CF40 (0xD10 bytes): the message box object (pool object update): runs the message script at +0x94
 * (glyphs and control codes 0xFFF2..0xFFFF), places the box over its owner and waits for the pad. */
void func_8002CF40(void *o) {
    switch (F8(o, 4)) {
    case 0:
        F16(o, 0xC8) = 0;
        F8(o, 5) = 0;
        F8(o, 4)++;
        return;
    case 1:
        break;
    case 3:
        ObjFree(o);
        return;
    default:
        return;
    }
    if ((s16)++F16(o, 0x22) >= 0x3C) F16(o, 0x22) = 0;
    if (S16(o, 0xC6) != -1) D_800B1418 = D_8009C960 == 6 ? 4 : 0x78;

    if (F8(o, 5) == 0) {
        if (PAD_HIT()) F16(o, 0x20) = 1;
        while ((u16)--F16(o, 0x20) == 0) {
            u16 *pc = *(u16 **)((u8 *)o + 0x94);
            u16 c;
            F16(o, 0x20) = F16(o, 0xB4);
            *(u16 **)((u8 *)o + 0x94) = pc + 1;
            c = *pc;
            switch ((u16)(c + 0xE) < 0xE ? (s16)(c + 0xE) : -1) {
            case 13:                                    /* 0xFFFF: end, close */
                F16(o, 0xB4) = 0;
                F16(o, 0x20) = 2;
                freeMessageGlyphs(F16(o, 0xCA));
                msg_notify_close();
                F8(o, 4) = 2;
                break;
            case 12:                                    /* 0xFFFE: end, free the object */
                F16(o, 0xB4) = 1;
                F16(o, 0x20) = 2;
                freeMessageGlyphs(F16(o, 0xCA));
                msg_notify_close();
                F8(o, 4) = 3;
                break;
            case 1:                                     /* 0xFFF3: stay on this code */
                *(u16 **)((u8 *)o + 0x94) -= 1;
                msg_notify_close();
                F16(o, 0x20) = 2;
                break;
            case 0: {                                   /* 0xFFF2 n */
                u16 *p = *(u16 **)((u8 *)o + 0x94);
                *(u16 **)((u8 *)o + 0x94) = p + 1;
                D_8009D00F = (s8)*p;
                F16(o, 0x20) = 1;
                break;
            }
            case 11:                                    /* 0xFFFD: new line */
                F16(o, 0xCE) = 8;
                F16(o, 0x20) = 1;
                F16(o, 0xD0) += 0xD;
                break;
            case 3:                                     /* 0xFFF5 */
                F16(o, 0xB8) = 1;
                F16(o, 0x20) = 1;
                break;
            case 10:                                    /* 0xFFFC: wait for the pad */
                if (F16(o, 0xB4) == 2) F16(o, 0x20) = 0xB4;
                F8(o, 5) = 1;
                break;
            case 9:                                     /* 0xFFFB: clear the box */
                freeMessageGlyphs(F16(o, 0xCA));
                *(s16 *)(D_800A4648 + F16(o, 0xCA) * 8) = 0;
                F16(o, 0xCE) = 8;
                F16(o, 0xD0) = 6;
                F16(o, 0x20) = 1;
                break;
            case 8: {                                   /* 0xFFFA w h: box size */
                u16 *p = *(u16 **)((u8 *)o + 0x94);
                u16 oldc4 = F16(o, 0xC4);
                u16 halfw = F16(o, 0xC2) >> 1;
                F16(o, 0xC2) = p[0];
                F16(o, 0xC4) = p[1];
                *(u16 **)((u8 *)o + 0x94) = p + 2;
                F16(o, 0xCE) = 8;
                F16(o, 0xD0) = 6;
                F16(o, 0x20) = 1;
                F16(o, 0xBA) = -(s16)(F16(o, 0xC2) >> 1);
                F16(o, 0xBC) = -(s32)F16(o, 0xC4) - 0x30;
                F16(o, 0xBE) = F16(o, 0xBE) + halfw + F16(o, 0xBA);
                F16(o, 0xC0) = F16(o, 0xC0) + oldc4 + 0x30 + F16(o, 0xBC);
                break;
            }
            case 4: {                                   /* 0xFFF6 dx dy: move the box */
                u16 *p = *(u16 **)((u8 *)o + 0x94);
                *(u16 **)((u8 *)o + 0x94) = p + 2;
                F16(o, 0x20) = 1;
                F16(o, 0xBE) += (s16)p[0];
                F16(o, 0xC0) += p[1];
                break;
            }
            case 7: {                                   /* 0xFFF9 n: delay */
                u16 *p = *(u16 **)((u8 *)o + 0x94);
                *(u16 **)((u8 *)o + 0x94) = p + 1;
                F16(o, 0x20) = *p;
                break;
            }
            case 6: {                                   /* 0xFFF8 sfx voice */
                u16 *p = *(u16 **)((u8 *)o + 0x94);
                S16(o, 0xC6) = (s16)p[0];
                F16(o, 0xB6) = p[1];
                *(u16 **)((u8 *)o + 0x94) = p + 2;
                if (p[1] == 8) F16(o, 0xB6) = 0xFFFF;
                D_8009D00F = 1;
                F16(o, 0x20) = 1;
                break;
            }
            case 5: {                                   /* 0xFFF7 n: palette */
                u16 *p = *(u16 **)((u8 *)o + 0x94);
                *(u16 **)((u8 *)o + 0x94) = p + 1;
                F16(o, 0x20) = 1;
                F16(o, 0xC8) = *p;
                break;
            }
            default: {                                  /* a glyph (0xFFF4 too: its jump table entry is this one) */
                int slot = F16(o, 0xCA);
                u16 n = *(u16 *)(D_800A4648 + slot * 8);
                *(u16 *)(D_800A4648 + slot * 8) = n + 1;
                s16 g = loadMessageGlyph(slot, c & 0xFFF, 0);
                u8 *rec = D_800A4DE0 + ((s32)((u32)n << 16) >> 13) + (slot << 10);
                if ((c & 0x7000) == 0x4000) {
                    *(u16 *)(rec + 0) = g | 0xC000;
                    *(u16 *)(rec + 4) = F16(o, 0xC2) - 0x10;
                    *(u16 *)(rec + 6) = F16(o, 0xC4) - 0x10;
                } else {
                    *(u16 *)(rec + 0) = g | 0x8000;
                    *(u16 *)(rec + 4) = F16(o, 0xCE);
                    *(u16 *)(rec + 6) = F16(o, 0xD0);
                }
                *(u16 *)(rec + 2) = GetClut(0x150, F16(o, 0xC8) + 0x1F0);
                if ((c & 0x7000) == 0x5000) {
                    F16(o, 0x20) = 1;
                    F16(o, 0xCE) += D_800A5DE6[g * 10];
                } else if ((c & 0x7000) != 0x6000) {
                    F16(o, 0xCE) += D_800A5DE6[g * 10];
                }
                if (PAD_HIT()) F16(o, 0x20) = 1;
                if (S16(o, 0xC6) != -1) {
                    if ((u16)--F16(o, 0xCC) == 0) {
                        SfxPlay2(0xB, F16(o, 0xC6));
                        F16(o, 0xCC) = 3;
                    }
                }
                break;
            }
            }
        }
    } else if (F8(o, 5) == 1) {
        if (F16(o, 0xB8) == 0) {
            switch (F16(o, 0xB4)) {
            case 3:
            case 6:
                if (PAD_HIT()) F8(o, 5) = 0;
                break;
            case 2:
                if ((u16)--F16(o, 0x20) == 0 || PAD_HIT()) { F16(o, 0x20) = 1; F8(o, 5) = 0; }
                break;
            }
        } else {
            if (SPAD(u8, 0x1BA) == 0 && (SPAD(u16, 0x1FC) & 0x4000)) {
                SfxPlay2(0xA, 0xA);
                openMenuScreen(4);
                F16(o, 0xB4) = 0;
                F16(o, 0x20) = 2;
                freeMessageGlyphs(F16(o, 0xCA));
                F8(o, 4) = 2;
                F8(o, 5) = 0;
            }
            if (SPAD(u16, 0x1FC) & 0x2000) {
                SfxPlay2(0x1B, 0x1C);
                F16(o, 0x20) = 1;
                F8(o, 5) = 0;
            }
        }
    }

    if (F16(o, 0x2E) != 0) {             /* follow the owner: project its position */
        u32 sxy;
        getBaseMatrix((void *)0x1F800020);
        SPAD(s16, 0x40) = (s16)F32(o, 0x30);
        SPAD(s16, 0x42) = (s16)F32(o, 0x34);
        SPAD(s16, 0x44) = (s16)F32(o, 0x38);
        MulMatrix0(D_1F8000C0, (void *)0x1F800020, (void *)0x1F800000);
        ApplyRotMatrix((void *)0x1F800040, (void *)0x1F800014);
        SPAD(s32, 0x14) += SPAD(s32, 0xD4);
        SPAD(s32, 0x18) += SPAD(s32, 0xD8);
        SPAD(s32, 0x1C) += SPAD(s32, 0xDC);
        SetRotMatrix((void *)0x1F800000);
        SetTransMatrix((void *)0x1F800000);
        SPAD(s16, 0x48) = 0; SPAD(s16, 0x4A) = 0; SPAD(s16, 0x4C) = 0;
        port_gte_mtc2(0, SPAD(u32, 0x48));
        port_gte_mtc2(1, SPAD(u32, 0x4C));
        port_gte_op(0x4A180001);                   /* RTPS */
        SPAD(u32, 0x50) = port_gte_mfc2(14);
        sxy = SPAD(u32, 0x50);
        F16(o, 0x12) = (u16)sxy;
        F16(o, 0xBE) = (u16)sxy + F16(o, 0xBA);
        F16(o, 0x1A) = 0;
        F16(o, 0xC0) = (u16)((s32)sxy >> 16) + F16(o, 0xBC);
        F16(o, 0x16) = SPAD(s16, 0x52);
    }
    if (S16(o, 0xBE) < 8) F16(o, 0xBE) = 8;
    if (S16(o, 0xC0) < 0x18) F16(o, 0xC0) = 0x18;
    {
        s32 d = 0x138 - (F16(o, 0xBE) + F16(o, 0xC2));
        if (d & 0x8000) F16(o, 0xBE) += d;
        d = 0xE8 - (F16(o, 0xC0) + F16(o, 0xC4));
        if (d & 0x8000) F16(o, 0xC0) += d;
    }
    *(u16 *)(D_800A4648 + F16(o, 0xCA) * 8 + 4) = F16(o, 0xBE);
    *(u16 *)(D_800A4648 + F16(o, 0xCA) * 8 + 6) = F16(o, 0xC0);
    if (*(s16 *)(D_800A4648 + F16(o, 0xCA) * 8) != -1) {
        F8(o, 1) = 1;
        ObjListPush_1F800220(o);
    }
    if (S16(o, 0xD2) != -1 && (u16)--F16(o, 0xD2) == 0) {
        freeMessageGlyphs(F16(o, 0xCA));
        msg_notify_close();
        F8(o, 4) = 3;
    }
}

const PortFunc port_extra_funcs[] = {
    {0x8002CF40, (void *)func_8002CF40, "func_8002CF40"},
    {0, 0, 0},
};
