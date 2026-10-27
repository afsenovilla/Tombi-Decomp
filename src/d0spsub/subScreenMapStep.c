// FUNC 800ee9ac 2956 D0SPSUB
// MATCHING 800ee9ac 2956
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
    char p6[0xC];
    short result;
    char p14[2];
    short cursor;
    char p18[6];
    short w1e;
} Menu;
extern Sprite SUBSCREEN_MAP_OBJECTS[];
extern Sprite SUBSCREEN_MAP_ICON_SPRITES[];
extern Save SUBSCREEN_SAVED_CAMERA, VIEW_CAMERA;
extern unsigned short JOYPAD_STATE;
extern volatile int D_1F8002EC;
extern char *D_1F800344;
extern unsigned char D_8010A44C;
extern int D_8010A3B8, D_8010A3BC, D_8010A3C0;
#define P344 (*(char * volatile *)&D_1F800344)
typedef struct { unsigned short room, sub; } RS;
extern RS GAME;
extern unsigned short D_8009D2C4[];
extern void *MAP_AREA_SPRITE_ANIMS;
extern void *MAP_ICON_ANIMS[];
extern Pt *MAP_ROOM_POSITIONS[];
extern Pt MAP_NODES[];
extern Link MAP_LINKS[];
extern unsigned char *D_800FDE50[];
extern XY SUBSCREEN_MAP_ICON_POSITIONS[];
extern unsigned short MAP_AREA_VIEW_ORIGIN[][6];
extern int SUBSCREEN_MAP_LINK;
extern short D_8010A484, D_8010A49A, D_8010A4AA;
extern unsigned char D_8010A486, D_8010A489;
extern signed char D_8010A48B;
extern void *D_8010A4A0;
extern int D_8010A4B8;
extern Vec D_8010A48C, D_8010A4AC;
extern int D_8010A490, D_8010A494;
extern short D_8010A3B0, D_8010A3C6, D_8010A3D6, D_8010A45C;
extern unsigned char D_8010A3A8, D_8010A3AA, D_8010A3AB, D_8010A3B5, D_8010A3B7, D_8010A3B4;
extern Vec D_8010A3D8;
extern char *D_8010A448;
extern short D_8011110C, D_80111110, D_80111114, D_801110FC, D_80111104, D_80111108;
extern unsigned short SUBSCREEN_MAP_VIEW_X, SUBSCREEN_MAP_VIEW_Y;
extern int subScreenMoveTabCursor(Menu *m, int n);
extern void subScreenMapInitMarker(void);
extern void subScreenMapInitAreaSprites(void);
extern int subScreenMapReadInput(void);
extern void subScreenMapScrollToMarker(Sprite *s);
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
extern void SfxPlay2(int a, int b);

