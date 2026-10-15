// FUNC 80118720 464 X004
// MATCHING 80118720 464
#include "spad.h"
/* MATCH: tb pinned in $11 and assigned after the first put (la scheduled after sll a1).
   Old notes: score 9 (pin `register int ny asm("$10")`, was 25): registers now match; only the `la t3,D_80133ADC` sits 2 insns early (game: after `sll a1,a1,6`). With the pin, tried: tb assignment at every statement position, asm volatile("") at every line (best 11, la at block top), clobbers of every temp reg at every line (no gain), y also pinned (37/28), p=D[idx] pointer forms (113), idx*64 in a separate temp, CC gcc-2.8.1 (103), AS gnu (11).
   Older: score 25 (n72, was 28): only register allocation differs (game: y t1, -y t2, table base t3; ours base t2, -y t3).
   n72 findings: the table base `tb` as a multi-set pointer (`tb = D; ... tb = (V4(*)[4])((V4*)tb + 3)` for the last
   element, symbol form `&D_80133ADC[idx][k]` for the first three so cse keeps `addiu a2,tb,16/32`) is not boosted
   by sched1 and lands after the two mults (luid ~17), which already moves y to t1 (25). Diagnosis confirmed with
   crutches: `__asm__ volatile("")` right after `tb = D` (la pinned at block top) gives the game's registers (score 11,
   the la itself then stays at the top in sched2); `register V4 (*tb)[4] asm("$11")` gives 9 (la 2 insns early).
   Local-alloc qty priorities (floor_log2(refs)*refs/(death-birth), tied regs merged): base {la, +48 result} 7 refs,
   y {sra temp, y} 6 refs, -y 3 refs; the game needs y > -y > base, i.e. the base qty below 1*3/len(-y): with 7 refs
   only a la at the very top of the sched1 block (len > ~79) or <= 4 refs does it. The la moves to the top only if
   one of its consumers is placed before the mult chains (nothing natural found: all consumers are the boosted
   per-element addu/addiu). Also tried: int/short locals, nx/ny temps, all 6 store orders, p init before/after x/y,
   (V4*)p+3 / (char*)p+48 / &p[0][3] forms, p for element 0 only (31), p for all (110-136: cse then shares one
   base+offsets). Camera must stay scalar externs + raw-offset stores (struct cam gets CSE'd into an la register).
   n77: exact local-alloc numbers (sched1 order, 80 insns): tb 7 refs span 17..79 (pri .226), y 6 refs 24..76 (.23),
   ny 3 refs 27..43 (.1875): tb needs birth <= 4 or <= 5 refs. `do { z = z0 + 0x220; } while (0);` (debt) gives the
   game's registers (score 13) but pins la+addiu sp at the top in sched2 too (game: la after sll a1, as in the plain
   form). Also tried: do{}while(0) around every other statement, all statement orders with tb, multi-set/single-set
   p for elem0/1, inline put(p,x,y,z) with its own V3 (60), -x/-y without temps, types. */
typedef struct { int x, y, z; } V3;
typedef struct { int x, y, z, pad; } V4;
extern V4 D_80133ADC[][4];
static __inline__ void put(V4 *p, V3 *v)
{
    *(int *)((char *)p + 0) = v->x - SPAD.id4;
    *(int *)((char *)p + 4) = v->y - SPAD.id8;
    *(int *)((char *)p + 8) = v->z - SPAD.idc;
}
void area4ScriptOpSetQuad(short z0, int idx)
{
    V3 v;
    int pad[3];
    int z, x, y, nx; register int ny asm("$10");
    register V4 (*tb)[4] asm("$11");
    z = z0 + 0x220;
    x = z * 220 / 544;
    y = z * 180 / 544;
    nx = -x;
    ny = -y;
    v.x = nx;
    v.y = ny;
    v.z = z;
    put(&D_80133ADC[idx][0], &v);
    tb = D_80133ADC;
    v.x = x;
    v.y = ny;
    v.z = z;
    put(&D_80133ADC[idx][1], &v);
    v.x = nx;
    v.y = y;
    v.z = z;
    put(&D_80133ADC[idx][2], &v);
    v.x = x;
    v.y = y;
    v.z = z;
    tb = (V4 (*)[4])((V4 *)tb + 3);
    put(&tb[idx][0], &v);
}
