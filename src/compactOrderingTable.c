// FUNC 80016fc0 124 MAIN0
// MATCHING 80016fc0 124
// Portado de psx_tomba (task.c, compactOrderingTable); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void compactOrderingTable(u_long* ot)
{
    s32     n;
    u_long* p;
    u_long* hole;
    u_long* next;
    u_long  val;
    u_long  cur;
    u_long  prev;
    int     differs;

    n = 0x327;
    hole = (u_long*)(((u_long)ot) & 0xFFFFFF);
    p = hole;

loop1:
    val = *p;
    prev = (u_long)(p - 1);
    if ((*p) == prev) {
        goto found;
    }
    val = (n--) == 0;
    if (val) {
        return;
    }
    p--;
    goto loop1;

found:
    hole = p;
    if ((n--) == 0) {
        return;
    }
    p++;
    p--;
    p--;

loop2:
    val = *p;
    cur = val;
    prev = (u_long)(p - 1);
    differs = cur != prev;
    next = (u_long*)val;
    if (differs) {
        goto link;
    }
    if ((n--) == 0) {
        return;
    }
    p = next;
    goto loop2;

link:
    *hole = (u_long)p;
    prev = (n--) == 0;
    if (prev) {
        return;
    }
    p--;
    goto loop1;
}
