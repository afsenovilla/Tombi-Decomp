// FUNC 80120280 140 X009
// MATCHING 80120280 140
extern long *drawArea9ModelFT3(long *, unsigned long *);
extern long *drawArea9ModelFT4(long *, unsigned long *);
extern long *drawArea9ModelGT3(long *, unsigned long *);
extern long *drawArea9ModelGT4(long *, unsigned long *);
extern long *modelDrawF3Lit(long *, long *, unsigned long *);
extern long *modelDrawF4Lit(long *, long *, unsigned long *);
extern long *modelDrawG3Lit(long *, long *, unsigned long *);
extern long *modelDrawG4Lit(long *, long *, unsigned long *);

void drawArea9SectionModel(long *f, unsigned long *ot)
{
    f = drawArea9ModelFT3(f, ot);
    f = drawArea9ModelFT4(f, ot);
    f = drawArea9ModelGT3(f, ot);
    f = drawArea9ModelGT4(f, ot);
    f = modelDrawF3Lit(f, f + 1, ot);
    f = modelDrawF4Lit(f, f + 1, ot);
    f = modelDrawG3Lit(f, f + 1, ot);
    modelDrawG4Lit(f, f + 1, ot);
}
