// FUNC 8010b784 252 X002
// MATCHING 8010b784 252
typedef struct N { unsigned char active; unsigned char b1; unsigned char type; char p[0x91]; struct N *next; } N;
extern N *D_8009F0EC;

void func_8010B784(void)
{
    N *p;
    switch (D_8009F0EC->type) {
    case 11:
        if (D_8009F0EC->next)
            D_8009F0EC->active = 3;
        else
            D_8009F0EC->active = 1;
    case 21:
        if (D_8009F0EC->next) {
            D_8009F0EC->active = 3;
            p = D_8009F0EC->next;
            if (!p->next) {
                p->active = 1;
            } else {
            loop:
                p->active = 3;
                p = p->next;
                if (p->next) goto loop;
                p->active = 1;
            }
        } else {
            D_8009F0EC->active = 1;
        }
        break;
    case 29:
    case 49:
        D_8009F0EC->active = 1;
        break;
    case 12:
    default:
        D_8009F0EC->active = 3;
        break;
    }
}
