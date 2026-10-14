// JPDEMO slpm80165 80028E88 228
// Demo version of PAL main0 80028F94 (handleLookScrollInput.c): pad bits 0x1000/0x4000 (PAL 0x10/0x40).
typedef struct S { char pad[0x6c]; signed char a6c, a6d, a6e, a6f; } S;
extern unsigned short DAT_1f8001fc;
extern unsigned char DAT_800a60d8;
int handleLookScrollInput(S *s)
{
    if ((DAT_1f8001fc & 0x1000) && DAT_800a60d8 != 4) {
        if (s->a6e == 1)
            goto one;
        if (s->a6e == 0) {
            s->a6c = 7;
            s->a6e = 1;
            s->a6d = 0;
            s->a6f = -10;
        } else {
            s->a6c = 7;
            s->a6d = 0;
            s->a6e = 0;
            s->a6f = 0;
        }
    } else {
        if (!(DAT_1f8001fc & 0x4000) || s->a6e == 2)
            goto one;
        if (s->a6e == 0) {
            s->a6c = 7;
            s->a6d = 1;
            s->a6e = 2;
            s->a6f = 10;
        } else {
            s->a6c = 7;
            s->a6d = 1;
            s->a6e = 0;
            s->a6f = 0;
        }
    }
    return 0;
one:
    return 1;
}
