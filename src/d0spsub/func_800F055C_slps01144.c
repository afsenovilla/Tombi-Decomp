// VERSION SLPS_01144 dspsub 800F055C 1380
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ef538 (subScreenMessageStep.c): confirm 0x820, skip 0x860, line start 8,8
// Message box state machine: steps the message script (control codes -1..-11, glyph loading) and places the box.
// Idioms: do{}while(0) around switch(o->wait) fixes the register allocation; in case -6 the x/y updates go through
// an `unsigned short s` intermediate (`s = o->x + ow; o->x = s + o->ba;`): the sum becomes a HI-mode pseudo, so
// expand keeps it as the first operand of the add and the x load's temporary gets v1 (kept after `sh ba`).
typedef struct {
    char p0[4];
    unsigned char state, wait;
    char p6[0x1A];
    short timer;
    short blink;
    char p24[0x70];
    short *script;
    char p98[0x1C];
    unsigned short speed, b6;
    char pb8[2];
    short ba, bc;
    unsigned short x, y, w, h, c6, pal, slot;
    char pcc[2];
    unsigned short cx, cy;
} Msg;
extern unsigned short D_8009F200;
extern unsigned short JOYPAD_STATE;
typedef struct { short n, pad, x, y; } Hdr;
typedef struct { unsigned short f, clut, x, y; } Ent;
extern Hdr D_800A6190[], D_800A6194[], D_800A6196[];
extern Ent D_800A6920[][128], D_800A6922[][128], D_800A6924[][128], D_800A6926[][128];
typedef struct { unsigned char u, v, w, h, p4, flag, p6, p7, p8, p9; } Img;
extern Img D_800A7924[];
extern char D_8010A4C4[];
extern void freeMessageGlyphs(int slot);
extern int loadMessageGlyph(int slot, int id, int bank);
extern void func_800492AC(void *o);

void func_800F055C(Msg *o, int bank)
{
    unsigned short c;
    int two, one, eight;
    switch (o->state) {
    case 0:
        o->pal = 0;
        o->wait = 0;
        o->state++;
        return;
    case 1:
        if (++o->blink >= 60) o->blink = 0;
        do { switch (o->wait) {
        case 0:
            if (*(volatile unsigned short *)&D_8009F200 & 0x820) o->timer = 1;
            two = 2;
            one = 1;
            eight = 8;
        loop: {
                if (--o->timer != 0) goto done;
                o->timer = o->speed;
                c = *o->script++;
                switch ((short)c) {
                case -1:
                    o->speed = 0;
                    o->timer = two;
                    freeMessageGlyphs(o->slot);
                    o->state = two;
                    break;
                case -3:
                    o->cx = eight;
                    o->timer = one;
                    o->cy += 0xD;
                    break;
                case -2:
                case -11:
                    o->speed = one;
                    o->timer = two;
                    freeMessageGlyphs(o->slot);
                    o->state = 3;
                    break;
                case -4:
                    o->wait = one;
                    break;
                case -5:
                    freeMessageGlyphs(o->slot);
                    D_800A6190[o->slot].n = 0;
                    o->cx = eight;
                    o->cy = eight;
                    o->timer = one;
                    break;
                case -6: {
                    unsigned short ow = o->w >> 1;
                    unsigned short oh = o->h;
                    o->w = *o->script++;
                    o->h = *o->script++;
                    o->cx = eight;
                    o->cy = eight;
                    o->timer = one;
                    o->ba = -(o->w >> 1);
                    o->bc = -o->h - 0x30;
                    { unsigned short s = o->x + ow; o->x = s + o->ba; }
                    { unsigned short s = o->y + oh + 0x30; o->y = s + o->bc; }
                    break;
                }
                case -10:
                {
                    int dx = *o->script++;
                    int dy = *o->script++;
                    o->timer = one;
                    o->x += dx;
                    o->y += dy;
                }
                    break;
                case -7:
                    o->timer = *o->script++;
                    break;
                case -8:
                    o->c6 = *o->script++;
                    o->b6 = *o->script++;
                    o->timer = one;
                    break;
                case -9:
                    o->pal = *o->script++;
                    o->timer = one;
                    break;
                default: {
                    unsigned short n = D_800A6190[o->slot].n;
                    unsigned short k;
                    D_800A6190[o->slot].n = n + 1;
                    k = loadMessageGlyph(o->slot, c & 0xFFF, bank);
                    if ((c & 0x7000) == 0x4000) {
                        D_800A6920[o->slot][(short)n].f = k | 0xC000;
                        D_800A6924[o->slot][(short)n].f = o->w - 0x10;
                        D_800A6926[o->slot][(short)n].f = o->h - 0x10;
                        D_800A6922[o->slot][(short)n].f = ((o->pal + 0x1F0) << 6) | 0x15;
                    } else {
                        D_800A6920[o->slot][(short)n].f = k | 0x8000;
                        D_800A6924[o->slot][(short)n].f = o->cx;
                        D_800A6926[o->slot][(short)n].f = o->cy;
                        D_800A6922[o->slot][(short)n].f = ((o->pal + 0x1F0) << 6) | 0x15;
                    }
                    if ((c & 0x7000) == 0x5000) {
                        o->cx += D_800A7924[(short)k].w;
                        o->timer = one;
                    } else if ((c & 0x7000) != 0x6000) {
                        o->cx += D_800A7924[(short)k].w;
                    }
                    break;
                }
                }
            goto loop; }
        done:
            break;
        case 1:
            if (JOYPAD_STATE & 0x860) o->wait = 0;
            break;
        } } while (0);
        D_800A6194[o->slot].n = o->x;
        D_800A6196[o->slot].n = o->y;
        if (D_800A6190[o->slot].n != -1) func_800492AC(D_8010A4C4);
        break;
    case 2:
        break;
    }
}
#endif
