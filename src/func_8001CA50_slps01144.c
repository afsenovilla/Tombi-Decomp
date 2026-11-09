// VERSION SLPS_01144 slps01144 8001CA50 1024
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
/* JP spawnEventTitle; form from psx_tomba (main/game/event.c, REGION_JP), MIT. */
typedef struct W { unsigned short unk0, unk2; } W;
extern unsigned short D_800A7B8A, D_800A60A2, PLAYER_Y, GAME;
extern unsigned short VIEW_CAMERA_Y;
extern int *D_1F80033C;
extern unsigned char *D_1F80039C;
extern unsigned char *D_80071CE4[];
extern signed char *D_80071A20[];
extern signed char *D_80071A3C;
extern unsigned char D_80071A54[];
extern unsigned char D_8009E4DB[];
extern short D_800718F2[];
extern unsigned short D_800718F8[];
extern unsigned short D_80071B1C[];
extern unsigned short D_80071B1E[];
extern short PLAYER_Z[];
extern unsigned char *allocObjectLayer8(void);
#define PL(o) (((unsigned char *)PLAYER_Z)[(o) - 0x1A])

void func_8001CA50(int event_id, int completed, int duration, int state)
{
    short x;
    short y;
    unsigned char *src;
    unsigned char *entry;
    signed char *p;
    unsigned char *obj;
    int i;
    int done;
    int once;
    int n;
    unsigned short *q;
    int temp;
    int *hdr;

    if (event_id == 1) {
        duration = 1;
        x = D_800A7B8A - D_800A60A2;
        y = PLAYER_Y - VIEW_CAMERA_Y;
    }
    src = D_1F80039C;
    src += *(unsigned short *)(src + 8);
    entry = D_80071CE4[GAME];
    entry += event_id;
    if (*entry == 0xFF) {
        q = (unsigned short *)(src + 2);
    } else {
        q = (unsigned short *)(src + *entry * 2);
    }
    src += *q;
    i = 0;
    if (*entry == 0xFF) {
        p = D_80071A3C;
    } else {
        p = D_80071A20[D_80071A54[event_id]];
    }
    done = 0;
    D_8009E4DB[0] = 1;
    once = 0;
    for (; p[0] != -1; p += 4) {
        obj = allocObjectLayer8();
        if (obj != 0) {
            obj[0] = 1;
            obj[2] = 1;
            obj[0xD] = 1;
            *(short *)(obj + 8) = 0x7D16;
            obj[3] = completed;
            *(short *)(obj + 0x1E) = 0;
            *(signed char *)(obj + 0xF) = -0x5A;
            *(short *)(obj + 0x2E) = 1;
            temp = obj[0x1C] | 0x80;
            obj[0x1C] = temp;
            *(short *)(obj + 0x20) = duration;
            temp = (int)&D_1F80033C;
            hdr = *(int *volatile *)temp;
            temp = (int)*(int *volatile *)temp;
            temp += hdr[1];
            *(int *)(obj + 0xA0) = temp;
            if (event_id == 1) {
                *(int *)(obj + 0x10) = (x - 0xA0) << 16;
                *(int *)(obj + 0x14) = y << 16;
                *(int *)(obj + 0x18) = (PLAYER_Z[0] + 10) << 16;
                *(int *)(obj + 0x30) = (x - 0xA0) << 16;
                *(int *)(obj + 0x34) = y << 16;
                *(int *)(obj + 0x38) = (PLAYER_Z[0] + 10) << 16;
            } else {
                *(int *)(obj + 0x10) = D_800718F2[0] << 16;
                *(int *)(obj + 0x14) = D_800718F2[1] << 16;
                *(int *)(obj + 0x18) = 0;
                *(int *)(obj + 0x30) = D_800718F2[0] << 16;
                *(int *)(obj + 0x34) = D_800718F2[1] << 16;
                *(int *)(obj + 0x38) = 0;
            }
            obj[0xC] = i;
            n = p[3] & 0xF;
            *(unsigned short *)(obj + 0xBC) = *(unsigned short *)src & 0xFFF;
            *(unsigned short *)(obj + 0xC0) = D_800718F8[n];
            *(short *)(obj + 0xC4) = p[1] * 12;
            *(short *)(obj + 0xC6) = p[2] * 12;
            *(unsigned short *)(obj + 0xC8) = D_80071B1C[n * 2];
            *(unsigned short *)(obj + 0xCA) = D_80071B1E[n * 2];
            *(unsigned short *)(obj + 0xCC) = (unsigned char)p[3];
            i++;
            src += 2;
            if (state != 0 && done == 0) {
                *(unsigned short *)(obj + 0xD0) = D_8009E4DB[12];
                D_8009E4DB[12] = 1;
                D_8009E4DB[15] = 1;
                if (PL(0x9E) == 0 || PL(0xAC) < 2) {
                    PL(4) = 5;
                    PL(5) = 0;
                    PL(6) = 0;
                    PL(7) = 0;
                } else if (D_8009E4DB[13] == 0) {
                    D_8009E4DB[11] = 1;
                }
                *(unsigned short *)(obj + 0xCE) = state;
                done = 1;
            } else {
                *(unsigned short *)(obj + 0xCE) = 0;
            }
            if (event_id == 0xF && completed == 1 && once == 0) {
                once = 1;
                *(unsigned short *)(obj + 0xD2) = 0x27;
                *(unsigned short *)(obj + 0xCE) = 2;
            } else {
                *(unsigned short *)(obj + 0xD2) = 0;
            }
        }
    }
}
#endif
