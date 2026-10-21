// FUNC 800ee580 308 D0SPSUB
// MATCHING 800ee580 308
// FLAGS -O2 -G0 -fno-schedule-insns
typedef struct { short x, y, z, id; } Spawn;
typedef struct { int x, y, z; } Vec;
typedef struct {
    char p0[8];
    short w08;
    unsigned char b0a, p0b, p0c, b0d, p0e, b0f;
    union { struct { int x, y, z; } s; Vec v; } p;
    char p1c[2];
    short w1e;
    char p20[4];
    void *anim;
    char p28[6];
    short w2e;
    Vec p0v;
    int d3c;
    char p40[0x94];
} Sprite;
extern unsigned short D_8009D2C4[];
extern Spawn *D_8010A02C[];
extern Sprite D_8010A550[];
extern void *D_800FECB8[];
extern int D_1F8002EC;

void subScreenMapInitAreaSprites(void)
{
    int n = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (D_8009D2C4[i] == 0) {
            Spawn *s = D_8010A02C[i];
            if (s->id != -1) {
                do {
                    Sprite *o = &D_8010A550[n];
                    o->w1e = 5;
                    o->b0d = 1;
                    o->w2e = 1;
                    o->w08 = 0x7FC8;
                    o->anim = D_800FECB8[s->id];
                    o->d3c = D_1F8002EC;
                    o->b0f = 0;
                    o->b0a = 1;
                    o->p.s.x = s->x << 16;
                    o->p.s.y = s->y << 16;
                    o->p.s.z = s->z << 16;
                    o->p0v = o->p.v;
                    s++;
                    n++;
                } while (s->id != -1);
            }
        }
    }
}
