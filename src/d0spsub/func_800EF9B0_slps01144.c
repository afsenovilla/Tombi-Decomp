// VERSION SLPS_01144 dspsub 800EF9B0 2420
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ee9ac (subScreenMapStep.c)
typedef struct { short x, y, z, pad; } Pt;
typedef struct { unsigned char a, b, c, d, e, f; } Link;
typedef struct { short x, y; } XY;
typedef struct { int x, y, z; } Vec;
typedef struct { int w[14]; } Save;
typedef struct {
    unsigned char b00, p01, b02, b03;
    char p04[4];
    short w08;
    unsigned char b0a, p0b, b0c, b0d, p0e;
    signed char b0f;
    union { struct { int x, y, z; } s; Vec v; } p;
    char p1c[2];
    short w1e;
    char p20[4];
    void *anim;
    char p28[6];
    short w2e;
    union { struct { int x, y, z; } s; Vec v; } q;
    int d3c;
    char p40[0x60];
    char *da0;
    unsigned char ba4;
    char pa5[0xF];
    unsigned short wb4;
    short wb6, wb8, wba;
    char pbc[0x18];
} Sprite;
typedef struct {
    unsigned char b0, state;
    char p2[2];
    short w4;
    char p6[0x10];
    short w16;
} Menu;
typedef struct { unsigned short room, sub; } RS;
extern Sprite SUBSCREEN_MAP_OBJECTS[];
extern Sprite D_80111214[];
extern Save D_8010A48C;
extern RS GAME;
extern unsigned short D_8009EE6C[];
extern void *D_801046FC;
extern void *D_80104720[];
extern Pt *D_80102EBC[];
extern Pt MAP_NODES[];
extern Link MAP_LINKS[];
extern unsigned char *D_801038A4[];
extern XY D_8010A314[];
extern unsigned short D_801029CC[][6];
extern int SUBSCREEN_MAP_LINK;
extern int D_801113FC, D_801113F8;
extern short D_801113D0, D_801113D4, D_801113D8, D_801113C0, D_801113C8, D_801113CC;
extern unsigned short D_801113BC, D_801113C4;
extern void func_800EF370(void);
extern void subScreenMapInitAreaSprites(void);
extern int func_800EF688(void);
extern void func_800EF7BC(Sprite *s);
extern void playSFX(int id);
extern void playSFXWithNote(int a, int b);
extern Save D_1F8000C0;
extern unsigned short D_1F8001FC;
extern volatile int D_1F8002EC;
extern char *D_1F800344;
#define VIEW_CAMERA D_1F8000C0
#define PAD D_1F8001FC
#define BANK D_1F8002EC
#define P344 (*(char * volatile *)&D_1F800344)

