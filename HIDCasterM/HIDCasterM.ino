#include "/EspUsbAllHost/src/EspUsbHost.cpp"
#include "/EspUsbAllHost/src/EspUsbHost.h"
#include <BleCombo.h>
const String bHash[] = { "000", "100", "001", "101", "010", "110", "011", "111" };
const mode_t mBDest[] = { MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT };
//BleAbsMouse bleAbsMouse;
int buttons = 0;
int xMov = 1;
int yMov = 2;
int wheel = 3;
int state[] = { 0, 0, 0, 0 };
int lastState[] = { 0, 0, 0, 0 };
int breakyK[3];
unsigned long sTimer;
class MyEspUsbHost : public EspUsbHost {
  void onMouseButtons(hid_mouse_report_t report, uint8_t last_buttons) {
    state[0] = report.buttons;
    state[1] = report.x;
    state[2] = report.y;
    state[3] = report.wheel;
  };

  void onMouseMove(hid_mouse_report_t report) {
    state[0] = report.buttons;
    state[1] = report.x;
    state[2] = report.y;
    state[3] = report.wheel;
  };
};


MyEspUsbHost usbHost;
void setup() { 
  Keyboard.begin();
  Mouse.begin();
  usbHost.begin();
}
void loop() {
  sTimer = millis();
  usbHost.task(); 
  if (Keyboard.isConnected()) {  // only if blutooth is connected, process state/state change
    if (state[0] != lastState[0] || state[1] != lastState[1] || state[2] != lastState[2] || state[3] != 0) {
      breakyK[0] = bHash[state[0]][0] - 48;
      breakyK[1] = bHash[state[0]][1] - 48;
      breakyK[2] = bHash[state[0]][2] - 48;
      for (int i = 0; i < 3; i++) {
        breakyK[i] == 1 ? Mouse.press(mBDest[i]) : Mouse.release(mBDest[i]);
      }
      Mouse.move(state[xMov], state[yMov], state[wheel]);
      lastState[0] = state[0];
      lastState[1] = state[1];
      lastState[2] = state[2];
      lastState[3] = state[3];
      state[3] = 0;
    }
  }
}
