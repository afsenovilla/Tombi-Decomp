// FUNC 80047484 1464 MAIN0
/* score 16: only s3/s4 swapped (game: s3 = hoisted constant 2, s4 = list pointer r). The game hoists BOTH 1 and 2 out of the loop; plain constants only hoist 1 (score 185), so `two` is a local set at the loop top. Tried: decl order/types of two, two set before the loop, r as int/index, *r++, for/do-while loop forms, one+two locals (40), register asm (8 but no hoist), if-chains for the nested switches. */
#include "TOBJ.H"
extern unsigned char D_8009C93A;
extern unsigned short D_8009C960;
extern short D_1F800246;
extern unsigned char **D_1F80021C;
extern short D_1F80019E;
extern short func_8004306C(TObj *, unsigned char *);
extern void FUN_80045684(TObj *, unsigned char *);
extern void FUN_80045800(TObj *, unsigned char *);
extern void func_80043260(TObj *, unsigned char *);
extern void func_8011C80C(TObj *, unsigned char *);
extern void func_8011C8C0(TObj *, unsigned char *);
extern void func_8011CA60(TObj *, unsigned char *);
extern void func_8011CC1C(TObj *, unsigned char *);
extern void func_8011CD6C(TObj *, unsigned char *);
extern void func_8011CE64(TObj *, unsigned char *);
extern void func_8011CF2C(TObj *, unsigned char *);
extern void func_8011D040(TObj *, unsigned char *);
extern void func_8011D138(TObj *, unsigned char *);
extern void func_8011DC34(TObj *, unsigned char *);
extern void func_8011DE58(TObj *, unsigned char *);
extern void func_8011E3B4(TObj *, unsigned char *);
extern void func_8011EFA4(TObj *, unsigned char *);
extern void func_8011F134(TObj *, unsigned char *);
extern void func_8011F508(TObj *, unsigned char *);
extern void func_8011F634(TObj *, unsigned char *);
extern void func_8011F6C8(TObj *, unsigned char *);
extern void func_8011F8CC(TObj *, unsigned char *);
extern void func_8011F9E0(TObj *, unsigned char *);
extern void func_8011FAD4(TObj *, unsigned char *);
extern void func_8011FC84(TObj *, unsigned char *);
extern void func_8011FD20(TObj *, unsigned char *);
extern void func_80120140(TObj *, unsigned char *);
extern void func_801203CC(TObj *, unsigned char *);
extern void func_801206F4(TObj *, unsigned char *);
extern void func_8012095C(TObj *, unsigned char *);
extern void func_80120C68(TObj *, unsigned char *);
extern void func_80120F34(TObj *, unsigned char *);
extern void func_80121180(TObj *, unsigned char *);
extern void func_801240A8(TObj *, unsigned char *);
extern void func_801242C0(TObj *, unsigned char *);
extern void func_80124348(TObj *, unsigned char *);
extern void func_801243D4(TObj *, unsigned char *);
extern void func_80124410(TObj *, unsigned char *);
extern void func_8012555C(TObj *, unsigned char *);
extern void func_801257F4(TObj *, unsigned char *);
extern void func_80125F30(TObj *, unsigned char *);
extern void func_80126010(TObj *, unsigned char *);
extern void func_801260F8(TObj *, unsigned char *);
extern void func_80126198(TObj *, unsigned char *);
extern void func_80126240(TObj *, unsigned char *);
extern void func_801262F8(TObj *, unsigned char *);
extern void func_80126464(TObj *, unsigned char *);
extern void func_80126564(TObj *, unsigned char *);
extern void func_80126698(TObj *, unsigned char *);
extern void func_8012687C(TObj *, unsigned char *);
extern void func_80126A70(TObj *, unsigned char *);
extern void func_80126BBC(TObj *, unsigned char *);

