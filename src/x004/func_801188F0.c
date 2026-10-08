// FUNC 801188f0 780 X004
// MATCHING 801188f0 780
typedef struct { int vx, vy, vz, pad; } VEC;
extern VEC D_80133ADC[4];
extern VEC D_80133B1C[4];

#define XAT(i) (D_80133ADC[i].vx + (D_80133ADC[i].vx - D_80133B1C[i].vx) * (p->vz - D_80133ADC[i].vz) / (D_80133ADC[i].vz - D_80133B1C[i].vz))

int func_801188F0(VEC *p)
{
    VEC v;

    if (p->vz + 45 < D_80133ADC[0].vz && p->vz + 45 < D_80133ADC[1].vz
        && p->vz + 45 < D_80133ADC[2].vz && p->vz + 45 < D_80133ADC[3].vz)
        return 0;
    if (D_80133B1C[0].vz < p->vz - 45 && D_80133B1C[1].vz < p->vz - 45
        && D_80133B1C[2].vz < p->vz - 45 && D_80133B1C[3].vz < p->vz - 45)
        return 0;
    v.vx = XAT(0);
    if (p->vx + 45 < v.vx) {
        v.vx = XAT(2);
        if (p->vx + 45 < v.vx)
            return 0;
    }
    v.vx = XAT(1);
    if (v.vx < p->vx - 45) {
        v.vx = XAT(3);
        if (v.vx < p->vx - 45)
            return 0;
    }
    return 1;
}
