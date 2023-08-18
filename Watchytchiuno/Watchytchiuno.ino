#include "Watchy_Watchytchi.h"
#include "Settings.h"

Watchytchi watchy(settings);

void setup() {
  Serial.begin(115200);
  Serial.print("Setup!!");
  Serial.println();
  watchy.init();
}

void loop(){
  
}
