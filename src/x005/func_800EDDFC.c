// FUNC 800eddfc 408 X005
// MATCHING 800eddfc 408
typedef struct O {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7;
    char p0[0x16 - 8]; unsigned short y;
    char p1[0x40 - 0x18]; unsigned short *h;
    char p2[0x69 - 0x44]; unsigned char b69;
    char p3[0x7a - 0x6a]; short s7a; unsigned short s7c; unsigned short s7e; short s80;
    char p4[0x84 - 0x82]; int d84; int d88; int d8c;
    char p5[0x94 - 0x90]; struct O *p94;
} O;
extern void ObjCullRegister(O *);
extern unsigned char D_800A60D6[];
extern unsigned short D_800A6066;

void func_800EDDFC(O *o)
{
    unsigned short v;
    int w;
    switch (o->b5) {
    case 0: {
        O *p = o->p94;
        ObjCullRegister(p);
        o->b1 = p->b1;
        if (o->b6 == 0) o->b6++;
        if (o->b69 & 2) {
            p = o->p94;
            p->b5 = 1;
            p->b6 = 0;
            o->b5 = 1;
            o->b6 = 0;
        }
        break;
    }
    case 1: {
        O *p = o->p94;
        ObjCullRegister(p);
        o->b1 = p->b1;
        switch (o->b6) {
        case 0:
            v = o->h[1];
            o->s80 = 0;
            w = o->y;
            o->s7e = w;
            o->d8c = 0x1000;
            o->b6++;
            o->s7c = v;
        case 1:
            if (D_800A6066 & 1) o->s7a = 1;
            else o->s7a = -1;
            o->b6++;
        case 2:
            p = o->p94;
            o->d8c = ((p->d84 >> 3) + 0x1000) & 0xfff;
            if (D_800A60D6[0] == 0) {
                o->d8c = 0;
                o->b69 = 0;
                o->b5 = 0;
                o->b6 = 0;
            }
            break;
        }
        break;
    }
    }
}
