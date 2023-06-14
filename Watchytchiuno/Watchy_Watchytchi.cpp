#include "Watchy_Watchytchi.h"
#include <stdlib.h>     //srand, rand

const unsigned char *dk_nums [10] = {dk0, dk1, dk2, dk3, dk4, dk5, dk6, dk7, dk8, dk9};
const unsigned char *food_stages[7] = {img_FoodBerry_Stage0, img_FoodBerry_Stage1, img_FoodBerry_Stage2, img_FoodBerry_Stage3, 
  img_FoodBerry_Stage4, img_FoodBerry_Stage5, img_FoodBerry_Stage6};

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

Watchytchi::Watchytchi(){} //constructor

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
    startProfile();

    /*# Background and environment: #*/
    auto isLateNight = currentTime.Hour >= 21 || currentTime.Hour <= 6;
    if (!isLateNight)
      invertColors = false;
    auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
    auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
    
    auto isElectricLit = !invertColors && isLateNight;
    //BG
    display.fillScreen(color_bg);
    endProfileAndStart("Section 1: Initialize and fillscreen");

    if (isElectricLit)
      display.drawBitmap(0, 0, img_LightbulbLightGradient_1, 200, 200, color_fg);
    else if (!invertColors && (currentTime.Hour > 18 || (currentTime.Hour == 18 && currentTime.Minute >= 30)))
      display.drawBitmap(0, 0, img_DuskLightGradient, 200, 200, color_fg);
    if (isLateNight)
      display.drawBitmap(100 - (9/2), 0, img_HangingLightbulb, 9, 81, color_fg); 
    
    drawWeather();
    endProfileAndStart("Section 2: Draw weather and maybe lighting");

    for (auto i = 0; i < 8; i++)
    {
      drawUIButton(i, false);
    }

    endProfileAndStart("Section 3: UI");

    // Draw a very small debug clock:
    auto Hour = currentTime.Hour;
    if(twelve_mode) {
        if (Hour > 12) {
          Hour = Hour - 12;
        } else if (Hour == 0) {
          Hour = 12;
        }
    }
    //Hour
    display.drawBitmap(183-46, 195, img_smallFontArr[Hour/10], 3, 5, color_fg); //first digit
    display.drawBitmap(187-46, 195, img_smallFontArr[Hour%10], 3, 5, color_fg); //second digit

    //Minute
    display.drawBitmap(192-46, 195, img_smallFontArr[currentTime.Minute/10], 3, 5, color_fg); //first digit
    display.drawBitmap(196-46, 195, img_smallFontArr[currentTime.Minute%10], 3, 5, color_fg); //second digit

    // Age
    if (dayBorn == -1)
      dayBorn = currentTime.Day;
    auto age = currentTime.Day - dayBorn;
    if (age > 9)
      age = 9;

    // Hunger
    auto oldHunger = hunger;
    if (!isLateNight)
      hunger -= 0.002f;
    if (hunger < 0.f)
      hunger = 0.f;

    int hungerNumIdx = (int)(hunger * 10.f);
    if (hungerNumIdx > 9)
      hungerNumIdx = 9;
    hasStatusDisplay &= menuIdx == 0;
    if (hasStatusDisplay)
    {
      display.drawBitmap(10, 110, dk_nums[hungerNumIdx], 28, 26, color_fg); //first digit
      display.drawBitmap(158, 110, dk_nums[age], 28, 26, color_fg);
    }
      


    endProfile("Section 4: Clock digits");
    if(isEating) {
      drawEatAnim();
    } 
    // Animate an idle loop every 3 minutes
    else if (currentTime.Hour >= 6 && (currentTime.Minute - lastAnimateMinute > 3 || lastAnimateMinute > currentTime.Minute))
    {
      isPeriodicAnim = currentTime.Minute % 4 == 0;
      const int numAnimFrames = isPeriodicAnim ? 10 : 16;
      for (auto i = 0; i < numAnimFrames; i++)
      {
        isPeriodicAnim &= (i != numAnimFrames - 1);
        drawIdleCreature();
        display.display(true);
        if (i != numAnimFrames - 1)
          clearCreatureBackground();
      }
      lastAnimateMinute = currentTime.Minute;
    }
    else {
      drawIdleCreature();
    }


    auto isNight = currentTime.Hour <= 6 || currentTime.Hour >= 19;
    if (!isNight && hunger <= 0.1f && (lastHungerCryMinute == -1 || currentTime.Minute - lastHungerCryMinute >= 5 || currentTime.Minute < lastHungerCryMinute))
    {
      const float numVibes = 4;
      for (int i = 0; i < numVibes; i++)
      {
        vibrate(5, 50);
        delay(1000);
      }
      
      lastHungerCryMinute = currentTime.Minute;
    }
    if (oldHunger > 0.45f && hunger <= 0.45f)
    {
      for (int i = 0; i < 3; i++)
      {
        vibrate(3, 50);
        delay(1000);
      }
    }
    
    if (hunger > 0.1f && lastHungerCryMinute != -1)
    {
      lastHungerCryMinute = -1;
    }
} 

