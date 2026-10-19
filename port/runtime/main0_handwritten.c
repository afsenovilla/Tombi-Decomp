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

/* func_800406E8 (0x1F0 bytes): floor probe against the slope list at *(u16 **)0x1F800278 (count, then records of
 * 4 halfwords: flags, y, dy, (span << 4) | start).  On a hit below the probe it snaps o->y (+0x16) onto it,
 * records the slope in 0x1F80027E..82 and returns 1. */
int func_800406E8(void *o, int x, int y) {
    u16 *p = SPAD(u16 *, 0x278);
    s16 n = (s16)*p;
    int t3 = x & 7, t4 = (s16)x, t5 = (s16)y;
    SPAD(u16 *, 0x278) = p + 1;
    if (n == 0) return 0;
    do {
        u16 *r = SPAD(u16 *, 0x278);
        u16 fl = r[0];
        n--;
        SPAD(u16 *, 0x278) = r + 4;
        if ((fl & 2) && !(fl & 0x10)) {
            u16 a3 = r[1], t2 = r[2], w = r[3];
            int start = w & 0xF, span = (w >> 4) & 0xF;
            if (span != 0 && t5 >= (s16)a3 - 0x10 && t3 >= start && start + span >= t3) {
                int v = (s16)t2 == 0 ? a3 : a3 + ((s16)t2 * ((t4 - start) % span)) / span;
                int d = v - y;
                if ((s16)d > 0) {
                    S16(o, 0x14) = 0;
                    SPAD(u16, 0x27E) = t2;
                    SPAD(u16, 0x280) = a3;
                    SPAD(u16, 0x282) = fl;
                    F16(o, 0x16) += d;
                    return 1;
                }
            }
        }
    } while (n != 0);
    return 0;
}

/* func_800416B8 (0x5F0 bytes): wall probe against the slope list at *(u16 **)0x1F800278 (count, then records of
 * 4 halfwords: flags, y, dy, (rise << 8) | (span << 4) | start), for an object moving left (animFrame even: flags
 * 0x14 / 0x4004) or right (odd: 0x18 / 0x4008).  A wall record whose height covers y switches to the plane list
 * of the neighbouring column (getCollisionPlaneAt, or the per-object table D_8007B5AC[mode] when mode != -1; at
 * most 7 hops).  A slope record pushes o->h (x) back onto the slope, sets the slope angle 0x1F80027C, and with a
 * rise (flags & 0xFF00) also steps o->y up by up to 8 pixels.  Returns 0 (clear), 1 (pushed) or 2 (stepped). */
extern char *getCollisionPlaneAt(short x, short y);
extern int angleFromDelta(int a, int b);
extern u8 *D_8007B5AC[];
static u16 *probe_column(void *o, int x, int mode, u8 *tbl) {
    if (mode != -1) {
        u8 *b = tbl + *(u16 *)(tbl + 8);
        int c = (s16)x;
        if (c < 0) c += 7;
        return (u16 *)(b + *(u16 *)(b + (c >> 3) * 2));
    }
    return (u16 *)getCollisionPlaneAt((s16)x, S16(F32(o, 0x44), 2));
}
int func_800416B8(void *o, int x, int y, int mode) {
    u16 *p = SPAD(u16 *, 0x278);
    int n = (s16)*p;
    u16 yy = (u16)y, x0 = (u16)x;
    u8 *tbl = D_8007B5AC[mode & 0xFF];
    int hops = 0;
    mode = (s16)mode;
    SPAD(u16 *, 0x278) = p + 1;
    if (n == 0) return 0;
    for (;;) {
        u16 *r = SPAD(u16 *, 0x278);
        u16 fl = r[0], ry = r[1], dy = r[2], w = r[3];
        int start, span, sub, d;
        SPAD(u16 *, 0x278) = r + 4;
        n--;
        if ((s16)dy == 0) goto next;
        start = w & 0xF;
        span = (w >> 4) & 0xF;
        d = (s16)(yy - ry);
        if ((s16)dy < 0) {
            if (d < (s16)dy) goto next;
            if (d > 0) return 0;
        } else {
            if ((s16)dy < d) return 0;
            if (d < 0) goto next;
        }
        sub = x & 7;
        if (F16(o, 0x2E) & 1) {                     /* moving right */
            if ((fl & 0x18) == 0x18) {
                x = x + ((start + span) - sub);
                if ((s16)++hops >= 8) return 0;
                goto column;
            }
            if ((fl & 0x4008) != 0x4008 || start + span < sub) goto next;
        } else {                                    /* moving left */
            if ((fl & 0x14) == 0x14) {
                x = (x - 1) - (sub - start);
                if ((s16)++hops >= 8) return 0;
                goto column;
            }
            if ((fl & 0x4004) != 0x4004 || sub < start) goto next;
        }
        {
            int q = (sub - start) - (span * ((s16)yy - (s16)ry)) / (s16)dy;
            u16 a, dh;
            if (F16(o, 0x2E) & 1) { if ((s16)q > 0) return 0; }
            else if ((s16)q < 0) return 0;
            x -= q;
            a = angleFromDelta(span, (s16)dy);
            SPAD(u16, 0x27C) = a;
            if ((s16)a < 0x40) SPAD(u16, 0x27C) = a + 0xC0;
            else if ((s16)a >= 0xC1) SPAD(u16, 0x27C) = a - 0xC0;
            F16(F32(o, 0x40), 2) -= (u16)(x0 - x);
            sub -= q;
            if ((w & 0xFF00) == 0) return 1;
            dh = (s16)dy > 0 ? (u16)(yy - ry) : (u16)(yy - ry - dy);
            if (dh >= 9) return 1;
            F16(o, 0x16) -= dh;
            if (span == 0) return 2;
            if ((s16)dy > 0) F16(F32(o, 0x40), 2) += start - sub;
            else F16(F32(o, 0x40), 2) += (start + span) - sub;
            return 2;
        }
    column:
        r = probe_column(o, x, mode, tbl);
        SPAD(u16 *, 0x278) = r + 1;
        n = (s16)*r;
        if (n == 0) return 0;
        continue;
    next:
        if ((s16)n == 0) return 0;
    }
}

