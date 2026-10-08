// FUNC 8004dc10 380 MAIN0
/* k es unsigned short (copia s3), o=s1 y k=s0 ya coinciden. Falta: DAT_800a4648 sale con la (direccion en registro),
   extension de n antes de la llamada (sra s2,16) y la cola comun del store a 4de6 (cross-jump). */
typedef struct { char p[0xc2]; unsigned short w2, w4; char q[4]; unsigned short idx; char r[2]; unsigned short wce, wd0; } O;
extern short DAT_800a4648[];
extern char DAT_800a4de0[], DAT_800a4de4[], DAT_800a4de6[];
extern unsigned char DAT_800a5de6[];
extern int FUN_8002df70(int, int, int);

void FUN_8004dc10(O *o, unsigned short k)
{
    short n;
    int r;
    n = DAT_800a4648[o->idx * 4]++;
    r = FUN_8002df70(o->idx, k & 0xfff, 0);
    if ((k & 0x7000) == 0x4000) {
        *(unsigned short *)(DAT_800a4de0 + (n * 8 + (o->idx << 10))) = r | 0x4000;
        *(short *)(DAT_800a4de4 + (n * 8 + (o->idx << 10))) = o->w2 - 0x10;
        *(short *)(DAT_800a4de6 + (n * 8 + (o->idx << 10))) = o->w4 - 0x10;
    } else {
        *(unsigned short *)(DAT_800a4de0 + (n * 8 + (o->idx << 10))) = r;
        *(short *)(DAT_800a4de4 + (n * 8 + (o->idx << 10))) = o->wce;
        *(short *)(DAT_800a4de6 + (n * 8 + (o->idx << 10))) = o->wd0;
    }
    if ((k & 0x7000) == 0x5000 || (k & 0x7000) != 0x6000)
        o->wce += DAT_800a5de6[r * 10];
}