int func_800EF9B0(Menu *m)
{
    Sprite *s = SUBSCREEN_MAP_OBJECTS;
    Link *l;
    unsigned char *p;
    int r;
    int i;
    unsigned char t;
    switch (m->state) {
    case 0:
        D_8010A48C = VIEW_CAMERA;
        SUBSCREEN_MAP_OBJECTS[2].w1e = 1;
        SUBSCREEN_MAP_OBJECTS[2].b0d = 1;
        SUBSCREEN_MAP_OBJECTS[2].w08 = 0x7A52;
        SUBSCREEN_MAP_OBJECTS[2].b0f = -0xA;
        SUBSCREEN_MAP_OBJECTS[2].b0a = 1;
        SUBSCREEN_MAP_OBJECTS[2].w2e = 1;
        SUBSCREEN_MAP_OBJECTS[2].anim = D_801046FC;
        SUBSCREEN_MAP_OBJECTS[2].d3c = BANK;
        {
            Pt *t = &D_80102EBC[GAME.room][GAME.sub];
            SUBSCREEN_MAP_OBJECTS[2].p.s.x = t->x << 16;
            SUBSCREEN_MAP_OBJECTS[2].p.s.y = t->y << 16;
            SUBSCREEN_MAP_OBJECTS[2].p.s.z = t->z << 16;
        }
        SUBSCREEN_MAP_OBJECTS[2].q.v = SUBSCREEN_MAP_OBJECTS[2].p.v;
        func_800EF370();
        SUBSCREEN_MAP_OBJECTS[1].w08 = 0x7D16;
        SUBSCREEN_MAP_OBJECTS[1].b00 = 1;
        SUBSCREEN_MAP_OBJECTS[1].b02 = 1;
        SUBSCREEN_MAP_OBJECTS[1].b03 = 0;
        SUBSCREEN_MAP_OBJECTS[1].w1e = 0;
        SUBSCREEN_MAP_OBJECTS[1].b0d = 1;
        SUBSCREEN_MAP_OBJECTS[1].b0f = 0;
        SUBSCREEN_MAP_OBJECTS[1].w2e = 1;
        SUBSCREEN_MAP_OBJECTS[1].da0 = *(char **)0x1F800344 + ((int *)D_1F800344)[1];
        SUBSCREEN_MAP_OBJECTS[1].ba4 = 1;
        SUBSCREEN_MAP_OBJECTS[1].p.s.x = 0;
        SUBSCREEN_MAP_OBJECTS[1].p.s.y = 0;
        SUBSCREEN_MAP_OBJECTS[1].p.s.z = 0;
        SUBSCREEN_MAP_OBJECTS[1].q.v = SUBSCREEN_MAP_OBJECTS[1].p.v;
        SUBSCREEN_MAP_OBJECTS[1].b0c = 0;
        SUBSCREEN_MAP_OBJECTS[1].wb4 = 0;
        subScreenMapInitAreaSprites();
        for (i = 0; i < 2; i++) {
            Sprite *o = &D_80111214[i];
            o->w1e = 1;
            o->b0d = 1;
            o->w2e = 1;
            o->w08 = 0x7A92;
            *(void * volatile *)&o->anim = D_80104720[i];
            o->d3c = BANK;
            o->b0f = 0xA;
            o->b0a = 1;
            o->p.s.x = D_8010A314[i].x << 16;
            o->p.s.y = D_8010A314[i].y << 16;
            o->p.s.z = 0;
            o->q.v = o->p.v;
        }
        D_801113FC = 1000;
        D_801113D0 = 0;
        D_801113D4 = 0;
        D_801113D8 = 0;
        D_801113C0 = 0;
        D_801113F8 = 0;
        D_801113C8 = 0x30E;
        D_801113CC = 0x64;
        D_801113BC = D_801029CC[GAME.room][0];
        D_801113C4 = D_801029CC[GAME.room][1];
        m->state++;
        break;
    case 1:
        if (PAD & 0x60) {
            VIEW_CAMERA = D_8010A48C;
            return 1;
        }
        m->w16 = 11;
        if (GAME.room == 14) break;
        l = &MAP_LINKS[SUBSCREEN_MAP_LINK];
        r = func_800EF688();
        if (r == l->e) {
            s->q.s.x = ((MAP_NODES[l->a].x << 16) - s->p.s.x) / 4;
            s->q.s.y = ((MAP_NODES[l->a].y << 16) - s->p.s.y) / 4;
            s->q.s.z = ((MAP_NODES[l->a].z << 16) - s->p.s.z) / 4;
            t = l->a;
            s->wba = 1;
            s->wb4 = t;
            m->w4 = 4;
            playSFX(8);
            m->state++;
        } else if (r == (l->e + 4) % 8) {
            s->q.s.x = ((MAP_NODES[l->b].x << 16) - s->p.s.x) / 4;
            s->q.s.y = ((MAP_NODES[l->b].y << 16) - s->p.s.y) / 4;
            s->q.s.z = ((MAP_NODES[l->b].z << 16) - s->p.s.z) / 4;
            t = l->b;
            s->wba = 1;
            s->wb4 = t;
            m->w4 = 4;
            playSFX(8);
            m->state++;
        }
        break;
    case 2:
        s->p.s.x += s->q.s.x;
        s->p.s.y += s->q.s.y;
        s->p.s.z += s->q.s.z;
        if (--m->w4 == 0) {
            s->wba = 0;
            m->state++;
        }
        break;
    case 3:
        if (PAD & 0x40) {
            VIEW_CAMERA = D_8010A48C;
            playSFXWithNote(0x1B, 0x1C);
            return 1;
        }
        if (GAME.room == 14) break;
        p = D_801038A4[s->wb4];
        r = func_800EF688();
        if (p[0] != 0xFF) {
        loop:
            SUBSCREEN_MAP_LINK = p[2];
            l = &MAP_LINKS[SUBSCREEN_MAP_LINK];
            if (p[0] == r && D_8009EE6C[l->c]) {
                s->q.s.x = ((((MAP_NODES[l->a].x + MAP_NODES[l->b].x) / 2) << 16) - s->p.s.x) / 4;
                s->q.s.y = ((((MAP_NODES[l->a].y + MAP_NODES[l->b].y) / 2) << 16) - s->p.s.y) / 4;
                s->q.s.z = ((((MAP_NODES[l->a].z + MAP_NODES[l->b].z) / 2) << 16) - s->p.s.z) / 4;
                s->wb6 = l->c;
                t = l->d;
                s->wba = 1;
                s->wb8 = t;
                m->w4 = 4;
                playSFX(8);
                m->state++;
                break;
            }
            p += 4;
            if (p[0] != 0xFF) goto loop;
        }
        break;
    case 4:
        s->p.s.x += s->q.s.x;
        s->p.s.y += s->q.s.y;
        s->p.s.z += s->q.s.z;
        if (--m->w4 == 0) m->state = 1;
        break;
    }
    func_800EF7BC(s);
    return 0;
}
#endif
