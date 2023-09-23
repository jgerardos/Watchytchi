#pragma once
#include "Watchy_Watchytchi.h"
#include <stdlib.h>     //srand, rand
#include "SpeciesCode.h"

const unsigned char *dk_nums [10] = {dk0, dk1, dk2, dk3, dk4, dk5, dk6, dk7, dk8, dk9};
const unsigned char *foodBerry_stages[7] = {img_FoodBerry_Stage0, img_FoodBerry_Stage1, img_FoodBerry_Stage2, img_FoodBerry_Stage3, 
  img_FoodBerry_Stage4, img_FoodBerry_Stage5, img_FoodBerry_Stage6};
  const unsigned char *foodCucumber_stages[7] = {img_FoodCucumberPlate_Stage0, img_FoodCucumberPlate_Stage1, img_FoodCucumberPlate_Stage2, 
  img_FoodCucumberPlate_Stage3, img_FoodCucumberPlate_Stage4, img_FoodCucumberPlate_Stage5, img_FoodCucumberPlate_Stage6};
const unsigned char* flower_stages[6] = { img_GrowingFlower1, img_GrowingFlower2, img_GrowingFlower3, 
  img_GrowingFlower4, img_GrowingFlower5, img_GrowingFlower6};
const unsigned char *menu_reset_press_stages[4] = {img_MenuIcon_ResetSave_Active, img_MenuIcon_ResetSave_ActivePress1, img_MenuIcon_ResetSave_ActivePress2, img_MenuIcon_ResetSave_ActivePress3};

const float k_secDurationToFullyDepleteHunger = 4.f * 60.f * 60.f;
const int k_maxSecondsDeltaForUpdate = 5 * 60;

DaisyHog hog = DaisyHog();
MugSnake snake = MugSnake();
DeerSlug deerSlug = DeerSlug();

const unsigned char *img_smallFontArr[10] = {
  img_smallFont_0,
  img_smallFont_1,
  img_smallFont_2,
  img_smallFont_3,
  img_smallFont_4,
  img_smallFont_5,
  img_smallFont_6,
  img_smallFont_7,
  img_smallFont_8,
  img_smallFont_9
};

static float floatMap(float val, float fromLow, float fromHigh, float toLow, float toHigh, float precision = 1000.f)
{
  return (float)map(precision * val, precision * fromLow, precision * fromHigh, precision * toLow, precision * toHigh) / precision;
}

static float floatModulo(float val, float range)
{
  while (val >= range)
    val -= range;  
  return val;
}

void Watchytchi::drawWatchFace(){
  // For some reason we need to clear alarm 1, otherwise the watch updates every single frame
  if (Watchy::RTC.rtcType == DS3231) {
    Watchy::RTC.rtc_ds.clearAlarm(DS3232RTC::ALARM_1);
  }

  /*# Load Data: #*/
  loadSaveData();

  /*# Tick state! #*/
  tickCreatureState();

  // Execute the draw function appropriate to our current game state
  (this->*drawFuncsByState[(int)gameState])();

  writeSaveData();
  executeScheduledVibration();
} 

void Watchytchi::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

  if (IS_DOUBLE_TAP) {
    while (!sensor.getINT()) {
      // Wait until interrupt is cleared.
      // Otherwise it will fire again and again.
    }
  }
  RTC.read(currentTime);
  loadSaveData();
  // Call the active handle button press function depending on my gamestate
  DBGPrintF("Handling button press ins state "); DBGPrint(gameState); DBGPrintln();
  auto didHandlePress = (this->*handleButtonFuncsByState[(int)gameState])(wakeupBit);

  if (didHandlePress)
    return;
  else
    Watchy::handleButtonPress();
}

