#include "Watchy_Base.h"
#include "ImageAssets.h"

enum TimeOfDay {Daytime, Dusk, LateNight};

class Watchytchi : public WatchyBase{
    public:
        Watchytchi();
        TimeOfDay getTimeOfDay();
        bool isElectricLit();
        bool handleButtonPress();
        void drawWatchFace();
        void drawEatAnim();
        void drawIdleCreature();
        void drawWeather();
        void drawUIButton(int idx, bool quickCursorUpdate);
        void clearCreatureBackground();
};
