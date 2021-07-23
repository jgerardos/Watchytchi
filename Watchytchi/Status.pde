class StatusScreen extends UIScreen
{
  public void Draw()
  {
    float xPadding = (width - statusMenuBackground.width) / 2f;
    float yPadding = 2f;
    image(statusMenuBackground, xPadding, yPadding);

    fill(0, 0, 0);
    textFont(font_20);
    text("Hunger " + round(hunger), 14, 30);
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
