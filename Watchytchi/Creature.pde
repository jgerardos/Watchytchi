class Creature
{
  float xPos, yPos;
  Animation currentAnim;
  int frameIdx = 0;
  float frameTicker;
  PVector size = new PVector(0f, 0f);
  
  int faceDirection = 1;
  float speed = 20f;

  float desiredX = width;

  void SetAnim(Animation newAnim)
  {
    if (newAnim != currentAnim)
    {
      frameIdx = 0;
      frameTicker = 0f;
      currentAnim = newAnim;
    }
  }

  Creature(float xIn, float yIn)
  {
    xPos = xIn;
    yPos = yIn;
    
    for (int i = 0; i < idleAnim.frames.length; i++)
    {
      size.x = max(size.x, idleAnim.frames[i].width);
      size.y = max(size.y, idleAnim.frames[i].height);
    }
  }
  
  void Tick(float dt)
  {
    if (activeFoods.size() == 0)
    {
      SetAnim(stepAnim);

      // Basic ping ponging walk behavior
      xPos += speed * dt * faceDirection;
      if (faceDirection == -1 && xPos < 0)
      {
        faceDirection = 1;
        xPos = -xPos;
      }
      else if (faceDirection == 1 && xPos > width)
      {
        faceDirection = -1;
        xPos = width + width - xPos;
      }
    }
    else
    {
      float desiredX;
      if (activeFoods.get(0).IsFalling())
        desiredX = 60;
      else
        desiredX = 110f;

      float deltaToTarget = desiredX - xPos;
      if (abs(deltaToTarget) > 0.000001f)
      {
        faceDirection = deltaToTarget > 0 ? 1 : -1;
        float walkDelta = min(abs(deltaToTarget), speed * dt) * faceDirection;
        xPos += walkDelta;
        SetAnim(stepAnim);
      }
      else if (activeFoods.get(0).IsFalling())
      {
        faceDirection = 1;
        SetAnim(idleAnim);
      }
      else
      {
        faceDirection = 1;
        SetAnim(eatAnim);
        activeFoods.get(0).TickEat(dt);
      }
    }
  }

  void Draw(float dt)
  {
    frameTicker += dt;
    while (frameTicker > currentAnim.frameInterval)
    {
      frameIdx = (frameIdx + 1) % currentAnim.frames.length;
      frameTicker -= currentAnim.frameInterval;
    }

    pushMatrix();
    translate(xPos - size.x / 2f * faceDirection, yPos - size.y);
    scale(faceDirection, 1.0);
    image(currentAnim.frames[frameIdx], 0, 0);
    popMatrix();

    if (doDebug)
    {
      fill(255, 0, 255);
      square(xPos, yPos, 2);
    }
  }
}
