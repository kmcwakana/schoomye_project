#define SENSOR_BIT 9
#include <SchooMyUtilities.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>
SchooMyUtilities scmUtils = SchooMyUtilities();

TM1637Display num_display_18_19(18, 19);
OneWire _tempWire_10(10);
DallasTemperature _tempSensors_10(&_tempWire_10);
int _sbeGetPercent(int pinNumber, int res) {
  return (int)(analogRead(pinNumber) / (float)res * 100);
}

float _sbeGetTemperature(DallasTemperature& sensors) {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void dispNumber(TM1637Display numDisplay, float f) {
  String val = String(f, 1);
  int num = val.toInt();
  uint8_t dot = 0x0;
  if(!val.substring(val.length() - 1).equals("0")) {
    num = val.toFloat() * 10;
    dot = 0b00100000;
  }
  numDisplay.showNumberDecEx(num, dot, false);
}
void dispBlank(TM1637Display numDisplay) {
  uint8_t SEG_BLANK[] = {0,0,0,0};
  numDisplay.setSegments(SEG_BLANK);
}

void setup() {
  Serial.begin(9600);
  pinMode(A1, INPUT);
  _tempSensors_10.begin();
  _tempSensors_10.setResolution(SENSOR_BIT);
  num_display_18_19.setBrightness(0x0f);
}

void loop() {
  if ((_sbeGetPercent(A1, 1023) > 50)) {
    if ((_sbeGetTemperature(_tempSensors_10) > 30)) {
      dispNumber(num_display_18_19, constrain(1111, -999, 9999));
    } else {
      dispNumber(num_display_18_19, constrain(0, -999, 9999));
    }
  } else {
    dispNumber(num_display_18_19, constrain(0, -999, 9999));
  }
}