void FUN_80047484(TObj *o)
{
    unsigned char **r;
    unsigned char *q;
    short v;
    int two;

    if (D_8009C93A == 0) return;
    r = D_1F80021C;
    D_1F80019E = D_1F800246;
    while (D_1F80019E != 0) {
        q = *r;
        two = 2;
        D_1F80019E = D_1F80019E - 1;
        r++;
        if (q[0] & 1) {
            switch (q[2]) {
            case 0:
                func_801262F8(o, q);
                break;
            case 1:
                func_80125F30(o, q);
                break;
            case 2:
                switch (D_8009C960) {
                case 0: func_801257F4(o, q); break;
                case 1: func_801240A8(o, q); break;
                case 3: func_8011F8CC(o, q); break;
                case 4: func_8011EFA4(o, q); break;
                }
                break;
            case 4:
            case 0x1b:
                v = func_8004306C(o, q);
                if (v != 0) {
                    if (*(unsigned char *)&o->wac == 1 && v == 1) {
                        q[0] = two;
                        q[4] = two;
                        q[5] = 1;
                        q[6] = 0;
                        q[0x69] = 0;
                        *(unsigned char **)((char *)o + 0xe4) = q;
                        *(unsigned char *)&o->wac = two;
                    }
                    D_1F80019E = 0;
                }
                break;
            case 3:
            case 0x12:
                func_801260F8(o, q);
                break;
            case 6:
                func_80126240(o, q);
                break;
            case 7:
                switch (D_8009C960) {
                case 0: func_8012555C(o, q); break;
                case 4: func_8011F134(o, q); break;
                case 10: func_8012095C(o, q); break;
                }
                break;
            case 8:
                func_80126464(o, q);
                break;
            case 0x1f:
                if (D_8009C960 == 1) func_80124410(o, q);
                else func_8011DE58(o, q);
                break;
            case 9:
                func_80126698(o, q);
                break;
            case 10:
                func_80120C68(o, q);
                break;
            case 0xb:
                func_8012687C(o, q);
                break;
            case 0xe:
                FUN_80045684(o, q);
                break;
            case 0xf:
                func_80126A70(o, q);
                break;
            case 0x11:
                func_80126BBC(o, q);
                break;
            case 0x15:
                func_80126198(o, q);
                break;
            case 0x13:
                func_80126564(o, q);
                break;
            case 0x1a:
                func_801203CC(o, q);
                break;
            case 0x1c:
                func_80121180(o, q);
                break;
            case 0x17:
                func_80124348(o, q);
                break;
            case 0x16:
                func_801242C0(o, q);
                break;
            case 0x1d:
                func_8011FD20(o, q);
                break;
            case 0x1e:
                func_8011F6C8(o, q);
                break;
            case 0x21:
                func_80120F34(o, q);
                break;
            case 0x22:
                func_801243D4(o, q);
                break;
            case 0x28:
                func_80120140(o, q);
                break;
            case 0x29:
                func_8011F9E0(o, q);
                break;
            case 0x2b:
                func_80126010(o, q);
                break;
            case 0x24:
                func_8011FC84(o, q);
                break;
            case 0x37:
                func_8011FAD4(o, q);
                break;
            case 0x38:
                func_8011F634(o, q);
                break;
            case 0x39:
                func_8011F508(o, q);
                break;
            case 0x3a:
                func_801206F4(o, q);
                break;
            case 0x3b:
            case 0x3c:
            case 0x3d:
            case 0x3e:
            case 0x3f:
            case 0x40:
            case 0x41:
            case 0x42:
                func_8011D138(o, q);
                break;
            case 0x43:
                func_8011D040(o, q);
                break;
            case 0x46:
                func_8011C80C(o, q);
                break;
            case 0x47:
                func_8011CA60(o, q);
                break;
            case 0x48:
                func_8011CC1C(o, q);
                break;
            case 0x49:
                func_8011CD6C(o, q);
                break;
            case 0x4b:
                func_8011CE64(o, q);
                break;
            case 0x18:
            case 0x23:
            case 0x32:
            case 0x4e:
            case 0x55:
            case 0x58:
            case 0x59:
            case 0x5a:
                FUN_80045800(o, q);
                break;
            case 0x4f:
                func_8011CF2C(o, q);
                break;
            case 0x50:
                func_8011C8C0(o, q);
                break;
            case 0x53:
                func_8011E3B4(o, q);
                break;
            case 0x27:
                func_8011DC34(o, q);
                break;
            case 0x57:
                func_80043260(o, q);
                break;
            }
        }
    }
}
