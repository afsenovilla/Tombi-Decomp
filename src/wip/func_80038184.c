// FUNC 80038184 180 MAIN0
// Score 29: branch flag==0 uses p+2 (lbu 2(v1)) where the game reuses q (lbu 0(a2)); loop copy regs differ.
typedef struct {
    char pad[0x8a];
    unsigned short pc;
    char pad2[0x1090 - 0x8c];
    int vars[1];
} S80038184;

extern S80038184 *D_8009F0F0;
extern unsigned char *D_8009D60C;

static __inline__ void copy4(char *d, unsigned char *q, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        *d++ = *q++;
    }
}

void func_80038184(unsigned char flag)
{
    S80038184 *s = D_8009F0F0;
    unsigned char *p = D_8009D60C + s->pc;
    unsigned char idx = p[1];
    unsigned char *q = p + 2;
    int v;
    char buf[4];

    if (flag == 0) {
        v = s->vars[*q];
    } else {
        copy4(buf, q, 4);
        v = *(int *)buf;
    }
    s->vars[idx] = v;
    if (flag == 0) {
        s->pc += 3;
    } else {
        s->pc += 6;
    }
}
