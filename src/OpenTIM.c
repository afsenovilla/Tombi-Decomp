// FUNC 80061ce0 16 MAIN0
// MATCHING 80061ce0 16
// Portado de psx_tomba (psyq/libgpu/tmd.c, OpenTIM); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"

#define LOAD_SVECTOR_XYZ(dest, base, index, offset) do { \
    (dest).vx = (*(SVECTOR *)((index * 8) + base)).vx; \
    (dest).vy = (*(SVECTOR *)((index * 8) + base)).vy; \
    (dest).vz = (*(SVECTOR *)((index * 8) + base)).vz; \
} while(0)

typedef struct {
    int vert;
    int nvert;
    int norm;
    int nnorm;
    int prim;
    int nprim;
    int scaling;
} TmdObj;

typedef struct {
    int id;
    int flags;
    int nobj;
    TmdObj obj[1];
} TMD;

extern u32* D_8009BF28;
extern s32 D_8009B294;
extern s32 D_8009B298;
extern s32 D_8009B29C;
extern s32 D_8009B2A0;

int OpenTIM(u_long* addr)
{
    D_8009BF28 = addr;
    return 0;
}

TIM_IMAGE* ReadTIM(TIM_IMAGE* timimg);

int OpenTMD(u_long* tmd, int obj_no);

TMD_PRIM* ReadTMD(TMD_PRIM* tmdprim);

s32 get_tim_addr(u32* timaddr, TIM_IMAGE* img);

u_long get_tmd_addr( TMD* tmd, int objid, u_long** t_prim, u_long** v_ofs, u_long** n_ofs);

s32 unpack_packet(PACKET* arg0, TMD_PRIM* arg1);
