// FUNC 8001c218 252 MAIN0
extern unsigned short DAT_8009c960[];
extern unsigned short DAT_8009f838;
extern unsigned short DAT_8009c982;
extern unsigned short DAT_8009d2a8, DAT_8009d2aa, DAT_8009d2ac;
extern unsigned short DAT_1f8001de, DAT_1f8001dc;
extern unsigned char DAT_8009c967;
extern unsigned char *PTR_80077b64[];
extern unsigned short FUN_8001beec(void);
extern void FUN_8004f3ec(void);
extern int FUN_8001d1bc(int a);
extern void FUN_8004f490(int a, int b, int c);
extern void FUN_80039338(void);
extern void FUN_8004f3bc(void);

void FUN_8001c218(unsigned short p)
{
    unsigned short u;
    int r;
    unsigned short *q;
    unsigned short a, b;
    u = FUN_8001beec();
    q = DAT_8009c960;
    a = q[0];
    b = q[1];
    DAT_1f8001de = 0;
    DAT_8009d2a8 = a;
    DAT_8009d2aa = b;
    DAT_8009d2ac = DAT_8009c982;
    DAT_1f8001dc = PTR_80077b64[a + DAT_8009f838][b];
    FUN_8004f3ec();
    r = FUN_8001d1bc((short)p);
    if (r != -1) DAT_8009c967 = 2;
    FUN_8004f490(q[0] + DAT_8009f838, q[1], (short)(p | (u & 0xff)));
    FUN_80039338();
    FUN_8004f3bc();
}
