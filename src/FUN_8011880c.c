// FUNC 8011880c 400 X000
// MATCHING 8011880c 400
typedef struct P { unsigned short x, y; int pad; } P;
typedef struct S {
    char p0[6];
    unsigned char state;
    char p7[5];
    unsigned char b0c;
    char pd[0x22 - 0xd];
    short w22;
    char p24[0x7e - 0x24];
    short w7e;
    char p80[2];
    short w82;
    char p84[0xb4 - 0x84];
    P e[4];
} S;

static __inline__ void f(S *o)
{
    short w;
    switch (o->state) {
    case 0:
        o->w82 = 0x80;
        o->w7e = -8;
        o->w22 = 0;
        o->e[0].y += o->b0c * 0x48;
        o->e[1].y += o->b0c * 0x48;
        o->e[2].y += o->b0c * 0x48;
        o->e[3].y += o->b0c * 0x48;
        o->state = (o->b0c & 1) + 1;
        break;
    case 1:
        w = o->w22 + 1;
        o->w22 = w;
        o->e[0].x--;
        o->e[1].x--;
        o->e[2].x++;
        o->e[3].x++;
        if (w >= 0x50)
            o->state = o->state + 1;
        break;
    case 2:
        w = o->w22 - 1;
        o->w22 = w;
        o->e[0].x++;
        o->e[1].x++;
        o->e[2].x--;
        o->e[3].x--;
        if (w <= -0x50)
            o->state = o->state - 1;
        break;
    }
}

void FUN_8011880c(S *o)
{
    f(o);
}
