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

// Vervolgens wordt de PS4 bluetooth connectie gemaakt door het verwijzen van de bluetooth instance in de constructor van de PS4BT class
PS4BT PS4(&Btd, PAIR);

//Alle pin waarden zijn variabelen gemaakt zodat het makkelijk aan te passen is in de software mocht je op een andere pin een draadje willen aansluiten
const int pwmPin = 3;
const int forwardPin = 9;
const int backwardPin = 7;
const int reversePin = 8;

//Dit zijn de waarden die gemeten zijn in de dode zones proef 
const int upperLimit = 175;
const int lowerLimit = 85;

//Mocht deze boolean true zijn dan zorgt het ervoor dat er geen stroom meer kan naar de motor
bool killSwitch = false;
//De tijd in ms hoelang de controller niet verbonden mag zijn
const int safetyTime = 250;

void setup() {
  // Debug code voor het kunnen outputen van teksten in de console.
  Serial.begin(9600);
#if !defined(__MIPSEL__)
  while (!Serial)
    ; 
#endif
  // Check of de Usb class juist is gemaakt en die vervolgens initaliseren
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);  
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));

  //Zorgt ervoor dat alle pins als output pins kunnen worden gebruikt
  pinMode(pwmPin, OUTPUT);
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  pinMode(reversePin, OUTPUT);
}

void loop() {
  //Om alles overzichtelijk te maken zijn alle dingen die te maken hebben met het in de gaten houden van de veiligheid naar een aparte functie verplaatst.
  safetyLoop();
  
  // Functie van de Usb class die wordt gebruikt om alle functies te callen binnen de Usb class die elke keer nodig zijn.
  Usb.Task();
  if (PS4.connected()) {
    //Dit zorgt ervoor dat L2 ook gebruikt kan worden om de gasdosering te bepalen. Dit kan het afremmen helpen en overzichtelijker maken
    int difference = PS4.getAnalogButton(R2) - PS4.getAnalogButton(L2);
    //Als de kill swich true is zou er geen gas gegeven mogen worden, vandaar deze if-statement
    if(!killSwitch) {
      analogWrite(pwmPin,(difference/255)*(upperLimit-lowerLimit)+lowerLimit);
    }
    else {
      //Mocht de kill switch true zijn willen we 0V naar de motor sturen.
      analogWrite(pwmPin, 0);
    }

    //PS4.getAnalogHat(LeftHatX) is de waarde van de x richting van de linker joystick. Deze is 0 als de joystick helemaal naar links staat en 255 als hij helemaal naar rechts staat. Mocht de waarde dus kleiner 
    //zijn dan de helft staat de joystick naar links en groter voor de helft staat hij naar rechts.
    if (PS4.getAnalogHat(LeftHatX) < 119) {
      //Naar links willen we dat de piston naar buiten schuift 
      digitalWrite(forwardPin, HIGH);
      digitalWrite(backwardPin, LOW);
    } else if (PS4.getAnalogHat(LeftHatX) > 137) {
      //Naar rechts willen we dat de piston naar binnen schuift 
      digitalWrite(forwardPin, LOW);
      digitalWrite(backwardPin, HIGH);
    } else {
      //Als de joystick neutraal is willen we niet dat de piston beweegt.
      digitalWrite(forwardPin, LOW);
      digitalWrite(backwardPin, LOW);
    }

    //Als de knop L1 wordt ingedrukt willen we dat de skelter in zijn achteruit gaat
    if(PS4.getButtonClick(L1)) {
      digitalWrite(reversePin, HIGH);
    }
    //Als R1 wordt ingedrukt willen we weer dat de skelter naar voren gaat.
    if(PS4.getButtonClick(R1)) {
      digitalWrite(reversePin, LOW);
    }

    //Dit geeft feedback voor de gebruiker als hij de knoppen L2 en R2 indrukt.
    PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
  } else {
    //Mocht de controller niet verbonden zijn willen, we natuurlijk dat er  geen stroom naar de motor gaat.
    analogWrite(pwmPin, 0);
  }
}

void safetyLoop() {
  //De PS4 controller stuurt constant messages naar de arduino. Mocht dat ineens niet meer gebeuren, is hij niet meer verbonden. Als de tijd tussen de laatst ontvangen message en de huidige tijd te groot wordt, willen we de killswitch aanzetten.
  if (millis() - PS4.getLastMessageTime() > safetyTime) {
    killSwitch = true;
  }
  //Mocht er toch iets fout gaan kan de motor meteen uitgezet worden met de knop "rondje" op de PS controller
  if(PS4.getButtonPress(CIRCLE)){
    killSwitch = true;
    return;
  }

  //De kill switch kan weer uitgezet worden met de knop "driehoekje"
  if(PS4.getButtonClick(TRIANGLE)){
    killSwitch = false;
  }
}


