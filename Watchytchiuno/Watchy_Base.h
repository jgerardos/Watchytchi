#pragma once
//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#ifndef WATCHY_BASE_H
#define WATCHY_BASE_H
#include <Watchy.h>
#include "ArduinoNvs.h"
#include "DebugSettings.h"

#if __has_include("config.h") && __has_include(<stdint.h>)
# include "config.h"
#endif

enum CreatureSpecies {Hog, Snake, Deer, COUNT};
enum ScheduledAlertType {None, CloseUp, AskAboutDay};
enum GameState {BaseMenu, Eating, AlertInteraction, StrokingMode, HowWasYourDay, Ending, SharedWalk, CNT};

struct HappyContributor
{
    public: 
        float defaultValue;
        float value; 
        float min; 
        float max;
        HappyContributor(float startVal, float min, float max);
        void AddTo(float delta);
        void MoveTowards(float target, float delta);
        void WriteSaveData(String nvsKey);
        void LoadSaveData(String nvsKey);
};

/*# State #*/
/*## State: System ##*/
extern RTC_DATA_ATTR int lastUpdateTsEpoch;
const String nvsKey_lastUpdateTsEpoch = "prevTs";
extern RTC_DATA_ATTR int numResetPresses;

/*## State: UI ##*/
extern RTC_DATA_ATTR int menuIdx;
extern RTC_DATA_ATTR int lastAdvanceIdxMinute;
extern RTC_DATA_ATTR bool hasStatusDisplay;
extern RTC_DATA_ATTR bool invertColors;
const String nvsKey_invertColors = "invertColors";

/*## State: Saveable pet stats ##*/
extern RTC_DATA_ATTR CreatureSpecies species;
const String nvsKey_species = "species";
extern RTC_DATA_ATTR int numSecondsAlive;
const String nvsKey_numSecondsAlive = "secsAlive";
extern RTC_DATA_ATTR float hunger;
const String nvsKey_hunger = "hunger";
extern RTC_DATA_ATTR HappyContributor foodHappy;
const String nvsKey_foodHappy = "foodHappy";
extern RTC_DATA_ATTR HappyContributor strokeHappy;
const String nvsKey_strokeHappy = "strokeHappy";
extern RTC_DATA_ATTR HappyContributor walkHappy;
const String nvsKey_walkHappy = "walkHappy";
extern RTC_DATA_ATTR HappyContributor poopHappy;
const String nvsKey_poopHappy = "poopHappy";
extern RTC_DATA_ATTR HappyContributor sleepHappy;
const String nvsKey_sleepHappy = "sleepHappy";
static float lastHappyDelta;

extern RTC_DATA_ATTR bool hasPoop;
const String nvsKey_hasPoop = "hasPoop";
extern RTC_DATA_ATTR int lastPoopHour;
const String nvsKey_lastPoopHour = "lastPoopHour";

/*## State: Game State (General) ##*/
extern RTC_DATA_ATTR GameState gameState;
extern RTC_DATA_ATTR bool playAnim;
extern RTC_DATA_ATTR int idleAnimIdx;
extern RTC_DATA_ATTR bool isPeriodicAnim;
extern RTC_DATA_ATTR int lastHungerCryMinute;
extern RTC_DATA_ATTR int lastAnimateMinute;

/*## State: Game State (Stroking) ##*/
extern RTC_DATA_ATTR bool isStrokingLeftSide;

/*## State: Game State (Events & Alerts) ##*/
extern RTC_DATA_ATTR int nextAlertTs;
const String nvsKey_nextAlertTs = "nextAlertTs";
extern RTC_DATA_ATTR ScheduledAlertType nextAlertType;
const String nvsKey_nextAlertType = "nextAlertType";
extern RTC_DATA_ATTR int emotionSelectIdx;

/*## State: Game State (Ending) ##*/
extern RTC_DATA_ATTR bool hasExecutedEnding;

/*## State: Game State (Shared Walk) ##*/
extern RTC_DATA_ATTR int bmaStepsAtWalkStart;
extern RTC_DATA_ATTR int lastStepsDuringWalkCount;

// Compile out macros to increase speed of serial printing
#define DBGPrint(content) if (VERBOSE_LOGGING_ENABLED) {Serial.print(content);}
#define DBGPrintF(content) if (VERBOSE_LOGGING_ENABLED) {Serial.print(F(content));}
#define DBGPrintln() if (VERBOSE_LOGGING_ENABLED) {Serial.println();}

// Btn definitions
#define IS_DOUBLE_TAP       (wakeupBit & ACC_INT_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_UP     (wakeupBit & UP_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_LEFT_UP      (wakeupBit & BACK_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_DOWN   (wakeupBit & DOWN_BTN_MASK && guiState == WATCHFACE_STATE)
#define EXT_INT_MASK        MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK

#define IS_KEY_CURSOR IS_BTN_LEFT_UP
#define IS_KEY_SELECT IS_BTN_RIGHT_UP
#define IS_KEY_CANCEL IS_BTN_RIGHT_DOWN

const int MENUIDX_NOTHING = -1;
const int MENUIDX_INSPECT = 0;
const int MENUIDX_STROKE = 1;
const int MENUIDX_FEED = 2;
const int MENUIDX_ALERT = 3;
const int MENUIDX_CLEAN = 4;
const int MENUIDX_WALK = 5;
const int MENUIDX_LIGHT = 6;
const int MENUIDX_RESET = 7;

class WatchyBase : public Watchy {
    using Watchy::Watchy;
    public:
        virtual void handleButtonPress();
        void scheduleVibration(uint8_t times=1, uint32_t delay_time=40);
        void vibrate(uint8_t times=1, uint32_t delay_time=40);
        void executeScheduledVibration();
        esp_sleep_wakeup_cause_t wakeup_reason;
        void startProfile();
        void endProfile(const char* label);
        void endProfileAndStart(const char* label);
        int profileMs[16];
        int nextProfileIdx;
        virtual void drawUIButton(int idx, bool quickCursorUpdate) {}
    private:
        void _rtcConfig();
};

#endif
