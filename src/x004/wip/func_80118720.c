// FUNC 80118720 464 X004
/* score 28: only register allocation differs (game: D base in t3, y/-y in t1/t2; ours D in t1). Tried: put() inline
   variants (arg order, idx/k params, row pointer), local p, x/y/z int/short types, v store permutations,
   pad sizes. Camera must be scalar externs + raw-offset stores (struct cam gets CSE'd into an la register).
   Also tried (o17): put(V3*,V4*) / scalar-arg / macro / (d,i,k) forms, nx/ny locals, y before x, v.z set once,
   v.y skipped when unchanged, all x/y/z store orders. Everything is one block (local-alloc only). */
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
    int z = z0 + 0x220;
    int x = z * 220 / 544;
    int y = z * 180 / 544;

    v.x = -x;
    v.y = -y;
    v.z = z;
    put(&D_80133ADC[idx][0], &v);
    v.x = x;
    v.y = -y;
    v.z = z;
    put(&D_80133ADC[idx][1], &v);
    v.x = -x;
    v.y = y;
    v.z = z;
    put(&D_80133ADC[idx][2], &v);
    v.x = x;
    v.y = y;
    v.z = z;
    put(&D_80133ADC[idx][3], &v);
}
