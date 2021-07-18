 

class FoodData
{
  public PImage[] eatFrames;
  public float eatDuration;

  FoodData(String spritePrefix, int numEatFrames, float eatDurationIn)
  {
    eatFrames = LoadImageArray(spritePrefix, numEatFrames, 0); // TODO: inconsistent food indices
    eatDuration = eatDurationIn;
  }
}

class FoodInstance extends Turtle
{
  public FoodData data;
  public float eatProgress = 0f;
  public SpriteRenderer sprRenderer;

  public FoodInstance(PVector posIn, FoodData dataIn)
  {
    super(posIn);
    data = dataIn;
    sprRenderer = new SpriteRenderer(this, dataIn.eatFrames[0], new PVector(0.5f, 1f));
    renderer = sprRenderer;
  }

  public void TickEat(float dt)
  {
    eatProgress = eatProgress + (dt / data.eatDuration);
    if (eatProgress > 1f + (1f / data.eatDuration))
    {
      activeFoods.remove(this);
    }
    sprRenderer.image =  data.eatFrames[floor(eatProgress * (data.eatFrames.length - 1f))];
  }

  public void Tick(float dt)
  {
    // Apply gravity
    if (pos.y < floorY)
      pos.y = constrain(pos.y + dt * gravitySpeed, 0, floorY);
  }

  public boolean IsFalling()
  {
    return pos.y < floorY;
  }
}

void CreateFood()
{
  FoodInstance newFood = new FoodInstance(new PVector(164f, 0f), foodData_Berry);
  activeFoods.add(newFood);
}
