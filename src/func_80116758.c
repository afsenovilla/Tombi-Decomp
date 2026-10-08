// FUNC 80116758 192 X000
// MATCHING 80116758 192
typedef struct { char p[2]; unsigned short s2; } P;
typedef struct { char p[0x32]; short w32; P *p34; } S;
extern unsigned short D_8009C962;
extern unsigned char D_8009CE3D;
void func_80116758(S *o)
{
    short t;
    if (D_8009C962 == 1) {
        t = o->p34->s2 - 0x948;
        if ((unsigned short)t >= 0x69) {
            if (t >= 0) o->w32 = -0x154;
        } else if (t != 0) {
            o->w32 += -(t * 190) / 104;
        }
    } else if (D_8009C962 == 5) {
        if (D_8009CE3D == 0xff) o->w32 = -0x54;
    }
}
