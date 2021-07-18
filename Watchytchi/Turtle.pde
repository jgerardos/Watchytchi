public abstract class Turtle
{
  public PVector pos;
  public Renderer renderer;

  public Turtle(PVector posIn)
  {
    pos = posIn;
  }
  
  // R00 TODO: move to factory method(?)
  public Turtle(PVector posIn, PImage image, PVector anchor)
  {
    pos = posIn;
    renderer = new SpriteRenderer(this, image, anchor);
  }

  public void Tick(float dt) {};
  
  public void Draw()
  {
    if (renderer != null)
      renderer.Draw();
  }
}

public abstract class Renderer
{
  public Turtle owner;
  public Renderer(Turtle ownerIn)
  {
    owner = ownerIn;
  }
  public void Tick() {}
  public void Draw()
  {
    if (doDebug)
    {
      fill(255, 0, 255);
      square(owner.pos.x, owner.pos.y, 2);
    }
  }
}

public class SpriteRenderer extends Renderer
{
  public PImage image;
  public PVector anchor;
  
  public SpriteRenderer(Turtle ownerIn, PImage imageIn, PVector anchorIn)
  {
    super(ownerIn);
    image = imageIn;
    anchor = anchorIn;
  }
  
  public void Draw()
  {
    image(image, owner.pos.x + anchor.x * -image.width, owner.pos.y - anchor.y * image.height);
    super.Draw();
  }
}