void Watchytchi::drawUIButton(int idx, bool quickCursorUpdate)
{
    auto isLateNight = currentTime.Hour >= 21 || currentTime.Hour <= 6;
    if (!isLateNight)
      invertColors = false;
    auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
    auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
    auto isElectricLit = !invertColors && isLateNight;
  
    int numButtons = 8;
    int width = 200;
    int height = 200;
    int buttonWidth = 32;
    int numPerRow = numButtons / 2;
    float separation = width / (float)numPerRow; 
    auto iconColor = isElectricLit ? color_bg : color_fg;
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
      display.fillRect(xPos, yPos, 32, 32, isElectricLit ? color_fg : color_bg);

    if (idx == MENUIDX_INSPECT)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Status_Active : img_MenuIcon_Status_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_FEED)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Feed_Active : img_MenuIcon_Feed_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_LIGHT)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Lights_Active : img_MenuIcon_Lights_Inactive, 32, 32, iconColor);
    else if (idx == MENUIDX_READ)
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Read_Active : img_MenuIcon_Read_Inactive, 32, 32, iconColor);
    else
      display.drawBitmap(xPos, yPos, idx == menuIdx ? img_MenuIcon_Placeholder_Active : img_MenuIcon_Placeholder_Inactive, 32, 32, iconColor);
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

//  Serial.print("t (* 100) is ");
//  Serial.print(t * 100.f);
//  Serial.print(", cloudT (* 100) is ");
//  Serial.print(cloud1T * 100.f);
//  Serial.print(", cloudX is ");
//  Serial.print(cloud1X);
//  Serial.println();

  auto isDark = currentTime.Hour > 18 || (currentTime.Hour == 18 && currentTime.Minute >= 30);
  display.drawBitmap(cloud1X, 42, isDark ? img_DarkCloud1 : img_Cloud1, 99, 54, color_fg);
  display.drawBitmap(cloud2X, 56, isDark ? img_DarkCloud2 : img_Cloud2, 128, 62, color_fg);
  display.drawBitmap(cloud3X, 30, isDark ? img_DarkCloud3 : img_Cloud3, 130, 35, color_fg);
}

void Watchytchi::drawIdleCreature(){
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;

  auto isLateNight = currentTime.Hour >= 21 || currentTime.Hour <= 6;
  
    //Static Creature
    if (isLateNight && !invertColors)
      display.drawBitmap(100 - 36, 110, idleAnimIdx % 2 == 0 ? img_DaisyHog_Sleepy1 : img_DaisyHog_Sleepy2, 72, 55, color_fg);
    else if (isLateNight && invertColors)
      display.drawBitmap(100 - 36, 110, idleAnimIdx % 2 == 0 ? img_DaisyHog_Sleep1 : img_DaisyHog_Sleep2, 72, 55, color_fg);
    else if (hunger <= 0.1f)
    {
      display.drawBitmap(100 - 36, 110, img_DaisyHog_VeryHungry1, 72, 55, color_fg);
      display.drawBitmap(100 - 36 + 25, 110-16+7, idleAnimIdx % 2 == 0 ? img_Emote_Hungry1 : img_Emote_Hungry2, 28, 19, color_fg);
    }
    else if (hunger <= 0.45f)
    {
      display.drawBitmap(100 - 36, 110, img_DaisyHog_Hungry1, 72, 55, color_fg);
    }
    else if (currentTime.Hour % 2 == 0 && currentTime.Minute >= 20 && currentTime.Minute <= 40)
    {
      display.drawBitmap(100 - 36, 110 + 4, idleAnimIdx % 2 == 0 ? img_DaisyHog_SkyGaze1 : img_DaisyHog_SkyGaze2, 72, 55, color_fg);
      display.drawBitmap(112, 110, idleAnimIdx % 2 == 0 ? img_Emote_Music1 : img_Emote_Music2, 28, 19, color_fg);
    }
    else if (isPeriodicAnim)
    {
      display.drawBitmap(100 - 36, 110, idleAnimIdx % 2 == 0 ? img_DaisyHog_Twitch1 : img_DaisyHog_Twitch2, 80, 55, color_fg);
    }
    else
      display.drawBitmap(100 - 36, 110, idleAnimIdx % 2 == 0 ? img_DaisyHog_Idle1 : img_DaisyHog_Idle2, 72, 55, color_fg);
    idleAnimIdx = (idleAnimIdx + 1) % 2;
}

void Watchytchi::drawEatAnim(){    
    auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
    auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
     // Animate eating
     int numFrames = 56;
     for (int i = 0; i < numFrames; i++)
     {
       display.drawBitmap(100 - 36, 110, i % 2 == 0 ? img_DaisyHog_Eat1 : img_DaisyHog_Eat2, 72, 55, color_fg);
       display.drawBitmap(144 - 18, 126, food_stages[i / 8], 36, 36, color_fg);
       display.display(true);
       clearCreatureBackground();
     }
     isEating = false;
     hunger = 1.f;

     //Hide Ghosting
     for(uint8_t i=0; i<3; i++){
       display.fillRect(100 - 36, 110, 120, 55, color_bg);
       drawIdleCreature();
       display.display(true);
     }
}

void Watchytchi::clearCreatureBackground()
{
  auto color_bg = invertColors ? GxEPD_BLACK : GxEPD_WHITE;
  auto color_fg = invertColors ? GxEPD_WHITE : GxEPD_BLACK;
  display.fillRect(100 - 36, 110, 156 - (100 - 36), 55, color_bg);
}
