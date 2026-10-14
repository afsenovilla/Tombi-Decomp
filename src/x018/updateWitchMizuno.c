// FUNC 8011a32c 372 X018
// MATCHING 8011a32c 372
#include "TOBJ.H"

extern void func_8011A1B4(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);
extern void func_8011A038(TObj *);
extern void witchMizunoIntroTalk(TObj *);
extern void witchMizunoLostCrystalBallsTalk(TObj *);
extern void witchMizunoCrystalBallsFoundTalk(TObj *);
extern void witchMizunoBusyTalk(TObj *);
extern void witchMizunoMagicMirrorTalk(TObj *);
extern void witchMizunoKeepMirrorTalk(TObj *);
extern void witchMizunoSneezeTalk(TObj *);
extern void witchMizunoCuredTalk(TObj *);
extern void witchMizunoHuntPigsTalk(TObj *);
extern void witchMizunoCandyRecipeTalk(TObj *);

void updateWitchMizuno(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8011A1B4(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_8011A038(o);
            break;
        case 1:
            witchMizunoIntroTalk(o);
            break;
        case 2:
            witchMizunoLostCrystalBallsTalk(o);
            break;
        case 3:
            witchMizunoCrystalBallsFoundTalk(o);
            break;
        case 4:
            witchMizunoBusyTalk(o);
            break;
        case 5:
            witchMizunoMagicMirrorTalk(o);
            break;
        case 6:
            witchMizunoKeepMirrorTalk(o);
            break;
        case 7:
            witchMizunoSneezeTalk(o);
            break;
        case 8:
            witchMizunoCuredTalk(o);
            break;
        case 9:
            witchMizunoHuntPigsTalk(o);
            break;
        case 10:
            witchMizunoCandyRecipeTalk(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
