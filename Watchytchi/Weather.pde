public static class TimeAndWeather
{
  public enum TimeOfDay {Morning, Afternoon, Evening, Night}
  
  public static TimeOfDay GetTimeOfDay()
  {
    int hr = hour();
    if (hr >= 6 && hr <= 11)
        return TimeOfDay.Morning;
    if (hr >= 12 && hr <= 16)
        return TimeOfDay.Afternoon;
    if (hr >= 17 && hr <= 19)
        return TimeOfDay.Evening;
    if (hr >= 20 || hr <= 5)
        return TimeOfDay.Night;
    throw new IllegalStateException();
  }
}

public class HeavenlyBody extends Turtle
{
  PVector heightRange = new PVector(30f, 120f); 
  public HeavenlyBody()
  {
    super(new PVector(0f, 0f), sunSprite, new PVector(0.5f, 0.5f));
  }
  
  public void Tick(float dt)
  {
    super.Tick(dt);
    PImage desiredSprite = GetDesiredSprite();
    if (((SpriteRenderer)renderer).image != desiredSprite)
      ((SpriteRenderer)renderer).image = desiredSprite;
      
    // HACK: I feel like the sun got pretty messy when it was all over the screen. Going to try locking it for now
    // pos = new PVector(160f, 60f);
    
    // Position according to time of day:   
    float hourF = hour() + (minute() / 60f);
    //float hourF = ((float)second()) % 24f; // Uncomment this to fast debug the sun
    float t = ((hourF + 18f) % 12) / 12f;
    float theta = t * PI;
    
    pos = new PVector(map(cos(theta), -1f, 1f, 0, 1f) * width, lerp(heightRange.x, heightRange.y, (1f - abs(sin(theta)))));

  }

  PImage GetDesiredSprite()
  {
    switch (TimeAndWeather.GetTimeOfDay())
    {
      case Morning : return sunSprite;
      case Afternoon : return sunSprite;
      case Evening : return moonSprite;
      case Night : return moonSprite;
      default: throw new IllegalStateException();
    }
  }
}

public class Cloud extends Turtle
{
  PVector spawnDelayRange = new PVector(2f, 10f);
  PVector startYRange = new PVector(40f, 80f);
  PVector speedRange = new PVector(0.05f, 0.3f);
  float speed;
  SpriteRenderer sprRenderer;
  float respawnTimer;

  public Cloud()
  {
    super();
    sprRenderer = AddSpriteRenderer(null, new PVector(0.5f, 0.5f));
    Spawn();
    pos.x = random(0.2, width * 2.5f);
  }

  public void Spawn()
  {
    PImage sprite = cloudSprites[(int)random(0, cloudSprites.length)];
    sprRenderer.image = sprite;
    pos = new PVector(width + sprite.width / 2f, random(startYRange.x, startYRange.y));
    speed = random(speedRange.x, speedRange.y);
    respawnTimer = random(spawnDelayRange.x, spawnDelayRange.y);
  }

  public void Tick(float dt)
  {
    pos.x -= speed;
    if (pos.x < -sprRenderer.image.width )
    {
      respawnTimer -= dt;
      if (respawnTimer <= 0f)
        Spawn();
    }
  }
}
