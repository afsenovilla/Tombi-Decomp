// FUNC 80068f7c 24 MAIN0
// MATCHING 80068f7c 24
// Portado de psx_tomba (psyq/libetc/vmode.c, SetVideoMode); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

extern int D_800981A0;
int StartPAD(void);
extern int EnterCriticalSection(void);
extern void ExitCriticalSection(void);
int ChangeClearRCnt(int, int);
int func_80069A9C(int, void*);
extern int D_8009B2E0[];
int func_800691C4(void);
typedef struct {
    u_char unk0[2];
    u_char unk2;
    u_char unk3;
    u_char unk4;
} padPort_act;
typedef struct padPort {
    int unk0;
    padPort_act* unk4;
    int unk8;
    struct padPort* unkC;
    struct padPort* unk10;
    int unk14;
    int unk18;
    char unk1C[0xC];
    u_char* unk28;
    u_char* unk2C;
    u_char* unk30;
    u_char unk34;
    u_char unk35;
    u_char unk36;
    u_char unk37;
    u_char unk38;
    u_char unk39;
    char unk3A[2];
    u_char* unk3C;
    u_char* unk40;
    char unk44;
    u_char unk45;
    u_char unk46;
    u_char unk47;
    u_char unk48;
    u_char unk49;
    char unk4A[7];
    u_char unk51[2];
    u_char unk53;
    char unk54[3];
    u_char unk57[6];
    u_char unk5D[6];
    char unk63[0x80];
    u_char unkE3;
    u_char unkE4;
    char unkE5;
    u_short unkE6;
    u_char unkE8;
    u_char unkE9;
    u_char unkEA;
    char unkEB[5];
} padPort;
extern padPort* D_80097544;
extern int D_8009755C;
typedef struct {
    u_long stat;
    u_long mask;
} padIntr;
extern void (*D_80097538)(void);
extern volatile padIntr* D_80097570;
typedef struct {
    u_long data;
    u_short stat;
    u_short unk6;
    u_short mode;
    u_short ctrl;
    u_short unkC;
    u_short baud;
} padSio;
int chkRC2wait(void);
extern volatile padSio* D_80097574;
void func_8006A378(padPort*);
void func_8006A38C(padPort*, int);
void func_8006A3CC(padPort*, int);
void func_8006A3AC(padPort*, int);
void func_8006A3EC(padPort*);
extern void (*D_80097514)(padPort*);
extern padPort D_8009B3A0[];

long SetVideoMode(long mode) {
    long prev = D_800981A0;
    D_800981A0 = mode;
    return prev;
}

int GetVideoMode(void);

int func_800689FC(void);

int PadInit(void);

void PadStop(void);

int PadChkMtap(int port);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068AA8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068B68);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068C60);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068D34);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068DDC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068E14);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068E5C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068EAC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80068FC4);

int func_80068FF0(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069058);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_800691C4);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", StartPAD);

void StopPAD(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_800692E8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_800694FC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_800695C4);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006979C);

int _padClrIntSio0(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069A60);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069A8C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069A9C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069AAC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069AB8);

void _padSendAtLoadInfo(padPort* port);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069B4C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069C98);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069CD0);

void func_80069DA4(padPort* port);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_80069E4C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A0C0);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A128);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A144);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A20C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A2A4);

int func_8006A2F8(padPort* port);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A358);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A378);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A38C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A3AC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A3CC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A3EC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A40C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A44C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A524);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A5E4);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A670);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006A9EC);

void func_8006AB4C(padPort* port);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006ABB4);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006ACA8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006ACB8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006AD74);

int func_8006AFF0(padPort* port);

padPort* func_8006B028(int port);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B04C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B080);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B154);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B3B8);

int func_8006B494(padPort* port);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B4CC);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", chkRC2wait);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B58C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", __main);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B6A4);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libetc/vmode", func_8006B70C);
