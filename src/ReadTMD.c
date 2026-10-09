// FUNC 80061d98 684 MAIN0
// MATCHING 80061d98 684
// Portado de psx_tomba (psyq/libgpu/tmd.c, ReadTMD); licencia MIT del proyecto original.
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

extern u32* D_8009B290;
extern s32 D_8009BF2C;
extern s32 D_8009BF30;
extern s32 D_8009BF34;
extern s32 D_8009B2A0;

int OpenTIM(u_long* addr);

TIM_IMAGE* ReadTIM(TIM_IMAGE* timimg);

int OpenTMD(u_long* tmd, int obj_no);

TMD_PRIM* ReadTMD(TMD_PRIM* tmdprim)
{
    s32 packet;
    packet = unpack_packet((u8* ) D_8009BF34, tmdprim);
    if (packet < 0) return 0;
    
    D_8009BF34 += packet;
    tmdprim->v_ofs = (SVECTOR*)(D_8009BF2C);
    tmdprim->n_ofs = (SVECTOR*)(D_8009BF30);
    
    LOAD_SVECTOR_XYZ(tmdprim->n0, D_8009BF30, tmdprim->norm0, 0);
    LOAD_SVECTOR_XYZ(tmdprim->n1, D_8009BF30, tmdprim->norm1, 0);
    LOAD_SVECTOR_XYZ(tmdprim->n2, D_8009BF30, tmdprim->norm2, 0);
    LOAD_SVECTOR_XYZ(tmdprim->n3, D_8009BF30, tmdprim->norm3, 0);
    LOAD_SVECTOR_XYZ(tmdprim->x0, D_8009BF2C, tmdprim->vert0, 0);
    LOAD_SVECTOR_XYZ(tmdprim->x1, D_8009BF2C, tmdprim->vert1, 0);
    LOAD_SVECTOR_XYZ(tmdprim->x2, D_8009BF2C, tmdprim->vert2, 0);
    LOAD_SVECTOR_XYZ(tmdprim->x3, D_8009BF2C, tmdprim->vert3, 0);
    return tmdprim;
}

s32 get_tim_addr(u32* timaddr, TIM_IMAGE* img);

u_long get_tmd_addr( TMD* tmd, int objid, u_long** t_prim, u_long** v_ofs, u_long** n_ofs);

s32 unpack_packet(PACKET* arg0, TMD_PRIM* arg1);
