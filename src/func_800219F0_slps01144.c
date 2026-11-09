// VERSION SLPS_01144 slps01144 800219F0 80
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
typedef struct {
    unsigned short slot;
    char pad[0x32];
    int flags[1][8];
} GameFlags;
extern GameFlags GAME;

int func_800219F0(int n)
{
    int q = n / 32;
    int r = n % 32;

    return GAME.flags[GAME.slot][q] & (1 << r);
}
#endif
