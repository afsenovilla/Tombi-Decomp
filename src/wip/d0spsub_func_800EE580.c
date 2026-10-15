// FUNC 800ee580 308 D0SPSUB
typedef struct { short x, y, z, id; } Spawn;
typedef struct {
    char p0[8];
    short w08;
    unsigned char b0a, p0b, p0c, b0d, p0e, b0f;
    int x, y, z;
    char p1c[2];
    short w1e;
    char p20[4];
    void *anim;
    char p28[6];
    short w2e;
    int x0, y0, z0;
    int d3c;
    char p40[0x94];
} Sprite;
extern unsigned short D_8009D2C4[];
extern Spawn *D_8010A02C[];
extern Sprite D_8010A550[];
extern void *D_800FECB8[];
extern int D_1F8002EC;

void func_800EE580(void)
{
    int n = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (D_8009D2C4[i] == 0) {
            Spawn *s = D_8010A02C[i];
            if (s->id != -1) {
                Sprite *o = &D_8010A550[n];
                do {
                    o->w1e = 5;
                    o->b0d = 1;
                    o->w2e = 1;
                    o->w08 = 0x7FC8;
                    o->anim = D_800FECB8[s->id];
                    o->b0f = 0;
                    o->b0a = 1;
                    o->d3c = D_1F8002EC;
                    n++;
                    o->x = s->x << 16;
                    o->y = s->y << 16;
                    o->z = s->z << 16;
                    o->x0 = o->x;
                    o->y0 = o->y;
                    o->z0 = o->z;
                    s++;
                    o++;
                } while (s->id != -1);
            }
        }
    }
}
