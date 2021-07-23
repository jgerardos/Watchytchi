import processing.sound.*;

/*# Constants #*/
String savePath = "save/SaveData.json";
float maxHunger = 100;
int drawFrameRate = 16;
int tickFrameRate = 2;
float gravitySpeed = 75f;
float floorY = 200f-40f;
boolean doDebug = false;

/*# Assets #*/
PFont font_20;
Animation stepAnim;
Animation idleAnim;
Animation eatAnim;
Animation sleepAnim;
SoundFile sfx_VibeCursor;
SoundFile sfx_VibeFail;
SoundFile sfx_VibeSelect;
FoodData foodData_Berry;
PImage sunSprite;
PImage moonSprite;
PImage[] cloudSprites;
PImage statusMenuBackground;

/*# Turtles #*/
Creature creature;
ArrayList<Turtle> turtles = new ArrayList<Turtle>();
ArrayList<UIScreen> uiStack = new ArrayList<UIScreen>();
ArrayList<FoodInstance> activeFoods = new ArrayList<FoodInstance>();

/*# UI #*/
MainMenuUI mainMenu;
StatusScreen statusScreen;

/*# State #*/
JSONObject saveJson;
int cursorIdx;
float hunger = maxHunger;
float age = 0;
long lastUpdateTs;
long frameNumber = 0;

void RestoreFromDisk()
{
  try {
    saveJson = loadJSONObject(savePath);
  }
  // HACK: we'll get an exception if the file doesn't exist. There's probably a better way to check if it exists
  catch (Exception e) {
    // If file doesn't exist, create it
    PrintWriter writer = createWriter(savePath);
    writer.println("{}");
    writer.flush();
    saveJson = loadJSONObject(savePath);
    return;
  }
    
  age = saveJson.getFloat("age");
  hunger = saveJson.getFloat("hunger");
}

void SaveToDisk()
{
  println("In save method");
  saveJson.setFloat("age", age);
  saveJson.setFloat("hunger", hunger);
  saveJSONObject(saveJson, "save/SaveData.json");
}

void setup()
{
  // Initialize drawing parameters
  frameRate(drawFrameRate);
  noStroke();
  size(200, 200);

  // Load resources
  font_20 = loadFont("Cambria-20.vlw");
  sfx_VibeCursor = new SoundFile(this, "sfx_VibeCursor.wav");
  sfx_VibeFail = new SoundFile(this, "sfx_VibeFail.wav");
  sfx_VibeSelect = new SoundFile(this, "sfx_VibeSelect.wav");
  stepAnim = new Animation("DaisyHog_Step", 2, 0.5f);
  idleAnim = new Animation("DaisyHog_Idle", 2, 0.5f);
  eatAnim = new Animation("DaisyHog_Eat", 2, 0.5f);
  sleepAnim = new Animation("DaisyHog_Sleep", 2, 1f);
  sunSprite = loadImage("Sun_Dithered.png");
  moonSprite = loadImage("Moon_Dithered.png");
  cloudSprites = LoadImageArray("Cloud", 3);
  statusMenuBackground = loadImage("StatusBackground.png");

  // Initialize UI:
  mainMenu = new MainMenuUI();
  statusScreen = new StatusScreen();
  uiStack.add(mainMenu);

  // Load resources (food)
  foodData_Berry = new FoodData("FoodBerry_Stage", 7, 20f);

  // Initialize creature
  creature = new Creature(width / 2f, floorY);
  
  // Initialize Environment
  turtles.add(new HeavenlyBody());
  for (int i = 0; i < 2; i++)
    turtles.add(new Cloud());
    
  // Load save data
  RestoreFromDisk();
  
  lastUpdateTs = System.currentTimeMillis();
}

void exit()
{
  println("About to save");
  SaveToDisk();
  println("Saved");
  super.exit();
  println("Exited");
}

void draw()
{
  background(255, 255, 255);
  
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
    age += dt / 60f;
    creature.Tick(dt);
    for (int i = 0; i < activeFoods.size(); i++)
    {
      activeFoods.get(i).Tick(dt);
    }
    for (int i = 0; i < turtles.size(); i++)
    {
      turtles.get(i).Tick(dt);
    }
  }
  
  for (int i = 0; i < activeFoods.size(); i++)
  {
    activeFoods.get(i).Draw();
  }
  for (int i = 0; i < turtles.size(); i++)
  {
    turtles.get(i).Draw();
  }
  creature.Draw(dt);
  
  // Draw UI:
  for (int i = 0; i < uiStack.size(); i++)
  {
    uiStack.get(i).Draw();
  }
}

void keyPressed()
{
  for (int i = uiStack.size() - 1; i >= 0; i--)
  {
    if (uiStack.get(i).HandleInput())
      break;
  }
}

PImage[] LoadImageArray(String baseString, int numVariants)
{
  return LoadImageArray(baseString, numVariants, 1);
}

PImage[] LoadImageArray(String baseString, int numVariants, int startIdx)
{
  PImage[] output = new PImage[numVariants];
  for (int i = 0; i < numVariants; i++)
      output[i] = loadImage(baseString + (i + startIdx) + ".png");
  return output;
}


class Animation
{
  public float frameInterval;
  public PImage[] frames;

  Animation(String baseString, int numFrames, float intervalIn)
  {
    frames = LoadImageArray(baseString, numFrames);
    frameInterval = intervalIn;
  }

  Animation(PImage[] framesIn, float intervalIn)
  {
    frameInterval = intervalIn;
    frames = framesIn;
  }
}
