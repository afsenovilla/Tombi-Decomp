// FUNC 8013bc24 380 X001
// MATCHING 8013bc24 380
#include "TOBJ.H"

extern unsigned char D_8009CE59;
extern unsigned char D_8009CEDE;
void func_8013A4B0(TObj *o);
short func_8013A664(TObj *o);
void func_8013A8CC(TObj *o);
void func_8013AF10(TObj *o);
void func_8013B0F4(TObj *o);
void func_8013AAB8(TObj *o);
void PoolFree_1F800210(TObj *o);

void func_8013BC24(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8013A4B0(o);
        break;
    case 1:
        if (func_8013A664(o)) {
            switch (D_8009CE59) {
            case 1:
                switch (D_8009CEDE) {
                case 0:
                    func_8013A8CC(o);
                    break;
                case 1:
                    func_8013AAB8(o);
                    break;
                }
                break;
            case 2:
                if (D_8009CEDE == 0) {
                    func_8013AF10(o);
                }
                break;
            case 3:
                if (D_8009CEDE == 0) {
                    func_8013B0F4(o);
                }
                break;
            case 0xff:
                func_8013AAB8(o);
                break;
            }
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
