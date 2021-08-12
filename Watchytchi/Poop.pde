class PoopManager
{
  Turtle[] poopColumns = new Turtle[5];

  public void AddPoop(PVector targetPos)
  {
    int nearestColumn = floor(targetPos.x / 40f);  
    int dir = targetPos.x % 40f > 20 ? 1 : -1;
    while (nearestColumn >= 0 && nearestColumn < 5 && poopColumns[nearestColumn] != null)
      nearestColumn += dir;
    if (nearestColumn < 0 || nearestColumn >= 5)      
      return;
    
    CreatePoop(nearestColumn);
    sfx_VibeCursor.play();
  }

  private void CreatePoop(int column)
  {
    Turtle poop = new Turtle(new PVector(column * 40f + 20f, floorY));
    poop.AddAnimRenderer(poopAnim, new PVector(0.5f, 1.0f));
    turtles.add(poop);
    poopColumns[column] = poop;
  }

  public void RemovePoop(int column)
  {
    turtles.remove(poopColumns[column]);
    poopColumns[column] = null;
  }

  void RestoreFromDisk(JSONObject obj)
  {
    if (obj.isNull("poops"))  
      return;
    JSONArray array = obj.getJSONArray("poops");
    for (int i = 0; i < array.size(); i++)
    {
      if (array.getBoolean(i))
      {
        CreatePoop(i);
      }
    }
  }

  void SaveToDisk(JSONObject obj)
  {
    JSONArray poopBools = new JSONArray();
    for (int i = 0; i < poopColumns.length; i++)
    {
      poopBools.setBoolean(i, poopColumns[i] != null);
    }
    obj.setJSONArray("poops", poopBools);
  }
}