/* func_80038184 (0xB4 bytes): script opcode "set variable": the record at script context +0x8A (an offset into
 * the script at D_8009D60C) is [op, dst, src...]; flag 0 copies variable src (a byte) to variable dst, flag 1
 * stores the 32-bit immediate that follows.  The variables are words at context +0x1090; the cursor advances 3
 * or 6 bytes. */
extern u8 *D_8009F0F0;
extern u32 D_8009D60C;
void func_80038184(int imm) {
    u8 *ctx = D_8009F0F0;
    u8 *rec = (u8 *)(F16(ctx, 0x8A) + D_8009D60C);
    u8 dst = rec[1];
    u8 *q = rec + 2;
    u32 v;
    if ((imm & 0xFF) == 0) v = F32(ctx, 0x1090 + q[0] * 4);
    else v = q[0] | q[1] << 8 | q[2] << 16 | (u32)q[3] << 24;
    F32(ctx, 0x1090 + dst * 4) = v;
    F16(ctx, 0x8A) += (imm & 0xFF) ? 6 : 3;
}

/* func_8004AE7C (0x5D8 bytes): layer-2 object handler (MAIN0 table D_80077D6C slot 0x60): the event actor that
 * carries / walks the player in cutscenes.  Init picks its sprite (+0x1E) and animation script (+0xA8, one of
 * four in MAIN0 .rodata) by area and section, plus the event kind +0x6A (0: eventTakePlayerControlA/
 * eventCarryPlayerB, 1..3: eventTakePlayerControlC/D, chosen by D_8009CFF9).  Subtype 1 starts on the player. */
