// FUNC 8004f538 1352 MAIN0
// MATCHING 8004f538 1352
#include "globals.h"
typedef struct {
    short id;
    unsigned char idx;
    unsigned char type;
    char *ptr;
    int size;
    int pad;
    int flags;
} Rel;

typedef struct { Rel *e; char *base; } RelLog;

extern char *LOAD_SLOTS[];
extern char *SPR_DATA[];
extern char *D_1f800310[];
extern char *WMD_DATA[];
extern char *WPP_DATA[];
extern char *WHM_DATA[];
extern char *APD_DATA[];
extern char *SEQ_DATA[];
extern char *D_1f8002b8[];
extern char *WFM3_DATA[];
extern char D_800e8028[];
static char D_80098c44[844] = {
    4,
};
extern RelLog CD_QUEUE[];
extern int CD_QUEUE_HEAD;
extern char *LOAD_NEXT_ADDR;

int queueLoadList(Rel *e, int unused, char *base)
{
    int pending = 0;
    char *saved;
    unsigned char t;
    int lo;

    while (e->id != -1) {
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
            saved = base;
            base = D_800e8028;
            pending = 1;
        } else if (e->type == 0xf8) {
            pending = 1;
            saved = base;
            base = D_80098c44;
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
            case 0x40: D_1f800310[lo] = base; break;
            case 0x50: WMD_DATA[lo] = base; break;
            case 0x60: WPP_DATA[lo] = base; break;
            case 0x70: WHM_DATA[lo] = base; break;
            case 0x80: APD_DATA[lo] = base; break;
            case 0x90: case 0xa0: break;
            case 0xb0: SEQ_DATA[lo] = base; break;
            case 0xc0: D_1f8002b8[lo] = base; break;
            case 0xd0: WFM3_DATA[lo] = base; break;
            }
            if ((e->type & 0xf0) != 0x10) base += e->size;
            e++;
        } else {
            do {
                t = e->type;
                lo = t & 0xf;
                switch (t & 0xf0) {
                case 0x00: case 0x10: case 0x20: break;
                case 0x30: SPR_DATA[lo] = base; break;
                case 0x40: D_1f800310[lo] = base; break;
                case 0x50: WMD_DATA[lo] = base; break;
                case 0x60: WPP_DATA[lo] = base; break;
                case 0x70: WHM_DATA[lo] = base; break;
                case 0x80: APD_DATA[lo] = base; break;
                case 0x90: WVD_BODIES[lo] = base; break;
                case 0xa0: WVD_HEADERS[lo] = base; break;
                case 0xb0: SEQ_DATA[lo] = base; break;
                case 0xc0: D_1f8002b8[lo] = base; break;
                case 0xd0: WFM3_DATA[lo] = base; break;
                }
                if ((e->type & 0xf0) != 0x10) base += e->size;
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
