// FUNC 800eb148 428 D0SPSUB
// MATCHING 800eb148 428
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; unsigned short attr; int img; } Text;
extern Slot D_80109C40[];
extern Text D_8011113C;
extern unsigned char D_8009CD9C[];
extern void subScreenDrawSprite(int n);

void subScreenDrawGaugePercent(Menu *m, int i)
{
    Slot *s = &D_80109C40[i];
    int pct;
    int dx;
    D_8011113C.x = s->x;
    D_8011113C.y = s->y;
    D_8011113C.w = m->w10 + s->dw;
    D_8011113C.attr = 0x7C15;
    D_8011113C.img = m->p58[s->idx];
    subScreenDrawSprite(1);
    pct = (60 - D_8009CD9C[i]) * 100 / 60;
    dx = 0;
    if (pct == 100) {
        D_8011113C.x = s->x + 0x26;
        D_8011113C.y = s->y;
        D_8011113C.img = m->p5c[1];
        subScreenDrawSprite(1);
        dx = 8;
    }
    { int o = dx + 0x26; D_8011113C.x = s->x + o; }
    D_8011113C.y = s->y;
    D_8011113C.img = m->p5c[pct / 10 % 10];
    subScreenDrawSprite(1);
}
