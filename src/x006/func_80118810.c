// FUNC 80118810 1116 X006
// MATCHING 80118810 1116

typedef struct {
    short a;     /* 0xb4 */
    short b;     /* 0xb6 */
    unsigned char pad[0x12];
    short ca;    /* 0xca */
} Sub;

typedef struct {
    unsigned char pad00[0x2c];
    unsigned short animTimer; /* 0x2c */
    unsigned char pad2e[0x4c];
    short w7a;                /* 0x7a */
    unsigned char pad7c[0x10];
    int d8c;                  /* 0x8c */
    unsigned char pad90[0xc];
    unsigned char b9c;        /* 0x9c */
    unsigned char pad9d[0xb];
    char *da8;                /* 0xa8 */
    unsigned char padac[8];
    Sub sub;                  /* 0xb4 */
    unsigned char padcc[4];
    short wd0;                /* 0xd0 */
} O;

extern unsigned char D_8009C93E, D_8009C958;
extern volatile unsigned short D_8009D670[];
extern void playSFX(int);
extern void FUN_80025f40(int, int, int, int);

void func_80118810(O *o)
{
    Sub *s = &o->sub;
    int t;

    if (D_8009C93E) {
        switch (D_8009C958) {
        case 0:
            s->b = 0;
            break;
        case 1:
        case 2:
            s->b = 0x200;
            break;
        case 3:
            s->b = 0x300;
            break;
        case 4:
            s->b = 0;
            s->ca = 1;
            D_8009C93E = 0;
            playSFX(0xfc);
            break;
        case 5:
            s->b = 0x200;
            D_8009C93E = 0;
            playSFX(0xfc);
            break;
        }
        return;
    }
    if (o->wd0 == 0) return;
    t = ((short *)(o->da8 + (o->animTimer << 3)))[3];
    o->w7a = t;
    if (o->b9c) t = 0;
    switch (t) {
    case 0:
        if (D_8009D670[0] & 0x20) {
            o->sub.b += 8;
            if (o->sub.b > 0x500) o->sub.b = 0x500;
        } else if (D_8009D670[0] & 0x80) {
            o->sub.b -= 0xc;
        } else {
            o->sub.b -= 8;
        }
        break;
    case 1:
        if (o->sub.b != 0) FUN_80025f40(0, 0, 0x60, 5);
        if (D_8009D670[0] & 0x20) {
            short u = o->sub.b;
            if (u < 0x200) o->sub.b = u + 6;
            else o->sub.b = u - 0x10;
        } else if (D_8009D670[0] & 0x80) {
            short u = o->sub.b;
            if (u < 0x200) o->sub.b = u - 0x18;
            else o->sub.b = u - 0x14;
        } else {
            o->sub.b -= 0x10;
        }
        break;
    case 2:
        if (o->sub.b != 0) FUN_80025f40(0, 0, 0x48, 5);
        if (D_8009D670[0] & 0x20) {
            short u = o->sub.b;
            if (u < 0x200) o->sub.b = u + 4;
            else o->sub.b = u - 0x14;
        } else if (D_8009D670[0] & 0x80) {
            short u = o->sub.b;
            if (u < 0x200) o->sub.b = u - 0x18;
            else o->sub.b = u - 0x14;
        } else {
            o->sub.b -= 0x14;
        }
        break;
    case 3:
        if (o->sub.b != 0) FUN_80025f40(0, 0, 0x70, 5);
        if (D_8009D670[0] & 0x20) {
            short u = o->sub.b;
            if (u < 0x100) o->sub.b = u + 4;
            else o->sub.b = u - 0x18;
        } else if (D_8009D670[0] & 0x80) {
            short u = o->sub.b;
            if (u < 0x100) o->sub.b = u - 0x1c;
            else s->b -= 0x18;
        } else {
            o->sub.b -= 0x18;
        }
        break;
    }
    if (s->b < 0) s->b = 0;
    if (o->b9c) {
        if (D_8009D670[0] & 0x20) o->d8c += s->b >> 8;
        else if (D_8009D670[0] & 0x80) o->d8c -= s->b >> 6;
        else if (o->b9c == 1) o->d8c -= s->b >> 6;
        else o->d8c += s->b >> 7;
        o->d8c &= 0xfff;
    }
}
