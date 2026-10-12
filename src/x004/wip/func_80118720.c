// FUNC 80118720 464 X004
/* score 25 (n72, was 28): only register allocation differs (game: y t1, -y t2, table base t3; ours base t2, -y t3).
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
   base+offsets). Camera must stay scalar externs + raw-offset stores (struct cam gets CSE'd into an la register). */
typedef struct { int x, y, z; } V3;
typedef struct { int x, y, z, pad; } V4;
extern V4 D_80133ADC[][4];
extern int D_1F8000D4, D_1F8000D8, D_1F8000DC;
static __inline__ void put(V4 *p, V3 *v)
{
    *(int *)((char *)p + 0) = v->x - D_1F8000D4;
    *(int *)((char *)p + 4) = v->y - D_1F8000D8;
    *(int *)((char *)p + 8) = v->z - D_1F8000DC;
}
void func_80118720(short z0, int idx)
{
    V3 v;
    int pad[3];
    int z, x, y, nx, ny;
    V4 (*tb)[4] = D_80133ADC;
    z = z0 + 0x220;
    x = z * 220 / 544;
    y = z * 180 / 544;
    nx = -x;
    ny = -y;
    v.x = nx;
    v.y = ny;
    v.z = z;
    put(&D_80133ADC[idx][0], &v);
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
