// FUNC 80028f94 228 MAIN0
// MATCHING 80028f94 228
typedef struct S { char pad[0x6c]; signed char a6c, a6d, a6e, a6f; } S;
extern unsigned short DAT_1f8001fc;
extern unsigned char DAT_800a60d8;
int FUN_80028f94(S *s)
{
    if ((DAT_1f8001fc & 0x10) && DAT_800a60d8 != 4) {
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
        if (!(DAT_1f8001fc & 0x40) || s->a6e == 2)
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
