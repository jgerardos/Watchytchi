#pragma once
#include "Watchy_Base.h"
#include "ImageAssets.h"

enum TimeOfDay {Daytime, Dusk, LateNight};
enum HappyTier {Sad, Neutral, Happy, Blissful};

class SpeciesBase; // Needed to prevent circular include

class Watchytchi : public WatchyBase{
    using WatchyBase::WatchyBase;
    public:
        SpeciesBase* critter = nullptr;
        const int k_alertInterval = 3 * 60 * 60;
        const int k_alertExpirationWindow = 30 * 60;
        void resetSaveData();
        TimeOfDay getTimeOfDay();
        TimeOfDay getTimeOfDay(const tmElements_t &tm);
        HappyTier getHappyTier();
        HappyTier getHappyTier(float happyPercent);
        int getHappyTrendingDirection();
        bool isElectricLit();
        bool hasActiveAlert();
        void scheduleNextAlert();
        void executeCloseUp();
        void poseHWYDQuestion();
        void drawHWYDMoodSelection();
        void executeHWYDResponse();
        void handleButtonPress();
        void drawBgEnvironment();
        void drawWatchFace();
        void drawEatAnim();
        void drawIdleCreature(bool isAnimating);
        void drawWeather();
        void drawUIButton(int idx, bool quickCursorUpdate);
        void clearCreatureBackground();
};
