public abstract class Renderer
{
  public Turtle owner;
  public Renderer(Turtle ownerIn)
  {
    owner = ownerIn;
  }
  public void Tick(float dt) {}
  public void Draw()
  {
    if (doDebug)
    {
      fill(255, 0, 255);
      square(owner.pos.x, owner.pos.y, 2);
    }
  }

  public void DrawSprite(PVector pos, PImage image, PVector anchor, PVector scale)
  {
    if (image != null)
    {
      pushMatrix();
      translate(pos.x, pos.y);
      if (scale.x != 1f || scale.y != 1f)
        scale(scale.x, scale.y);
      image(image, anchor.x * -image.width, - anchor.y * image.height);
      popMatrix();
    }
  }
}

public class SpriteRenderer extends Renderer
{
  public PImage image;
  public PVector anchor;
  public PVector scale = new PVector(1f, 1f);
  
  public SpriteRenderer(Turtle ownerIn, PImage imageIn, PVector anchorIn)
  {
    super(ownerIn);
    image = imageIn;
    anchor = anchorIn;
  }
  
  public void Draw()
  {
    DrawSprite(owner.pos, image, anchor, scale);
    super.Draw();
  }
}

public class AnimatedRenderer extends Renderer
{
  /*# Config #*/
  public AnimationData anim;
  public PVector anchor;
  public PVector scale = new PVector(1f, 1f);

  /*# State #*/
  int frameIdx = 0;
  int frameSkipIdx = 0;
  
  public AnimatedRenderer(Turtle ownerIn, AnimationData animIn, PVector anchorIn)
  {
    super(ownerIn);
    anim = animIn;
    anchor = anchorIn;
  }

  public void Tick(float dt)
  {
    if (anim == null)
      return;
    
    frameSkipIdx = (frameSkipIdx + 1) % (anim.numFrameSkips + 1);
    if (frameSkipIdx == anim.numFrameSkips)
      frameIdx = (frameIdx + 1) % anim.frames.length;
    super.Tick(dt);
  }
  
  public void Draw()
  {
    if (anim == null)
      return;

    PImage image = anim.frames[frameIdx];
    DrawSprite(owner.pos, image, anchor, scale);
    super.Draw();
  }

  public void Reset()
  {
    frameIdx = 0;
    frameSkipIdx = 0;
  }
}
