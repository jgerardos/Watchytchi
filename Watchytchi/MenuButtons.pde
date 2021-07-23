class MenuButton extends Turtle
{
  int id;
  float xPos, yPos;
  float size;
  PImage inactiveIcon;
  PImage activeIcon;

  MenuButton(int idIn, float xIn, float yIn, String name)
  {
    id = idIn;
    xPos = xIn;
    yPos = yIn;
    inactiveIcon = loadImage("MenuIcon_" + name + "_Inactive.png");
    activeIcon = loadImage("MenuIcon_" + name + "_Active.png");
    size = inactiveIcon.width; 
  }

  public void Draw()
  {
    image(mainMenu.cursorIdx == id ? activeIcon : inactiveIcon, xPos - size / 2f, yPos - size / 2f);
  }

  public void Click()
  {
    sfx_VibeFail.play();
  }
}

class FoodButton extends MenuButton
{  
   FoodButton(int idIn, float xIn, float yIn, String name)
   {
     super(idIn, xIn, yIn, name);
   }
  
  public void Click()
  {
    sfx_VibeSelect.play();
    CreateFood();
  }
}

class StatusButton extends MenuButton
{
  StatusButton(int idIn, float xIn, float yIn, String name)
  {
    super(idIn, xIn, yIn, name);
  }
  
  public void Click()
  {
    sfx_VibeSelect.play();
    statusScreen.Show();
  }
}

class LightsButton extends MenuButton
{
  LightsButton(int idIn, float xIn, float yIn, String name)
  {
    super(idIn, xIn, yIn, name);
  }
  
  public void Click()
  {
    areLightsOn = !areLightsOn;
    sfx_VibeSelect.play();
  }
}