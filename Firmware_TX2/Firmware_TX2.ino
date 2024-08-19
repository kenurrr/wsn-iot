
#include <SPI.h>
#include "RF24.h"
#include <Servo.h>
#include <Chrono.h>
char total[32];
RF24 radio(7, 8);  // PIN CE, CSN
//RF24 radio(7, 8); // PIN CE, CSN
const byte address[6] = "00002";   /// alamat NRF
const byte address2[6] = "00003";  /// alamat NRF
#define Trigpin A0
#define Echopin A1
#define pin_rel1 6
#define rel1On digitalWrite(pin_rel1, LOW)
#define rel1Off digitalWrite(pin_rel1, HIGH)
#define pin_rel2 5
#define rel2On digitalWrite(pin_rel2, LOW)
#define rel2Off digitalWrite(pin_rel2, HIGH)
#define buka_servo servone.write(0)
#define tutup_servo servone.write(90)

Servo servone;
bool terima_data = false;
bool action = false;
int max_dalam = 100;
int set_air = 40;
int state_servo = 0;
int state_pump = 0;
int siram = 0;
int kelembaban = 0;
int pos = 0;
int set_point = 50;
int level;
int nomor = 2;
int mode_send = 0;
float durasi, cm;
char total_pesan[32];
char pesan_kirim[32];

String dataIN;
Chrono tKirim;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin_rel1, OUTPUT);
  pinMode(pin_rel2, OUTPUT);
  pinMode(Trigpin, OUTPUT);
  pinMode(Echopin, INPUT);
  rel1Off;
  rel2Off;
  servone.attach(3);
  // buka_servo;delay(1000);
  tutup_servo;
  Serial.println("OK SETUP TX2");
  radio.begin();                      // inisialisasi NRF
  radio.openReadingPipe(0, address);  // membuka mode baca nrf
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();  // mulai sebagai receiver
  Serial.println("OK TX2 v2.1");
}

void loop() {
  if (mode_send == 0) {
    if (radio.available()) {  // jika ada masuk di nRF
      tKirim.restart();
      radio.read(&total_pesan, sizeof(total_pesan));  // membaca data dari NRF dengan menghitung sepanjang data masuk
      Serial.println("===============================");
      Serial.println("Terima dari TX1");
      Serial.println(total_pesan);
      Serial.println("===============================");
      Serial.println("");
      digitalWrite(Trigpin, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trigpin, LOW);
      durasi = pulseIn(Echopin, HIGH);
      cm = 0.017 * durasi;
      if (cm > max_dalam) {
        cm = max_dalam;
      }
      if (cm < 0) {
        cm = 0;
      }
      level = max_dalam - cm;
      if (level < 0) {
        level = 0;
      }
      if (level > max_dalam) {
        level = max_dalam;
      }
      dataIN = String(total_pesan);
      kelembaban = dataIN.toInt();

      action = true;
      if (terima_data == true) {
        if (kelembaban < set_point) {
          siram = 1;
        }
        if (kelembaban > set_point) {
          siram = 0;
        }
        if (siram == 0) {
          Serial.println(" hum aman");
          rel1Off;
          tutup_servo;
          state_servo = 0;
          state_pump = 0;
          sprintf(pesan_kirim, "#%d#%d#%d#%d#", nomor, kelembaban, state_servo, state_pump);
          Serial.println(pesan_kirim);
          Serial.print("Kelembaban = ");
          Serial.println(kelembaban);
          Serial.print("level = ");
          Serial.println(level);
          Serial.print("state servo = ");
          Serial.println(state_servo);
          Serial.print("state pump = ");
          Serial.println(state_pump);
          terima_data = false;
        }
        if (siram == 1 && level > set_air + 1 && action == true) {
          Serial.println("servo on");
          buka_servo;
          rel1Off;
          state_servo = 1;
          state_pump = 0;
          sprintf(pesan_kirim, "#%d#%d#%d#%d#", nomor, kelembaban, state_servo, state_pump);
          Serial.println(pesan_kirim);
          Serial.print("Kelembaban = ");
          Serial.println(kelembaban);
          Serial.print("level = ");
          Serial.println(level);
          Serial.print("state servo = ");
          Serial.println(state_servo);
          Serial.print("state pump = ");
          Serial.println(state_pump);
          action = false;
        }
        if (siram == 1 && level < set_air - 1 && action == true) {
          Serial.println("pump on");
          tutup_servo;
          rel1On;
          state_servo = 0;
          state_pump = 1;
          sprintf(pesan_kirim, "#%d#%d#%d#%d#", nomor, kelembaban, state_servo, state_pump);
          Serial.println(pesan_kirim);
          Serial.print("Kelembaban = ");
          Serial.println(kelembaban);
          Serial.print("level = ");
          Serial.println(level);
          Serial.print("state servo = ");
          Serial.println(state_servo);
          Serial.print("state pump = ");
          Serial.println(state_pump);
          action = false;
        }
      }
      terima_data = true;
      mode_send = 1;
    }
  }
  if (mode_send == 1) {
    radio.begin();  // inisalisasi NRF
    radio.openWritingPipe(address2);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();                           // mulai sebagai receiver
    radio.write(&pesan_kirim, sizeof(pesan_kirim));  // sub untuk kirim data via NRF
    Serial.print("KIRIM RX= ");
    Serial.println(pesan_kirim);        // tampilkan di serial monitor
    delay(1000);                         //
    radio.begin();                      // inisalisasi NRF
    radio.openReadingPipe(0, address);  // membuka mode baca nrf
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    mode_send = 0;
  }
}