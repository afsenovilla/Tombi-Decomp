// FUNC 8004d620 1324 MAIN0
// MATCHING 8004d620 1324
#include "gamestate.h"
typedef struct O {
    unsigned char f00;
    char pad0[1];
    unsigned char f02;
    char pad1[7];
    unsigned char f0A;
    char pad2[4];
    unsigned char f0F;
    union { int d; struct { short lo, hi; } p; } u10;
    union { int d; struct { short lo, hi; } p; } u14;
    int f18;
    char pad3[2];
    short f1E;
    char pad4[148];
    short fB4;
    short fB6;
    char pad5[2];
    short fBA;
    char pad6[8];
    short fC4;
    unsigned short fC6;
    char pad7[2];
    unsigned short fCA;
    short fCC;
    short fCE;
} O;
#define F00 o->f00
#define F02 o->f02
#define F0A o->f0A
#define F0F o->f0F
#define F10 (*(int *)((char *)o + 0x10))
#define F12 (*(short *)((char *)o + 0x12))
#define F14 o->u14.d
#define F16 (*(short *)((char *)o + 0x16))
#define F18 o->f18
#define F1E o->f1E
#define F20 (*(short *)((char *)o + 0x20))
#define F22 (*(short *)((char *)o + 0x22))
#define F2C (*(short *)((char *)o + 0x2c))
#define FB4 o->fB4
#define FB6 o->fB6
#define FBA o->fBA
#define FBC (*(short *)((char *)o + 0xbc))
#define FBE (*(short *)((char *)o + 0xbe))
#define FC0 (*(short *)((char *)o + 0xc0))
#define FC2 (*(short *)((char *)o + 0xc2))
#define FC4 (*(short *)((char *)o + 0xc4))
#define FC6 o->fC6
#define FC8 (*(short *)((char *)o + 0xc8))
#define FCA o->fCA
#define FCC (*(short *)((char *)o + 0xcc))
#define FCE o->fCE
#define FD0 (*(short *)((char *)o + 0xd0))
typedef struct G { short n, f2, f4, f6; } G;
extern G MESSAGE_SLOTS[];
extern unsigned char INFO_MESSAGE_SLOT;
extern unsigned char *D_8007DD34[];
extern unsigned char *D_8007DE38[];
extern unsigned char D_8007DD0C[];
extern unsigned char D_8007DE24[];
extern signed char D_8007DE88[];
extern unsigned short D_8007D454[];
extern unsigned short D_8007D5A4[];
extern char *WFM3_DATA;
extern void playSFX(int);
extern O *ObjAlloc(void);
extern void drawInfoMessageText(O *, int);
extern void func_8004DC10(O *, int);

static __inline__ void setpos(O *o, short x, short y)
{
    FBC = 0;
    FBA = -((unsigned short)FC2 >> 1);
    FC0 = y + FBC;
    FBE = x + FBA;
}

static __inline__ void callit(O *o, int a)
{
    int i = 0;
    signed char c;
    do {
        c = D_8007DE88[i];
        if (c == -1) { drawInfoMessageText(o, 0); return; }
        i++;
    } while (c != a);
    drawInfoMessageText(o, 1);
}

#define TBL(x) (*(unsigned short *)(WFM3_DATA + (x) * 2 + 0x10))

#ifdef PORT2
/* every popup: its box width comes from MAINn tables (+ per-language pads), not from the text it draws, so a translated
   verb/name (verb first: "Obtienes Bumerán de madera") overflowed the right border. Once the text is laid out, widen
   the box to the real glyph extent (the WFM advance widths; glyph x is relative to the box) plus the 8-pixel margin
   the text starts at on the left, recentred as setpos() does; never narrower than the table width.  Corner icons
   (0x4000 glyphs, placed at FC2 - 16 by drawMessageGlyph) follow the right edge. */
typedef struct { unsigned short id; short pad; short x, y; } PortGE;
extern PortGE MESSAGE_GLYPHS[][128];
extern unsigned char D_800A5DE4[];
static void port_fit_box(O *o)
{
    int i, n = MESSAGE_SLOTS[FCA].n, right = 0, need;
    for (i = 0; i < n; i++) {
        PortGE *g = &MESSAGE_GLYPHS[FCA][i];
        int r;
        if (g->id & 0x4000) continue;
        r = g->x + D_800A5DE4[(g->id & 0xfff) * 10 + 2];
        if (r > right) right = r;
    }
    need = right + 8;
    /* widen when the text does not fit; with --lang 5 (translated texts, verb-first popups) also shrink to the text,
       so "Usas Cubo de agua" does not keep the original's spare width */
    { extern int port_lang;
      if (need == FC2 || need > 0x130 || (need < FC2 && port_lang != 5)) return; }   /* 0x130: never past the 320-pixel screen */
    for (i = 0; i < n; i++)
        if (MESSAGE_GLYPHS[FCA][i].id & 0x4000) MESSAGE_GLYPHS[FCA][i].x += need - FC2;
    FBE += ((unsigned short)FC2 >> 1) - ((unsigned short)need >> 1);
    FC2 = need;
}
#endif