void Watchytchi::loadSaveData()
{
  // Prevent redundant loads from disk within a single wakeup. This value will reset each sleep cycle
  if (hasLoadedThisWake)
  {
    DBGPrintF("Skipping redundant load!"); DBGPrintln();
    return;
  }
  hasLoadedThisWake = true;

  NVS.begin();
  lastUpdateTsEpoch = NVS.getInt(nvsKey_lastUpdateTsEpoch, -1);
  invertColors = 1 == NVS.getInt(nvsKey_invertColors, 0);
  species = (CreatureSpecies)NVS.getInt(nvsKey_species, 0);
  numSecondsAlive = NVS.getInt(nvsKey_numSecondsAlive, 0);
  hunger = NVS.getFloat(nvsKey_hunger, 1.f);
  happyPercent = NVS.getFloat(nvsKey_happyPercent, 0.5f);
  hasPoop = 1 == NVS.getInt(nvsKey_hasPoop, 0);
  lastPoopHour = NVS.getInt(nvsKey_lastPoopHour, -1);
  nextAlertTs = NVS.getInt(nvsKey_nextAlertTs, -1);
  nextAlertType = (ScheduledAlertType)NVS.getInt(nvsKey_nextAlertType, 0);
  DBGPrintF("Loaded lastUpdateTsEpoch "); DBGPrint(lastUpdateTsEpoch); DBGPrintln();
  
  // Assign creature (TODO: convert to map or enum-linked array)
  if (species == CreatureSpecies::Hog)
    critter = &hog;
  else if (species == CreatureSpecies::Snake)
    critter = &snake;
  else if (species == CreatureSpecies::Deer)
    critter = &deerSlug;
  critter->owner = this;
}

void Watchytchi::writeSaveData()
{
  /*# Save data #*/
  NVS.setInt(nvsKey_lastUpdateTsEpoch, (int64_t)lastUpdateTsEpoch);
  DBGPrintF("Saving lastUpdateTsEpoch as "); DBGPrint(lastUpdateTsEpoch); DBGPrintln();
  NVS.setInt(nvsKey_invertColors, invertColors ? 1 : 0, false);
  NVS.setInt(nvsKey_species, (int)species, false);
  NVS.setInt(nvsKey_numSecondsAlive, numSecondsAlive, false);
  NVS.setFloat(nvsKey_hunger, hunger, false);
  NVS.setFloat(nvsKey_happyPercent, happyPercent, false);
  NVS.setInt(nvsKey_hasPoop, hasPoop ? 1 : 0, false);
  NVS.setInt(nvsKey_lastPoopHour, lastPoopHour, false);
  NVS.setInt(nvsKey_nextAlertTs, nextAlertTs);
  NVS.setInt(nvsKey_nextAlertType, (int)nextAlertType);
  auto didSave = NVS.commit();
  DBGPrintF("Save success? "); DBGPrint(didSave); DBGPrintln();
}

void Watchytchi::resetSaveData()
{
  NVS.begin();
  lastUpdateTsEpoch = -1;
  NVS.setInt(nvsKey_lastUpdateTsEpoch, lastUpdateTsEpoch);
  menuIdx = 0;
  lastAdvanceIdxMinute = 0;
  hasStatusDisplay = false;
  invertColors = false;
  NVS.setInt(nvsKey_invertColors, invertColors);
  species = CreatureSpecies::Deer;
  NVS.setInt(nvsKey_species, (int)species, false);
  numSecondsAlive = 0;
  NVS.setInt(nvsKey_numSecondsAlive, numSecondsAlive, false);
  hunger = 1.f;
  NVS.setFloat(nvsKey_hunger, hunger, false);
  happyPercent = 0.5f;
  NVS.setFloat(nvsKey_happyPercent, happyPercent, false);
  hasPoop = false;
  NVS.setInt(nvsKey_hasPoop, 0, false);
  lastPoopHour = -1;
  NVS.setInt(nvsKey_lastPoopHour, lastPoopHour, false);
  gameState = GameState::BaseMenu;
  playAnim = false;
  idleAnimIdx = 0;
  isPeriodicAnim = false;
  lastHungerCryMinute = -1;
  lastAnimateMinute = 0;
  isStrokingLeftSide = false;
  nextAlertTs = -1;
  NVS.setInt(nvsKey_nextAlertTs, nextAlertTs);
  nextAlertType = ScheduledAlertType::None;
  NVS.setInt(nvsKey_nextAlertType, (int)nextAlertType);
  emotionSelectIdx = 0;
  hasExecutedEnding = false;
  auto didSave = NVS.commit();
}

