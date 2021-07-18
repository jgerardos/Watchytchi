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
  float arcFloor = 100f; 
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
      
    // Position according to time of day:   
    
    float hourF = hour() + (minute() / 60f);
    //float hourF = ((float)second()) % 24f; // Uncomment this to fast debug the sun
    float t = ((hourF + 18f) % 12) / 12f;
    text("" + hourF, width / 2f, height / 2f);
    float theta = t * PI;
    
    pos = new PVector(map(cos(theta), -1f, 1f, 0, 1f) * width, arcFloor * (1f - abs(sin(theta))));
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
