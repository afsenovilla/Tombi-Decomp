// FUNC 8004f538 1352 MAIN0
// MATCHING 8004f538 1352
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

extern char *D_1f8002a4[];
extern char *D_1f8002c8[];
extern char *D_1f800310[];
extern char *D_1f800330[];
extern char *D_1f800348[];
extern char *D_1f800308[];
extern char *D_1f800358[];
extern char *D_8009d630[];
extern char *D_1f8002b8[];
extern char *D_1f800398[];
extern char *D_8009d2f0[];
extern char *D_8009d3f0[];
extern char D_800e8028[];
extern char D_80098c44[];
extern RelLog D_8009f3e0[];
extern int D_1f80029c;
extern char *D_1f800298;

int FUN_8004f538(Rel *e, int unused, char *base)
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
            if (e->idx < 0x80) D_1f8002a4[e->idx] = base;
            else base = D_1f8002a4[e->idx & 0x7f];
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
        D_8009f3e0[D_1f80029c].e = e;
        D_8009f3e0[D_1f80029c].base = base;
        D_1f80029c = (D_1f80029c + 1) & 0x7f;
        if ((e->flags & 0xf) < 3) {
            t = e->type;
            lo = t & 0xf;
            switch (t & 0xf0) {
            case 0x00: case 0x10: case 0x20: break;
            case 0x30: D_1f8002c8[lo] = base; break;
            case 0x40: D_1f800310[lo] = base; break;
            case 0x50: D_1f800330[lo] = base; break;
            case 0x60: D_1f800348[lo] = base; break;
            case 0x70: D_1f800308[lo] = base; break;
            case 0x80: D_1f800358[lo] = base; break;
            case 0x90: case 0xa0: break;
            case 0xb0: D_8009d630[lo] = base; break;
            case 0xc0: D_1f8002b8[lo] = base; break;
            case 0xd0: D_1f800398[lo] = base; break;
            }
            if ((e->type & 0xf0) != 0x10) base += e->size;
            e++;
        } else {
            do {
                t = e->type;
                lo = t & 0xf;
                switch (t & 0xf0) {
                case 0x00: case 0x10: case 0x20: break;
                case 0x30: D_1f8002c8[lo] = base; break;
                case 0x40: D_1f800310[lo] = base; break;
                case 0x50: D_1f800330[lo] = base; break;
                case 0x60: D_1f800348[lo] = base; break;
                case 0x70: D_1f800308[lo] = base; break;
                case 0x80: D_1f800358[lo] = base; break;
                case 0x90: D_8009d2f0[lo] = base; break;
                case 0xa0: D_8009d3f0[lo] = base; break;
                case 0xb0: D_8009d630[lo] = base; break;
                case 0xc0: D_1f8002b8[lo] = base; break;
                case 0xd0: D_1f800398[lo] = base; break;
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
    if (pending == 1) D_1f800298 = saved;
    else D_1f800298 = base;
    return 0;
}