void Watchytchi::tickCreatureState()
{
  /*# Track time changes #*/
  time_t currentEpochTime = makeTime(currentTime);
  if (lastUpdateTsEpoch <= 0)
    lastUpdateTsEpoch = currentEpochTime;
  auto timeDelta = (float)(currentEpochTime - lastUpdateTsEpoch);
  if (timeDelta < 0.f)
    timeDelta = 0.f;
#if !DISABLE_MAX_TIMESTAMP
  if (timeDelta > k_maxSecondsDeltaForUpdate)
    timeDelta = k_maxSecondsDeltaForUpdate;
#endif
  DBGPrintF("Current epoch time "); DBGPrint(currentEpochTime); DBGPrintF(", prev epoch time "); DBGPrint(lastUpdateTsEpoch); DBGPrintF(", delta of "); DBGPrint(timeDelta); DBGPrintln();
  lastUpdateTsEpoch = currentEpochTime;

  // Calculate number of days alive
  numSecondsAlive += timeDelta;

  // Schedule alerts
  auto beforeAlertTs = nextAlertTs;
  /*# Interaction Alert #*/
  // Schedule a new alert if we don't have one scheduled, if our current alert is expired, or if it is too far in the future due to a time change
  if (nextAlertTs < 0 || currentEpochTime - nextAlertTs >= k_alertExpirationWindow  || nextAlertTs >= currentEpochTime + k_alertInterval * 2)
    scheduleNextAlert();
  // Reschedule alerts if we're unhappy
  if (getHappyTier() <= HappyTier::Sad)
    scheduleNextAlert();
  // If we have a forced alert that isn't already scheduled, schedule one
  if (FORCED_NEXT_EVENT != -1 && (!hasActiveAlert() || nextAlertType != (ScheduledAlertType)FORCED_NEXT_EVENT))
    scheduleNextAlert();

  // Automatically turn off lights if its daytime
  if (getTimeOfDay() != TimeOfDay::LateNight)
    invertColors = false;

  // Force good value for species enum
  if ((int)species < 0 || (int)species >= (int)CreatureSpecies::COUNT)
  {
    species = (CreatureSpecies)constrain((int)species, 0, (int)CreatureSpecies::COUNT - 1);
    NVS.begin();
    NVS.setInt(nvsKey_species, (int)species, true);
  }

  // Grow up into a bigger critter!
  if (species == CreatureSpecies::Deer && numSecondsAlive >= 2.5 * 24 * 60 * 60)
  {
    srand(lastUpdateTsEpoch);
    species = rand() % 2 == 0 ? CreatureSpecies::Hog : CreatureSpecies::Snake;
  }

  // Handle the ending!
  auto maximumAge = 6 * 24 * 60 * 60;
  if (gameState != GameState::Ending && numSecondsAlive >= maximumAge)
    gameState = GameState::Ending;
  if (gameState == GameState::Ending)
    return;

  // Clear the number of reset button presses if the player navigates away in the menu
  if (menuIdx != MENUIDX_RESET)
    numResetPresses = 0;

  // Put down status display if we select a new menu option
  hasStatusDisplay &= menuIdx == 0;

  /*# Aprophy Hunger #*/
  auto oldHunger = hunger;
  auto hungerDelta = timeDelta / k_secDurationToFullyDepleteHunger;
  if (getTimeOfDay() != TimeOfDay::LateNight)
    hunger -= hungerDelta;
  if (hunger < 0.f)
    hunger = 0.f;
  DBGPrintF("Hunger delta = "); DBGPrint(hungerDelta); DBGPrintF(", new hunger = "); DBGPrint(hunger); DBGPrintln();
  // Hunger alert:
  if (getTimeOfDay() == TimeOfDay::Daytime && hunger <= 0.1f && (lastHungerCryMinute == -1 || currentTime.Minute - lastHungerCryMinute >= 30 || currentTime.Minute < lastHungerCryMinute))
  {
    scheduleVibration(5, 50);
    lastHungerCryMinute = currentTime.Minute;
  }
  else if (oldHunger > 0.45f && hunger <= 0.45f)
  {
    scheduleVibration(3, 50);
  }
  if (hunger > 0.1f && lastHungerCryMinute != -1)
  {
    lastHungerCryMinute = -1;
  }


  /*# Atrophy happiness #*/
  auto oldHappyPercent = happyPercent;
  const float happinessFullyChangeDuration = 6 * 60 * 60;
  // If starving, happiness instantly goes down to a low number
  if (hunger <= 0.001f)
    happyPercent = constrain(happyPercent, 0.f, 0.25f);

  // Move happiness up or down depending on state:
  auto happyTrendDir = getHappyTrendingDirection();
  happyPercent += timeDelta * happyTrendDir / happinessFullyChangeDuration;
  
  happyPercent = constrain(happyPercent, 0.f, 1.f);
  DBGPrintF("New happyPercent "); DBGPrint(happyPercent); DBGPrintF(", from old happy percent "); DBGPrint(oldHappyPercent); DBGPrintln();

  /*# Atrophy poop: #*/
  if (!hasPoop && getTimeOfDay() != TimeOfDay::LateNight && (lastPoopHour == -1 || currentTime.Hour >= lastPoopHour + 4 || currentTime.Hour < lastPoopHour) 
    && lastAnimateMinute > 0)/*Hack: do this to avoid immediate poop at the start of a new game. */
  {
    DBGPrintF("Pooping! lastPoopHour = "); DBGPrint(lastPoopHour); DBGPrintF(", lastAnimateMinute = "); DBGPrint(lastAnimateMinute); DBGPrintln();
    hasPoop = true;
    lastPoopHour = currentTime.Hour;
    scheduleVibration(3, 50);
  }
  else
  {
    DBGPrintF("Chose not to poop! lastPoopHour = "); DBGPrint(lastPoopHour); DBGPrintF(", lastAnimateMinute = "); DBGPrint(lastAnimateMinute); DBGPrintln();
  }
}

