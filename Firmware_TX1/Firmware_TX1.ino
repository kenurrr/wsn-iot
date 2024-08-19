
#include <SPI.h>
#include "RF24.h"
#include <Chrono.h>
#define pin_analog A0
#define baca_analog analogRead(pin_analog)

Chrono tKirim;

char total[32];
RF24 radio(7, 8);                  // PIN CE, CSN
const byte address[6] = "00002";   /// alamat NRF
const byte address2[6] = "00003";  /// alamat NRF RX
int x = 200;
int kelembaban = 0;
int nomor = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  radio.begin();  // inisalisasi NRF
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.println("OK TX1");
  tKirim.restart();
}
bool sekali = true;
void loop() {
  kelembaban = map(baca_analog, 480, 180, 0, 100);
  if (kelembaban < 0) {
    kelembaban = 0;
  }
  if (kelembaban > 100) {
    kelembaban = 100;
  }
  if (tKirim.hasPassed(10000) && sekali == true) {
    // tKirim.restart();
    radio.begin();  // inisalisasi NRF
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
    sprintf(total, "%d", kelembaban);
    Serial.println("===============================");
    Serial.print("Kirim TX2 = ");
    Serial.println(total);
    Serial.println("===============================");
    Serial.println("");
    radio.write(&total, sizeof(total));  // sub untuk kirim data via NRF
    sekali = false;
  }
  if (tKirim.hasPassed(10000) && sekali == false) {
    tKirim.restart();
    radio.begin();  // inisalisasi NRF
    radio.openWritingPipe(address2);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
    sprintf(total, "#%d#%d#", nomor, kelembaban);
    Serial.println("===============================");
    Serial.print("Kirim RX = ");
    Serial.println(total);
    Serial.println("===============================");
    Serial.println("");
    radio.write(&total, sizeof(total));  // sub untuk kirim data via NRF
    sekali = true;
  }
}