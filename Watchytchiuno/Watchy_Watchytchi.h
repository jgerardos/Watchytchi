#ifndef WATCHY_DKTIME_H
#define WATCHY_DKTIME_H

#include "Watchy_Base.h"
#include "ImageAssets.h"

class Watchytchi : public WatchyBase{
    public:
        Watchytchi();
        void drawWatchFace();
        void drawEatAnim();
        void drawIdleCreature();
        void drawWeather();
        void drawUIButton(int idx, bool quickCursorUpdate);
        void clearCreatureBackground();
};

#endif
