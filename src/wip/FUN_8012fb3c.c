// FUNC 8012fb3c 116 X000
typedef struct O {
    char pad0[3];
    unsigned char sub;
    char pad1[2];
    unsigned char state;
    char pad2[0x94 - 7];
    struct O *next;
} O;
extern void FUN_8012fbb0();
extern void FUN_8012fe84();

void FUN_8012fb3c(O *o)
{
    O *p;
    int i;
    if (o->sub == 0) {
        i = 1;
        FUN_8012fbb0();
        p = o->next;
        do {
            FUN_8012fe84(p);
            i++;
            p->state = o->state;
            p = p->next;
        } while (i < 3);
    }
}
