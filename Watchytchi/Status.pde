class StatusScreen extends UIScreen
{
  public void Draw()
  {
    float xPadding = (width - statusMenuBackground.width) / 2f;
    float yPadding = 2f;
    image(statusMenuBackground, xPadding, yPadding);

    fill(0, 0, 0);
    textSize(20);
    text("Hunger: " + round(hunger), 30, 30);
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
