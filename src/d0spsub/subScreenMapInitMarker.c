// FUNC 800ee37c 516 D0SPSUB
// MATCHING 800ee37c 516
typedef struct { unsigned short x, y, z, pad; } Pt;
typedef struct { unsigned char a, b, c, d, e, f; } Link;
typedef struct { int x, y, z; } Vec;
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
typedef struct { unsigned short room, sub; } RS;
extern Sprite SUBSCREEN_MAP_OBJECTS[];
extern volatile int D_1F8002EC;
extern RS GAME;
extern void *MAP_MARKER_ANIM;
extern short *MAP_ROOM_LINK[];
extern Link MAP_LINKS[];
extern Pt MAP_NODES[];
extern int SUBSCREEN_MAP_LINK;

Sprite *subScreenMapInitMarker(void)
{
    short *e;
    Link *l;
    SUBSCREEN_MAP_OBJECTS->w1e = 1;
    SUBSCREEN_MAP_OBJECTS->b0d = 1;
    SUBSCREEN_MAP_OBJECTS->w08 = 0x78D2;
    SUBSCREEN_MAP_OBJECTS->b0f = -0x1E;
    SUBSCREEN_MAP_OBJECTS->b0a = 1;
    SUBSCREEN_MAP_OBJECTS->w2e = 1;
    SUBSCREEN_MAP_OBJECTS->anim = MAP_MARKER_ANIM;
    SUBSCREEN_MAP_OBJECTS->d3c = D_1F8002EC;
    e = &MAP_ROOM_LINK[GAME.room][GAME.sub];
    l = &MAP_LINKS[*e];
    {
        int a = l->a, b = l->b;
        short xa, xb, ya, yb;
        unsigned short za, zb;
        xa = MAP_NODES[a].x + 8;
        xb = MAP_NODES[b].x + 8;
        ya = MAP_NODES[a].y + 2;
        za = MAP_NODES[a].z;
        yb = MAP_NODES[b].y + 2;
        zb = MAP_NODES[b].z;
        SUBSCREEN_MAP_OBJECTS->p.s.x = (xa + xb) / 2 << 16;
        SUBSCREEN_MAP_OBJECTS->p.s.y = (ya + yb) / 2 << 16;
        SUBSCREEN_MAP_OBJECTS->p.s.z = ((short)za + (short)zb) / 2 << 16;
    }
    SUBSCREEN_MAP_OBJECTS->q.v = SUBSCREEN_MAP_OBJECTS->p.v;
    SUBSCREEN_MAP_LINK = *e;
    SUBSCREEN_MAP_OBJECTS->wb4 = 0;
    SUBSCREEN_MAP_OBJECTS->wb6 = l->c;
    SUBSCREEN_MAP_OBJECTS->wb8 = l->d;
    SUBSCREEN_MAP_OBJECTS->wba = 1;
    return SUBSCREEN_MAP_OBJECTS;
}
