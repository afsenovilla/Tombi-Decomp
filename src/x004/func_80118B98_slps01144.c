// VERSION SLPS_01144 x04 80118B98 844
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

#define SPLIT3(q)             \
    a = q[-4];                \
    b = q[-2];                \
    c = q[0];                 \
    t = a & 0xff00;           \
    q[-4] = t;                \
    t = b & 0xff00;           \
    q[-2] = t;                \
    t = c & 0xff00;           \
    q[0] = t;                 \
    a &= 0xff;                \
    b &= 0xff;                \
    c &= 0xff;

#define SPLIT4(q)             \
    a = q[-6];                \
    b = q[-4];                \
    c = q[-2];                \
    d = q[0];                 \
    t = a & 0xff00;           \
    q[-6] = t;                \
    t = b & 0xff00;           \
    q[-4] = t;                \
    t = c & 0xff00;           \
    q[-2] = t;                \
    t = d & 0xff00;           \
    q[0] = t;                 \
    a &= 0xff;                \
    b &= 0xff;                \
    c &= 0xff;                \
    d &= 0xff;

#define BODY3(q)              \
    SPLIT3(q)                 \
    if (o->w22 & 3) {         \
        a2 = a - 0x30;        \
        b2 = b - 0x30;        \
        c2 = c - 0x30;        \
    } else {                  \
        a2 = a + 0x90;        \
        b2 = b + 0x90;        \
        c2 = c + 0x90;        \
    }                         \
    q[-4] |= a2;              \
    q[-2] |= b2;              \
    q[0] |= c2;

#define BODY4(q)              \
    SPLIT4(q)                 \
    if (o->w22 & 3) {         \
        a2 = a - 0x30;        \
        b2 = b - 0x30;        \
        c2 = c - 0x30;        \
        d2 = d - 0x30;        \
    } else {                  \
        a2 = a + 0x90;        \
        b2 = b + 0x90;        \
        c2 = c + 0x90;        \
        d2 = d + 0x90;        \
    }                         \
    q[-6] |= a2;              \
    q[-4] |= b2;              \
    q[-2] |= c2;              \
    q[0] |= d2;

void func_80118B98_slps01144(TObj *o)
{
    char *p;
    short n;
    short k;
    unsigned short a2, b2, c2, d2;
    register int t asm("$2");

    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 5;
        o->w22 = 0;
        break;
    case 1:
        if (--o->timer != 0) break;
        o->timer = 5;
        o->w22++;
        p = (char *)o->da0;
        n = *(unsigned short *)p;
        p += 4;
        {
        unsigned short *q;
        q = (unsigned short *)(p + 12);
        do {
            unsigned short a, b, c, d;

            BODY3(q)
            p += 0x28;
            q += 20;
        } while (--n);
        }
        n = *(unsigned short *)p;
        {
        register unsigned short *q asm("$7");
        q = (unsigned short *)(p + 20);
        do {
            unsigned short a, b, c, d;

            BODY4(q)
            q += 26;
        } while (--n);
        }
        break;
    case 2:
        p = (char *)o->da0;
        n = *(unsigned short *)p;
        p += 4;
        {
        unsigned short *q;
        q = (unsigned short *)(p + 12);
        do {
            unsigned short a, b, c, d;

            
            SPLIT3(q)
            k = (o->w22 & 3) * 0x30;
            q[-4] |= a + k;
            q[-2] |= b + k;
            q[0] |= c + k;
            p += 0x28;
            q += 20;
        } while (--n);
        }
        n = *(unsigned short *)p;
        {
        unsigned short *q;
        q = (unsigned short *)(p + 20);
        do {
            unsigned short a, b, c, d;

            
            SPLIT4(q)
            k = (o->w22 & 3) * 0x30;
            q[-6] |= a + k;
            q[-4] |= b + k;
            q[-2] |= c + k;
            q[0] |= d + k;
            q += 26;
        } while (--n);
        }
        break;
    }
}
#endif
