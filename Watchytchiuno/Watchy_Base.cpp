//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#include "Watchy_Base.h"

RTC_DATA_ATTR int lastUpdateTsEpoch = -1;
RTC_DATA_ATTR int16_t alarm_timer = -1;
RTC_DATA_ATTR bool sleep_mode = false;
RTC_DATA_ATTR bool playAnim = false;
RTC_DATA_ATTR int menuIdx;
RTC_DATA_ATTR float hunger = 1.f;
RTC_DATA_ATTR bool isEating = false;
RTC_DATA_ATTR bool hasPoop = false;
RTC_DATA_ATTR int lastPoopHour = -1;
RTC_DATA_ATTR int idleAnimIdx = 0;
RTC_DATA_ATTR bool hasStatusDisplay = false;
RTC_DATA_ATTR int lastAdvanceIdxMinute = 0;
RTC_DATA_ATTR bool invertColors = false;
RTC_DATA_ATTR int lastHungerCryMinute = -1;
RTC_DATA_ATTR int lastSecMatch = 0;
RTC_DATA_ATTR int lastAnimateMinute = 0;
RTC_DATA_ATTR bool isPeriodicAnim = false;
RTC_DATA_ATTR int dayBorn = -1;
RTC_DATA_ATTR int nextAlertTs = -1;
RTC_DATA_ATTR bool isExecutingAlertInteraction = false;
RTC_DATA_ATTR CreatureSpecies species = CreatureSpecies::Hog;


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
  return;
  DBGPrintF("startprofile!");
  DBGPrintln();

  DBGPrintF("before starting a new profile, nextProfileIdx is ");
  DBGPrint(nextProfileIdx);
  DBGPrintln();
  profileMs[nextProfileIdx] = millis();
  nextProfileIdx++;
  DBGPrintF("after starting a new profile, nextProfileIdx is ");
  DBGPrint(nextProfileIdx);
  DBGPrintln();
}

void WatchyBase::endProfile(char* label)
{
  DBGPrintF("Before endProfile, nextProfileIdx is ");
  DBGPrint(nextProfileIdx);
  DBGPrintln();

  auto idx = --nextProfileIdx;
  auto ms = millis();
  DBGPrintF("Profile label ");
  DBGPrint(label);
  DBGPrintF(" took ");
  DBGPrint(millis() - profileMs[idx]);
  DBGPrintF(" ms, ");
  DBGPrintF("After endProfile, nextProfileIdx is ");
  DBGPrint(nextProfileIdx);
  DBGPrintln();
}

void WatchyBase::endProfileAndStart(char* label)
{
  endProfile(label);
  startProfile();
}
