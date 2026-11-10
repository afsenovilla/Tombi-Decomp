// VERSION SLPM_80165 x00 800FC974 184
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerBiteEntryImpactBoxY (src/x018/playerBiteEntryImpactBoxY.c): no impact effect, the
// height offset is the +0x70 word alone; PlayerSetAnimIfChanged inline.
typedef struct H { short pad; unsigned short w; } H;
typedef struct O {
    char pad0[6];
    unsigned char state;
    char pad1[0x12 - 7];
    short a;
    char pad1b[2];
    short y;
    char pad1c[2];
    short b;
    char pad2[0x20 - 0x1c];
    short timer;
    char pad3[0x40 - 0x22];
    H *h;
    char pad4[0x70 - 0x44];
    unsigned short w70;
    unsigned short w72;
    char pad5[0x7c - 0x74];
    short velX;
    short velY;
    char pad6[0x9c - 0x80];
    char b9c;
    char pad7[0xac - 0x9d];
    char wac;
} O;
extern O *BITTEN_OBJECT;

extern void spawnImpactEffect();


extern struct DemoAnimExt { char pad[0x2c]; unsigned short timer, frame; } *demoAnimExt asm("PLAYER_EXT");
extern void demoAnimReset(void *) asm("func_800EDA04");
extern void demoAnimRestart(void *) asm("func_80020554");
static __inline__ void demoSetAnim(void *o, unsigned short anim)
{
    struct DemoAnimExt *p = demoAnimExt;
    p->timer = anim;
    if (p->frame != anim) {
        demoAnimReset(o);
        demoAnimRestart(o);
        demoAnimExt->frame = demoAnimExt->timer;
    }
}

void playerBiteEntryImpactBoxY(O *o)
{
    O *p;
    unsigned short s2, s3;
    o->timer = 0;
    demoSetAnim(o, 0xd);
    o->wac = 3;
    o->b9c = 0;
    p = BITTEN_OBJECT;
    o->velX = 0;
    o->velY = 0;
    o->h->w = p->h->w;
    s3 = p->y;
    s2 = p->w70;
    o->state = 2;
    o->y = s3 - s2;
}
#endif
