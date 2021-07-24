public enum CState {IdleWander, PursueFood, Sleep}

public class Creature extends Turtle
{
  /*# Brain State #*/
  public CState state;

  /*# Locomotion State #*/
  float desiredX = width;
  int faceDirection = 0;
  float speed = 20f;
  PVector size = new PVector(0f, 0f);
  PVector awakeDurationRange = new PVector (40f, 70);
  PVector sleepDurationRange = new PVector (90f, 120f);
  float sleepTimer;
  float poopTimer;
  boolean isTickingPoopTimer;

  /*# Components #*/
  AnimatedRenderer animator;


  void SetAnim(AnimationData newAnim)
  {
    if (newAnim != animator.anim)
    {
      animator.anim = newAnim;
      animator.Reset();
    }
  }

  Creature(PVector pos)
  {
    super(pos);
    for (int i = 0; i < idleAnim.frames.length; i++)
    {
      size.x = max(size.x, idleAnim.frames[i].width);
      size.y = max(size.y, idleAnim.frames[i].height);
    }

    sleepTimer = random(awakeDurationRange.x, awakeDurationRange.y);
    state = CState.IdleWander;
    animator = AddAnimRenderer(null, new PVector(0.5f, 1.0f));
  }

  void Tick(float dt)
  {
    super.Tick(dt);
    sleepTimer -= dt;
    if (sleepTimer < 0)
    {
      if (state == CState.Sleep)
      {
        sleepTimer = random(awakeDurationRange.x, awakeDurationRange.y);
        state = CState.IdleWander;
      } 
      else
      {
        sleepTimer = random(sleepDurationRange.x, sleepDurationRange.y);
        state = CState.Sleep;
        SetAnim(sleepAnim);
      }
    }

    if (state != CState.Sleep && activeFoods.size() != 0 && state != CState.PursueFood)
      state = CState.PursueFood;
    else if (activeFoods.size() == 0 && state == CState.PursueFood)
    {
      state = CState.IdleWander;
      faceDirection = 0;
    }

    if (state == CState.IdleWander)
    {
      SetAnim(stepAnim);

      // Basic ping ponging walk behavior
      if (faceDirection == 0 || (faceDirection == -1 && pos.x < 0 + size.x / 2f))
        desiredX = width + 20;
      else if (faceDirection == 1 && pos.x > width - size.x / 2f)
        desiredX = -20;
    }
    else if (state == CState.PursueFood)
    {
      if (activeFoods.get(0).IsFalling())
        desiredX = 60;
      else
        desiredX = 120f;

      if (IsAtTarget())
      {
        faceDirection = 1;
        animator.scale = new PVector(faceDirection, 1f);
        if (activeFoods.get(0).IsFalling())
          SetAnim(idleAnim);
        else
        {
          SetAnim(eatAnim);
          activeFoods.get(0).TickEat(dt);
          if (!isTickingPoopTimer)
          {
            poopTimer = constrain(poopTimer + dt, 0f, 20f);
            if (poopTimer >= 20f)
              isTickingPoopTimer = true;
          }
        }
      }
    }

    float deltaToTarget = desiredX - pos.x;
    if (!IsAtTarget() && state != CState.Sleep)
    {
      faceDirection = deltaToTarget > 0 ? 1 : -1;
      animator.scale = new PVector(faceDirection, 1f);
      float walkDelta = min(abs(deltaToTarget), speed * dt) * faceDirection;
      pos.x += walkDelta;
      SetAnim(stepAnim);
    }

    if (isTickingPoopTimer && state != CState.Sleep)
    {
      poopTimer = constrain(poopTimer - dt, 0f, 20f);
      if (poopTimer <= 0)
        Poop();
    }
  }

  boolean IsAtTarget()
  {
    return abs(desiredX - pos.x) <= 0.000001f;
  }

  void Poop()
  {
    poopManager.AddPoop(new PVector(pos.x - size.x / 2f * faceDirection, floorY));
    isTickingPoopTimer = false;
  }
}
