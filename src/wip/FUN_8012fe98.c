// FUNC 8012fe98 116 X000
#include "raw7.h"
extern void f(void *);
void FUN_8012fe98(char *o)
{
    char *p;
    int i;
    if (U8(o, 3) == 0) {
        i = 1;
        f(o);
        p = PTR(o, 0x94);
        do {
            f(p);
            i++;
            U8(p, 6) = U8(o, 6);
            p = PTR(p, 0x94);
        } while (i < 3);
    }
}
