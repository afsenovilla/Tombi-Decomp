// FUNC 80038184 180 MAIN0
// Score 29: flag==0 branch uses p+2 (lbu 2(v1)) where the game reuses q (lbu 0(a2)). Game loop = int-counter for loop strength-reduced (move a1,sp; move v1,a2(q copy); addiu a2,sp,4; slt) - direct "for(i<4) buf[i]=*q++" gives that loop shape but scores 41 (no q copy).
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
