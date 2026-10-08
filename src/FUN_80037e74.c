// FUNC 80037e74 328 MAIN0
// MATCHING 80037e74 328
typedef struct H { short m[2]; short x[2]; short d[64]; } H;
typedef struct D { short d[64]; char *base; char *cur; unsigned char f; } D;
extern int DAT_8009d69c;
extern int printf(char *, ...);

int FUN_80037e74(char *p, D *o)
{
    H h;
    int i;
    h = *(H *)p;
    if (*(int *)&h != 0x530057) {
        printf("Not Script File");
        DAT_8009d69c = 0;
        return -2;
    }
    for (i = 0; i < 64; i++)
        o->d[i] = h.d[i];
    o->base = p;
    o->cur = p + 0x88;
    o->f = 0;
    return 0;
}
