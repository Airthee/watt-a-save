// CONSTANTS
const int BUTTON_PIN = 2;
const int ANALOG_CURRENT_PIN = A0;
const int BITRATE = 9600; // bits per second
const int BASE_VOLTAGE = 5;//230; // in Volt
const float KWH_PRICE = 17.65; // centimes d'€

// Global variables
int buttonValue = LOW;
float currentSensorValue = 0;
float totalWH = 0; // centimes d'€
float price;

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
  currentSensorValue = analogRead(ANALOG_CURRENT_PIN);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float current = currentSensorValue * (1.0 / 1023.0);
  
  // Conversion en watt
  float watt =  current * BASE_VOLTAGE;
    
  // Affichage de la consommation instantannée sur l'écran
  Serial.print(current);
  Serial.print(" A => ");
  Serial.print(watt);
  Serial.println(" W");

  // Mise à jour et affichage du coût toutes les X secondes
  // Si le bouton de réinitialisation du coût est actionné, on remet à 0
  buttonValue = digitalRead(BUTTON_PIN);
  if (buttonValue == HIGH) {
      totalWH = 0;
      Serial.println("Réinitialisation consommation");
  }
  else {
    totalWH += watt / 3600; // ajout de la consommation sur une seconde
  }
  price = (totalWH / 1000) * KWH_PRICE; // centimes par watt seconde
  buttonValue = LOW;
  Serial.print(price);
  Serial.println(" centimes d'euro");

  if (totalWH < 1000) {
    Serial.print(totalWH * 1000);
    Serial.println(" mWh"); 
  }
  else {
    Serial.print(totalWH);
    Serial.println(" Wh"); 
  }
  
  // Si la connexion avec le serveur fonctionne
    // Envoi des données au serveur
  
  delay(1000);
}
