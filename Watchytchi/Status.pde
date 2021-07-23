class StatusScreen extends UIScreen
{
  public float descriptorInterval = 0.25f;
  public String[][] hungerDescriptors;
  private int descriptorIdx = -1;
  private int lastHungerTier = -1;
  
  public StatusScreen()
  {
    super();
    hungerDescriptors = new String[][]{
      new String[] {"Famished", "Starving", "Grumpy"}, // 0%
      new String[] {"Very hungry", "Yearning", "Insatiate"}, // 25%
      new String[] {"Hungry", "Unsatisfied", "Munchy"}, // 50%
      new String[] {"Peckish", "Craving", "Hankering"}, // 75%
      new String[] {"Full", "Stuffed", "Satiated"} // 100%
    };
  }
  
  public void Show()
  {
    UpdateDescriptor();
    super.Show();
  }
  
  void UpdateDescriptor()
  {
    float normalizedHunger = hunger / 100f;
    int hungerTier = round(normalizedHunger / descriptorInterval);
    if (lastHungerTier == -1 || lastHungerTier != hungerTier)
      descriptorIdx = floor(random(0, hungerDescriptors[hungerTier].length));
    lastHungerTier = hungerTier;
  }

  public void Tick(float dt)
  {
    UpdateDescriptor();
  }

  public void Draw()
  {
    float xPadding = (width - statusMenuBackground.width) / 2f;
    float yPadding = 2f;
    image(statusMenuBackground, xPadding, yPadding);

    fill(0, 0, 0);
    textFont(font_20);
    text(hungerDescriptors[lastHungerTier][descriptorIdx], 14, 30);
    text(round(age) + " yr", 135, 30);
  }

  public boolean HandleInput()
  {
    // Back button exits
    if (key == 'c')
    {
      sfx_VibeFail.play();
      Hide();
    }

    // Consume input in all cases
    return true;
  }
}
