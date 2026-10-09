// FUNC 80062164 408 MAIN0
// MATCHING 80062164 408
// Portado de psx_tomba (psyq/libgpu/tmd.c, get_tmd_addr); licencia MIT del proyecto original.
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
extern s32 D_8009B294;
extern s32 D_8009B298;
extern s32 D_8009B29C;
extern s32 D_8009B2A0;

int OpenTIM(u_long* addr);

TIM_IMAGE* ReadTIM(TIM_IMAGE* timimg);

int OpenTMD(u_long* tmd, int obj_no);

TMD_PRIM* ReadTMD(TMD_PRIM* tmdprim);

s32 get_tim_addr(u32* timaddr, TIM_IMAGE* img);

u_long get_tmd_addr(
    TMD* tmd, int objid, u_long** t_prim, u_long** v_ofs, u_long** n_ofs) {
    TmdObj* obj = tmd->obj;
    if (GetGraphDebug() == 2) {
        printf("analizing TMD...\n");
    }
    if (GetGraphDebug() == 2) {
        printf("\tid=%08X, flags=%d, nobj=%d, objid=%d\n", tmd->id, tmd->flags,
               tmd->nobj, objid);
    }
    if (GetGraphDebug() == 2) {
        printf("\tvert=%08X, nvert=%d\n", obj[objid].vert, obj[objid].nvert);
    }
    if (GetGraphDebug() == 2) {
        printf("\tnorm=%08X, nnorm=%d\n", obj[objid].norm, obj[objid].nnorm);
    }
    if (GetGraphDebug() == 2) {
        printf("\tprim=%08X, nprim=%d\n", obj[objid].prim, obj[objid].nprim);
    }
    *v_ofs = (u_long*)((unsigned char*)obj + obj[objid].vert);
    *n_ofs = (u_long*)((unsigned char*)obj + obj[objid].norm);
    *t_prim = (u_long*)((unsigned char*)obj + obj[objid].prim);
    return obj[objid].nprim;
}

s32 unpack_packet(PACKET* arg0, TMD_PRIM* arg1);
