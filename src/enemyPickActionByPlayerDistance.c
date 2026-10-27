// FUNC 80127720 256 X000
// MATCHING 80127720 256
// VERSION SLPM_80165 x00 8011D83C 256
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80127720 (FUN_80127720.c): y window 0x8d (PAL 0x6f).
extern unsigned short PLAYER_D_WHOLE, PLAYER_Y_WHOLE, PLAYER_H_WHOLE;
extern unsigned char *ENEMY_DISTANCE_ACTION_TABLES[];
extern unsigned FUN_8001f9e0(void);

unsigned char FUN_80127720(int a)
{
    short s;
    int d;
    short u;
    int v;
    unsigned char *t;
    if ((unsigned short)(*(unsigned short *)(*(int *)(a + 0x44) + 2) - PLAYER_D_WHOLE + 0x2d) >= 0x5b) return 0xff;
    if ((unsigned short)(*(unsigned short *)(a + 0x16) - PLAYER_Y_WHOLE + 0x46) >= 0x8d) return 0xff;
    d = *(unsigned short *)(*(int *)(a + 0x40) + 2) - PLAYER_H_WHOLE;
    if ((unsigned short)(d + 0x80) >= 0x101) return 0xff;
                s = d;
                if ((short)d < 0)
                    s = -d;
                v = s >= 0x42;
                u = v;
                if (s >= 0x52)
                    u = v + 1;
                t = ENEMY_DISTANCE_ACTION_TABLES[u];
    return t[FUN_8001f9e0() & 0xf];
}
#else
extern unsigned short PLAYER_D_WHOLE, PLAYER_Y_WHOLE, PLAYER_H_WHOLE;
extern unsigned char *ENEMY_DISTANCE_ACTION_TABLES[];
extern unsigned FUN_8001f9e0(void);

unsigned char enemyPickActionByPlayerDistance(int a)
{
    short s;
    int d;
    short u;
    int v;
    unsigned char *t;
    if ((unsigned short)(*(unsigned short *)(*(int *)(a + 0x44) + 2) - PLAYER_D_WHOLE + 0x2d) >= 0x5b) return 0xff;
    if ((unsigned short)(*(unsigned short *)(a + 0x16) - PLAYER_Y_WHOLE + 0x46) >= 0x6f) return 0xff;
    d = *(unsigned short *)(*(int *)(a + 0x40) + 2) - PLAYER_H_WHOLE;
    if ((unsigned short)(d + 0x80) >= 0x101) return 0xff;
                s = d;
                if ((short)d < 0)
                    s = -d;
                v = s >= 0x42;
                u = v;
                if (s >= 0x52)
                    u = v + 1;
                t = ENEMY_DISTANCE_ACTION_TABLES[u];
    return t[FUN_8001f9e0() & 0xf];
}
#endif
