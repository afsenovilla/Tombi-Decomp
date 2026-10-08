// FUNC 80020828 592 MAIN0
// MATCHING 80020828 592
typedef struct { unsigned char act; char p01[0xf]; char a10[8]; char a18[0x28]; void *p40; void *p44; char rest[0xec - 0x48]; } S1;
typedef struct { unsigned char act; char p01[0xf]; char a10[8]; char a18[0x28]; void *p40; void *p44; char rest[0xd4 - 0x48]; } S2;
typedef struct { unsigned char act; char p01[0xf]; char a10[8]; char a18[0x28]; void *p40; void *p44; char rest[0x6c - 0x48]; } S3;
extern unsigned short D_1f8001c8;
extern short D_1f80019c;
extern char D_800A6038[];
extern char D_1F8000EC[];
extern void *D_800A6078, *D_800A607C, *D_800A4584, *D_800A4588;
extern S1 D_800B1478[];
extern S2 D_800A6610[];
extern S3 D_800A49A8[];
void func_80020828(void)
{
    S2 *e2;
    S3 *e3;
    S1 *e1;
    char *b = D_800A6038;
    if (!(D_1f8001c8 & 1)) {
        D_800A6078 = b + 0x10;
        D_800A607C = b + 0x18;
    } else {
        D_800A607C = b + 0x10;
        D_800A6078 = b + 0x18;
    }
    if (!(D_1f8001c8 & 1)) {
        D_800A4584 = D_1F8000EC;
        D_800A4588 = D_1F8000EC + 8;
    } else {
        D_800A4588 = D_1F8000EC;
        D_800A4584 = D_1F8000EC + 8;
    }
    e1 = D_800B1478;
    for (D_1f80019c = 0; D_1f80019c < 4; D_1f80019c++, e1++) {
        if (e1->act) {
            if (!(D_1f8001c8 & 1)) {
                e1->p40 = e1->a10;
                e1->p44 = e1->a18;
            } else {
                e1->p44 = e1->a10;
                e1->p40 = e1->a18;
            }
        }
    }
    e2 = D_800A6610;
    for (D_1f80019c = 0; D_1f80019c < 200; D_1f80019c++, e2++) {
        if (e2->act) {
            if (!(D_1f8001c8 & 1)) {
                e2->p40 = e2->a10;
                e2->p44 = e2->a18;
            } else {
                e2->p44 = e2->a10;
                e2->p40 = e2->a18;
            }
        }
    }
    e3 = D_800A49A8;
    for (D_1f80019c = 0; D_1f80019c < 10; D_1f80019c++, e3++) {
        if (e3->act) {
            if (!(D_1f8001c8 & 1)) {
                e3->p40 = e3->a10;
                e3->p44 = e3->a18;
            } else {
                e3->p44 = e3->a10;
                e3->p40 = e3->a18;
            }
        }
    }
}
