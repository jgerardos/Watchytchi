public class Turtle
{
  public PVector pos;
  public Renderer renderer;

  public Turtle()
  {
    pos = new PVector(0f, 0f);
  }

  public Turtle(PVector posIn)
  {
    pos = posIn;
  }
  
  // R00 TODO: move to factory method(?)
  public Turtle(PVector posIn, PImage image, PVector anchor)
  {
    pos = posIn;
    AddSpriteRenderer(image, anchor);
  }

  public void Tick(float dt) 
  {
    if (renderer != null)
      renderer.Tick(dt);
  };
  
  public void Draw()
  {
    if (renderer != null)
      renderer.Draw();
  }

  public SpriteRenderer AddSpriteRenderer(PImage image, PVector anchor)
  {
    SpriteRenderer output = new SpriteRenderer(this, image, anchor);
    renderer = output;
    return output;
  }

  public AnimatedRenderer AddAnimRenderer(AnimationData anim, PVector anchor)
  {
    AnimatedRenderer output = new AnimatedRenderer(this, anim, anchor);
    renderer = output;
    return output;
  }
}
