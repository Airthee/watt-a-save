#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <stdlib.h>

// CONSTANTS
const int BUTTON_PIN = 2;
const int LED_PIN = 13;
const int ANALOG_CURRENT_PIN = A0;
const int BITRATE = 9600; // bits per second
const int BASE_VOLTAGE = 230; // in Volt
const float KWH_PRICE = 17.65; // centimes d'€
const int BENCH_ID = 1;
const String ROUTE = "/api/measures";

const char ssid[] = "Lecoussin";    // Network SSID (name)
const char pass[] = "rapedefromage";    // Network password (use for WPA, or use as key for WEP)
const char server[] = "benoitjaouen.fr";
const int port = 8000;

// Global variables
int buttonValue = LOW;
float currentSensorValue = 0;
float totalWH = 0; // centimes d'€
float price;



int status = WL_IDLE_STATUS;
WiFiClient client;
HttpClient http_client = HttpClient(client, server, port);

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication
  Serial.begin(BITRATE);

  //
  connect_to_wifi();
  // declare pins
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void connect_to_wifi() {
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("[WiFi] Connecting to: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(100);
  }

  // you're connected now, so print out the data:
  Serial.println("[WiFi] Connected");
}

void api_send(float consumption) {
  Serial.println("[HTTP] Connecting to api...");  
  String content = String("{\"bench_id\":") + String(BENCH_ID) +  String(", \"consumption\":") + String(consumption, 6) +  String("}");
  Serial.println(String("[HTTP] Message: ") + content);
  http_client.post(ROUTE, "application/json",  content);
  // read the status code and body of the response
  int statusCode = http_client.responseStatusCode();
  String response = http_client.responseBody();

  Serial.print("[HTTP] Status code: ");
  Serial.println(statusCode);
  Serial.print("[HTTP] Response: ");
  Serial.println(response);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(LED_PIN, HIGH);
  
  // Lecture du capteur de courant
  currentSensorValue = analogRead(ANALOG_CURRENT_PIN);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5A):
  float current = currentSensorValue * (5.0 / 1023.0);
  
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
    price = (totalWH / 1000) * KWH_PRICE; // centimes par watt seconde

    Serial.print(price);
    Serial.println(" centimes d'euro");
  
    if (totalWH < 1) {
      Serial.print(totalWH * 1000);
      Serial.println(" mWh"); 
    }
    else {
      Serial.print(totalWH);
      Serial.println(" Wh"); 
    }
    api_send(totalWH);
  }
  buttonValue = LOW;
  
  // Si la connexion avec le serveur fonctionne
    // Envoi des données au serveur
  
  Serial.println("");
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
