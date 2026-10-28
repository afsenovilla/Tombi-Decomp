// FUNC 80018ebc 172 MAIN0
// MATCHING 80018ebc 172
// VERSION SLPM_80165 slpm80165 80017AE4 172
// VERSION SLPS_01144 slps01144 80018490 172
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80018EBC (initSpriteSlots8.c): 5x20 slots from y 0xaa (PAL 4x24 from y 0x9e).
typedef struct { short a; short b; unsigned char x, y, z, w, v, u; } E;
extern E SPRITE_SLOTS[];

void initSpriteSlots8(void)
{
    int a = 0;
    char c = -0x56;
    int i;
    for (i = 0; i < 0x30; i++) {
        SPRITE_SLOTS[i].x = a;
        a += 5;
        SPRITE_SLOTS[i].a = -1;
        SPRITE_SLOTS[i].b = 0;
        SPRITE_SLOTS[i].y = c;
        SPRITE_SLOTS[i].z = 5;
        SPRITE_SLOTS[i].w = 0x14;
        SPRITE_SLOTS[i].v = 0;
        SPRITE_SLOTS[i].u = 0;
        if (a > 0x3b) {
            a = 0;
            c += 0x14;
        }
    }
}
#elif defined(VERSION_SLPS_01144)
typedef struct { short a; short b; unsigned char x, y, z, w, v, u; } E;
extern E SPRITE_SLOTS[];

void initSpriteSlots8(void)
{
    int a = 0;
    char c = -0x56;
    int i;
    for (i = 0; i < 0x30; i++) {
        SPRITE_SLOTS[i].x = a;
        a += 5;
        SPRITE_SLOTS[i].a = -1;
        SPRITE_SLOTS[i].b = 0;
        SPRITE_SLOTS[i].y = c;
        SPRITE_SLOTS[i].z = 5;
        SPRITE_SLOTS[i].w = 0x14;
        SPRITE_SLOTS[i].v = 0;
        SPRITE_SLOTS[i].u = 0;
        if (a > 0x3b) {
            a = 0;
            c += 0x14;
        }
    }
}
#else
typedef struct { short a; short b; unsigned char x, y, z, w, v, u; } E;
extern E SPRITE_SLOTS[];

void initSpriteSlots8(void)
{
    int a = 0;
    char c = -0x62;
    int i;
    for (i = 0; i < 0x30; i++) {
        SPRITE_SLOTS[i].x = a;
        a += 4;
        SPRITE_SLOTS[i].a = -1;
        SPRITE_SLOTS[i].b = 0;
        SPRITE_SLOTS[i].y = c;
        SPRITE_SLOTS[i].z = 4;
        SPRITE_SLOTS[i].w = 0x18;
        SPRITE_SLOTS[i].v = 0;
        SPRITE_SLOTS[i].u = 0;
        if (a > 0x3b) {
            a = 0;
            c += 0x18;
        }
    }
}
#endif
