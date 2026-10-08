// FUNC 80116dd0 124 X009
// MATCHING 80116dd0 124

extern unsigned short D_8009C962;
extern void func_80116278(void);
extern void func_801165A0(void);
extern void func_80116BD0(void);

void func_80116DD0(void)
{
    switch (D_8009C962) {
    case 0:
        func_80116278();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        func_801165A0();
        break;
    case 6:
        func_80116BD0();
        break;
    }
}
