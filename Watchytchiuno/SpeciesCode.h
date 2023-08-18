#pragma once
#include "Watchy_Base.h"
#include "Watchy_Watchytchi.h"

class SpeciesBase
{

  public:
    Watchytchi* owner;
    int getColor_fg()
    {
      return invertColors ? GxEPD_WHITE : GxEPD_BLACK;
    }

    virtual void DrawStrokingPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawEatingPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawSleepyPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawAsleepPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawStarvingPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawHungryPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawSadPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawTwitchAnimationPose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawIdlePose(int idleIdx, bool isAnimating) = 0;
    virtual void DrawCloseUpFrame(int idleIdx, bool isAnimating) = 0;
};

class DaisyHog : public SpeciesBase
{
  void DrawStrokingPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, isStrokingLeftSide ? img_DaisyHog_BeingStroked1 : img_DaisyHog_BeingStroked2, 72, 55, getColor_fg());
  }
  void DrawEatingPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, idleIdx % 2 == 0 ? img_DaisyHog_Eat1 : img_DaisyHog_Eat2, 72, 55, getColor_fg());
  }
  void DrawSleepyPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, idleIdx % 2 == 0 ? img_DaisyHog_Sleepy1 : img_DaisyHog_Sleepy2, 72, 55, getColor_fg());
  }
  void DrawAsleepPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, idleIdx % 2 == 0 ? img_DaisyHog_Sleep1 : img_DaisyHog_Sleep2, 72, 55, getColor_fg());
  }
  void DrawStarvingPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, img_DaisyHog_VeryHungry1, 72, 55, getColor_fg());
    owner->display.drawBitmap(100 - 36 + 25, 110-16+7, idleIdx % 2 == 0 ? img_Emote_Hungry1 : img_Emote_Hungry2, 28, 19, getColor_fg());
  }
  void DrawHungryPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, img_DaisyHog_Hungry1, 72, 55, getColor_fg());
  }
  void DrawSadPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, idleIdx % 2 == 0 ? img_DaisyHog_Sulking1 : img_DaisyHog_Sulking2, 72, 55, getColor_fg());
    if (owner->getHappyTrendingDirection() < 0)
      owner->display.drawBitmap(100 - 36 + 25, 85, idleIdx % 2 == 0 ? img_Emote_Stormcloud1 : img_Emote_Stormcloud2, 28, 28, getColor_fg());
  }
  void DrawTwitchAnimationPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 110, idleIdx % 2 == 0 ? img_DaisyHog_Twitch1 : img_DaisyHog_Twitch2, 80, 55, getColor_fg());
    if (owner->getHappyTier() >= HappyTier::Blissful)
      owner->display.drawBitmap(119, 115, idleIdx % 2 == 0 ? img_Emote_Hearts1 : img_Emote_Hearts2, 28, 19, getColor_fg());
  }
  void DrawIdlePose(int idleIdx, bool isAnimating) override
  {
    // Afternoon special: hind legs
    if (owner->getHappyTier() >= HappyTier::Happy && owner->currentTime.Hour >= 12 && owner->currentTime.Hour < 14)
    {
      owner->display.drawBitmap(100 - 36, 93 + 4, idleIdx % 2 == 0 ? img_DaisyHog_HindLegs1 : img_DaisyHog_HindLegs2, 72, 72, getColor_fg());
      if (isAnimating && owner->getHappyTier() >= HappyTier::Blissful)
        owner->display.drawBitmap(100 - 36 + 25, 95, idleIdx % 2 == 0 ? img_Emote_Hearts1 : img_Emote_Hearts2, 28, 19, getColor_fg());
    }
    // Every couple of hours: special idle
    else if (owner->getHappyTier() >= HappyTier::Happy && owner->currentTime.Hour % 2 == 0 && owner->currentTime.Minute >= 20 && owner->currentTime.Minute <= 40)
    {
      owner->display.drawBitmap(100 - 36, 110 + 4, idleIdx % 2 == 0 ? img_DaisyHog_SkyGaze1 : img_DaisyHog_SkyGaze2, 72, 55, getColor_fg());
      if (!isAnimating || owner->getHappyTier() < HappyTier::Blissful)
        owner->display.drawBitmap(112, 110, idleIdx % 2 == 0 ? img_Emote_Music1 : img_Emote_Music2, 28, 19, getColor_fg());
      else
        owner->display.drawBitmap(112, 110, idleIdx % 2 == 0 ? img_Emote_Hearts1 : img_Emote_Hearts2, 28, 19, getColor_fg());
    }
    // Default: Standing idle
    else
    {
      owner->display.drawBitmap(100 - 36, 110, idleAnimIdx % 2 == 0 ? img_DaisyHog_Idle1 : img_DaisyHog_Idle2, 72, 55, getColor_fg());
      if (isAnimating && owner->getHappyTier() >= HappyTier::Blissful)
        owner->display.drawBitmap(119, 115, idleIdx % 2 == 0 ? img_Emote_Hearts1 : img_Emote_Hearts2, 28, 19, getColor_fg());
    }
  }
  void DrawCloseUpFrame(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(0, 0, idleIdx % 2 == 0 ? img_CloseUp_Happy1 : img_CloseUp_Happy2, 200, 200, getColor_fg());
  }
};

