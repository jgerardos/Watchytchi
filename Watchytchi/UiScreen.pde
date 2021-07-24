public abstract class UIScreen extends Turtle
{
  public boolean HandleInput()
  {
    return false;
  }

  public void Show()
  {
    uiStack.add(this);
  }

  public void Hide()
  {
    uiStack.remove(this);
  }
}

public class MainMenuUI extends UIScreen
{
  /*# Constants #*/
  int numButtons = 8; 
  float buttonWidth = 32;
  
  /*# State #*/
  public MenuButton[] menuButtons;
  int cursorIdx = -1;

  public MainMenuUI()
  {
    menuButtons = new MenuButton[numButtons];
    // Initialize Menu Buttons
    int numPerRow = menuButtons.length / 2;
    float separation = width / (float)numPerRow; 
    for (int i = 0; i < menuButtons.length; i++)
    {
      int column = i % numPerRow;
      float xPos = width * ((float)column / numPerRow) + (separation / 2);
      float yPos;
      if (i < numPerRow)
        yPos = buttonWidth / 2 + 1;
      else
        yPos = height - (buttonWidth / 2) - 1;
      if (i == 0)
        menuButtons[i] = new StatusButton(i, xPos, yPos, "Status");
      else if (i == 2)
        menuButtons[i] = new FoodButton(i, xPos, yPos, "Feed");
      else if (i == 5)
        menuButtons[i] = new LightsButton(i, xPos, yPos, "Lights");
      else if (i == 7)
        menuButtons[i] = new MenuButton(i, xPos, yPos, "Read");
      else
        menuButtons[i] = new MenuButton(i, xPos, yPos, "Placeholder");
    }
  }

  public void Draw()
  {
    super.Draw();
    for (int i = 0; i < menuButtons.length; i++)
    {
      menuButtons[i].Draw();
    }
  }

  public boolean HandleInput()
  {
    if (key == 'z')
    {
      sfx_VibeCursor.play();
      cursorIdx = (cursorIdx + 1) % numButtons;
    }
    if (key == 'x')
    {
      if (cursorIdx >= 0)
        menuButtons[cursorIdx].Click();
      else
        sfx_VibeFail.play();
    }
    if (key == 'c')
    {
      sfx_VibeFail.play();
      cursorIdx = -1;
    }
    return true;
  }
}
