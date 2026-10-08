// FUNC 80116704 276 X000
// MATCHING 80116704 276
/* The first 84 bytes are the tail of the previous function's jump tables (jtbl_80116718 etc.),
   which splat merged into this entry; they are emitted as data before the code. */
__asm__(".word " "0x80137CA4, 0x80137D60, 0x80137DA4, 0x80137E40, 0x80137ED4, 0x80137984, 0x80137A20, 0x80137AF0, 0x80137BAC, 0x80137BF8, 0x80137BF8");
__asm__(".word " "0x80137BF8, 0x80137BF8, 0x80137BF8, 0x80137BF8, 0x80137C44, 0x80137C44, 0x80137C44, 0x80137C44, 0x80137C44, 0x80137C44");
typedef struct { char p[2]; unsigned short s2; } P;
typedef struct { char p[0x32]; short w32; P *p34; } S;
extern unsigned short D_8009C962;
extern unsigned char D_8009CE3D;
void func_80116704(S *o)
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
