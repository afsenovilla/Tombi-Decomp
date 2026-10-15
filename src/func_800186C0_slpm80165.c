// VERSION SLPM_80165 slpm80165 800186C0 112
#if defined(VERSION_SLPM_80165)
// Demo-only helper: walks a 64-byte entry list into func_8001836C while it returns > 0.
typedef struct { unsigned char a, b, c, d; unsigned char data[60]; } E;
extern int func_8001836C(char *, unsigned char *);
void func_800186C0_slpm80165(char *o, E *e)
{
    do {
        unsigned char b = e->b, c = e->c;
        *(short *)(o + 0x24) = e->a;
        *(short *)(o + 0x26) = b;
        *(short *)(o + 0x28) = c;
    } while (func_8001836C(o, (e++)->data) > 0);
}
#endif
