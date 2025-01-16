#include <PS4BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;
int upperLimit = 175;
int lowerLimit = 85;
int oldMillisValue = 0;
int deltaTime = 0;

void setup()
{
  pinMode(3, OUTPUT);
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1)
  {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ; // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}

bool reverse = false;

void loop()
{
  Usb.Task();

  if (PS4.connected())
  {

    if (PS4.getButtonClick(R1))
    {
      reverse = false;
    }
    else if (PS4.getButtonClick(L1))
    {
      reverse = true;
    }
    float differnce = PS4.getAnalogButton(R2) - PS4.getAnalogButton(L2);
    if (differnce <= 0)
    {
      differnce = 0;
    }
    if (!reverse)
    {
      digitalWrite(8, HIGH);
    }
    else
    {
      digitalWrite(8, LOW);
    }
    analogWrite(3, (differnce / 255) * (upperLimit - lowerLimit) + lowerLimit);

    // controller trilling
    PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
  }
  else
  {
    analogWrite(3, 0);
  }
}