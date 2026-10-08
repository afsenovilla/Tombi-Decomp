// FUNC 80118e70 288 X000
/* score 24: goto loop + c=2 local fixes constant hoisting; remaining: pp in a2 vs a3 and n in a3 vs a2 (decl/statement perms, struct fields, n types tried) */
extern unsigned short DAT_8009c960;
extern unsigned short DAT_1f800250;
extern unsigned short DAT_1f80019c;
typedef struct { unsigned int w; } W;
extern unsigned char **DAT_1f800260;

void func_80118E70(unsigned char *o)
{
    short n;
    unsigned char **pp;
    unsigned char *e;
    int c;

    if (DAT_8009c960 != 0)
        return;
    DAT_1f80019c = DAT_1f800250;
    pp = DAT_1f800260;
    if (DAT_1f800250 == 0)
        return;
    c = 2;
loop:
        e = *pp++;
        n = DAT_1f80019c - 1;
        DAT_1f80019c = n;
        if ((((W *)e)->w & 0xffff0000) == 0x1020000) {
            if ((*e & 1) == 0)
                return;
            if (e[4] != 1)
                return;
            if ((unsigned short)(*(unsigned short *)(e + 0x6c) + (-*(unsigned short *)(e + 0x12) + *(unsigned short *)(o + 0x12) + 0x40)) > *(short *)(e + 0x6e) + 0x60)
                return;
            if ((unsigned short)(*(unsigned short *)(e + 0x70) + (-*(unsigned short *)(e + 0x16) + *(unsigned short *)(o + 0x16) + 0x2c)) > *(short *)(e + 0x72) + 0x18)
                return;
            *e = c;
            e[4] = c;
            e[5] = 3;
            e[6] = 0;
            return;
        }
    if (n != 0) goto loop;
}
