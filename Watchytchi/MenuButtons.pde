class MenuButton
{
  int id;
  float xPos, yPos;
  float size;
  PImage inactiveIcon;
  PImage activeIcon;
  MenuButton(int idIn, float xIn, float yIn, PImage inactiveIconIn, PImage activeIconIn)
  {
    id = idIn;
    xPos = xIn;
    yPos = yIn;
    inactiveIcon = inactiveIconIn;
    activeIcon = activeIconIn;
    size = inactiveIconIn.width; 
  }
  void draw()
  {
    image(cursorIdx == id ? activeIcon : inactiveIcon, xPos - size / 2f, yPos - size / 2f);
  }
  void Click()
  {
    hunger = hunger + 1;
  }
}
class FoodButton extends MenuButton
{  
   FoodButton(int idIn, float xIn, float yIn, PImage inactiveIconIn, PImage activeIconIn)
   {
     super(idIn, xIn, yIn, inactiveIconIn, activeIconIn);
   }
  
  void Click()
  {
    hunger += 10;
  }
}
