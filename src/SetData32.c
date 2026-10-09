// FUNC 80063f78 12 MAIN0
// MATCHING 80063f78 12
// Portado de psx_tomba (psyq/libgte/mtx_12.c, SetData32); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libgte.h"
#include "psyq/gtemac.h"
#include "psyq/inline_c.h"

#define gte_SetTransMatrix(r0)                                                 \
    __asm__ volatile(                                                          \
        "lw	$8, 20( %0 );"                                                     \
        "lw	$9, 24( %0 );"                                                     \
        "lw	$10, 28( %0 );"                                                    \
        "ctc2	$8, $5;"                                                       \
        "ctc2	$9, $6;"                                                       \
        "ctc2	$10, $7"                                                       \
        :                                                                      \
        : "r"(r0)                                                              \
        : "$8", "$9", "$10")
#define gte_ldrgb1(r0) __asm__ volatile("lwc2	$20, 0( %0 )" : : "r"(r0))
#define gte_ldrgb2(r0) __asm__ volatile("lwc2	$21, 0( %0 )" : : "r"(r0))
#define gte_ldrgb3(r0) __asm__ volatile("lwc2	$22, 0( %0 )" : : "r"(r0))
#define gte_ldsv_(r0, r1, r2)                                                  \
    __asm__ volatile("mtc2	%0, $9;"                                           \
                     "mtc2	%1, $10;"                                          \
                     "mtc2	%2, $11"                                           \
                     :                                                         \
                     : "r"(r0), "r"(r1), "r"(r2))
#define gte_SetRii(r0, r1, r2)                                                \
    __asm__ volatile("ctc2	%0, $0;"                                           \
                     "ctc2	%1, $2;"                                           \
                     "ctc2	%2, $4"                                            \
                     :                                                         \
                     : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldMAC1(r0) __asm__ volatile("mtc2	%0, $25" : : "r"(r0))
#define gte_ldMAC2(r0) __asm__ volatile("mtc2	%0, $26" : : "r"(r0))
#define gte_ldMAC3(r0) __asm__ volatile("mtc2	%0, $27" : : "r"(r0))
#define gte_ldDQA(r0) __asm__ volatile("ctc2	%0, $27" : : "r"(r0))
#define gte_ldDQB(r0) __asm__ volatile("ctc2	%0, $28" : : "r"(r0))

void SetTransMatrix(MATRIX* m);

void SetVertex0(SVECTOR* r0);

void SetVertex1(SVECTOR* r0);

void SetVertex2(SVECTOR* r0);

void SetVertexTri(SVECTOR* r0, SVECTOR* r1, SVECTOR* r2);

void SetRGBfifo(int r0, int r1, int r2);

void SetIR123(int r0, int r1, int r2);

void SetIR0(int r0);

void SetSZfifo3(int r0, int r1, int r2);

void SetSZfifo4(int r0, int r1, int r2, int r3);

void SetSXSYfifo(int r0, int r1, int r2);

void SetRii(int r0, int r1, int r2);

void SetMAC123(int r0, int r1, int r2);

void SetData32(int r0)
{
    gte_ldlzc(r0);
}

void SetDQA(int r0);

void SetDQB(int r0);
