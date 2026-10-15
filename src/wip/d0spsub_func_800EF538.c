// FUNC 800ef538 1388 D0SPSUB
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
extern unsigned short D_8009D670;
extern unsigned short D_1F8001FC;
typedef struct { short n, pad, x, y; } Hdr;
typedef struct { unsigned short f, clut, x, y; } Ent;
extern Hdr D_800A4648[], D_800A464C[], D_800A464E[];
extern Ent D_800A4DE0[][128], D_800A4DE2[][128], D_800A4DE4[][128], D_800A4DE6[][128];
typedef struct { unsigned char u, v, w, h, p4, flag, p6, p7, p8, p9; } Img;
extern Img D_800A5DE4[];
extern char D_8010A200[];
extern void freeMessageGlyphs(int slot);
extern short loadMessageGlyph(int slot, int id, int bank);
extern void drawMessageBox(void *o);

void func_800EF538(Msg *o, int bank)
{
    unsigned short c;
    switch (o->state) {
    case 0:
        o->pal = 0;
        o->wait = 0;
        o->state++;
        return;
    case 1:
        if (++o->blink >= 60) o->blink = 0;
        switch (o->wait) {
        case 0:
            if (*(volatile unsigned short *)&D_8009D670 & 0x4008) o->timer = 1;
            while (--o->timer == 0) {
                o->timer = o->speed;
                c = *o->script++;
                switch ((short)c) {
                case -1:
                    o->speed = 0;
                    o->timer = 2;
                    freeMessageGlyphs(o->slot);
                    o->state = 2;
                    break;
                case -3:
                    o->cx = 8;
                    o->timer = 1;
                    o->cy += 0xD;
                    break;
                case -2:
                case -11:
                    o->speed = 1;
                    o->timer = 2;
                    freeMessageGlyphs(o->slot);
                    o->state = 3;
                    break;
                case -4:
                    o->wait = 1;
                    break;
                case -5:
                    freeMessageGlyphs(o->slot);
                    D_800A4648[o->slot].n = 0;
                    o->cx = 8;
                    o->cy = 6;
                    o->timer = 1;
                    break;
                case -6: {
                    unsigned short ow = o->w;
                    unsigned short oh = o->h;
                    o->w = *o->script++;
                    o->h = *o->script++;
                    o->cx = 8;
                    o->cy = 6;
                    o->timer = 1;
                    o->ba = -(o->w >> 1);
                    o->bc = -o->h - 0x30;
                    o->x = o->x + (ow >> 1) + o->ba;
                    o->y = o->y + oh + 0x30 + o->bc;
                    break;
                }
                case -10:
                {
                    int dx = *o->script++;
                    int dy = *o->script++;
                    o->timer = 1;
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
                    o->timer = 1;
                    break;
                case -9:
                    o->pal = *o->script++;
                    o->timer = 1;
                    break;
                default: {
                    unsigned short n = D_800A4648[o->slot].n;
                    short k;
                    D_800A4648[o->slot].n = n + 1;
                    k = loadMessageGlyph(o->slot, c & 0xFFF, bank);
                    if ((c & 0x7000) == 0x4000) {
                        D_800A4DE0[o->slot][(short)n].f = k | 0xC000;
                        D_800A4DE4[o->slot][(short)n].f = o->w - 0x10;
                        D_800A4DE6[o->slot][(short)n].f = o->h - 0x10;
                    } else {
                        D_800A4DE0[o->slot][(short)n].f = k | 0x8000;
                        D_800A4DE4[o->slot][(short)n].f = o->cx;
                        D_800A4DE6[o->slot][(short)n].f = o->cy;
                    }
                    D_800A4DE2[o->slot][(short)n].f = ((o->pal + 0x1F0) << 6) | 0x15;
                    if ((c & 0x7000) == 0x5000) {
                        o->timer = 1;
                        o->cx += D_800A5DE4[k].w;
                    } else if ((c & 0x7000) != 0x6000) {
                        o->cx += D_800A5DE4[k].w;
                    }
                    break;
                }
                }
            }
            break;
        case 1:
            if (D_1F8001FC & 0x6008) o->wait = 0;
            break;
        }
        D_800A464C[o->slot].n = o->x;
        D_800A464E[o->slot].n = o->y;
        if (D_800A4648[o->slot].n != -1) drawMessageBox(D_8010A200);
        break;
    case 2:
        break;
    }
}