O *printInfoMessage(int a, int b)
{
    int i;
    unsigned char *s1;
    unsigned char *s5;
    short flag;
    O *o;
    unsigned short *p;
    unsigned short k;
    unsigned short *q;
    char *base;
    int r;
    unsigned short h;
    unsigned int n;

    INFO_MESSAGE_SLOT = 0xff;
    for (i = 2; i >= 0; i--) {
        if (MESSAGE_SLOTS[i].n == -1) {
            INFO_MESSAGE_SLOT = i;
            MESSAGE_SLOTS[i].f2 = i;
            break;
        }
    }
    if (INFO_MESSAGE_SLOT == 0xff) return 0;
    if (b == 1 || b == 4) playSFX(0xc);
    if ((unsigned)(b - 2) < 2) {
        s1 = D_8007DD34[a];
        s1 += GAME.selectedArea * 2;
        if (*s1 == 0xff)
            { s1 = D_8007DD0C; s1 += GAME.selectedArea * 2; }
    } else {
        s5 = D_8007DE38[GAME.selectedArea];
        s5 += a;
    }
    flag = 0;
    o = ObjAlloc();
    if (o != 0) {
        F00 = 1;
        F02 = 0x1c;
        F0A = 0xc;
        F1E = 0x14;
        F2C = 0;
        F0F = 0;
        F10 = 0xa00000;
        F18 = 0;
        FB4 = a;
        FB6 = b;
        F14 = (0xd4 - (3 - INFO_MESSAGE_SLOT) * 20) << 16;
#ifdef PORT2
        /* the port plays other MAINn (--lang): the constants of the switch below differ per language (port2/runtime/lang.c) */
        { extern int port_lang_box_pad[5];
          if ((unsigned)b < 5) FC2 = (b == 2 || b == 3 ? D_8007D5A4[a] : D_8007D454[a]) + port_lang_box_pad[b]; }
#else
        switch (b) {
        case 0:
            FC2 = D_8007D454[a] + 0x60;
            break;
        case 1:
            FC2 = D_8007D454[a] + 0x58;
            break;
        case 2:
        case 3:
            FC2 = D_8007D5A4[a] + 0x10;
            break;
        case 4:
            FC2 = D_8007D454[a] + 0x78;
            break;
        }
#endif
        FC4 = 0x14;
        *(unsigned short *)((char *)o + 0xc6) = 0xffff;
        setpos(o, F12, F16);
        i = 1;
        *(unsigned short *)((char *)o + 0xca) = INFO_MESSAGE_SLOT;
        F20 = i;
        *(short *)((char *)o + 0xce) = 8;
        FD0 = 6;
        F22 = 0;
        o->fCC = i;
        *(short *)((char *)MESSAGE_SLOTS + FCA * 8) = 0;
        base = WFM3_DATA;
        if ((unsigned)(b - 2) < 2) {
            q = (unsigned short *)(s1[0] * 2 + (int)base); FC8 = q[8] + s1[1];
        } else {
            short c = *s5;
            q = (unsigned short *)(base + (D_8007DE24[GAME.selectedArea] * 2 + 0x10));
            if ((unsigned char)c == 0xff) {
                s1 = D_8007DD0C + GAME.selectedArea * 2;
                q = (unsigned short *)(s1[0] * 2 + (int)base); FC8 = q[8] + s1[1];
            } else {
                FC8 = c + *q;
            }
        }
        if (b == 1 || b == 4) {
            callit(o, a);
        }
#ifdef PORT2
        /* tools/verb_order.py patch in MAINn: "obtained" draws the verb (dialogue 2) before the name too (port2/runtime/lang.c) */
        { extern int port_verb_first; if (port_verb_first && (b == 0 || b == 3)) drawInfoMessageText(o, 2); }
#endif
        base = WFM3_DATA;
        base += *(short *)(base + 8);
        q = (unsigned short *)(base + (unsigned short)FC8 * 2);
        p = (unsigned short *)(base + *(short *)q);
        while (1) {
            k = *p++;
            if ((unsigned short)(k + 2) < 2) {
#ifdef PORT2
                { extern int port_verb_first; if (!port_verb_first && (b == 0 || b == 3)) drawInfoMessageText(o, 2); }
                port_fit_box(o);
                return o;
#endif
                if (b == 0 || b == 3) drawInfoMessageText(o, 2);
                return o;
            }
            if (flag == 0 || (short)k == -12) {
                switch ((short)k) {
                case -5:
                    FCE = FBE + 8;
                    FD0 = FC0 + 8;
                    break;
                case -6:
#ifdef PORT2
                    /* lost/escaped popups size their box from a MAINn table, not the {BOX}: a translated text longer than the
                       original overflowed ("La rana se escapó..."); take the BOX width when it is wider */
                    if ((b == 2 || b == 3) && (short)p[0] > (short)FC2) {
                        FBE -= ((unsigned short)p[0] >> 1) - ((unsigned short)FC2 >> 1);
                        FC2 = p[0];
                    }
#endif
                    h = (unsigned short)FC2 >> 1;
                    p += 2;
                    FBC = 0;
                    FBA = -h;
                    {short t = FC0 + o->fC4; FC0 = t + FBC;}
                    {short t = FBE + h; FBE = t + FBA;}
                    break;
                case -8:
                    FC6 = *p++;
                    break;
                case -12:
                    if (b != 1 && b != 4) flag ^= 1;
                    break;
                default:
                    func_8004DC10(o, (short)k);
                    break;
                case -7: case -4: case -3: case -2: case -1:
                    break;
                }
            }
        }
    }
    return o;
}