extern void AnimLoadDuration(void *);
extern int AnimAdvance(void *);
extern void ObjCullRegister(void *);
extern void freeObjectLayer2(void *);
extern void eventTakePlayerControlA(void *), eventCarryPlayerB(void *);
extern void eventTakePlayerControlC(void *), eventTakePlayerControlD(void *);
extern void func_80116398(void *);                 /* AREA13 overlay: dispatched by the call thunk */
extern u8 D_8009CFF9;
#define ANIM_28 0x80015028u
#define ANIM_40 0x80015040u
#define ANIM_54 0x80015054u
#define ANIM_6C 0x8001506Cu
static void evactor_pick(u8 *o) {
    int area = *(volatile u16 *)0x8009C960u, sec = *(volatile u16 *)0x8009C962u;
    int t = -1, kind = -1;                        /* -1: leave +0x1E / +0x6A as they are */
    u32 anim = ANIM_28;
    switch (area) {
    case 0: t = 0xE; anim = ANIM_40; break;
    case 1: anim = ANIM_40; if (sec < 2) t = 0xB; else if (sec < 5) t = 6; break;
    case 2:
        switch (sec) {
        case 0: t = 0xF; anim = ANIM_6C; kind = 1; break;
        case 1: case 2: t = 0xE; break;
        case 3: t = 0x10; anim = ANIM_54; kind = 3; break;
        case 4: case 5: t = 0x11; break;
        default: return;
        }
        break;
    case 3: if (sec < 7) t = (sec == 0 || sec == 4) ? 0xD : 5; break;
    case 4: t = 0x10; break;
    case 5:
        if (sec == 0 || sec == 2) { t = 0xF; anim = ANIM_6C; kind = 2; }
        else if (sec == 1 || sec == 3) t = 0xB;
        else return;
        break;
    case 6: t = 0xF; break;
    case 8:
        if (sec == 0 || sec == 2) { t = 0xF; anim = ANIM_6C; kind = 2; }
        else if (sec == 1 || sec == 3) t = 0xE;
        else return;
        break;
    case 9:
        switch (sec) {
        case 0: t = 0xC; anim = ANIM_40; break;
        case 1: t = 0x12; break;
        case 2: t = 0x11; break;
        case 3: t = 0xB; break;
        case 4: case 5: t = 0xF; break;
        default: return;
        }
        break;
    case 10: if (sec < 8) t = 7; else if (sec == 8) t = 0xC; break;
    case 11: case 13: t = 0xD; break;
    case 14: t = 0xA; break;
    case 16: if (sec < 4 || sec >= 7) return; t = 0xF; break;
    case 17: t = 0xB; break;
    case 18: if (sec == 1) t = 0xA; else if (sec == 2) t = 9; break;
    case 19:
        if (sec == 0) { t = 0xF; anim = ANIM_6C; kind = 1; }
        else if (sec == 1) t = 0xE;
        else if (sec == 2) { t = 0x12; anim = ANIM_54; kind = 2; }
        else return;
        break;
    default: return;
    }
    if (t >= 0) F16(o, 0x1E) = t;
    F32(o, 0xA8) = anim;
    if (kind >= 0) F8(o, 0x6A) = kind;
}
void func_8004AE7C(u8 *o) {
    u8 *pl = D_800A6038;
    switch (F8(o, 4)) {
    case 0:
        if (F8(o, 3) == 0) { F8(o, 4) = 3; return; }
        F8(o, 0x6A) = 0;
        evactor_pick(o);
        F8(o, 0x1C) |= 0x80;
        F8(o, 4)++;
        F8(o, 0xA) = 2;
        F16(o, 0x6E) = 0x20;
        F16(o, 0x72) = 0x20;
        F16(o, 0x2E) = 0;
        F32(o, 0x84) = 0;
        F32(o, 0x88) = 0;
        F32(o, 0x8C) = 0;
        F16(o, 0x6C) = 0x10;
        F16(o, 0x70) = 0x10;
        F8(o, 0xD) = 0x81;
        F8(o, 0xF) = F8(pl, 0xF) + 1;
        F16(o, 8) = GetClut(0x80, 0x1F9);
        F16(o, 0xAC) = 3;
        F32(o, 0x3C) = SPAD(u32, 0x304);
        F32(o, 0x24) = F32(F32(o, 0xA8), 0xC);
        AnimLoadDuration(o);
        if (F8(o, 3) == 1) {
            F16(o, 0x12) = F16(pl, 0x12);
            F16(o, 0x16) = F16(pl, 0x16);
            F16(o, 0x1A) = F16(pl, 0x1A);
        }
        if (*(volatile u32 *)0x8009C960u == 0x1000D) F8(o, 4) = 4;   /* AREA13 section 1 */
        return;
    case 1:
        ObjCullRegister(o);
        if (F8(o, 3) == 0) { AnimAdvance(o); return; }
        if (F8(o, 0x6A) == 0) {
            if (D_8009CFF9 == 0) eventTakePlayerControlA(o);
            else if (D_8009CFF9 == 1) eventCarryPlayerB(o);
        } else if (F8(o, 0x6A) < 4) {
            if (D_8009CFF9 == 0) eventTakePlayerControlC(o);
            else if (D_8009CFF9 == 1) eventTakePlayerControlD(o);
        }
        return;
    case 2: case 3:
        freeObjectLayer2(o);
        return;
    case 4:
        func_80116398(o);
        return;
    }
}

const PortFunc port_extra_funcs[] = {
    {0x8004AE7C, (void *)func_8004AE7C, "func_8004AE7C"},
    {0x800416B8, (void *)func_800416B8, "func_800416B8"},
    {0x80038184, (void *)func_80038184, "func_80038184"},
    {0x800406E8, (void *)func_800406E8, "func_800406E8"},
    {0x8002CF40, (void *)func_8002CF40, "func_8002CF40"},
    {0, 0, 0},
};
