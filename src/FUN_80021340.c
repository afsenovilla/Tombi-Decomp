// FUNC 80021340 312 MAIN0
// MATCHING 80021340 312
extern unsigned int *DAT_8009d540;
extern unsigned int *DAT_1f8001e0;
extern char DAT_800787f0[];
extern char DAT_800787f2[];
extern char DAT_800787f4[];
extern char DAT_800787f6[];
extern char DAT_800787f8[];
extern char DAT_800787fa[];
extern unsigned short GetClut(int, int);
typedef struct { unsigned addr : 24; unsigned len : 8; } Tag;

void FUN_80021340(short x, short y, short n)
{
    unsigned int *p;
    unsigned int *q;
    int o;
    p = DAT_8009d540;
    ((unsigned char *)p)[3] = 4;
    ((unsigned char *)p)[7] = 0x65;
    ((unsigned char *)p)[4] = 0x80;
    ((unsigned char *)p)[5] = 0x80;
    ((unsigned char *)p)[6] = 0x80;
    o = n * 12;
    ((short *)p)[4] = x;
    ((short *)p)[5] = y;
    ((unsigned char *)p)[7] &= 0xfd;
    ((unsigned char *)p)[12] = *(short *)(DAT_800787f0 + o);
    ((unsigned char *)p)[13] = *(short *)(DAT_800787f2 + o);
    ((short *)p)[8] = *(short *)(DAT_800787f4 + o);
    ((short *)p)[9] = *(short *)(DAT_800787f6 + o);
    ((unsigned short *)p)[7] = GetClut(*(short *)(DAT_800787f8 + o), *(short *)(DAT_800787fa + o));
    q = DAT_1f8001e0;
    ((Tag *)p)->addr = ((Tag *)q)->addr;
    ((Tag *)q)->addr = (unsigned int)p;
    DAT_8009d540 = DAT_8009d540 + 5;
}
