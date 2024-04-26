#include <Arduino.h>

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

const char* WIFI_SSID = "Hola";
const char* WIFI_PASSWORD= "12345678";


const char* API_KEY= "AIzaSyDXaA-qnnh9AhcB2hnUoDKAN3YVvX26Q7I";

const char* DATABASE_URL= "https://dianocherivera-96f9d-default-rtdb.firebaseio.com/"; //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app;

const char* USER_EMAIL= "brandon@rivera.com";
const char* USER_PASSWORD= "brandon@rivera.com";


FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;


void setup_WIFI(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  config.signer.tokens.legacy_token = "<database secret>";
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}
const int LDR_PIN = 34; 

void setup() {
    Serial.begin(115200);
    setup_WIFI();
    setupFirebase();
    pinMode(LDR_PIN, INPUT); // Configura el pin LDR_PIN como entrada
    // ...
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))  {
      int LDR = analogRead(LDR_PIN); // Lee el valor del sensor LDR
      int hora = map(LDR, 0, 4096, 0, 24); // Mapea el valor a un rango de 0 a 24
      Serial.printf("MandandoDatos... %s\n", Firebase.RTDB.setInt(&fbdo, F("/notificaciones/hora"), hora) ? "ok" : fbdo.errorReason().c_str());
  }
  delay(500);
}


  
