//Derived from peerdavid's source at: https://github.com/peerdavid/Watchy
#include "Watchy_Base.h"

RTC_DATA_ATTR int16_t alarm_timer = -1;
RTC_DATA_ATTR bool twelve_mode = true;
RTC_DATA_ATTR bool sleep_mode = false;
RTC_DATA_ATTR bool playAnim = false;
RTC_DATA_ATTR int menuIdx;
RTC_DATA_ATTR float hunger = 1.f;
RTC_DATA_ATTR bool isEating = false;
RTC_DATA_ATTR int idleAnimIdx = 0;
RTC_DATA_ATTR bool hasStatusDisplay = false;
RTC_DATA_ATTR int lastAdvanceIdxMinute = 0;
RTC_DATA_ATTR bool invertColors = false;
RTC_DATA_ATTR int lastHungerCryMinute = -1;
RTC_DATA_ATTR int lastSecMatch = 0;
RTC_DATA_ATTR int lastAnimateMinute = 0;
RTC_DATA_ATTR bool isPeriodicAnim = false;
RTC_DATA_ATTR int dayBorn = -1;

WatchyBase::WatchyBase() {}

void WatchyBase::init() {
  wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
  Wire.begin(SDA, SCL); //init i2c

  auto shouldSleep = true;
  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm

      // Handle classical tick
      RTC.setAlarm(ALM1_MATCH_SECONDS, lastSecMatch, 0, 0, 0); //alarm wakes up Watchy every quarter minute
      lastSecMatch = (lastSecMatch + 15) % 60;
      RTC.alarm(ALARM_1); //resets the alarm flag in the RTC1
      
      RTC.alarm(ALARM_2); //resets the alarm flag in the RTC

      if (guiState == WATCHFACE_STATE) {
        RTC.read(currentTime);
        playAnim = true;
        showWatchFace(true); //partial updates on tick
      }
      break;

    case ESP_SLEEP_WAKEUP_EXT1: //button Press + no handling if wakeup
      if (sleep_mode) {
        sleep_mode = false;
        RTC.alarmInterrupt(ALARM_2, true);
        RTC.alarm(ALARM_2); //resets the alarm flag in the RTC

        RTC.read(currentTime);
        showWatchFace(false); //full update on wakeup from sleep mode
        break;
      }

      shouldSleep &= handleButtonPress();
      break;

    default: //reset
      _rtcConfig();
      _bmaConfig();
      showWatchFace(true); //full update on reset
      break;
  }

  // Sometimes BMA crashes - simply try to reinitialize bma...
  if (sensor.getErrorCode() != 0) {
    sensor.shutDown();
    sensor.wakeUp();
    sensor.softReset();
    _bmaConfig();
  }

//  if (shouldSleep)
    deepSleep();
//  else
//  {
//    Serial.print("Skipped deep sleep because cursor button was pressed");
//    Serial.println();
//  }
}

void WatchyBase::deepSleep() {
  esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
  esp_sleep_enable_ext1_wakeup(EXT_INT_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
  esp_deep_sleep_start();
}

bool WatchyBase::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

  if (IS_DOUBLE_TAP) {
    while (!sensor.getINT()) {
      // Wait until interrupt is cleared.
      // Otherwise it will fire again and again.
    }

    // To be defined in the watch face what we want exactly
    // to do. Therefore, no return;
  }

  if (IS_KEY_SELECT) {
    hasStatusDisplay = menuIdx == 0;
    if (menuIdx == 2)
      isEating = true;
    if (menuIdx == 5 && (currentTime.Hour >= 21 || currentTime.Hour <= 6))
      invertColors = !invertColors;
    RTC.read(currentTime);
    if (menuIdx == 0 || menuIdx == 2 || (menuIdx == 5 && (currentTime.Hour >= 21 || currentTime.Hour <= 6)))
      vibrate(1, 50);
    showWatchFace(true);
    return true;
  }

  if (IS_KEY_CURSOR) {
    RTC.read(currentTime);
    menuIdx = (menuIdx + 1) % 8;
    vibrate();
    lastAdvanceIdxMinute = currentTime.Minute;

    // Partial redraw
    startProfile();
//    display.init(0, false); //_initial_refresh to false to prevent full update on init
//    display.setFullWindow();
//    showWatchFace(true);
//    //drawUIButton(menuIdx-1, true);
//    //drawUIButton(menuIdx, true);
//    //display.display(true); //partial refresh
//    // display.hibernate();
//    guiState = WATCHFACE_STATE;
    showWatchFace(true);

    endProfile("Partial cursor update");
    return false;
  }

  if (IS_KEY_CANCEL) {
    RTC.read(currentTime);
    vibrate();
    menuIdx = -1;
    vibrate(1, 30);
    showWatchFace(true);
    return true;
  }

  Watchy::handleButtonPress();
}

void WatchyBase::vibrate(uint8_t times, uint32_t delay_time) {
  // Ensure that no false positive double tap is produced
  sensor.enableFeature(BMA423_WAKEUP, false);

  pinMode(VIB_MOTOR_PIN, OUTPUT);
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(VIB_MOTOR_PIN, true);
    delay(delay_time);
    digitalWrite(VIB_MOTOR_PIN, false);
    if (i != times - 1)
      delay(delay_time);
  }

  sensor.enableFeature(BMA423_WAKEUP, true);
}