int subScreenMapStep(Menu *m)
{
    Sprite *s = SUBSCREEN_MAP_OBJECTS;
    Link *l;
    unsigned char *p;
    int r;
    int i;
    unsigned char t;
    switch (m->state) {
    case 0:
        SUBSCREEN_SAVED_CAMERA = VIEW_CAMERA;
        SUBSCREEN_MAP_OBJECTS[2].w1e = 1;
        SUBSCREEN_MAP_OBJECTS[2].b0d = 1;
        SUBSCREEN_MAP_OBJECTS[2].w08 = 0x7A52;
        SUBSCREEN_MAP_OBJECTS[2].b0f = -0xA;
        SUBSCREEN_MAP_OBJECTS[2].b0a = 1;
        SUBSCREEN_MAP_OBJECTS[2].w2e = 1;
        SUBSCREEN_MAP_OBJECTS[2].anim = MAP_AREA_SPRITE_ANIMS;
        SUBSCREEN_MAP_OBJECTS[2].d3c = D_1F8002EC;
        {
            Pt *t = &MAP_ROOM_POSITIONS[GAME.room][GAME.sub];
            SUBSCREEN_MAP_OBJECTS[2].p.s.x = t->x << 16;
            SUBSCREEN_MAP_OBJECTS[2].p.s.y = (t->y - 0x14) << 16;
            SUBSCREEN_MAP_OBJECTS[2].p.s.z = (t->z - 0x14) << 16;
        }
        SUBSCREEN_MAP_OBJECTS[2].q.v = SUBSCREEN_MAP_OBJECTS[2].p.v;
        subScreenMapInitMarker();
        SUBSCREEN_MAP_OBJECTS[1].w08 = 0x7D16;
        SUBSCREEN_MAP_OBJECTS[1].b00 = 1;
        SUBSCREEN_MAP_OBJECTS[1].b02 = 1;
        SUBSCREEN_MAP_OBJECTS[1].b03 = 0;
        SUBSCREEN_MAP_OBJECTS[1].w1e = 0;
        SUBSCREEN_MAP_OBJECTS[1].b0d = 1;
        SUBSCREEN_MAP_OBJECTS[1].b0f = 0;
        SUBSCREEN_MAP_OBJECTS[1].w2e = 1;
        D_8010A44C = 1;
        D_8010A3B8 = 0;
        D_8010A3BC = 0;
        D_8010A3C0 = 0;
        SUBSCREEN_MAP_OBJECTS[1].da0 = *(char **)0x1F800344 + ((int *)D_1F800344)[1];
        SUBSCREEN_MAP_OBJECTS[1].q.v = SUBSCREEN_MAP_OBJECTS[1].p.v;
        SUBSCREEN_MAP_OBJECTS[1].b0c = 0;
        SUBSCREEN_MAP_OBJECTS[1].wb4 = 0;
        subScreenMapInitAreaSprites();
        for (i = 0; i < 2; i++) {
            Sprite *o = &SUBSCREEN_MAP_ICON_SPRITES[i];
            o->w1e = 1;
            o->b0d = 1;
            o->w2e = 1;
            o->w08 = 0x7A92;
            *(void * volatile *)&o->anim = MAP_ICON_ANIMS[i];
            o->d3c = D_1F8002EC;
            o->b0f = 0xA;
            o->b0a = 1;
            o->p.s.x = SUBSCREEN_MAP_ICON_POSITIONS[i].x << 16;
            o->p.s.y = SUBSCREEN_MAP_ICON_POSITIONS[i].y << 16;
            o->p.s.z = 0;
            o->q.v = o->p.v;
        }
        m->cursor = 7;
        D_8011110C = 0;
        D_80111110 = 0;
        D_80111114 = 0;
        D_801110FC = 0;
        D_80111104 = 0x30E;
        D_80111108 = 0x64;
        SUBSCREEN_MAP_VIEW_X = MAP_AREA_VIEW_ORIGIN[GAME.room][0];
        SUBSCREEN_MAP_VIEW_Y = MAP_AREA_VIEW_ORIGIN[GAME.room][1];
        m->w1e = 0;
        m->state++;
        break;
    case 1:
        if (subScreenMoveTabCursor(m, 1)) return 0;
        if (JOYPAD_STATE & 0x2000) {
            VIEW_CAMERA = SUBSCREEN_SAVED_CAMERA;
            SfxPlay2(0x1B, 0x1C);
            return 1;
        }
        if (!(JOYPAD_STATE & 0x4008)) break;
        switch (m->cursor) {
        case 0:
            break;
        case 4:
            m->result = 1;
            m->cursor = 0;
            m->w1e = -1;
            m->state = 0;
            VIEW_CAMERA = SUBSCREEN_SAVED_CAMERA;
            playSFXAndSetNote(10, 10);
            return 2;
        case 5:
            m->result = 2;
            m->cursor = 0;
            m->w1e = -1;
            m->state = 0;
            VIEW_CAMERA = SUBSCREEN_SAVED_CAMERA;
            playSFXAndSetNote(10, 10);
            return 2;
        case 6:
            m->result = 0;
            m->state = 0;
            VIEW_CAMERA = SUBSCREEN_SAVED_CAMERA;
            playSFXAndSetNote(10, 10);
            return 2;
        case 7:
            m->result = 3;
            m->cursor = 0;
            if (m->w1e == 0) {
                s->wba = 1;
                m->state = 2;
            } else {
                s->wba = 0;
                m->state = 4;
            }
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            VIEW_CAMERA = SUBSCREEN_SAVED_CAMERA;
            SfxPlay2(10, 10);
            return 1;
        }
        playSFXAndSetNote(10, 10);
        break;
    case 2:
        m->w1e = 0;
        if (JOYPAD_STATE & 0x2000) goto cancel;
        if (GAME.room == 14) break;
        l = &MAP_LINKS[SUBSCREEN_MAP_LINK];
        r = subScreenMapReadInput();
        if (r == l->e) {
            s->q.s.x = (((MAP_NODES[l->a].x + 8) << 16) - s->p.s.x) / 4;
            s->q.s.y = (((MAP_NODES[l->a].y + 2) << 16) - s->p.s.y) / 4;
            s->q.s.z = ((MAP_NODES[l->a].z << 16) - s->p.s.z) / 4;
            t = l->a;
            s->wba = 1;
            s->wb4 = t;
            m->w4 = 4;
            playSFX(8);
            m->state++;
        } else if (r == (l->e + 4) % 8) {
            s->q.s.x = (((MAP_NODES[l->b].x + 8) << 16) - s->p.s.x) / 4;
            s->q.s.y = (((MAP_NODES[l->b].y + 2) << 16) - s->p.s.y) / 4;
            s->q.s.z = ((MAP_NODES[l->b].z << 16) - s->p.s.z) / 4;
            t = l->b;
            s->wba = 1;
            s->wb4 = t;
            m->w4 = 4;
            playSFX(8);
            m->state++;
        }
        break;
    case 3:
        s->p.s.x += s->q.s.x;
        s->p.s.y += s->q.s.y;
        s->p.s.z += s->q.s.z;
        if (--m->w4 == 0) {
            s->wba = 0;
            m->state++;
        }
        break;
    case 4:
        m->w1e = 1;
        if (JOYPAD_STATE & 0x2000) {
        cancel:
            SfxPlay2(0x1B, 0x1C);
            m->cursor = 7;
            m->state = 1;
            return 0;
        }
        if (GAME.room == 14) break;
        p = D_800FDE50[s->wb4];
        r = subScreenMapReadInput();
        if (p[0] != 0xFF) {
        loop:
            SUBSCREEN_MAP_LINK = p[2];
            l = &MAP_LINKS[SUBSCREEN_MAP_LINK];
            if (p[0] == r && D_8009D2C4[l->c]) {
                s->q.s.x = ((((MAP_NODES[l->a].x + 8 + MAP_NODES[l->b].x + 8) / 2) << 16) - s->p.s.x) / 4;
                s->q.s.y = ((((MAP_NODES[l->a].y + 2 + MAP_NODES[l->b].y + 2) / 2) << 16) - s->p.s.y) / 4;
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
    case 5:
        s->p.s.x += s->q.s.x;
        s->p.s.y += s->q.s.y;
        s->p.s.z += s->q.s.z;
        if (--m->w4 == 0) m->state = 2;
        break;
    }
    subScreenMapScrollToMarker(s);
    return 0;
}
