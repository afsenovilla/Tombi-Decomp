// FUNC 800ebecc 268 I0NFO3
// MATCHING 800ebecc 268
#include "TOBJ.H"
extern void func_80018934(TObj *o);
extern unsigned short D_800A4670[], D_800A4672[];
extern short D_800A4674[], D_800A4676[];
extern unsigned char *TUNNEL_MAP_TILES;

void tunnelMapObjHandler(TObj *o)
{
    unsigned short *map;
    unsigned char *src;
    int i;
    switch (o->b04) {
    case 0:
        map = *(unsigned short **)0x1F800390;
        D_800A4670[0] = map[0];
        D_800A4672[0] = map[1];
        D_800A4674[0] = map[0] >> 2;
        src = (unsigned char *)(map + 2);
        TUNNEL_MAP_TILES = (unsigned char *)0x80124000;
        D_800A4676[0] = map[1] >> 2;
        for (i = 0; i < 0x9FF0; i++) {
            TUNNEL_MAP_TILES[i] = *src++;
        }
        o->b04++;
        break;
    case 1:
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018934(o);
        break;
    }
}
