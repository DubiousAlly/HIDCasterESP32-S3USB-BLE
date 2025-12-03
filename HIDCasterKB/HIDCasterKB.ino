#include "C:\Users\Administrator\Documents\Arduino\libraries\EspUsbAllHost\src\EspUsbHost.cpp"
#include "C:\Users\Administrator\Documents\Arduino\libraries\EspUsbAllHost\src\EspUsbHost.h"
//#include <Streaming.h>
#include <BleCombo.h>

uint8_t KKa[256] = {
  0, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 10, KEY_ESC, 8,
  9, 32, '-', '=', '[', ']', '\\', '\,', ';', '\'', '`', '\,', '.', '/', KEY_CAPS_LOCK, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, '/', '*', '-', '+', 10, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 0, 0, 0, '=', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 129, 130,
  131, 132, 133, 134, 135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
  };
//modmask
uint8_t modMask[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t lastModMask[] = { 0, 0, 0, 0, 0, 0, 0, 0 };


class MyEspUsbHost : public EspUsbHost {
  void onKeyboard(hid_keyboard_report_t report, hid_keyboard_report_t last_report) {
    ////Serial << (report.modifier), (report.keycode[0]), (report.keycode[1]), (report.keycode[2]), (report.keycode[3]), (report.keycode[4]) << endl;
//    Serial.printf("%d %d %d %d %d %d %d", report.modifier, report.keycode[0], report.keycode[1], report.keycode[2], report.keycode[3], report.keycode[4], report.keycode[5]);
//    Serial.println();

    for (int i = 0; i < 8; i++) {
      lastModMask[i] = modMask[i];
      modMask[i] = bitRead(report.modifier, i);
      bitRead(report.modifier, i) == 1 ? modMask[i] = KKa[i + 128] : modMask[i] = 0;
      if (lastModMask[i] != modMask[i]) {
        ////modMask[i] == 0 ? Serial << "release " << KKa[i+128] : Serial << "press " << KKa[i+128];
        if (Keyboard.isConnected()) {
          modMask[i] == 0 ? Keyboard.release(KKa[i + 128]) : Keyboard.press(KKa[i + 128]);
        }
      }
    }
//    Serial << endl;
    for (int i = 0; i < 6; i++) {
      if (last_report.keycode[i] != 0 && last_report.keycode[i] != 1) {  // any keys from prev report don't appear on new report
        if (last_report.keycode[i] != report.keycode[0] && last_report.keycode[i] != report.keycode[1] && last_report.keycode[i] != report.keycode[2] && last_report.keycode[i] != report.keycode[3] && last_report.keycode[i] != report.keycode[4] && last_report.keycode[i] != report.keycode[5]) {
          Keyboard.isConnected() == true ? Keyboard.release(KKa[last_report.keycode[i]]) : report.keycode[i];
          ////Serial <<"release " << _DEC(KKa[last_report.keycode[i]]) << ", ";
          //keyboard.release()
        }
      }
      if (report.keycode[i] != 0 && report.keycode[i] != 1) {  // any keys from new report don't appear on prev report
        if (last_report.keycode[0] != report.keycode[i] && last_report.keycode[1] != report.keycode[i] && last_report.keycode[2] != report.keycode[i] && last_report.keycode[3] != report.keycode[i] && last_report.keycode[4] != report.keycode[i] && last_report.keycode[5] != report.keycode[i]) {
          Keyboard.isConnected() && KKa[report.keycode[i]] != 0 ? Keyboard.press(KKa[report.keycode[i]]) : report.keycode[i];
          ////Serial << "press " << _DEC(KKa[report.keycode[i]]) << ", ";
        }
      }
    }
    ////Serial << endl;
  }
  void onGone() {
    Keyboard.releaseAll();
  }
};

MyEspUsbHost usbHost;

void setup() {
  Keyboard.begin();
//  Serial.begin(115200);
//  delay(500);

  usbHost.begin();
}

void loop() {
  usbHost.task();
}
