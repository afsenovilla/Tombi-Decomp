// FUNC 8013b254 228 X001
// MATCHING 8013b254 228
#include "gcd.h"
extern void raceStartCountdown(void);
extern void raceOfferRematch(void);
extern void raceStartRematch(void);
extern void raceResult(void);

void stepRaceEvent(void)
{
    switch (GCD.bce59) {
    case 1:
        switch (GCD.bcede) {
        case 0:
            raceStartCountdown();
            break;
        case 1:
            raceResult();
            break;
        }
        break;
    case 2:
        if (GCD.bcede == 0) raceOfferRematch();
        break;
    case 3:
        if (GCD.bcede == 0) raceStartRematch();
        break;
    case 0xff:
        raceResult();
        break;
    }
}
