// VERSION BETA_980203 beta980203 800215BC 1656
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_800200C4_slps01144.c (func_800200C4_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
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
extern char D_800E9AF8_beta980203[];
extern char *D_8009EE98_beta980203[];
extern char *D_8009EF98_beta980203[];
extern unsigned char D_8009EE6B_beta980203[];
#define D_8009EE6B_F D_8009EE6B_beta980203[0]
extern int D_8007239C_beta980203[];
extern RelLog CD_QUEUE[];

int func_800215BC_beta980203(Rel *e, int unused, char *base)
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
            base = D_800E9AF8_beta980203;
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
            if ((e->type & 0xf0) == 0x90) base += D_8007239C_beta980203[e->pad2 * 2];
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
                case 0x90: D_8009EE98_beta980203[lo] = base; break;
                case 0xa0: D_8009EF98_beta980203[lo] = base; break;
                case 0xb0: SEQ_DATA[lo] = base; break;
                case 0xc0: WSS_DATA[lo] = base; break;
                case 0xd0: WFM3_DATA[lo] = base; break;
                }
                if ((e->type & 0xf0) == 0x90) base += D_8007239C_beta980203[e->pad2 * 2];
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
