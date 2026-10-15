// FUNC 800ec504 160 D0SPSUB2
// MATCHING 800ec504 160
extern short D_800F70D8, D_800F70DA, D_800F70DC, D_800F70DE;
extern int D_800F70E8;
extern void *D_800F70EC;
extern char D_800EDC7C[];
extern int *D_800F6FF0[];
extern unsigned short GetClut(int x, int y);
extern void func_800EB61C(int a, int b);

void func_800EC504(int tbl, int idx, int x, int y)
{
    D_800F70D8 = x;
    D_800F70DA = y;
    D_800F70DC = 4;
    D_800F70DE = GetClut(0x150, 0x1F0);
    D_800F70EC = D_800EDC7C;
    D_800F70E8 = D_800F6FF0[tbl][idx];
    func_800EB61C(5, 0);
}
