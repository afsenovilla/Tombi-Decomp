// VERSION SLPS_01144 dspsub2 800EB8E8 792
#if defined(VERSION_SLPS_01144)
// JP memCardScreenStep (PAL d0spsub2 800e9308): one card slot, no slot select
typedef struct { unsigned char state, sub, b2, b3; short timer; short x6; char p8[0x10]; short card; short x1A; short x1C; } Menu;
typedef struct { char p0[0x140]; short status, prev, p144, type; char p148[0x12]; } Card;
asm(".set JS_,0x1f8001fc");
extern unsigned short JS_;
extern Menu D_800A6120;
/* the menu fields, as retail names them */
extern unsigned char D_800A6121, D_800A6122;
extern short D_800A6124, D_800A6126, D_800A6138, D_800A613A;
extern Card D_800F30D0[];
extern short D_800F3210, D_800F3212, D_800F3216; /* D_800F30D0[0].status / prev / type */
extern short D_800F30C4;         /* file count */
extern short D_800F3098, D_800F309C, D_800F30A0; /* cursor y target, cursor y, list scroll */
extern int func_800EA334(Card *c, int n); /* memCardProbe */
extern int func_800EAC14(Menu *m, Card *c); /* memCardFileListStep */
extern int func_800EB728(Menu *m, Card *c); /* memCardFormatStep */

int func_800EB8E8(void)
{
    Menu *m = &D_800A6120;
    Card *c;
    int r;
    switch (D_800A6121) {
    case 0:
        D_800F3210 = -3;
        D_800F3212 = -3;
        D_800F30C4 = 0;
        D_800F3216 = 0;
        D_800A6138 = 0;
        D_800A613A = 1;
        D_800A6124 = 4;
        D_800A6126 = 1;
        D_800A6122 = 0;
        D_800A6121++;
        break;
    case 1:
        c = &D_800F30D0[D_800A6138];
        if (--D_800A6124 == 0) {
            D_800A6124 = 0x5A;
            c->status = func_800EA334(c, D_800A6138);
            if (c->status != c->prev) {
                c->prev = c->status;
                D_800A6122 = 0;
            }
        }
        switch (c->status) {
        case 0:
            r = func_800EAC14(m, c);
            if (r != 0) return r;
            break;
        case 1:
            if (m->x1C) {
                if (func_800EB728(m, c) == -1) return 1;
                break;
            }
            m->x1A = 4;
            if (JS_ & 0x40) return -1;
            break;
        case -1:
            m->x1A = 2;
            if (JS_ & 0x40) return -1;
            break;
        case -2:
            m->x1A = 0;
            if (JS_ & 0x40) return -1;
            break;
        default:
            return 0;
        }
        break;
    case 2:
        {
        int d = D_800F3098 - D_800F30A0;
        if (d > 56) {
            D_800A6121++;
        } else if (d < 0) {
            D_800A6121++;
        } else {
            D_800F309C = D_800F3098;
            D_800A6121--;
        }
        }
        break;
    case 3:
        if (D_800F3098 != D_800F309C) {
            if (D_800F3098 - D_800F309C > 0) D_800F309C += 8;
            else D_800F309C -= 8;
            if (D_800F309C - D_800F30A0 > 56) D_800F30A0 += 8;
            if (D_800F309C - D_800F30A0 < 0) D_800F30A0 -= 8;
            return 0;
        }
        D_800A6121 = 1;
        break;
    }
    return 0;
}
#endif
