#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ThingSpeak.h"
//#include <FirebaseESP8266.h>

//=====Sub pada Thingspeak====
const char *ssid = "Warkop_Sayuk2";
const char *pass = "menolaksadar";

// ThingSpeak settings
const char *apikeyWrite = "CM4WHKZVX97A9ZGK";
const char *apikeyread = "UGDN8MUXUKZ73N8X";
const char *server = "api.thingspeak.com";
unsigned long chanel = 2563990;
WiFiClient client;
String dataIn;
String terima_kelembaban;
String terima_servo;
String terima_pump;
bool parsing = false;
int state_servo = 0;
int state_pump = 0;
int kelembabane = 0;
int x = 0;
int y = 1, z = 0;

// Firebase settings
//#define FIREBASE_HOST "https://irigasisawah-599a5-default-rtdb.firebaseio.com/"
//#define FIREBASE_AUTH "eL82BwJ27NDfqPMGx2dyB2bldxGi0RDxypX3Q4Cx"

//FirebaseData firebaseData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("Konek!");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);

  // Firebase initialization
 // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 // Firebase.reconnectWiFi(true);
}

void loop() {
  if (Serial.available() > 0) {  // jika ada data masuk
    char c = Serial.read();      // Baca Data nya simpan di char c
    if (c != '\n') {
      dataIn += c;  // tampung di dataIn
      Serial.print("data Masuksd = ");
      Serial.println(dataIn);
    } else {
      parsing = true;  // setelah masuk semua ubah mode ke parsing true
      while (Serial.available()) Serial.read();
    }
  }
  if (parsing == true) {  // Jika Parsing benar
    parsingRespon();  // maka Parsing Data
    parsing = false;  // ubah status Parsing
    dataIn = "";      // kosongkan data penampung
    kirim();
    delay(1000);
    kirim();
    delay(10);
    Serial.println("Done");
  }
}

void kirim() {
  ThingSpeak.writeField(chanel, 1, kelembabane, apikeyWrite);
  delay(1000);
  ThingSpeak.writeField(chanel, 2, state_servo, apikeyWrite);
  delay(1000);
  ThingSpeak.writeField(chanel, 3, state_pump, apikeyWrite);
  delay(1000);
  ThingSpeak.writeField(chanel, 3, state_pump, apikeyWrite);
  delay(10000);

  // Send data to Firebase
 // Firebase.setInt(firebaseData, "/kelembaban", kelembabane);
 // delay(100);
  //Firebase.setInt(firebaseData, "/state_servo", state_servo);
 // delay(100);
 // Firebase.setInt(firebaseData, "/state_pump", state_pump);
 // delay(100);
}

void parsingRespon() {                                    // Sub program Parsing DATA
  int awal = dataIn.indexOf('#');                         //Jika Data Awal Berupa #
  int akhir = dataIn.indexOf('#', awal + 1);              //data akhirberupa#
  terima_kelembaban = dataIn.substring(awal + 1, akhir);  //data Awal +1 Berarti Data Yg dikirim berupa NILAi simpan di variabel terima suhu
  kelembabane = terima_kelembaban.toInt();                // ubah dataa dari String ke float

  awal = dataIn.indexOf('#', akhir);  // cek lagi setelah pager ada data atau tidak
  akhir = dataIn.indexOf('#', awal + 1);
  terima_servo = dataIn.substring(awal + 1, akhir);
  state_servo = terima_servo.toInt();

  awal = dataIn.indexOf('#', akhir);  // cek lagi setelah pager ada data atau tidak
  akhir = dataIn.indexOf('#', awal + 1);
  terima_pump = dataIn.substring(awal + 1, akhir);
  state_pump = terima_pump.toInt();
  Serial.print("kelembaban = ");
  Serial.println(kelembabane);
  Serial.print("State Servo = ");
  Serial.println(state_servo);
  Serial.print("State Pump = ");
  Serial.println(state_pump);
  Serial.println("OK KIRIM");
}
