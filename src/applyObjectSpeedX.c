// FUNC 8001fd14 28 MAIN0
// MATCHING 8001fd14 28
// Ported from psx_tomba (cdfile.c, applyObjectSpeedX); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//#include "psyq/libcd.h"

s32 fixedMulSin(s16 arg0, s16 arg1);
s32 fixedMulCos(s16 arg0, s16 arg1);
s32 fixedMulSin2(s16 arg0, s16 arg1);
s32 fixedMulCos2(s16 arg0, s16 arg1);


// #define CD_REQ   (*(u8**)0x1F800288)   /* entrada corrente da fila */
// #define CD_HDR   (*(s16**)0x1F80028C)  /* cabecalho do arquivo */
// #define CD_DST   (*(u8**)0x1F800290)   /* destino da leitura */
// #define CD_NSEC  (*(s32*)0x1F800294)   /* setores a ler */
// #define CD_HEAD  (*(s32*)0x1F80029C)
// #define CD_TAIL  (*(s32*)0x1F8002A0)
// #define CD_FLAGS (*(s32*)((u8*)CD_HDR + 0x10))

// void func_80021340(void)
// {
//     RECT    rect;
//     u_char  param[40];
//     s32     i;
//     s32     r;
//     s32     base;
//     s32     nvag;
//     s32     nprog;
//     s32     tail;
//     s32     head;
//     s32     id;
//     s32     flags;
//     u32     spuAddr;
//     u32     len;
//     u8*     vagTop;
//     u8*     progTop;
//     u8*     vagCur;
//     u8*     progCur;
//     u8*     a;
//     u8*     b;
//     u8*     entry;
//     s16*    hdr;
//     s16     vabId;

//     LOAD_COMPLETE = 0;
//     D_8009C8B0 = 0;
//     param[0] = 0x80;
//     CURRENT_TASK->state0 = 0;
//     CURRENT_TASK->state1 = 0;
//     while (CdControl(14, param, 0) == 0) {
//     }

//     for (;;) {
//         switch ((u16)CURRENT_TASK->state0) {
//         case 0:
//             if (CD_HEAD == CD_TAIL) {
//                 break;
//             }
//             CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//             break;

//         case 1:
//             entry = (u8*)(&D_8009E748 + CD_TAIL * 2);
//             hdr = *(s16**)entry;
//             id = *hdr;
//             CD_REQ = entry;
//             CD_HDR = hdr;
//             CdControlF(2, (u8*)&D_800791A0 + (id * 8));
//             CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//             break;

//         case 2:
//             r = CdSync(1, 0);
//             if (r == 2) {
//                 CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//             } else if (r == 5) {
//                 CURRENT_TASK->state0 = 1;
//                 D_8009C8B0++;
//             }
//             break;

//         case 3:
//             flags = CD_FLAGS;
//             CD_NSEC = (u32)(D_800791A4[*CD_HDR * 2] + 0x7FF) >> 11;
//             switch (flags & 0xF) {
//             case 0:
//             case 3:
//                 if (CD_FLAGS & 0x10) {
//                     CD_DST = (u8*)&D_800B3188;
//                 } else {
//                     CD_DST = D_800A3348;
//                 }
//                 break;
//             case 1:
//             case 2:
//             case 4:
//                 CD_DST = *(u8**)(CD_REQ + 4);
//                 break;
//             }
//             if (CdRead(CD_NSEC, (u_long*)CD_DST, 0x80) == 0) {
//                 D_8009C8B0++;
//             } else {
//                 CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//             }
//             break;

//         case 4:
//             r = CdReadSync(1, 0);
//             if (r == 0) {
//                 CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//             } else if (r == -1) {
//                 CURRENT_TASK->state0 = 1;
//                 D_8009C8B0++;
//             }
//             break;

//         case 5:
//             switch ((u16)CURRENT_TASK->state1) {
//             case 0:
//                 switch (CD_FLAGS & 0xF) {
//                 case 0:
//                     switch (*((u8*)CD_HDR + 3) & 0xF0) {
//                     case 0x10:
//                         CURRENT_TASK->state1 = 2;
//                         break;
//                     case 0x90:
//                         CURRENT_TASK->state1 = 5;
//                         break;
//                     }
//                     break;
//                 case 1:
//                     CURRENT_TASK->state1 = (u16)CURRENT_TASK->state1 + 1;
//                     break;
//                 case 2:
//                 case 4:
//                     if ((*((u8*)CD_HDR + 3) & 0xF0) == 0x90) {
//                         CURRENT_TASK->state1 = 5;
//                         break;
//                     }
//                     CURRENT_TASK->state1 = 0;
//                     CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//                     break;
//                 case 3:
//                     CURRENT_TASK->state1 = 4;
//                     break;
//                 }
//                 break;

