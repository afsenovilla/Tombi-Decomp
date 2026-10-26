// FUNC 8010b784 252 X003
// MATCHING 8010b784 252
typedef struct N { unsigned char active; unsigned char b1; unsigned char type; char p[0x91]; struct N *next; } N;
extern N *GRABBED_OBJECT;

void playerRestoreClingObjectActive(void)
{
    N *p;
    switch (GRABBED_OBJECT->type) {
    case 11:
        if (GRABBED_OBJECT->next)
            GRABBED_OBJECT->active = 3;
        else
            GRABBED_OBJECT->active = 1;
    case 21:
        if (GRABBED_OBJECT->next) {
            GRABBED_OBJECT->active = 3;
            p = GRABBED_OBJECT->next;
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
            GRABBED_OBJECT->active = 1;
        }
        break;
    case 29:
    case 49:
        GRABBED_OBJECT->active = 1;
        break;
    case 12:
    default:
        GRABBED_OBJECT->active = 3;
        break;
    }
}
