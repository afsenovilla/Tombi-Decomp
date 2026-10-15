// FUNC 80129a9c 200 X004
// MATCHING 80129a9c 200
#include "TOBJ.H"
extern void initScaredGoldVolunteer(TObj *);
extern void scaredGoldVolunteerTalk(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void updateScaredGoldVolunteer(TObj *o)
{
    switch (o->b04) {
    case 0:
        initScaredGoldVolunteer(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->step == 0) goto run;
        if (o->step == 1) {
        run:
            scaredGoldVolunteerTalk(o);
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