//             case 1:
//                 if (CD_FLAGS & 0x10) {
//                     lzDecompress(*(u8**)(CD_REQ + 4), (u8*)&D_800B3188);
//                 } else {
//                     lzDecompress(*(u8**)(CD_REQ + 4), D_800A3348);
//                 }
//                 CURRENT_TASK->state1 = (u16)CURRENT_TASK->state1 + 1;
//                 /* fallthrough */
//             case 2:
//                 rect.x = CD_HDR[4];
//                 rect.y = CD_HDR[5];
//                 rect.w = CD_HDR[6];
//                 rect.h = CD_HDR[7];
//                 if (CD_FLAGS & 0x10) {
//                     LoadImage(&rect, (u_long*)&D_800B3188);
//                 } else {
//                     LoadImage(&rect, (u_long*)D_800A3348);
//                 }
//                 CURRENT_TASK->state1 = (u16)CURRENT_TASK->state1 + 1;
//                 break;

//             case 3:
//                 DrawSync(0);
//                 CURRENT_TASK->state1 = 0;
//                 CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//                 break;

//             case 4:
//                 if (CD_FLAGS & 0x10) {
//                     lzDecompress((u8*)&D_800B3188, *(u8**)(CD_REQ + 4));
//                 } else {
//                     lzDecompress(D_800A3348, *(u8**)(CD_REQ + 4));
//                 }
//                 CURRENT_TASK->state1 = 0;
//                 CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//                 break;

//             case 5:
//                 base = *((u8*)CD_HDR + 3) & 0xF;
//                 if (base == 0xF) {
//                     base = 0;
//                 }
//                 a = *(u8**)(CD_REQ + 4);
//                 vagTop = a + *(s32*)(a + 4);
//                 progTop = a + *(s32*)a;
//                 len = (u32)*(s32*)progTop;
//                 vagCur = vagTop;
//                 progCur = progTop;
//                 nvag = ((u32)*(s32*)vagCur >> 2) - 1;
//                 nprog = (len >> 2) - 1;

//                 i = 0;
//                 if (nvag > 0) {
//                     do {
//                         vabId = D_1F8003A8[base + i];
//                         if (vabId != -1) {
//                             SsVabClose(vabId);
//                         }
//                         i++;
//                     } while (i < nvag);
//                 }

//                 i = 0;
//                 if (nvag > 0) {
//                     do {
//                         D_8009C758[base + i] = (s32)(vagTop + *(s32*)vagCur);
//                         i++;
//                         vagCur += 4;
//                     } while (i < nvag);
//                 }

//                 i = 0;
//                 if (nprog > 0) {
//                     do {
//                         D_8009C658[base + i] = (s32)(progTop + *(s32*)progCur);
//                         i++;
//                         progCur += 4;
//                     } while (i < nprog);
//                 }

//                 spuAddr = D_80077D50[CD_HDR[7] * 2];
//                 i = 0;
//                 if (nvag > 0) {
//                     do {
//                         if (i == (nvag - 1)) {
//                             a = (u8*)D_8009C658[base + i];
//                             b = progTop + *(s32*)progCur;
//                         } else {
//                             b = (u8*)D_8009C65C[base + i];
//                             a = (u8*)D_8009C658[base + i];
//                         }
//                         len = b - a;
//                         SpuSetTransferStartAddr(spuAddr);
//                         SpuRead((u_char*)D_8009C658[base + i], len);
//                         SpuIsTransferCompleted(1);
//                         vabId = SsVabFakeHead((u_char*)D_8009C758[base + i], -1, spuAddr);
//                         spuAddr += len;
//                         D_1F8003A8[base + i] = vabId;
//                         SsVabFakeBody(vabId);
//                         i++;
//                     } while (i < nvag);
//                 }
//                 CURRENT_TASK->state1 = 0;
//                 CURRENT_TASK->state0 = (u16)CURRENT_TASK->state0 + 1;
//                 break;
//             }
//             break;

//         case 6:
//             tail = CD_TAIL;
//             head = CD_HEAD;
//             base = (s32)&CD_TAIL;
//             tail = (tail + 1) & 0x7F;
//             *(s32*)base = tail;
//             if (head == tail) {
//                 LOAD_COMPLETE = 1;
//                 exitTask();
//             } else {
//                 CURRENT_TASK->state0 = 1;
//                 CURRENT_TASK->state1 = 0;
//             }
//             break;
//         }
//         sleepTask(1);
//     }
// }


//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/system/cdfile", stubCdFunction);




























//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/system/cdfile", applyObjectSpeedVertical);

void applyObjectSpeedX(u8* self)
{
    s32* p = *(s32**)(self + 0x40);

    *p += *(s16*)(self + 0x80) << 8;
}
