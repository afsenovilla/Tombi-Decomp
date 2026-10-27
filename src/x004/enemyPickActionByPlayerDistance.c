// FUNC 8011ffdc 256 X004
// MATCHING 8011ffdc 256
extern unsigned short PLAYER_D_WHOLE, PLAYER_Y_WHOLE, PLAYER_H_WHOLE;
extern unsigned char *PTR_801310dc[];
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
                t = PTR_801310dc[u];
    return t[FUN_8001f9e0() & 0xf];
}
