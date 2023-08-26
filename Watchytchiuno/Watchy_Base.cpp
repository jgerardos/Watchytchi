//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#include "Watchy_Base.h"

/*# State #*/
/*## State: System ##*/
RTC_DATA_ATTR int lastUpdateTsEpoch = -1;
RTC_DATA_ATTR int numResetPresses = 0;

/*## State: UI ##*/
RTC_DATA_ATTR int menuIdx;
RTC_DATA_ATTR int lastAdvanceIdxMinute = 0;
RTC_DATA_ATTR bool hasStatusDisplay = false;
RTC_DATA_ATTR bool invertColors = false;

/*## State: Saveable pet stats ##*/
RTC_DATA_ATTR CreatureSpecies species = CreatureSpecies::Deer;
RTC_DATA_ATTR int numSecondsAlive = 0;
RTC_DATA_ATTR float hunger = 1.f;
RTC_DATA_ATTR float happyPercent = 0.5f;
RTC_DATA_ATTR bool hasPoop = false;
RTC_DATA_ATTR int lastPoopHour = -1;

/*## State: Game State (General) ##*/
RTC_DATA_ATTR GameState gameState;
RTC_DATA_ATTR bool playAnim = false;
RTC_DATA_ATTR int idleAnimIdx = 0;
RTC_DATA_ATTR bool isPeriodicAnim = false;
RTC_DATA_ATTR int lastHungerCryMinute = -1;
RTC_DATA_ATTR int lastAnimateMinute = 0;

/*## State: Game State (Stroking) ##*/
RTC_DATA_ATTR bool isStrokingLeftSide = false;

/*## State: Game State (Events & Alerts) ##*/
RTC_DATA_ATTR int nextAlertTs = -1;
RTC_DATA_ATTR ScheduledAlertType nextAlertType = ScheduledAlertType::None;
RTC_DATA_ATTR int emotionSelectIdx = 0;


void WatchyBase::handleButtonPress() {
  // Handled in base class
}

void WatchyBase::vibrate(uint8_t times, uint32_t delay_time) {
  // Ensure that no false positive double tap is produced
  sensor.enableFeature(BMA423_WAKEUP, false);

  pinMode(VIB_MOTOR_PIN, OUTPUT);
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(VIB_MOTOR_PIN, true);
    delay(delay_time);
    digitalWrite(VIB_MOTOR_PIN, false);
    if (i != times - 1)
      delay(delay_time);
  }

  sensor.enableFeature(BMA423_WAKEUP, true);
}

void WatchyBase::_rtcConfig() {
  RTC.read(currentTime);
}

void WatchyBase::startProfile()
{
  return; // TODO: remove this early out, this was a temporary debug measure that was not supposed to be pushed
  DBGPrintF("startprofile!"); DBGPrintln();

  DBGPrintF("before starting a new profile, nextProfileIdx is "); DBGPrint(nextProfileIdx); DBGPrintln();
  profileMs[nextProfileIdx] = millis();
  nextProfileIdx++;
  DBGPrintF("after starting a new profile, nextProfileIdx is "); DBGPrint(nextProfileIdx); DBGPrintln();
}

void WatchyBase::endProfile(const char* label)
{
  DBGPrintF("Before endProfile, nextProfileIdx is "); DBGPrint(nextProfileIdx); DBGPrintln();

  auto idx = --nextProfileIdx;
  auto ms = millis();
  DBGPrintF("Profile label "); DBGPrint(label); DBGPrintF(" took "); DBGPrint(millis() - profileMs[idx]); DBGPrintF(" ms, "); 
  DBGPrintF("After endProfile, nextProfileIdx is "); DBGPrint(nextProfileIdx); DBGPrintln();
}

void WatchyBase::endProfileAndStart(const char* label)
{
  endProfile(label);
  startProfile();
}
