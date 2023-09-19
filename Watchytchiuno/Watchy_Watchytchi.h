#pragma once
#include "Watchy_Base.h"
#include "ImageAssets.h"

enum TimeOfDay {Daytime, Dusk, LateNight};
enum HappyTier {Sad, Neutral, Happy, Blissful};

class SpeciesBase; // Needed to prevent circular include

class Watchytchi : public WatchyBase{
    using WatchyBase::WatchyBase;
    public:
        /*# Constants #*/
        SpeciesBase* critter = nullptr;
        const int k_alertInterval = 3 * 60 * 60;
        const int k_alertExpirationWindow = 30 * 60;

        /*# Lifecycle #*/
        void drawWatchFace();
        void handleButtonPress();

        /*# Save/load #*/
        void loadSaveData();
        void writeSaveData();
        void resetSaveData();

        /*# Game Logic #*/
        void tickCreatureState();
        TimeOfDay getTimeOfDay();
        TimeOfDay getTimeOfDay(const tmElements_t &tm);
        HappyTier getHappyTier();
        HappyTier getHappyTier(float happyPercent);
        int getHappyTrendingDirection();
        bool isElectricLit();
        /*## Game Logic (Schedulable Events) ##*/
        bool hasActiveAlert();
        void scheduleNextAlert();
        void executeCloseUp();
        void poseHWYDQuestion();

        /*# Draw functions #*/
        void clearCreatureBackground();
        void clearScreen();
        void drawUIButton(int idx, bool quickCursorUpdate);
        void drawAllUIButtons();
        void drawBgEnvironment();
        void drawWeather();
        void drawIdleCreature(bool isAnimating);
        void drawEatAnim();
        void drawAgeFlower();
        void drawDebugClock();
        void drawPoop();
        void drawStatusDisplay();
        /*## Draw functions (Schedulable Alerts) ##*/
        void executeHWYDResponse();

        /*# State-based handlers: #*/ 
        bool dummy_handleButtonPress(uint64_t wakeupBit);
        void baseMenu_draw();
        bool baseMenu_handleButtonPress(uint64_t wakeupBit);
        void eating_draw();
        void alertInteraction_draw();
        void stroking_draw();
        bool stroking_handleButtonPress(uint64_t wakeupBit);
        void howWasYourDay_draw();
        bool howWasYourDay_handleButtonPress(uint64_t wakeupBit);
        
        typedef void (Watchytchi::*DrawFunc)();
        DrawFunc drawFuncsByState[(int)GameState::CNT] = {
            &Watchytchi::baseMenu_draw, // BASEMENU
            &Watchytchi::eating_draw, // EATING
            &Watchytchi::alertInteraction_draw, // ALERT INTERACTION
            &Watchytchi::stroking_draw, // STROKING
            &Watchytchi::howWasYourDay_draw // HOW WAS YOUR DAY
        };

        typedef bool (Watchytchi::*ButtonFunc)(long long unsigned int);
        ButtonFunc handleButtonFuncsByState[(int)GameState::CNT] = {
            &Watchytchi::baseMenu_handleButtonPress, // BASEMENU
            &Watchytchi::dummy_handleButtonPress, // EATING
            &Watchytchi::dummy_handleButtonPress, // ALERT INTERACTION
            &Watchytchi::stroking_handleButtonPress, // STROKING
            &Watchytchi::howWasYourDay_handleButtonPress // HOW WAS YOUR DAY
        };
};
