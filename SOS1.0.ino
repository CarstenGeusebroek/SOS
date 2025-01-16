#include <PS4BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;

BTD Btd(&Usb);

PS4BT PS4(&Btd, PAIR);

void setup()
{
  pinMode(3, OUTPUT);

  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
#endif
    if (Usb.Init() == -1)
    {
      Serial.print(F("\r\nOSC did not start"));
      while (1)
        ; // Halt
      Serial.print(F("\r\nPS4 Bluetooth Library Started"));
    }

  void loop()
  {
    Usb.Task();
    if (PS4.connected())
    {
      analogWrite(3, PS4.getAnalogButton(R2));
    }

    PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
  }
  else
  {
    analogWrite(3, 0);
  }
}
