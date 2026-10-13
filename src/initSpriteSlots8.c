// FUNC 80018ebc 172 MAIN0
// MATCHING 80018ebc 172
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
