// VERSION SLPS_01144 dspsub 800EB350 256
#if defined(VERSION_SLPS_01144)
// JP only: picks the frame pair of sub-screen entry k (second table when its bit of D_8009EE6B is set)
extern unsigned char *D_80105688[], *D_801056B0[];
extern unsigned char D_8009EE6B;
extern unsigned char D_80111400, D_80111404;

void func_800EB350(int k, int j)
{
    unsigned char *p = D_80105688[k];
    int f;
    switch (k) {
    case 1:
        f = D_8009EE6B & 1;
        break;
    case 3:
        if (j == 1) f = D_8009EE6B & 4;
        else f = D_8009EE6B & 2;
        break;
    case 4:
        f = D_8009EE6B & 0x10;
        break;
    case 5:
        f = D_8009EE6B & 8;
        break;
    case 6:
        f = D_8009EE6B & 0x20;
        break;
    case 7:
        f = D_8009EE6B & 0x80;
        break;
    case 8:
        f = D_8009EE6B & 0x40;
        break;
    default:
        goto done;
    }
    if (f) p = D_801056B0[k];
done:
    p += j * 2;
    D_80111400 = p[0];
    D_80111404 = p[1];
}
#endif
