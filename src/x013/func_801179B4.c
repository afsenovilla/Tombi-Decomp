// FUNC 801179b4 408 X013
// MATCHING 801179b4 408
typedef struct { short a, b, c; } R6;
extern R6 D_80118210[];
extern R6 D_80118224[];
extern R6 D_8011825C[];
extern R6 D_801182A4[];
extern int D_8009C96C;
void func_80117B4C(R6 *p);
void func_80117C10(int a, int b, int c);
void func_80117E14(int a, int b, int c, int d);
int GetGraphType(void);
void AddDrawMode(int a, int b);

void func_801179B4(int a, int b, int c, int d)
{
    R6 *p;

    for (p = D_80118210; p->a != -1;) {
        func_80117B4C(p++);
    }
    for (p = D_80118224; p->a != -1;) {
        func_80117B4C(p++);
    }
    for (p = D_8011825C; p->a != -1;) {
        func_80117B4C(p++);
    }
    for (p = D_801182A4; p->a != -1;) {
        func_80117B4C(p++);
    }
    func_80117C10(D_8009C96C, 0xc0, 0x3c);
    func_80117E14(a, b, 0xc0, 0x5c);
    func_80117C10(c, 0xc0, 0x7c);
    func_80117C10(d, 0xc0, 0x9c);
    if (GetGraphType() == 1) {
        AddDrawMode(0xe, 5);
    } else {
        GetGraphType();
        AddDrawMode(0xe, 5);
    }
}
