// FUNC 8004c418 344 MAIN0
// MATCHING 8004c418 344
// VERSION SLPM_80165 slpm80165 8004ED00 320
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8004C418 (FUN_8004c418.c): only sets the step, not the active byte.
typedef struct E { char a; char pad[3]; char b; char pad2[0x1c-5]; char f; signed char g; char pad3[0xd4-0x1e]; } E;
typedef struct E3 { char a; char pad[3]; char b; char pad2[0x1c-5]; char f; signed char g; char pad3[0x6c-0x1e]; } E3;
extern E OBJECT_POOL[];
extern E OBJECT_POOL_LAYER8[];
extern E3 OBJECT_POOL_LAYER7[];

void FUN_8004c418(signed char *o)
{
    E *e;
    E3 *e3;
    int i;
    e = OBJECT_POOL;
    i = 0;
    do {
        if (e->a != 0 && (e->f & 0x80) == 0 && o[0xe] + o[0xf] + e->g != 0) {
            e->b = 3;
        }
        i++;
        e++;
    } while (i < 200);
    i = 0;
    do {
        e = &OBJECT_POOL_LAYER8[i];
        if (e->a != 0 && (e->f & 0x80) == 0 && o[0xe] + o[0xf] + e->g != 0) {
            e->b = 3;
        }
        i++;
    } while (i < 0x2d);
    i = 0;
    do {
        e3 = &OBJECT_POOL_LAYER7[i];
        if (e3->a != 0 && (e3->f & 0x80) == 0 && o[0xe] + o[0xf] + e3->g != 0) {
            e3->b = 3;
        }
        i++;
    } while (i < 10);
}
#else
typedef struct E { char a; char pad[3]; char b; char pad2[0x1c-5]; char f; signed char g; char pad3[0xd4-0x1e]; } E;
typedef struct E3 { char a; char pad[3]; char b; char pad2[0x1c-5]; char f; signed char g; char pad3[0x6c-0x1e]; } E3;
extern E OBJECT_POOL[];
extern E OBJECT_POOL_LAYER8[];
extern E3 OBJECT_POOL_LAYER7[];

void despawnOtherSectionObjects(signed char *o)
{
    E *e;
    E3 *e3;
    int i;
    e = OBJECT_POOL;
    i = 0;
    do {
        if (e->a != 0 && (e->f & 0x80) == 0 && o[0xe] + o[0xf] + e->g != 0) {
            e->a = 2;
            e->b = 3;
        }
        i++;
        e++;
    } while (i < 200);
    i = 0;
    do {
        e = &OBJECT_POOL_LAYER8[i];
        if (e->a != 0 && (e->f & 0x80) == 0 && o[0xe] + o[0xf] + e->g != 0) {
            e->a = 2;
            e->b = 3;
        }
        i++;
    } while (i < 0x2d);
    i = 0;
    do {
        e3 = &OBJECT_POOL_LAYER7[i];
        if (e3->a != 0 && (e3->f & 0x80) == 0 && o[0xe] + o[0xf] + e3->g != 0) {
            e3->a = 2;
            e3->b = 3;
        }
        i++;
    } while (i < 10);
}
#endif
