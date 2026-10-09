// FUNC 801165c0 976 X001
// MATCHING 801165c0 976
/* The csv entry 8011658C starts with 13 data words; the code starts at 801165C0 and also covers the
   csv piece 80116920. */
typedef struct { unsigned short frac; short whole; } FP;
typedef struct {
    char p00[0x30];
    short w30, w32;
    FP *p34;
} O;
extern unsigned short D_8009C962;
extern unsigned char D_8009D2C3, D_8009CE3D;

void func_801165C0(O *o)
{
    unsigned short x = o->p34->whole;
    short v;

    if (D_8009C962 == 0) {
        if ((short)x < 0x6a1) o->w32 = (short)x * 0xaa / 0x6a0 - 0xaa;
        else if ((short)x < 0xa44) o->w32 = 0;
        else if ((short)x < 0xaa8) {
            v = x - 0xa44;
            x = -(v * 0x122) / 99;
            o->w32 = x;
        } else o->w32 = -0x122;
    } else if (D_8009C962 == 1) {
        if (D_8009D2C3 & 1) o->w32 = -0xed;
    } else if (D_8009C962 == 2) {
        if (D_8009CE3D == 0xff) {
            if ((short)x < 0x118) o->w32 = -0x102;
            else if ((short)x < 0x140) {
                v = x - 0x118;
                o->w32 = v * 0xd0 / 0x28 - 0x102;
            } else if ((short)x < 0x711) o->w32 = -0x32;
            else if ((short)x < 0x732) {
                v = x - 0x711;
                o->w32 = -(v * 0xd0) / 0x21 - 0x32;
            } else o->w32 = -0x102;
        }
    } else if (D_8009C962 == 3) {
        if ((short)x < 0x384) o->w32 = -0x1a2;
        else if ((short)x < 0x475) {
            v = x - 0x384;
            o->w32 = v * -0x9b / 0xf0 - 0x1a2;
        } else if ((short)x < 0x579) o->w32 = -0x23d;
        else if ((short)x < 0x6cd) {
            v = x - 0x578;
            o->w32 = v * -0x7f / 0x154 - 0x23d;
        } else if ((short)x < 0x80d) o->w32 = -0x2bc;
        else if ((short)x < 0x943) {
            v = x - 0x80c;
            o->w32 = v * 0xa0 / 0x136 - 0x2bc;
        } else if ((short)x < 0xaf0) {
            v = x - 0x942;
            o->w32 = v * 0x1c / 0x186 - 0x21c;
        }
    } else if (D_8009C962 == 4) {
        if ((short)x < 0x6e1) o->w32 = -0x2c6;
        else if ((short)x < 0x8a8) {
            v = x - 0x6e0;
            o->w32 = v * 0xe6 / 0x1c7 - 0x2c6;
        }
    }
}
