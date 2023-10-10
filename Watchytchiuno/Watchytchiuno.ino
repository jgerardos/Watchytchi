#include "Watchy_Watchytchi.h"
#include "Settings.h"

Watchytchi watchy(settings);

void setup() {
  Serial.begin(115200);
  watchy.init();
}

void loop(){
  
}