class MugSnake : public SpeciesBase
{
  void DrawStrokingPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, isStrokingLeftSide ? img_MugSnake_BeingPet1 : img_MugSnake_BeingPet2, 72, 72, getColor_fg());
  }
  void DrawEatingPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, idleIdx % 2 == 0 ? img_MugSnake_Eating1 : img_MugSnake_Eating2, 72, 72, getColor_fg());
  }
  void DrawSleepyPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, img_MugSnake_Hungry, 72, 72, getColor_fg()); // TODO: bespoke mugsnake animation for sleepy
  }
  void DrawAsleepPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, idleIdx % 2 == 0 ? img_MugSnake_Sleeping1 : img_MugSnake_Sleeping2, 72, 72, getColor_fg());
  }
  void DrawStarvingPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, img_MugSnake_VeryHungry, 72, 72, getColor_fg());
    owner->display.drawBitmap(100 - 36 + 25, 97, idleIdx % 2 == 0 ? img_Emote_Hungry1 : img_Emote_Hungry2, 28, 19, getColor_fg());
  }
  void DrawHungryPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, img_MugSnake_Hungry, 72, 72, getColor_fg());
  }
  void DrawSadPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, idleIdx % 2 == 0 ? img_MugSnake_Sulking1 : img_MugSnake_Sulking2, 72, 72, getColor_fg());
    if (owner->getHappyTrendingDirection() < 0)
      owner->display.drawBitmap(100 - 36 + 25, 95, idleIdx % 2 == 0 ? img_Emote_Stormcloud1 : img_Emote_Stormcloud2, 28, 28, getColor_fg());
  }
  void DrawTwitchAnimationPose(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(100 - 36, 97, idleIdx % 2 == 0 ? img_MugSnake_Rocking1 : img_MugSnake_Rocking2, 72, 72, getColor_fg());
    if (owner->getHappyTier() >= HappyTier::Blissful)
      owner->display.drawBitmap(119, 115, idleIdx % 2 == 0 ? img_Emote_Hearts1 : img_Emote_Hearts2, 28, 19, getColor_fg());
  }
  void DrawIdlePose(int idleIdx, bool isAnimating) override
  {
    if (owner->getHappyTier() >= HappyTier::Happy && owner->currentTime.Hour % 2 == 0 && owner->currentTime.Minute >= 20 && owner->currentTime.Minute <= 40)
    {
      owner->display.drawBitmap(100 - 36, 97, idleAnimIdx % 2 == 0 ? img_MugSnake_TippedOverIdle1 : img_MugSnake_TippedOverIdle2, 72, 72, getColor_fg());
      if (!isAnimating || owner->getHappyTier() < HappyTier::Blissful)
        owner->display.drawBitmap(120, 130, idleAnimIdx % 2 == 0 ? img_Emote_Music1 : img_Emote_Music2, 28, 19, getColor_fg());
      else
        owner->display.drawBitmap(120, 130, idleAnimIdx % 2 == 0 ? img_Emote_Hearts1 : img_Emote_Hearts2, 28, 19, getColor_fg());
    }
    else
    {
      owner->display.drawBitmap(100 - 36, 97, idleIdx % 2 == 0 ? img_MugSnake_Idle1 : img_MugSnake_Idle2, 72, 72, getColor_fg());
      if (isPeriodicAnim && owner->getHappyTier() >= HappyTier::Blissful)
        owner->display.drawBitmap(100 - 36 + 25, 95, idleIdx % 2 == 0 ? img_Emote_Hearts1 : img_Emote_Hearts2, 28, 19, getColor_fg());
    }
  }
  void DrawCloseUpFrame(int idleIdx, bool isAnimating) override
  {
    owner->display.drawBitmap(0, 0, idleIdx % 2 == 0 ? img_MugSnake_CloseUp_Happy1 : img_MugSnake_CloseUp_Happy2, 200, 200, getColor_fg());
  }
};