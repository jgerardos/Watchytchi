//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#ifndef WATCHY_BASE_H
#define WATCHY_BASE_H

#include <Watchy.h>
#include "ArduinoNvs.h"

#if __has_include("config.h") && __has_include(<stdint.h>)
# include "config.h"
#endif

enum CreatureSpecies {Hog, Snake};

extern RTC_DATA_ATTR int lastUpdateTsEpoch;
extern RTC_DATA_ATTR bool playAnim;
extern RTC_DATA_ATTR int menuIdx;
extern RTC_DATA_ATTR float hunger;
extern RTC_DATA_ATTR bool isEating;
extern RTC_DATA_ATTR bool hasPoop;
extern RTC_DATA_ATTR int lastPoopHour;
extern RTC_DATA_ATTR int idleAnimIdx;
extern RTC_DATA_ATTR bool hasStatusDisplay;
extern RTC_DATA_ATTR int lastAdvanceIdxMinute;
extern RTC_DATA_ATTR bool invertColors;
extern RTC_DATA_ATTR int lastHungerCryMinute;
extern RTC_DATA_ATTR int lastSecMatch;
extern RTC_DATA_ATTR int lastAnimateMinute;
extern RTC_DATA_ATTR bool isPeriodicAnim;
extern RTC_DATA_ATTR int dayBorn;
extern RTC_DATA_ATTR int nextAlertTs;
extern RTC_DATA_ATTR bool isExecutingAlertInteraction;
extern RTC_DATA_ATTR bool isStrokingMode;
extern RTC_DATA_ATTR bool isStrokingLeftSide;
extern RTC_DATA_ATTR CreatureSpecies species;

// Compile out macros to increase speed of serial printing
#define VERBOSE_LOGGING_ENABLED 0
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
const int MENUIDX_LIGHT = 5;
const int MENUIDX_PLACEHOLDER6 = 6;
const int MENUIDX_READ = 7;

class WatchyBase : public Watchy {
    using Watchy::Watchy;
    public:
        virtual void handleButtonPress();
        void vibrate(uint8_t times=1, uint32_t delay_time=40);
        esp_sleep_wakeup_cause_t wakeup_reason;
        void startProfile();
        void endProfile(char* label);
        void endProfileAndStart(char* label);
        int profileMs[16];
        int nextProfileIdx;
        virtual void drawUIButton(int idx, bool quickCursorUpdate) {}
    private:
        void _rtcConfig();
};

#endif
