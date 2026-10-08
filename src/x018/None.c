// FUNC 8011657c 44 X018
// MATCHING 8011657c 44
typedef struct { char p[3]; unsigned char b3; int d4; } S;
extern unsigned short D_8009C962;

void func_8011657C(S *a, S *b)
{
    if (D_8009C962 == 1) {
        char *t = (char *)b + b->d4;
        a->b3 = 1;
        a->d4 = (int)t;
    }
}
