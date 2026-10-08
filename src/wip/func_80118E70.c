// FUNC 80118e70 288 X000
/* diff: game keeps o in a0, list in a3 and does not hoist 0xffff0000/0x01020000 out of the loop */
extern unsigned short D_8009C960;
extern unsigned short D_1F80019C;
void func_80118E70(unsigned char *o)
{
    unsigned char **list;
    unsigned char *e;
    short n;
    if (D_8009C960 != 0) return;
    n = *(unsigned short *)0x1F800250;
    list = *(unsigned char ***)0x1F800260;
    D_1F80019C = n;
    while (n != 0) {
        e = *list;
        n = D_1F80019C - 1;
        D_1F80019C = n;
        list++;
        if ((*(int *)e & 0xffff0000) != 0x01020000) continue;
        if (!(e[0] & 1)) return;
        if (e[4] != 1) return;
        if ((unsigned short)(*(unsigned short *)(e + 0x6c) + (*(unsigned short *)(o + 0x12) - *(unsigned short *)(e + 0x12) + 0x40)) > *(short *)(e + 0x6e) + 0x60) return;
        if ((unsigned short)(*(unsigned short *)(e + 0x70) + (*(unsigned short *)(o + 0x16) - *(unsigned short *)(e + 0x16) + 0x2c)) > *(short *)(e + 0x72) + 0x18) return;
        e[0] = 2;
        e[4] = 2;
        e[5] = 3;
        e[6] = 0;
        return;
    }
}
