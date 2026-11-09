// JP queueLoadList (retail form; PAL queueLoadList 8004f538 + conditional entries)
// VERSION SLPS_01144 slps01144 800200C4 1656
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "globals.h"
typedef struct {
    short id;
    unsigned char idx;
    unsigned char type;
    char *ptr;
    int size;
    short cond;
    short pad2;
    int flags;
} Rel;

typedef struct { Rel *e; char *base; } RelLog;

extern char *SEQ_DATA[];
extern char *LOAD_SLOTS[];
extern char *SPR_DATA[];
extern char *FOUR_DATA[];
extern char *WMD_DATA[];
extern char *WPP_DATA[];
extern char *WHM_DATA[];
extern char *APD_DATA[];
extern char *WSS_DATA[];
extern char *WFM3_DATA[];
extern int CD_QUEUE_HEAD;
extern char *LOAD_NEXT_ADDR;
extern char D_800E9AF8[];
extern char *D_8009EE98[];
extern char *D_8009EF98[];
extern unsigned char D_8009EE6B[];
#define D_8009EE6B_F D_8009EE6B[0]
extern int D_8007239C[];
extern RelLog CD_QUEUE[];

int func_800200C4_slps01144(Rel *e, int unused, char *base)
{
    int pending = 0;
    char *saved;
    unsigned char t;
    int lo;

    while (e->id != -1) {
        t = e->type;
        if ((t & 0xf0) == 0x90 || (t & 0xf0) == 0xb0) {
            switch (e->cond) {
            case 1: if (D_8009EE6B_F & 2) { e++; continue; } break;
            case 2: if (!(D_8009EE6B_F & 2)) { e++; continue; } break;
            case 3: if (D_8009EE6B_F & 4) { e++; continue; } break;
            case 4: if (!(D_8009EE6B_F & 4)) { e++; continue; } break;
            }
        }
        if ((e->type & 0xd0) != 0xd0 && e->type != 0x55 && pending == 1) {
            base = saved;
            pending = 0;
        }
        if (e->ptr != 0) base = e->ptr;
        if (e->idx != 0xff) {
            if (e->idx < 0x80) LOAD_SLOTS[e->idx] = base;
            else base = LOAD_SLOTS[e->idx & 0x7f];
        }
        if (e->type == 0xf0) {
            pending = 1;
            saved = base;
            base = D_800E9AF8;
        }
        CD_QUEUE[CD_QUEUE_HEAD].e = e;
        CD_QUEUE[CD_QUEUE_HEAD].base = base;
        CD_QUEUE_HEAD = (CD_QUEUE_HEAD + 1) & 0x7f;
        if ((e->flags & 0xf) < 3) {
            t = e->type;
            lo = t & 0xf;
            switch (t & 0xf0) {
            case 0x00: case 0x10: case 0x20: break;
            case 0x30: SPR_DATA[lo] = base; break;
            case 0x40: FOUR_DATA[lo] = base; break;
            case 0x50: WMD_DATA[lo] = base; break;
            case 0x60: WPP_DATA[lo] = base; break;
            case 0x70: WHM_DATA[lo] = base; break;
            case 0x80: APD_DATA[lo] = base; break;
            case 0x90: case 0xa0: break;
            case 0xb0: SEQ_DATA[lo] = base; break;
            case 0xc0: WSS_DATA[lo] = base; break;
            case 0xd0: WFM3_DATA[lo] = base; break;
            }
            if ((e->type & 0xf0) == 0x90) base += D_8007239C[e->pad2 * 2];
            else if ((e->type & 0xf0) != 0x10) base += e->size;
            e++;
        } else {
            do {
                t = e->type;
                lo = t & 0xf;
                switch (t & 0xf0) {
                case 0x00: case 0x10: case 0x20: break;
                case 0x30: SPR_DATA[lo] = base; break;
                case 0x40: FOUR_DATA[lo] = base; break;
                case 0x50: WMD_DATA[lo] = base; break;
                case 0x60: WPP_DATA[lo] = base; break;
                case 0x70: WHM_DATA[lo] = base; break;
                case 0x80: APD_DATA[lo] = base; break;
                case 0x90: D_8009EE98[lo] = base; break;
                case 0xa0: D_8009EF98[lo] = base; break;
                case 0xb0: SEQ_DATA[lo] = base; break;
                case 0xc0: WSS_DATA[lo] = base; break;
                case 0xd0: WFM3_DATA[lo] = base; break;
                }
                if ((e->type & 0xf0) == 0x90) base += D_8007239C[e->pad2 * 2];
                else if ((e->type & 0xf0) != 0x10) base += e->size;
                e++;
            } while (e->flags == -1);
        }
        switch ((int)base & 3) {
        case 1: base++;
        case 2: base++;
        case 3: base++;
        }
    }
    if (pending == 1) LOAD_NEXT_ADDR = saved;
    else LOAD_NEXT_ADDR = base;
    return 0;
}
#endif
