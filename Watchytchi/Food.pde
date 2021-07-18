 

class FoodData
{
  public PImage[] eatFrames;

  FoodData(PImage[] eatFramesIn)
  {
    eatFrames = eatFramesIn;
  }
}

class FoodInstance
{
  public float xPos;
  public float yPos;
  public FoodData data;
  public float eatProgress = 0f;

  public FoodInstance(float xIn, float yIn, FoodData dataIn)
  {
    xPos = xIn;
    yPos = yIn;
    data = dataIn;
  }

  public void TickEat(float eatDelta)
  {
    eatProgress = constrain(eatProgress + eatDelta, 0f, 1f);
  }

  public void Tick(float dt)
  {
    // Apply gravity
    if (yPos < floorY)
      yPos = constrain(yPos + dt * gravitySpeed, 0, floorY);
  }

  public void Draw()
  {
    PImage frame = data.eatFrames[floor(eatProgress * (data.eatFrames.length - 1f))];
    image(frame, xPos, yPos - frame.height);
  }

  public boolean IsFalling()
  {
    return yPos < floorY;
  }
}

void CreateFood()
{
  FoodInstance newFood = new FoodInstance(150f, 0f, foodData_Berry);
  activeFoods.add(newFood);
}
