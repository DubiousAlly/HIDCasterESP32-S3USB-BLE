# HIDCaster
Switch control targets instead of HIDs. PLug your mouse and keyboard into their own devoted ESP32-S3 as USB Hosts and redirect their output as Bluetooth Low Energy. I'm using a selector switch with my Winedows work machine as one endpoint and the ESP32-S3 pair as the switches other USB endpoint (BT EP is Lenovvo thinkcenter running Ubuntu 24.04). There are a couple of ways I could probably have collapsed this down to needing only one S3, but they're still dirt-cheap and this leaves both HIDs unencumbered by eachothers crosstalk. The ESP USB Host library by TANAKA Masayuki may work for other hardware out-of-the-box, but I had to do a lttle surgery to get it to talk to my salvaged LENOVO SK-8827 SD50LB0031 keyboard and my Kensington k72356eu Wired USB 3.0 Mouse. I hypothesize the the initial setup expects the mouse its hosting to have a back/forward button and thats why I had to adjust it to work with my dumpster mouse. I adjusted EspUsbHost.cpp lines 578-581 to output the data I needed to the context callbacks. Notes included in that file with original values.

I was annoyed to no end to discover how much heavy lifting SDL has been doing for my whole life making keyboard data make sense becasue the keys come out of the HID keyboard with 'a' as 0x04. Many if not perhaps even most of you already knew that, but it was news to me. YUCK. I've yet to tackle the indicator lights for NUM, CAPS, SCROLL lock, but I did learn the host manages tracking those locks and their lights, not the keyboard firmware, as I had always assumed. EspUsbHost.h to the rescue with its handy lookup-table on lines 92-198. I copied it at the top of my KB
implementation and convert it to a lookup table for the Keyboard.press(KEY_) equivolents.

<hr>

### Libs 

Something happened in the last two years of updates to the ESP32 libraries that broke the compilability of all the HID/BLE accompanying libraries so after some research I implemented the laziest solution the internet had for that and rolled back my esp32 libs to version 2.0.18. The alternative sounded almost equally easy-- simply changing a data-type in couple of places.

The inbuilt USB HID modifiers protocol makes a lovely prefab bitmask for CTRL, SHIFT, ALT, and META-- even diferentiating left and right packing 8 keys worth of state into one 8 bit value. I hypothesize its possibnle to simply pass a copy the usb hid report to the BLE endpoint, but I haven't figured that one out yet (sounds simple-- must be easy! hehe) The included BLE libraries are are compipling no problem in Arduiino IDE 2.3.5 with ESP32 v2.0.18.

<hr>

### Conclusions 

My honest assesment of the functionality is that the mouse passthrough is entirely seamless, as good as I ever would have hoped to make it. The Keyboard is fine if perhaps incomplete. I can't recall ever wanting numlock off, but it doesn't work yet. I've yet to discover any primary dysfunction or malfunction, but it doesn't feel as effortless and clean as the mouse does. This mostly exists to type enough into the youtube search that predictive text saves me from needing to know how to spell what I'm looking for; so for my application: adaquate.

https://www.amazon.com/dp/B00M7X6XNG?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
I use one of these on each esp32-S3 because hosting the keyboarddoesn't come with power-delivery-negotiation. this way they keep quietly paired until they start getting HID data-- anyway, I don't spend any time or energy re-establishing the BT connection, I simply never break it.

This is a little project that's lived on the back burner for a year or so now,
finally picked at sufficiently to have become functional. There's probably a product
to acheive this effect the world would sell me at this point, but then what would I have learned?

<hr>

### Credits

ESP32-BLE-Combo from BlynkGo. I don't recall changing much of anything to get a repeatably quality result from this library. Using this same library for both keyboard and mouse.
https://github.com/BlynkGO/ESP32-BLE-Combo/blob/main/


Original EspUsbHost from tanakamasayuki
This is on a very short list of USB host libraries I've managed to get working at all and I didn't do much to get it there. if its giving weird data, crank the log-verbosity and a solution will emerge. oddly slim pickings for usb hci libraries, so we should be glad this is a good one. Again, same library for both KB and mouse.
https://github.com/tanakamasayuki/EspUsbHost