TimeOfDay Watchytchi::getTimeOfDay()
{
  return getTimeOfDay(currentTime);
}

TimeOfDay Watchytchi::getTimeOfDay(const tmElements_t &tm)
{
  if (tm.Hour >= 21 || tm.Hour <= 6)
    return TimeOfDay::LateNight;
  else if (tm.Hour > 18 || (tm.Hour == 18 && tm.Minute >= 30))
    return TimeOfDay::Dusk;
  else
    return TimeOfDay::Daytime;
}

HappyTier Watchytchi::getHappyTier()
{
  return getHappyTier(happyPercent);
}

HappyTier Watchytchi::getHappyTier(float hPercent)
{
  if (hPercent >= 0.95f)
    return HappyTier::Blissful;
  if (hPercent >= 0.66f)
    return HappyTier::Happy;
  if (hPercent >= 0.33f)
    return HappyTier::Neutral;
  
  return HappyTier::Sad;
}

int Watchytchi::getHappyTrendingDirection()
{
  // If hungry, not put to bed ontime, or there is active poop, happiness decreases
  if ((getTimeOfDay() == TimeOfDay::LateNight && isElectricLit()) || hunger <= 0.45f || hasPoop)
    return -1;
  // Otherwise, if I'm well-fed, happiness increases!
  else if (hunger >= 0.6f)
    return 1;
  return 0;
}

bool Watchytchi::isElectricLit()
{
  return getTimeOfDay() == TimeOfDay::LateNight && !invertColors;
}

void Watchytchi::clearCreatureBackground()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  display.fillRect(100 - 36, 97, 156 - (100 - 36) + 8, 72, color_bg);
}

void Watchytchi::clearScreen()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  display.fillRect(0, 0, 200, 200, color_bg);
}

