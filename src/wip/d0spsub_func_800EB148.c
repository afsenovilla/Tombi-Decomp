// FUNC 800eb148 428 D0SPSUB
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y; short p4[2]; int img; } Text;
extern Slot D_80109C40[];
extern Text D_8011113C;
extern short D_80111140, D_80111142;
extern unsigned char D_8009CD9C[];
extern void func_800EB2F4(int n);

void func_800EB148(Menu *m, int i)
{
    Slot *s = &D_80109C40[i];
    int pct;
    int dx;
    D_8011113C.x = s->x;
    D_8011113C.y = s->y;
    D_80111142 = 0x7C15;
    D_8011113C.p4[0] = m->w10 + s->dw;
    D_8011113C.img = m->p58[s->idx];
    func_800EB2F4(1);
    pct = (60 - D_8009CD9C[i]) * 100 / 60;
    dx = 0;
    if (pct == 100) {
        D_8011113C.x = s->x + 0x26;
        D_8011113C.y = s->y;
        D_8011113C.img = m->p5c[1];
        func_800EB2F4(1);
        dx = 8;
    }
    { int o = dx + 0x26; D_8011113C.x = s->x + o; }
    D_8011113C.y = s->y;
    D_8011113C.img = m->p5c[pct / 10 % 10];
    func_800EB2F4(1);
}
