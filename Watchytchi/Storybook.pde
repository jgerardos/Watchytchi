class StorybookScreen extends UIScreen
{
  final int numSentences = 3;
  int sentenceIdx = 0;
  String sentence;
  int charIndex = 0;
  int numNewlines = 0;

  void Show()
  {
    sentenceIdx = 0;
    GoToNextSentence();
    super.Show();
  }
  
  void GoToNextSentence()
  {
    charIndex = 0;
    sentence = velveteenData.getString(velveteenIndex);
    numNewlines = 0;
    velveteenIndex++;
    sentenceIdx++;
  }

  public boolean BlocksDrawingBelow()
  {
    return true;
  }

  void Tick(float dt)
  {
    if (charIndex == sentence.length())
    {
      if (sentenceIdx == numSentences - 1)
      {
        Hide();
        return;
      }
      else  
      {
        GoToNextSentence();
        AdvanceToNextWord();
      }
      
      return;
    }

    AdvanceToNextWord();

    super.Tick(dt);
  }

  void AdvanceToNextWord()
  {
    charIndex++;
    while (charIndex < sentence.length() && sentence.charAt(charIndex) != ' ')
      charIndex++;

    while(charIndex < sentence.length() - 1 && sentence.charAt(charIndex + 1) == ' ')
      charIndex++;
    
    int numDesiredNewlines = charIndex / 15;
    if (numNewlines < numDesiredNewlines && charIndex < sentence.length())
    {
      println("CharIndex is " + charIndex);
      sentence = sentence.substring(0, charIndex) + "\n" + sentence.substring(charIndex + 1, sentence.length());
      numNewlines = numDesiredNewlines;
      charIndex++;
    }
    
    println("Advanced to next word, my sentence is now " + sentence.substring(0, charIndex));
  }

  void Draw()
  {
    fill(0, 0, 0);
    textFont(font_20);
    text(sentence.substring(0, charIndex), 14, 30);
  }

  boolean HandleInput()
  {
    return true;
  }
}
