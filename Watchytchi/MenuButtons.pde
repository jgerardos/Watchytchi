class MenuButton
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
  void draw()
  {
    image(cursorIdx == id ? activeIcon : inactiveIcon, xPos - size / 2f, yPos - size / 2f);
  }
  void Click()
  {
    sfx_Vibrate.play();
  }
}
class FoodButton extends MenuButton
{  
   FoodButton(int idIn, float xIn, float yIn, String name)
   {
     super(idIn, xIn, yIn, name);
   }
  
  void Click()
  {
    CreateFood();
  }
}
