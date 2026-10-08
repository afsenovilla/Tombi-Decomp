// FUNC 80118e70 288 X000
/* score 30: goto loop + c=2 before loop; remaining: list in a2 vs a3 and n in a3 vs a2, and the order of loads inside the loop */
extern unsigned short D_8009C960;
extern unsigned short D_1F80019C;
void func_80118E70(unsigned char *o)
{
    unsigned char **list;
    unsigned char *e;
    short n;
    int c;
    if (D_8009C960 != 0) return;
    n = *(unsigned short *)0x1F800250;
    list = *(unsigned char ***)0x1F800260;
    D_1F80019C = n;
    if (n == 0) return;
    c = 2;
loop:
    e = *list;
    n = D_1F80019C - 1;
    D_1F80019C = n;
    list++;
    if ((*(int *)e & 0xffff0000) == 0x01020000) {
        if (!(e[0] & 1)) return;
        if (e[4] != 1) return;
        if ((unsigned short)(*(unsigned short *)(e + 0x6c) + (*(unsigned short *)(o + 0x12) - *(unsigned short *)(e + 0x12) + 0x40)) > *(short *)(e + 0x6e) + 0x60) return;
        if ((unsigned short)(*(unsigned short *)(e + 0x70) + (*(unsigned short *)(o + 0x16) - *(unsigned short *)(e + 0x16) + 0x2c)) > *(short *)(e + 0x72) + 0x18) return;
        e[0] = c;
        e[4] = c;
        e[5] = 3;
        e[6] = 0;
        return;
    }
    if (n != 0) goto loop;
}
