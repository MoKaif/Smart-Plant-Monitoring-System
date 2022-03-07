/* Sending Sensor Data to Firebase Database by CircuitDigest(www.circuitdigest.com) */
#include <Firebase_ESP_Client.h>
#include <ESP8266WiFi.h>                                                    // esp8266 library
//#include <FirebaseArduino.h>                                                // firebase library
#include <DHT.h>                                                            // dht11 temperature and humidity sensor library
#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"
#include <Wire.h>
//#define FIREBASE_HOST "smart-plant-monitoring-s-94e5c-default-rtdb.asia-southeast1.firebasedatabase.app"                          // the project name address from firebase id
//#define FIREBASE_AUTH "qo3vPAw8zplyRczglUevcno6Iepn4qLFWEFCQQaM"                                                                  // the secret key generated from firebase


#define API_KEY "AIzaSyB5kvQ4ZbdC6mV8amtzgI3fzmBiZMlrgQg"
#define WIFI_SSID "JioFiberdesh"                                                                                                              // input your home or public wifi name 
#define WIFI_PASSWORD "kshitijaom01"                                                                                        //password of wifi ssid
#define DATABASE_URL "https://smart-plant-monitoring-s-94e5c-default-rtdb.asia-southeast1.firebasedatabase.app/" 

#define USER_EMAIL "omd142002@gmail.com"
#define USER_PASSWORD "12341234"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;

#define DHTPIN D2                                                           // what digital pin we're connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE); 

String databasePath;
String tempPath;
String humPath;
String SoilPath;

unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 1000;
                                                    
const int sensor_pin = A0;


void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}


void setup() {
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
//  Serial.println();
//  Serial.print("Connected to ");
//  Serial.println(WIFI_SSID);
//  Serial.print("IP Address is : ");
//  Serial.println(WiFi.localIP());                                            //print local IP address
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);     

    Serial.println(WiFi.localIP());
    Serial.println();
// connect to firebase
  dht.begin();     
  

   config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database path for sensor readings
  tempPath = databasePath + "/temperature"; // --> UsersData/<user_uid>/temperature
  humPath = databasePath + "/humidity"; // --> UsersData/<user_uid>/humidity
  SoilPath = databasePath + "/SoilMoistureContent";
  
  //Start reading dht sensor
}

void loop() { 

  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
  
  int moisture_percentage;
  
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
 
    Serial.print("Soil Moisture(in Percentage) = ");
    Serial.print(moisture_percentage);
    Serial.println("%");
  String firesoil = String(moisture_percentage) + String("%");
  float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();                                           // Read temperature as Celsius (the default)
    
  if (isnan(h) || isnan(t)) 
                {                                                // Check if any reads failed and exit early (to try again).
                    Serial.println(F("Failed to read from DHT sensor!"));
                    return;
                }
  
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  String fireTemp = String(t) + String("°C");                                                     //convert integer temperature to string temperature
  delay(4000);


  sendFloat(tempPath, t);
  sendFloat(humPath, h);
  sendFloat(SoilPath, float(moisture_percentage));
  
//Firebase.pushString("/DHT11/Humidity", fireHumid);                                  //setup path and send readings
//Firebase.pushString("/DHT11/Temperature", fireTemp);                                //setup path and send readings
//Firebase.pushString("/Soil/SoilMoisture", firesoil);                                //setup path and send readings
//Firebase.setfloat()
}


}
