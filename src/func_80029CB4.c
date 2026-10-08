// FUNC 80029cb4 616 MAIN0
// MATCHING 80029cb4 616
typedef struct { unsigned char b0; char p0[0x15]; short w16; char p1[0x16]; unsigned short w2e; char p2[0x79]; unsigned char ba9; } S;
typedef struct { short w0; short w2; } P;
typedef struct {
    char p0[0x24]; int d24; char p1[8]; short w30; short w32; P *d34; P *d38;
    unsigned char b3c; unsigned char b3d; char p2[0x1a]; unsigned short w58;
} O;
extern S D_800A6038;
extern P *D_800A6078;
extern P *D_800A607C;
extern int D_8009C960;
extern unsigned short D_8009C960h;
extern unsigned char D_8009C939;
extern unsigned char D_800A60D6;
extern short D_1F800286;
extern void FUN_80027c74(O *);
extern void func_8002795C(O *);
extern void func_80028BE0(O *);
extern void func_80029078(O *);

static __inline__ void Clamp(O *o)
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

void func_80029CB4(O *o)
{
    S *s = &D_800A6038;
    int t;
    if (D_8009C939 == 0) {
        switch (D_800A60D6) {
        case 4:
            if (D_8009C960h != 10 && s->ba9 == 0)
                goto c4;
        case 0: case 6: case 7: case 8: case 9: case 10: case 11: case 12:
            FUN_80027c74(o);
            func_8002795C(o);
            o->b3d = 0;
            o->b3c = 0;
            break;
        c4:
            t = s->w2e & 1;
            if (t != o->w58) {
                o->w58 = t;
                o->b3d = 0;
                o->b3c = 0;
            }
            if (o->b3c == 0)
                FUN_80027c74(o);
            func_8002795C(o);
            o->b3d = 0;
            break;
        case 3:
            t = s->w2e & 1;
            if (t != o->w58) {
                o->w58 = t;
                o->b3d = 0;
            }
            FUN_80027c74(o);
            if (o->b3d == 0)
                func_8002795C(o);
            break;
        case 1: case 2: case 5:
            t = s->w2e & 1;
            if (t != o->w58) {
                o->w58 = t;
                o->b3d = 0;
                o->b3c = 0;
            }
            if (o->b3c == 0)
                FUN_80027c74(o);
            if (o->b3d == 0)
                func_8002795C(o);
            break;
        }
        o->d38->w2 = D_800A607C->w2;
    }
    func_80028BE0(o);
    func_80029078(o);
    D_1F800286 = -o->d24 >> 6;
    Clamp(o);
}
