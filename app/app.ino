// CONSTANTS
const int BUTTON_PIN = 2;
const int ANALOG_CURRENT_PIN = A0;
const int BITRATE = 9600; // bits per second
const int BASE_VOLTAGE = 230; // in Volt
const int LOOP_DELAY = 1000; // in ms

// Global variables
int buttonValue = 0;
int currentSensorValue = 0;
int totalPrice = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication
  Serial.begin(BITRATE);
  
  // declare pushbutton as input
  pinMode(BUTTON_PIN, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // Lecture du capteur de courant
  int currentSensorValue = analogRead(ANALOG_CURRENT_PIN);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float current = currentSensorValue * (1.0 / 1023.0);
  
  // Conversion en watt
  float watt = (current * 5) * BASE_VOLTAGE;
    
  // Affichage de la consommation instantannée sur l'écran
  Serial.print(watt);
  Serial.println(" Watt");

  // Mise à jour et affichage du coût toutes les X secondes
  // Si le bouton de réinitialisation du coût est actionné, on remet à 0
  buttonValue = digitalRead(BUTTON_PIN);
  if (buttonValue == LOW) {
      totalPrice = 0;
  }
  else {
    // totalPrice += watt * ();
    // prix kw/h
  }
  
  // Si la connexion avec le serveur fonctionne
    // Envoi des données au serveur
  
  delay(1000);
}
