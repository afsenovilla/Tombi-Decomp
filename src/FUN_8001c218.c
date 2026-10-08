// FUNC 8001c218 252 MAIN0
// MATCHING 8001c218 252
typedef struct { unsigned short a, b; } P2;
extern P2 DAT_8009c960;
extern unsigned short DAT_8009f838;
extern unsigned short DAT_8009c982;
extern unsigned short DAT_8009d2a8, DAT_8009d2aa, DAT_8009d2ac;
extern unsigned short DAT_1f8001de, DAT_1f8001dc;
extern unsigned char DAT_8009c967;
extern unsigned char *PTR_80077b64[];
extern int FUN_8001beec(void);
extern void FUN_8004f3ec(int);
extern int FUN_8001d1bc(int a);
extern void FUN_8004f490(int a, int b, int c);
extern void FUN_80039338(void);
extern void FUN_8004f3bc(void);

void FUN_8001c218(unsigned short p)
{
    int u;
    int idx;
    unsigned char c;
    u = FUN_8001beec() & 0xff;
    idx = DAT_8009c960.a + DAT_8009f838;
    c = PTR_80077b64[idx][DAT_8009c960.b];
    DAT_1f8001de = 0;
    DAT_8009d2a8 = DAT_8009c960.a;
    DAT_8009d2aa = DAT_8009c960.b;
    DAT_8009d2ac = DAT_8009c982;
    DAT_1f8001dc = c;
    FUN_8004f3ec(idx);
    if (FUN_8001d1bc((short)p) != -1)
        DAT_8009c967 = 2;
    FUN_8004f490(DAT_8009c960.a + DAT_8009f838, DAT_8009c960.b, (short)(p | u));
    FUN_80039338();
    FUN_8004f3bc();
}
