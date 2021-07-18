class Creature
{
  float xPos, yPos;
  Animation currentAnim;
  int frameIdx = 0;
  float frameTicker;
  PVector size = new PVector(0f, 0f);
  
  int faceDirection = 1;
  float speed = 20f;

  Creature(float xIn, float yIn, Animation animIn)
  {
    xPos = xIn;
    yPos = yIn;
    currentAnim = animIn;
    
    for (int i = 0; i < animIn.frames.length; i++)
    {
      size.x = max(size.x, animIn.frames[i].width);
      size.y = max(size.y, animIn.frames[i].height);
    }
  }
  
  void Tick(float dt)
  {
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

  void Draw(float dt)
  {
    frameTicker += dt;
    while (frameTicker > currentAnim.frameInterval)
    {
      frameIdx = (frameIdx + 1) % currentAnim.frames.length;
      frameTicker -= currentAnim.frameInterval;
    }

    if (faceDirection == -1)
    {
       pushMatrix();
       translate(xPos, yPos);
       scale(-1.0, 1.0);
       image(currentAnim.frames[frameIdx], 0, 0);
       popMatrix();
    }
    else
      image(currentAnim.frames[frameIdx], xPos, yPos);
  }
}
