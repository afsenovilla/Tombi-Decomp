// FUNC 800eac9c 652 D0SPSUB
// MATCHING 800eac9c 652
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[8]; short sel; char p20[0x3C]; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; int pC; int str; void *p14; } Text;
extern Slot D_80108848[];
extern Text D_8011113C;
extern unsigned short D_8009D2A4;
extern unsigned char D_8010A0C8[];
extern unsigned char D_801096D4[];
extern int D_8010979C[];
extern unsigned char D_80109AF8[];
extern unsigned char D_8009D0A4[];
extern char D_800FFD98[];
extern void subScreenDrawString(int a, int b);
extern void subScreenDrawSprite(int n);

void subScreenDrawItemNames(Menu *m)
{
    Slot *s;
    for (s = D_80108848; s->x != -1; s++) {
        int i;
        int n;
        D_8011113C.x = s->x + 0x18;
        D_8011113C.y = s->y;
        D_8011113C.w = m->w10 + 1;
        i = m->page * 10 + s->idx;
        if (i >= D_8009D2A4) continue;
        D_8011113C.str = D_8010979C[D_801096D4[D_8010A0C8[i]]];
        D_8011113C.p14 = D_800FFD98;
        if (i == m->sel) D_8011113C.attr = 0x7C15;
        else D_8011113C.attr = (((D_80109AF8[D_8010A0C8[i]] & 0x7F) + 0x1F0) << 6) | 0x15;
        subScreenDrawString(5, 1);
        n = D_8009D0A4[D_8010A0C8[i]];
        if (D_80109AF8[D_8010A0C8[i]] & 0x80) {
            D_8011113C.x += 0xB0;
            D_8011113C.y += 0x10;
            D_8011113C.w = m->w10;
            D_8011113C.img = m->p5c[10];
            subScreenDrawSprite(1);
            D_8011113C.x += 0xA;
            if (n >= 10) {
                D_8011113C.img = m->p5c[n / 10 % 10];
                subScreenDrawSprite(1);
            }
            D_8011113C.x += 0xA;
            D_8011113C.img = m->p5c[n % 10];
            subScreenDrawSprite(1);
        }
    }
}
