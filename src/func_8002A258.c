// FUNC 8002a258 380 MAIN0
// MATCHING 8002a258 380
typedef struct {
    unsigned char b0, b1, b2, b3;
    char pad[0x34 - 4];
    unsigned short *p34;
    char pad2[0x44 - 0x38];
    unsigned short w44;
} S8002A258;

extern short DAT_1f8000e6;
extern short DAT_1f8000e6_a[];
extern unsigned short DAT_1f8000f2;
extern int DAT_1f800168;
extern unsigned short DAT_1f80016a;
extern int DAT_1f80016c;
extern unsigned short DAT_1f80016e;
extern int DAT_1f80018c;
extern int DAT_1f800190;
extern unsigned char D_8009C966;
extern unsigned char D_8009CDA6;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C942;
void FUN_80027ed8(S8002A258 *o);
void func_8002809C(S8002A258 *o);

void func_8002A258(S8002A258 *o)
{
    short v;
    o->w44 = 0;
    v = DAT_1f8000e6_a[0];
    if (v != 0) {
        if (v > 0) {
            DAT_1f8000e6 = v - 2;
            if (DAT_1f8000e6 < 0) DAT_1f8000e6 = 0;
        } else {
            DAT_1f8000e6 = v + 2;
            if (DAT_1f8000e6 > 0) DAT_1f8000e6 = 0;
        }
    }
    DAT_1f80018c = DAT_1f800168;
    DAT_1f800190 = DAT_1f80016c;
    FUN_80027ed8(o);
    func_8002809C(o);
    if ((unsigned short)(o->p34[1] - DAT_1f80016a + 0x40) < 0x80 || (o->w44 & 3)) {
        if ((unsigned short)(DAT_1f8000f2 - DAT_1f80016e + 0x5a) < 0xb4 || (o->w44 & 0xc)) {
            o->b3 = 1;
            if (D_8009C966 == 3 || D_8009CDA6 == 0xff) o->b3 = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
        }
    }
}
