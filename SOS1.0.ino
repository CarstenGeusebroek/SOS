// Include de eerder genoemde library's voor het maken van een bluetooth connectie met de controller
#include <PS4BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

// Het maken van instances van de USB verbinding en daarmee de bluetooth instance
USB Usb;
BTD Btd(&Usb);

// Vervolgens wordt de PS4 bluetooth connectie gemaakt door het verwijzen van de bluetooth instance in de contstructor van de PS4BT class
PS4BT PS4(&Btd, PAIR);

void setup()
{
  // Zorgt ervoor dat pin 3 als output kan worden gebruikt later
  pinMode(3, OUTPUT);

  // Debug code voor het kunnen outputen van teksten in de console.
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
#endif

    // Check of de Usb class juist is gemaakt en die vervolgens initaliseren
    if (Usb.Init() == -1)
    {
      Serial.print(F("\r\nOSC did not start"));
      while (1)
        ; // Halt
      Serial.print(F("\r\nPS4 Bluetooth Library Started"));
    }

  void loop()
  {
    // Functie van de Usb class die wordt gebruikt om alle functies te callen binnen de Usb class die elke keer nodig zijn.
    Usb.Task();

    if (PS4.connected())
    {
      // Output een PWM signaal naar de eerder benoemde output pin. Analog write accepteert waarden tussen de 0 en 255 en de getAnalogButton functie output waarden tussen de 0 en 255 dus dit komt perfect uit.
      analogWrite(3, PS4.getAnalogButton(R2));
    }

    // Laat de controller fysiek trillen, dit zorgt voor het makkelijk debuggen van de code.
    PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
  }
}
