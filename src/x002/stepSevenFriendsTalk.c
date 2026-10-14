// FUNC 801182ac 192 X002
// MATCHING 801182ac 192
#include "TOBJ.H"

extern void sevenFriendsWaitTalk(TObj *);
extern void sevenFriendsGoodLuckTalk(TObj *);

void stepSevenFriendsTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->b68 == 0) break;
        switch (o->b0c) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            o->step = 2;
            o->state = 0;
            break;
        case 6:
        case 8:
            break;
        case 7:
            o->step = 1;
            o->state = 0;
            break;
        }
        break;
    case 1:
        sevenFriendsWaitTalk(o);
        break;
    case 2:
        sevenFriendsGoodLuckTalk(o);
        break;
    }
}
