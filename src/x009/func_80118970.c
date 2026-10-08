// FUNC 80118970 272 X009
// MATCHING 80118970 272

typedef struct { short vx, vy, vz, pad; } SV;
typedef struct {
    unsigned char pad[0x24];
    unsigned short *anim;
    unsigned char pad28[0xb4 - 0x28];
    SV sv[4];
} P;
typedef struct { short v[8]; } E;

extern E D_8012AAC0[];

void func_80118970(P *o)
{
    o->sv[0].vx = D_8012AAC0[*o->anim].v[0];
    o->sv[0].vy = D_8012AAC0[*o->anim].v[1];
    o->sv[0].vz = 0;
    o->sv[1].vx = D_8012AAC0[*o->anim].v[2];
    o->sv[1].vy = D_8012AAC0[*o->anim].v[3];
    o->sv[1].vz = 0;
    o->sv[2].vx = D_8012AAC0[*o->anim].v[4];
    o->sv[2].vy = D_8012AAC0[*o->anim].v[5];
    o->sv[2].vz = 0;
    o->sv[3].vx = D_8012AAC0[*o->anim].v[6];
    o->sv[3].vy = D_8012AAC0[*o->anim].v[7];
    o->sv[3].vz = 0;
}
