// VERSION PCPX_96084 tomba 80070D44 272
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of enemyPickActionByPlayerDistance (src/enemyPickActionByPlayerDistance.c): wider windows, subtype 8 returns 8, player position at 0x1F8002BA..
#define PLAYER_D_WHOLE D_1F8002C2
#define PLAYER_Y_WHOLE D_1F8002BE
#define PLAYER_H_WHOLE D_1F8002BA
extern unsigned short PLAYER_D_WHOLE, PLAYER_Y_WHOLE, PLAYER_H_WHOLE;
extern unsigned char *D_800AEB84[];
extern unsigned Rand(void);

unsigned char func_80070D44_pcpx96084(int a)
{
    short s;
    int d;
    short u;
    int v;
    unsigned char *t;
    if ((unsigned short)(*(unsigned short *)(*(int *)(a + 0x44) + 2) - PLAYER_D_WHOLE + 0x2d) >= 0x5b) return 0xff;
    if ((unsigned short)(*(unsigned short *)(a + 0x16) - PLAYER_Y_WHOLE + 0x46) >= 0x8d) return 0xff;
    if (*(unsigned char *)(a + 3) == 8) return 8;
    d = *(unsigned short *)(*(int *)(a + 0x40) + 2) - PLAYER_H_WHOLE;
    if ((unsigned short)(d + 0x60) >= 0xc1) return 0xff;
                s = d;
                if ((short)d < 0)
                    s = -d;
                v = s >= 0x42;
                u = v;
                if (s >= 0x52)
                    u = v + 1;
                t = D_800AEB84[u];
    return t[Rand() & 0xf];
}
#endif
