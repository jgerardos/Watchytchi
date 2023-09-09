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
    
        // State-based handlers:
        bool dummy_handleButtonPress(uint64_t wakeupBit);
        void baseMenu_draw();
        bool baseMenu_handleButtonPress(uint64_t wakeupBit);
        void stroking_draw();
        bool stroking_handleButtonPress(uint64_t wakeupBit);
        void howWasYourDay_draw();
        bool howWasYourDay_handleButtonPress(uint64_t wakeupBit);
        
        typedef bool (Watchytchi::*ButtonFunc)(long long unsigned int);
        ButtonFunc handleButtonFuncsByState[(int)GameState::CNT] = {
            &Watchytchi::baseMenu_handleButtonPress, // BASEMENU
            &Watchytchi::dummy_handleButtonPress, // EATING
            &Watchytchi::dummy_handleButtonPress, // ALERT INTERACTION
            &Watchytchi::stroking_handleButtonPress, // STROKING
            &Watchytchi::howWasYourDay_handleButtonPress // HOW WAS YOUR DAY
        }; 
};
