
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 7);                  // PIN CE, CSN
const byte address[6] = "00003";   /// alamat NRF RX
const byte address2[6] = "00002";  /// alamat NRF
char total_pesan[32];
String dataIN;
String terima_nomor, terima_kelembaban, terima_stateservo, terima_statepump, terima_level;
bool parsing = false;
int nomor;
int mode_send = 0;
int simpan_kelembaban = 0;
int simpan_servo = 0;
int simpan_pump = 0;
int simpan_level = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  radio.begin();                      // inisialisasi NRF
  radio.openReadingPipe(0, address);  // membuka mode baca nrf
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();  // mulai sebagai receiver
  Serial.println("OK RXv2");
}

void loop() {
  if (mode_send == 0) {
    if (radio.available()) {                          // jika ada masuk di nRF
      radio.read(&total_pesan, sizeof(total_pesan));  // membaca data dari NRF dengan menghitung sepanjang data masuk
      Serial.println(total_pesan);                    // tampilkan di serial monitor
      parsing = true;
    }
    if (parsing == true) {  // Jika Parsing benar
      dataIN = String(total_pesan);
      parsingRespon();
      parsing = false;  // ubah status Parsing
      //    hasil = "";
    }
  }
}

void parsingRespon() {                               // Sub program Parsing DATA
  int awal = dataIN.indexOf('#');                    //Jika Data Awal Berupa #
  int akhir = dataIN.indexOf('#', awal + 1);         //data akhirberupa#
  terima_nomor = dataIN.substring(awal + 1, akhir);  //data diantara # berarti data nya
  nomor = terima_nomor.toInt();
  if (nomor == 1) {
    awal = dataIN.indexOf('#', akhir);
    akhir = dataIN.indexOf('#', awal + 1);
    terima_kelembaban = dataIN.substring(awal + 1, akhir);
    simpan_kelembaban = terima_kelembaban.toInt();
    if (simpan_kelembaban < 0) {
      simpan_kelembaban = 0;
    }
    if (simpan_kelembaban > 100) {
      simpan_kelembaban = 100;
    }
    Serial.println("===============================");
    Serial.println("terima Dari TX1");
    Serial.print("HUM = ");
    Serial.println(simpan_kelembaban);
    Serial.println("===============================");
    Serial.println("");
  }
  if (nomor == 2) {
    awal = dataIN.indexOf('#', akhir);
    akhir = dataIN.indexOf('#', awal + 1);
    terima_kelembaban = dataIN.substring(awal + 1, akhir);
    simpan_kelembaban = terima_kelembaban.toInt();
    if (simpan_kelembaban < 0) {
      simpan_kelembaban = 0;
    }
    if (simpan_kelembaban > 100) {
      simpan_kelembaban = 100;
    }
    awal = dataIN.indexOf('#', akhir);
    akhir = dataIN.indexOf('#', awal + 1);
    terima_stateservo = dataIN.substring(awal + 1, akhir);
    simpan_servo = terima_stateservo.toInt();

    awal = dataIN.indexOf('#', akhir);
    akhir = dataIN.indexOf('#', awal + 1);
    terima_statepump = dataIN.substring(awal + 1, akhir);
    simpan_pump = terima_statepump.toInt();
    Serial.println("===============================");
    Serial.println("terima Dari TX2");
    Serial.print("HUM = ");
    Serial.println(simpan_kelembaban);
    Serial.print("state_servo = ");
    Serial.println(simpan_servo);
    Serial.print("state_pump = ");
    Serial.println(simpan_pump);
    Serial.println("===============================");
    Serial.println("");
  }
}