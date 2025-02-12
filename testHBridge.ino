//Dit zijn de pin nummers die we gaan gebruiken voor het naar voren en achter gaan van de piston
const int forwardPin = 7;
const int backwardPin = 9;

void setup() {
  //Deze functie moet gecalled worden om de pin later als "output" pin te kunnen gebruiken
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
}

void loop() {
  //De piston gaat hier naar voren doordat we een voltage sturen naar de transistors die corresponderen met de stroomrichting waarin de piston naar voren gaat
  digitalWrite(forwardPin, HIGH);
  digitalWrite(backwardPin, LOW);
  //Dit gebeurt voor 2s omdat het programma voor 2s niks doet door de lijn hier onder
  delay(2000);
  //Vervolgens zetten we stroom op de andere transistors waardoor de stroomrichting en dus de richting van de piston zou moeten veranderen
  digitalWrite(forwardPin, LOW);
  digitalWrite(backwardPin, HIGH);
  //Dit doen we wederom voor 2s
  delay(2000);
}
