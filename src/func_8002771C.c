// FUNC 8002771c 220 MAIN0
// MATCHING 8002771c 220
typedef struct { unsigned char b0; char p[0x15]; short w16; } S;
typedef struct { short w0; short w2; } P;
typedef struct { char p0[0x30]; short w30; short w32; P *d34; } O;
extern S D_800A6038;
extern P *D_800A6078;
extern int D_8009C960;

void func_8002771C(O *o)
{
    S *s = &D_800A6038;
    char pad;
    if (s->b0 < 4 || s->b0 == 7) {
        unsigned short x = o->d34->w2;
        short t;
        t = x - 0x90;
        if (D_800A6078->w2 < t) {
            D_800A6078->w2 = t;
        } else {
            t = x + 0x90;
            if (t < D_800A6078->w2)
                D_800A6078->w2 = t;
        }
        if (D_8009C960 == 0x2000E) {
            if (s->w16 < o->w30 - 0x5c)
                s->w16 = o->w30 - 0x5c;
        } else {
            if (s->w16 < o->w30 - 0x90)
                s->w16 = o->w30 - 0x90;
        }
    }
}
