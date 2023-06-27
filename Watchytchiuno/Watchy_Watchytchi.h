#include "Watchy_Base.h"
#include "ImageAssets.h"

enum TimeOfDay {Daytime, Dusk, LateNight};

class Watchytchi : public WatchyBase{
    using WatchyBase::WatchyBase;
    public:
        TimeOfDay getTimeOfDay();
        bool isElectricLit();
        void handleButtonPress();
        void drawWatchFace();
        void drawEatAnim();
        void drawIdleCreature();
        void drawWeather();
        void drawUIButton(int idx, bool quickCursorUpdate);
        void clearCreatureBackground();
};
