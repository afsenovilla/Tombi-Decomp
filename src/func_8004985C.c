// FUNC 8004985c 152 MAIN0
// MATCHING 8004985c 152
extern unsigned short D_8009C960;
extern void func_8012CCF8(void);
extern void func_8012EFC4(void);
extern void func_80124B78(void);
extern void func_80123750(void);

void func_8004985C(void)
{
    switch (D_8009C960) {
    case 0:
        func_8012CCF8();
        break;
    case 1:
        func_8012EFC4();
        break;
    case 3:
        func_80124B78();
        break;
    case 4:
        func_80123750();
        break;
    }
}
