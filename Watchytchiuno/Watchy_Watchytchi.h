#include "Watchy_Base.h"
#include "ImageAssets.h"

enum TimeOfDay {Daytime, Dusk, LateNight};
enum HappyTier {Sad, Neutral, Happy, Blissful};

class Watchytchi : public WatchyBase{
    using WatchyBase::WatchyBase;
    public:
        TimeOfDay getTimeOfDay();
        TimeOfDay getTimeOfDay(const tmElements_t &tm);
        HappyTier getHappyTier();
        HappyTier getHappyTier(float happyPercent);
        int getHappyTrendingDirection();
        bool isElectricLit();
        bool hasActiveAlert();
        void scheduleNextAlert();
        void handleButtonPress();
        void drawWatchFace();
        void drawEatAnim();
        void drawIdleCreature();
        void drawWeather();
        void drawUIButton(int idx, bool quickCursorUpdate);
        void clearCreatureBackground();
};
