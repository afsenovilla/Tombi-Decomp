// FUNC 8002dd1c 596 MAIN0
typedef struct {
    unsigned char active, b01, type, b03;
    char p04[6];
    unsigned char b0a, b0b, b0c, b0d, b0e, b0f;
    int x, y, z;
    char p1c[2];
    short w1e, w20, w22;
    char p24[8];
    unsigned short w2c, w2e;
    int d30, d34, d38;
    char p3c[0x58];
    int d94;
    char p98[0x1c];
    short b4;
    unsigned short b6;
    short b8;
    unsigned short ba;
    short bc, be, c0;
    unsigned short c2;
    unsigned short c4;
    unsigned short c6;
    short c8, ca, cc, ce, d0, d2;
} E2d;
extern short DAT_800a4648[];
extern unsigned char DAT_8009d67c;
extern char *DAT_1f800398[];
extern E2d *ObjAlloc(void);

E2d *FUN_8002dd1c(int k, short dx, short *pos, short anim, unsigned short w)
{
    int i;
    short *p;
    E2d *o;
    unsigned short v;
    char *b;
    for (i = 0, p = DAT_800a4648; i < 4; i++, p += 4) {
        if (*p == -1 && (o = ObjAlloc()) != 0) {
            o->type = 0x1a;
            o->b0a = 10;
            o->w1e = 0x14;
            o->b0f = 0x32;
            o->active = 1;
            o->b0c = k;
            o->w2c = 0;
            o->w2e = anim;
            if (anim != 0) {
                o->d30 = pos[1];
                o->d34 = pos[3];
                o->d38 = pos[5];
            } else {
                o->x = pos[1] << 16;
                o->y = pos[3] << 16;
                o->z = 0;
            }
            switch (DAT_8009d67c) {
            case 0:
                o->b4 = 6;
                break;
            case 1:
                o->b4 = 3;
                break;
            case 2:
                o->b4 = 2;
                break;
            }
            o->c2 = 0x100;
            o->c4 = 0x30;
            o->b6 = 0xffff;
            o->b8 = 0;
            o->ba = -(o->c2 >> 1);
            o->bc = -0x30 - o->c4;
            o->be = pos[1] + o->ba;
            o->ce = 8;
            o->d0 = 6;
            o->c6 = 0xffff;
            o->ca = i;
            o->cc = 1;
            o->d2 = w;
            o->w20 = 1;
            o->w22 = 0;
            o->c0 = pos[3] + o->bc;
            *p = 0;
            v = *(short *)(DAT_1f800398[0] + k * 2 + 0x10) + dx;
            o->c8 = v;
            b = DAT_1f800398[0] + *(unsigned short *)(DAT_1f800398[0] + 8);
            o->d94 = (int)(b + *(unsigned short *)(b + v * 2));
            return o;
        }
    }
}