void WatchyBase::_rtcConfig() {
  //https://github.com/JChristensen/DS3232RTC
  RTC.squareWave(SQWAVE_NONE); //disable square wave output
  //RTC.set(compileTime()); //set RTC time to compile time
  RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
  RTC.alarmInterrupt(ALARM_2, true); //enable alarm interrupt
  RTC.setAlarm(ALM1_MATCH_SECONDS, lastSecMatch, 0, 0, 0); //alarm wakes up Watchy every half minute
  lastSecMatch = (lastSecMatch + 15) % 60;
  RTC.alarmInterrupt(ALARM_1, true); //enable alarm interrupt
  RTC.read(currentTime);
}

void WatchyBase::_bmaConfig() {

  if (sensor.begin(_readRegister, _writeRegister, delay) == false) {
    //fail to init BMA
    return;
  }

  // Accel parameter structure
  Acfg cfg;
  /*!
      Output data rate in Hz, Optional parameters:
          - BMA4_OUTPUT_DATA_RATE_0_78HZ
          - BMA4_OUTPUT_DATA_RATE_1_56HZ
          - BMA4_OUTPUT_DATA_RATE_3_12HZ
          - BMA4_OUTPUT_DATA_RATE_6_25HZ
          - BMA4_OUTPUT_DATA_RATE_12_5HZ
          - BMA4_OUTPUT_DATA_RATE_25HZ
          - BMA4_OUTPUT_DATA_RATE_50HZ
          - BMA4_OUTPUT_DATA_RATE_100HZ
          - BMA4_OUTPUT_DATA_RATE_200HZ
          - BMA4_OUTPUT_DATA_RATE_400HZ
          - BMA4_OUTPUT_DATA_RATE_800HZ
          - BMA4_OUTPUT_DATA_RATE_1600HZ
  */
  cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
  /*!
      G-range, Optional parameters:
          - BMA4_ACCEL_RANGE_2G
          - BMA4_ACCEL_RANGE_4G
          - BMA4_ACCEL_RANGE_8G
          - BMA4_ACCEL_RANGE_16G
  */
  cfg.range = BMA4_ACCEL_RANGE_2G;
  /*!
      Bandwidth parameter, determines filter configuration, Optional parameters:
          - BMA4_ACCEL_OSR4_AVG1
          - BMA4_ACCEL_OSR2_AVG2
          - BMA4_ACCEL_NORMAL_AVG4
          - BMA4_ACCEL_CIC_AVG8
          - BMA4_ACCEL_RES_AVG16
          - BMA4_ACCEL_RES_AVG32
          - BMA4_ACCEL_RES_AVG64
          - BMA4_ACCEL_RES_AVG128
  */
  cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

  /*! Filter performance mode , Optional parameters:
      - BMA4_CIC_AVG_MODE
      - BMA4_CONTINUOUS_MODE
  */
  cfg.perf_mode = BMA4_CONTINUOUS_MODE;

  // Configure the BMA423 accelerometer
  sensor.setAccelConfig(cfg);

  // Enable BMA423 accelerometer
  // Warning : Need to use feature, you must first enable the accelerometer
  // Warning : Need to use feature, you must first enable the accelerometer
  sensor.enableAccel();

  struct bma4_int_pin_config config ;
  config.edge_ctrl = BMA4_LEVEL_TRIGGER;
  config.lvl = BMA4_ACTIVE_HIGH;
  config.od = BMA4_PUSH_PULL;
  config.output_en = BMA4_OUTPUT_ENABLE;
  config.input_en = BMA4_INPUT_DISABLE;
  // The correct trigger interrupt needs to be configured as needed
  sensor.setINTPinConfig(config, BMA4_INTR1_MAP);

  struct bma423_axes_remap remap_data;
  remap_data.x_axis = 1;
  remap_data.x_axis_sign = 0xFF;
  remap_data.y_axis = 0;
  remap_data.y_axis_sign = 0xFF;
  remap_data.z_axis = 2;
  remap_data.z_axis_sign = 0xFF;
  // Need to raise the wrist function, need to set the correct axis
  sensor.setRemapAxes(&remap_data);

  // Enable BMA423 isStepCounter feature
  sensor.enableFeature(BMA423_STEP_CNTR, true);
  // Enable BMA423 isTilt feature
  sensor.enableFeature(BMA423_TILT, true);
  // Enable BMA423 isDoubleClick feature
  //sensor.enableFeature(BMA423_WAKEUP, true);

  // Reset steps
  //sensor.resetStepCounter();

  // Turn on feature interrupt
  //sensor.enableStepCountInterrupt();
  //sensor.enableTiltInterrupt();
  // It corresponds to isDoubleClick interrupt
  //sensor.enableWakeupInterrupt();
}
uint16_t WatchyBase::_readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)address, (uint8_t)len);
  uint8_t i = 0;
  while (Wire.available()) {
    data[i++] = Wire.read();
  }
  return 0;
}

uint16_t WatchyBase::_writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data, len);
  return (0 !=  Wire.endTransmission());
}

void WatchyBase::startProfile()
{
  profileMs[nextProfileIdx] = millis();
  nextProfileIdx++;
}

int WatchyBase::endProfile(char* label)
{
  auto idx = --nextProfileIdx;
  auto ms = millis();
  Serial.print("Profile label ");
  Serial.print(label);
  Serial.print(" took ");
  Serial.print(millis() - profileMs[idx]);
  Serial.print(" ms");
  Serial.println();
  
}

int WatchyBase::endProfileAndStart(char* label)
{
  endProfile(label);
  startProfile();
}