void Watchytchi::drawUIButton(int idx, bool quickCursorUpdate)
{
    if (getTimeOfDay() != TimeOfDay::LateNight)
      invertColors = false;
    auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
    auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  
    int numButtons = 8;
    int width = 200;
    int height = 200;
    int buttonWidth = 32;
    int numPerRow = numButtons / 2;
    float separation = width / (float)numPerRow; 
    auto iconColor = isElectricLit() ? color_bg : color_fg;
    int column = idx % numPerRow;
    int xPos = (int)(width * ((float)column / numPerRow) + (separation / 2)) - buttonWidth / 2;
    int yPos;
    if (idx < numPerRow)
      yPos = 1;
    else
      yPos = height - buttonWidth - 1;

    // Cursor timeout
    if (currentTime.Minute - lastAdvanceIdxMinute >= 2 || (lastAdvanceIdxMinute > currentTime.Minute && currentTime.Minute >= 1))
        menuIdx = -1;

    if (quickCursorUpdate)
      display.fillRect(xPos, yPos, 32, 32, isElectricLit() ? color_fg : color_bg);

    auto selected = idx == menuIdx;
    if (idx == MENUIDX_INSPECT)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Status_Active : img_MenuIcon_Status_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_FEED)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Feed_Active : img_MenuIcon_Feed_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_STROKE)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Stroke_Active : img_MenuIcon_Stroke_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_ALERT)
    {
      // The alert menu icon draws differently depending on whether there is an active alert
      auto hasAlert = hasActiveAlert();
      if (hasAlert && selected)
        display.drawBitmap(xPos, yPos, img_MenuIcon_Alert_Active_HasNotification, 32, 32, iconColor);
      else if (hasAlert)
        display.drawBitmap(xPos, yPos, img_MenuIcon_Alert_Inactive_HasNotification, 32, 32, iconColor);
      else 
        display.drawBitmap(xPos, yPos, img_MenuIcon_Alert_Inactive, 32, 32, iconColor);
    }
    else if (idx == MENUIDX_CLEAN)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Clean_Active : img_MenuIcon_Clean_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_LIGHT)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Lights_Active : img_MenuIcon_Lights_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_READ)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Read_Active : img_MenuIcon_Read_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_RESET)
    {
      if (idx != menuIdx)
        display.drawBitmap(xPos, yPos, img_MenuIcon_ResetSave_Inactive, 32, 32, iconColor);
      else
        display.drawBitmap(xPos, yPos, menu_reset_press_stages[constrain(numResetPresses, 0, 4)], 32, 32, iconColor);
    }
    else
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Placeholder_Active : img_MenuIcon_Placeholder_Inactive, 32, 32, iconColor);
}

void Watchytchi::drawAllUIButtons()
{
  for (auto i = 0; i < 8; i++)
  {
    drawUIButton(i, false);
  }
}

void Watchytchi::drawBgEnvironment()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  // Black or white background depending on time of day
  display.fillScreen(color_bg);

  // Draw some shaded radial gradients if it's evening or night but the lights are on
  if (isElectricLit())
    display.drawBitmap(0, 0, img_LightbulbLightGradient_1, 200, 200, color_fg);
  else if (getTimeOfDay() == TimeOfDay::Dusk)
    display.drawBitmap(0, 0, img_DuskLightGradient, 200, 200, color_fg);
  
  if (getTimeOfDay() == TimeOfDay::LateNight)
    display.drawBitmap(100 - (9/2), 0, img_HangingLightbulb, 9, 81, color_fg); 
}

void Watchytchi::drawWeather(){
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  
  // Position Sun / moon according to time of day:   
  bool fastDebug = false;
  float hourF;
  if (!fastDebug)
    hourF = (float)currentTime.Hour + (currentTime.Minute / 60.f) + (currentTime.Second / (60.f * 60.f));  
  else
    hourF = (float)((currentTime.Second % 48) / 2.f);

  float adjustedHour = floatModulo(hourF + 6.f, 12.f);
    
  float t = (adjustedHour  / 12.f);
  float theta = t * PI;

  float x = floatMap(cos(theta), -1.f, 1.f, 0, 1.f) * 200.f;
  float y = floatMap(1.f - abs(sin(theta)), 0.f, 1.f, 40.f, 125.f);

  display.drawBitmap(x - 21, y - 21, hourF <= 6 || hourF >= 18 ? img_Moon : img_Sun, 42, 42, color_fg);


  // Draw clouds
  auto cloud1T = floatModulo(t * 30.f, 1.f);
  float cloud1X = floatMap(cloud1T, 0.f, 1.f, 200.f, -100.f);
  auto cloud2T = floatModulo((t + 0.3) * 12.5f, 1.f);
  float cloud2X = floatMap(cloud2T, 0.f, 1.f, 400.f, -130.f);
  auto cloud3T = floatModulo((t + 0.7) * 2.5f, 1.f);
  float cloud3X = floatMap(cloud3T, 0.f, 1.f, 500.f, -300.f);

  auto isDark = currentTime.Hour > 18 || (currentTime.Hour == 18 && currentTime.Minute >= 30);
  display.drawBitmap(cloud1X, 42, isDark ? img_DarkCloud1 : img_Cloud1, 99, 54, color_fg);
  display.drawBitmap(cloud2X, 56, isDark ? img_DarkCloud2 : img_Cloud2, 128, 62, color_fg);
  display.drawBitmap(cloud3X, 30, isDark ? img_DarkCloud3 : img_Cloud3, 130, 35, color_fg);
}

