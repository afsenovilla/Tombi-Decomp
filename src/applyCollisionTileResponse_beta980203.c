// VERSION BETA_980203 beta980203 800420F8 480
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/applyCollisionTileResponse.c without the 0xa0/0xbe clears (the caller clears them), flags read first.
#define COLLISION_HIT_SURFACE D_1F800284
#define TILE_RESPONSE_ACTIVE D_1F8001D2
#include "TOBJ.H"
extern short COLLISION_HIT_HEIGHT;
extern unsigned char TILE_RESPONSE_ACTIVE;
extern unsigned short COLLISION_HIT_SURFACE[], COLLISION_HIT_FLAGS[];
typedef struct { char pad[0xa0]; unsigned char ba0; } XA0;
#define BA0(o) (((XA0 *)(o))->ba0)

void applyCollisionTileResponse(TObj *o)
{
    char pad[4];
    short a;
    unsigned int w;
    short hi;
    short e;
    short n;
    short m;
    a = COLLISION_HIT_HEIGHT;
    if (a < 0)
        a = -a;
    if (a > 8)
        a = 8;
    if (COLLISION_HIT_HEIGHT < 0)
        a = -a;
    o->b69 = 1;
    w = COLLISION_HIT_FLAGS[0];
    e = COLLISION_HIT_SURFACE[0];
    o->wb0 = a;
    TILE_RESPONSE_ACTIVE = 1;
    hi = w >> 15;
    o->wae = e;
    m = (w >> 5) & 0xf;
    n = m;
    if (!(w & 0x3000)) {
        switch (n) {
        case 0: o->bbe = 0; break;
        case 1: o->bbe = 2; break;
        case 2: o->bbe = 0x10; break;
        case 3: o->bbe = 0x20; break;
        }
        o->bbe = hi | o->bbe;
        return;
    }
    if (w & 0x2000) {
        n = (unsigned)n >> 2;
        switch (n) {
        case 0: BA0(o) = 2; break;
        case 1: BA0(o) = 1; break;
        case 2: BA0(o) = 3; break;
        }
    }
    if (COLLISION_HIT_FLAGS[0] & 0x1000) {
        switch (m & 3) {
        case 0: BA0(o) |= 0x10; break;
        case 1: BA0(o) |= 0x20; break;
        case 2: BA0(o) |= 0x30; break;
        case 3: BA0(o) |= 0x40; break;
        }
    }
}
#endif
