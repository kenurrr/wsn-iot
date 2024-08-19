#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);                  // PIN CE, CSN
const byte address[6] = "00003";   // alamat NRF RX
char total_pesan[32];
String dataIN;
String terima_nomor, terima_kelembaban, terima_stateservo, terima_statepump, terima_level;
bool parsing = false;
int nomor;
int simpan_kelembaban = 0;
int simpan_servo = 0;
int simpan_pump = 0;

void setup() {
  Serial.begin(115200);
  radio.begin();                      // inisialisasi NRF
  radio.openReadingPipe(0, address);  // membuka mode baca nrf
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();  // mulai sebagai receiver
  Serial.println("OK RXv2");
}

void loop() {
  if (radio.available()) {                          // jika ada data masuk di nRF
    radio.read(&total_pesan, sizeof(total_pesan));  // membaca data dari NRF
    Serial.println(total_pesan);                    // tampilkan di serial monitor
    parsing = true;
  }
  if (parsing == true) {  // Jika Parsing benar
    dataIN = String(total_pesan);
    parsingRespon();
    parsing = false;  // ubah status Parsing
  }
}

void parsingRespon() {                               // Sub program Parsing DATA
  int awal = dataIN.indexOf('#');                    //Jika Data Awal Berupa #
  int akhir = dataIN.indexOf('#', awal + 1);         // data akhirberupa#
  terima_nomor = dataIN.substring(awal + 1, akhir);  // data diantara # berarti data nya
  nomor = terima_nomor.toInt();
  if (nomor == 1) {
    awal = dataIN.indexOf('#', akhir);
    akhir = dataIN.indexOf('#', awal + 1);
    terima_kelembaban = dataIN.substring(awal + 1, akhir);
    simpan_kelembaban = terima_kelembaban.toInt();
    simpan_kelembaban = constrain(simpan_kelembaban, 0, 100);  // Batas nilai kelembaban

    Serial.println("===============================");
    Serial.println("terima Dari TX1");
    Serial.print("HUM = ");
    Serial.println(simpan_kelembaban);
    Serial.println("===============================");
    Serial.println("");
    
    // Kirim data ke NodeMCU
    String dataToSend = "#" + String(nomor) + "#" + String(simpan_kelembaban) + "#";
    Serial.println(dataToSend);  // Mengirimkan data via Serial ke NodeMCU
  }
  if (nomor == 2) {
    awal = dataIN.indexOf('#', akhir);
    akhir = dataIN.indexOf('#', awal + 1);
    terima_kelembaban = dataIN.substring(awal + 1, akhir);
    simpan_kelembaban = terima_kelembaban.toInt();
    simpan_kelembaban = constrain(simpan_kelembaban, 0, 100);  // Batas nilai kelembaban

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

    // Kirim data ke NodeMCU
    String dataToSend = "#" + String(nomor) + "#" + String(simpan_kelembaban) + "#" + String(simpan_servo) + "#" + String(simpan_pump) + "#";
    Serial.println(dataToSend);  // Mengirimkan data via Serial ke NodeMCU
  }
}