void Watchytchi::drawIdleCreature(bool isAnimating){
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  // Late night with lights on: Sleepy pose
  if (getTimeOfDay() == TimeOfDay::LateNight && !invertColors)
    critter->DrawSleepyPose(idleAnimIdx, isAnimating);
  // Late night lights off: Asleep pose
  else if (getTimeOfDay() == TimeOfDay::LateNight && invertColors)
    critter->DrawAsleepPose(idleAnimIdx, isAnimating);
  // Extreme hunger: Starving pose
  else if (hunger <= 0.1f)
    critter->DrawStarvingPose(idleAnimIdx, isAnimating);
  // Medium hunger: Hungry pose
  else if (hunger <= 0.45f)
    critter->DrawHungryPose(idleAnimIdx, isAnimating);
  // Unhappy: Sulking pose
  else if (getHappyTier() <= HappyTier::Sad)
    critter->DrawSadPose(idleAnimIdx, isAnimating);
  // Do a twich instead of the standing idle frames if we're on our periodic animation
  else if (isPeriodicAnim)
    critter->DrawTwitchAnimationPose(idleAnimIdx, isAnimating);
  // Default: Standing idle
  else
    critter->DrawIdlePose(idleAnimIdx, isAnimating);

  idleAnimIdx = (idleAnimIdx + 1) % 2;
}

void Watchytchi::drawEatAnim(){    
    auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
    auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
    int foodType;
    int numFrames;
    const int BERRIES = 0;
    const int CUCUMBER = 1;
    srand(lastUpdateTsEpoch);
    if (rand() % 2 == 0)
    {
      foodType = BERRIES;
      numFrames = 56;
    }
    else
    {
      foodType = CUCUMBER;
      numFrames = 28;
    }

     // Animate eating
     for (int i = 0; i < numFrames; i++)
     {
       critter->DrawEatingPose(i, true);
       display.drawBitmap(144 - 18, 126, foodType == BERRIES ? foodBerry_stages[i / 8] : foodCucumber_stages[i / 4], 36, 36, color_fg);
       display.display(true);
       clearCreatureBackground();
     }

     //Hide Ghosting
     for(uint8_t i=0; i<3; i++){
       display.fillRect(100 - 36, 97, 120, 72, color_bg);
       drawIdleCreature(false);
       display.display(true);
     }
}

void Watchytchi::drawAgeFlower()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  // Draw a flower that grows a little bit every day:
  auto age = (int)(numSecondsAlive / (24 * 60 * 60));
  auto flowerGrowthIdx = constrain(age, 0, 5);
  display.drawBitmap(156, 91, flower_stages[flowerGrowthIdx], 30, 45, color_fg);
  DBGPrintF("numSecondsAlive is "); DBGPrint(numSecondsAlive); DBGPrintF(", age in days is "); DBGPrint(age);
}

void Watchytchi::drawDebugClock()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  // Draw a very small debug clock:
  //Hour
  display.drawBitmap(183-46, 195, img_smallFontArr[currentTime.Hour/10], 3, 5, color_fg); //first digit
  display.drawBitmap(187-46, 195, img_smallFontArr[currentTime.Hour%10], 3, 5, color_fg); //second digit

  //Minute
  display.drawBitmap(192-46, 195, img_smallFontArr[currentTime.Minute/10], 3, 5, color_fg); //first digit
  display.drawBitmap(196-46, 195, img_smallFontArr[currentTime.Minute%10], 3, 5, color_fg); //second digit
}

