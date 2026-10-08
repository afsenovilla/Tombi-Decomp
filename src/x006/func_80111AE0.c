// FUNC 80111ae0 96 X006
// MATCHING 80111ae0 96
extern unsigned short D_8009C960;
extern void func_8012ED60(void);
extern void func_80123B94(void);
extern void func_80121BAC(void);
void func_80111AE0(void)
{
    unsigned short m = D_8009C960;
    if (m == 0) {
        func_8012ED60();
    } else if (m == 4) {
        func_80123B94();
    } else if (m == 10) {
        func_80121BAC();
    }
}
