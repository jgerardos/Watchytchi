/*# Constants #*/
int numButtons = 8; 
float buttonWidth = 32;
float maxHunger = 100;
int drawFrameRate = 16;
int tickFrameRate = 2;


/*# State #*/
int cursorIdx;
MenuButton[] menuButtons = new MenuButton[numButtons];
float hunger = maxHunger;
PImage dHogStep1;
PImage dHogStep2;
long lastUpdateTs;
long frameNumber = 0;

Creature creature;

void setup()
{
  // Initialize drawing parameters
  frameRate(drawFrameRate);
  noStroke();
  size(200, 200);

  // Load resources
  dHogStep1 = loadImage("DaisyHog_Step1.png");
  dHogStep2 = loadImage("DaisyHog_Step2.png");
  PImage activeIcon = loadImage("MenuIcon_Placeholder.png");
  PImage inactiveIcon = loadImage("MenuIcon_Placeholder_NoSelect.png");

  // Initialize creature
  creature = new Creature(width / 2f, height / 2f, new Animation(new PImage[]{dHogStep1, dHogStep2}, 0.5f));
  
  // Initialize Menu Buttons
  int numPerRow = menuButtons.length / 2;
  float separation = width / (float)numPerRow; 
  for (int i = 0; i < menuButtons.length; i++)
  {
    int column = i % numPerRow;
    float xPos = width * ((float)column / numPerRow) + (separation / 2);
    float yPos;
    if (i < numPerRow)
      yPos = buttonWidth / 2;
    else
      yPos = height - (buttonWidth / 2);
    if (i == 0)
      menuButtons[i] = new FoodButton(i, xPos, yPos, inactiveIcon, activeIcon);
    else
      menuButtons[i] = new MenuButton(i, xPos, yPos, inactiveIcon, activeIcon);
  }
  
  lastUpdateTs = System.currentTimeMillis();
}

void draw()
{
  background(255, 255, 255);
  for (int i = 0; i < menuButtons.length; i++)
  {
    menuButtons[i].draw();
  }
  
  int tickInterval = drawFrameRate / tickFrameRate;
  boolean isTickFrame = frameNumber % tickInterval == 0; 
  frameNumber++;
  
  float dt = 0f;
  // Calculate deltaTime
  if (isTickFrame)
  {
    long currentTs = System.currentTimeMillis();
    dt = (currentTs - lastUpdateTs) / 1000f;
    lastUpdateTs = currentTs;
    
    hunger -= dt;
    creature.Update(dt);
  }
  
  textSize(20);
  text("xPos: " + creature.xPos, 0, height / 2);
  
  creature.Draw(dt);
}

void keyPressed()
{
  if (key == ' ')
    cursorIdx = (cursorIdx + 1) % numButtons;
  if (key == 'a')
    menuButtons[cursorIdx].Click();
}


class Animation
{
  public float frameInterval;
  public PImage[] frames;

  Animation(PImage[] framesIn, float intervalIn)
  {
    frameInterval = intervalIn;
    frames = framesIn;
  }
}