void Watchytchi::drawPoop()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  if (hasPoop)    
    display.drawBitmap(32, 200 - 32 - 20 - 4, idleAnimIdx % 2 == 0 ? img_SmallPoop_1 : img_SmallPoop_2, 20, 20, color_fg);
}

void Watchytchi::drawStatusDisplay()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  int hungerNumIdx = (int)(hunger * 10.f);
  if (hungerNumIdx > 9)
    hungerNumIdx = 9;
  auto age = (int)(numSecondsAlive / (24 * 60 * 60));
  display.drawBitmap(10, 110, dk_nums[hungerNumIdx], 28, 26, color_fg); //first digit
  display.drawBitmap(158, 110, dk_nums[constrain(age, 0, 9)], 28, 26, color_fg);
  // Draw a face according to current mood:

  auto happyTier = getHappyTier();
  if (happyTier == HappyTier::Blissful)
    display.drawBitmap(85, 75, img_HappinessMoodle_Blissful, 30, 30, color_fg);
  else if (happyTier == HappyTier::Happy)
    display.drawBitmap(85, 75, img_HappinessMoodle_Happy, 30, 30, color_fg);
  else if (happyTier == HappyTier::Neutral)
    display.drawBitmap(85, 75, img_HappinessMoodle_Neutral, 30, 30, color_fg);
  else
    display.drawBitmap(85, 75, img_HappinessMoodle_Sad, 30, 30, color_fg);
}

bool Watchytchi::dummy_handleButtonPress(uint64_t)
{
  return false;
}

void Watchytchi::baseMenu_draw()
{
  drawBgEnvironment();
  drawWeather();
  drawAllUIButtons();
  drawDebugClock();

  /*# Status Display #*/
  if (hasStatusDisplay) // TODO: make into own gamestate
    drawStatusDisplay();

  // Animate an idle loop every 3 minutes
  if (currentTime.Hour >= 6 && (currentTime.Minute - lastAnimateMinute > 3 || lastAnimateMinute > currentTime.Minute))
  {
    isPeriodicAnim = currentTime.Minute % 4 == 0;
    const int numAnimFrames = isPeriodicAnim ? 10 : 16;
    for (auto i = 0; i < numAnimFrames; i++)
    {
      isPeriodicAnim &= (i != numAnimFrames - 1);
      drawIdleCreature(i != numAnimFrames - 1);
      display.display(true);
      if (i != numAnimFrames - 1)
        clearCreatureBackground();
    }
    lastAnimateMinute = currentTime.Minute;
  }
  else {
    drawIdleCreature(false);
  }

  drawAgeFlower();
  drawPoop();    
}

