// FUNC 8004c418 344 MAIN0
extern char DAT_800a6610[];

typedef struct E { char a; char pad[3]; char b; char pad2[0x1c-5]; char f; signed char g; } E;
typedef struct E3 { char a; char pad[3]; char b; char pad2[0x1c-5]; char f; signed char g; } E3;
extern E DAT_800b1828[];
extern E3 DAT_800a49a8[];
void FUN_8004c418(signed char *o)
{
    char *p, *q;
    E *e;
    E3 *e3;
    int i;
    p = DAT_800a6610;
    i = 0;
    q = p + 4;
    do {
        if (*p != 0 && (q[0x18] & 0x80) == 0 && o[0xe] + o[0xf] + (signed char)q[0x19] != 0) {
            *p = 2;
            *q = 3;
        }
        i++;
        q += 0xd4;
        p += 0xd4;
    } while (i < 200);
    i = 0;
    e = DAT_800b1828;
    do {
        if (e->a != 0 && (e->f & 0x80) == 0 && o[0xe] + o[0xf] + e->g != 0) {
            e->a = 2;
            e->b = 3;
        }
        i++;
        e = (E *)((char *)e + 0xd4);
    } while (i < 0x2d);
    i = 0;
    e3 = DAT_800a49a8;
    do {
        if (e3->a != 0 && (e3->f & 0x80) == 0 && o[0xe] + o[0xf] + e3->g != 0) {
            e3->a = 2;
            e3->b = 3;
        }
        i++;
        e3 = (E3 *)((char *)e3 + 0x6c);
    } while (i < 10);
}
