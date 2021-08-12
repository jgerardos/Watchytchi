class CleaningScreen extends UIScreen
{
  int cursorColumn = 0;
  PImage img_cursor;

  public CleaningScreen()
  {
    img_cursor = loadImage("ColumnCursor.png");
  }

  public boolean HandleInput()
  {
    // Back button exits
    if (key == key_back)
    {
      sfx_VibeFail.play();
      Hide();
    }

    if (key == key_cursor)
    {
      cursorColumn = (cursorColumn + 1) % 5;
      sfx_VibeCursor.play();
    }
    else if (key == key_select)
    {
      poopManager.RemovePoop(cursorColumn);
      sfx_VibeSelect.play();
    }


    // Consume input in all cases
    return true;
  }

  public void Draw()
  {
    image(img_cursor, cursorColumn * 40f, 36f);
    super.Draw();
  }
}