bool Watchytchi::baseMenu_handleButtonPress(uint64_t wakeupBit)
{
  // Process selection
  if (IS_KEY_SELECT) {
    auto didPerformAction = false;
    // Open up the menu UI
    if (menuIdx == MENUIDX_INSPECT)
    {
      hasStatusDisplay = true;
      didPerformAction = true;
    }
    else
      hasStatusDisplay = false;
    if (menuIdx == MENUIDX_STROKE)
    {
      gameState = GameState::StrokingMode;
      DBGPrintF("Entering stroke mode!"); DBGPrintln();
    }      
    // Start feeding
    if (menuIdx == MENUIDX_FEED)
    {
      gameState = GameState::Eating;
      didPerformAction = true;
    }
    // React to alert
    if (menuIdx == MENUIDX_ALERT && hasActiveAlert())
    {
      gameState = GameState::AlertInteraction;
      didPerformAction = false;
    }
    // Clean Poop
    if (hasPoop && menuIdx == MENUIDX_CLEAN)
    {
      hasPoop = false;
      didPerformAction = true;
      auto prevHour = lastPoopHour;
      lastPoopHour = currentTime.Hour; // Cleaning resets last poop hour in order to prevent immediate poop once again
      lastAnimateMinute = -99; // Do a little dance afterwards by resetting the last animate minute
      NVS.begin();
      NVS.setInt(nvsKey_hasPoop, 0, false);
      NVS.setInt(nvsKey_lastPoopHour, lastPoopHour, false);
      NVS.commit();
      DBGPrintF("Cleaned poop! New lastPoopHour ="); DBGPrint(lastPoopHour); DBGPrintF(", previously it was"); DBGPrint(prevHour); DBGPrintln();
    }
    // Toggle Light
    if (menuIdx == MENUIDX_LIGHT && (currentTime.Hour >= 21 || currentTime.Hour <= 6))
    {
      invertColors = !invertColors;
      NVS.begin();
      NVS.setInt(nvsKey_invertColors, invertColors ? 1 : 0, true);
      didPerformAction = true;
    }
    // HACK: for debugging purposes, the not-yet-implemented read icon will toggle the species
    if (menuIdx == MENUIDX_READ)
    {
      species = (CreatureSpecies)(((int)species + 1) % (int)CreatureSpecies::COUNT);
      NVS.begin();
      NVS.setInt(nvsKey_species, (int)species, true);
      didPerformAction = true;
    }
    // HACK: until we have a settings menu, resetting save data is an option from one of the 8 care buttons
    if (menuIdx == MENUIDX_RESET)
    {
      numResetPresses++;
      didPerformAction = true;
      if (numResetPresses >= 4)
        resetSaveData();
    }
    // Vibrate if this selection resulted in an action
    if (didPerformAction)
      vibrate(1, 50);
    showWatchFace(true);
    return true;
  }

  if (IS_KEY_CURSOR) {
    menuIdx = (menuIdx + 1) % 8;
    // Skip the alert icon if there is no active alert. Disallow certain care actions while it's night
    while ((!hasActiveAlert() && menuIdx == MENUIDX_ALERT)
      || (getTimeOfDay() == TimeOfDay::LateNight && (menuIdx == MENUIDX_FEED || menuIdx == MENUIDX_STROKE || menuIdx == MENUIDX_CLEAN)))
    {
      menuIdx = (menuIdx + 1) % 8;
    }
    vibrate();
    lastAdvanceIdxMinute = currentTime.Minute;

    // Partial redraw -- commented out because it wasn't working
//    display.init(0, false); //_initial_refresh to false to prevent full update on init
//    display.setFullWindow();
//    showWatchFace(true);
//    //drawUIButton(menuIdx-1, true);
//    //drawUIButton(menuIdx, true);
//    //display.display(true); //partial refresh
//    // display.hibernate();
//    guiState = WATCHFACE_STATE;
    showWatchFace(true);
    return true;
  }

  if (IS_KEY_CANCEL) {
    vibrate();
    menuIdx = MENUIDX_NOTHING;
    vibrate(1, 30);
    showWatchFace(true);
    return true;
  }
  return false;
}

void Watchytchi::eating_draw()
{
  drawBgEnvironment();
  drawWeather();
  drawDebugClock();
  drawPoop();
  drawEatAnim();
  
  // After drawing the eat animation, exit the eating state and draw the default screen
  gameState = GameState::BaseMenu;
  hunger = 1.f;
  baseMenu_draw();
}

void Watchytchi::stroking_draw()
{
  drawBgEnvironment();
  drawWeather();
  drawDebugClock();
  drawPoop();
  critter->DrawStrokingPose(idleAnimIdx, false);
  idleAnimIdx = (idleAnimIdx + 1) % 2;
  drawAgeFlower();
}

bool Watchytchi::stroking_handleButtonPress(uint64_t wakeupBit)
{
  // In stroking mode, the left (cursor) and right (select) button moves the hand back and forth
  auto didPet = false;
  if (IS_KEY_SELECT && isStrokingLeftSide) 
  {
    isStrokingLeftSide = false;
    didPet = true;
  }

  if (IS_KEY_CURSOR && !isStrokingLeftSide) 
  {
      isStrokingLeftSide = true;
      didPet = true;
  }

  if (IS_KEY_SELECT || IS_KEY_CURSOR)
  {
    if (didPet)
    {
      happyPercent += 0.025f;
      NVS.begin();
      NVS.setFloat(nvsKey_happyPercent, happyPercent, true);
      vibrate(1, 30);
    }
    showWatchFace(true);
    return true;
  }

  if (IS_KEY_CANCEL) {
    gameState = GameState::BaseMenu;
    vibrate(1, 50);
    showWatchFace(true);
    return true;
  }

  return false;
}

