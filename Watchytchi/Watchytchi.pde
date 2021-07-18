import processing.sound.*;

/*# Constants #*/
int numButtons = 8; 
float buttonWidth = 32;
float maxHunger = 100;
int drawFrameRate = 16;
int tickFrameRate = 2;
float gravitySpeed = 75f;
float floorY = 200f-40f;
boolean doDebug = true;

/*# Assets #*/
Animation stepAnim;
Animation idleAnim;
Animation eatAnim;
SoundFile sfx_VibeCursor;
SoundFile sfx_VibeFail;
SoundFile sfx_VibeSelect;
FoodData foodData_Berry;

/*# Turtles #*/
Creature creature;
MenuButton[] menuButtons = new MenuButton[numButtons];
ArrayList<FoodInstance> activeFoods = new ArrayList<FoodInstance>();

/*# State #*/
int cursorIdx;
float hunger = maxHunger;
long lastUpdateTs;
long frameNumber = 0;


void setup()
{
  // Initialize drawing parameters
  frameRate(drawFrameRate);
  noStroke();
  size(200, 200);

  // Load resources
  sfx_VibeCursor = new SoundFile(this, "sfx_VibeCursor.wav");
  sfx_VibeFail = new SoundFile(this, "sfx_VibeFail.wav");
  sfx_VibeSelect = new SoundFile(this, "sfx_VibeSelect.wav");
  stepAnim = new Animation("DaisyHog_Step", 2, 0.5f);
  idleAnim = new Animation("DaisyHog_Idle", 2, 0.5f);
  eatAnim = new Animation("DaisyHog_Eat", 2, 0.5f);


  // Load resources (food)
  int k_numEatFrames = 7;
  PImage[] berryFrames = new PImage[k_numEatFrames];
  for (int i = 0 ; i < k_numEatFrames; i++)
    berryFrames[i] = loadImage("FoodBerry_Stage" + i + ".png");
  foodData_Berry = new FoodData(berryFrames, 20f);

  // Initialize creature
  creature = new Creature(width / 2f, floorY);
  
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
    if (i == 2)
      menuButtons[i] = new FoodButton(i, xPos, yPos, "Feed");
    else
      menuButtons[i] = new MenuButton(i, xPos, yPos, "Placeholder");
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
    creature.Tick(dt);
    for (int i = 0; i < activeFoods.size(); i++)
    {
      activeFoods.get(i).Tick(dt);
    }
  }
  
  // textSize(20);
  // text("xPos: " + creature.xPos, 0, height / 2);
  
  for (int i = 0; i < activeFoods.size(); i++)
  {
    activeFoods.get(i).Draw();
  }
  creature.Draw(dt);
}

void keyPressed()
{
  if (key == 'z')
  {
    sfx_VibeCursor.play();
    cursorIdx = (cursorIdx + 1) % numButtons;
  }
  if (key == 'x')
    menuButtons[cursorIdx].Click();
  if (key == 'c')
  {
     sfx_VibeFail.play();
     cursorIdx = 0;
  }
   
}


class Animation
{
  public float frameInterval;
  public PImage[] frames;

  Animation(String baseString, int numFrames, float intervalIn)
  {
    frames = new PImage[numFrames];
    for (int i = 0; i < numFrames; i++)
      frames[i] = loadImage(baseString + (i + 1) + ".png");
    frameInterval = intervalIn;
  }

  Animation(PImage[] framesIn, float intervalIn)
  {
    frameInterval = intervalIn;
    frames = framesIn;
  }
}
