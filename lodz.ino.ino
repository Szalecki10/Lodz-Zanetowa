#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// Piny dla CE i CSN
#define CE_PIN 9
#define CSN_PIN 10
#define ONE_WIRE_BUS 2

const int trigPin1 = A0;  
const int echoPin1 = A1; 

const int trigPin2 = A5;  
const int echoPin2 = A4; 

float durationl, distancel;
float durationr, distancer;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Servo Servo1;
Servo Silnik;
Servo Zaneta;
struct Sterowanie {
  int speed = 0;
  int lr = 0;
  bool zaneta = true;
};
Sterowanie sterowanieData;

struct Dane {
  float temperatura=0;
  float lewa=0;
  float prawa=0;
  float predkosc=0;
  int kierunek = 0;
  uint8_t zaneta;
};
Dane data;
// Adresy komunikacyjne
const uint64_t nadaj = 0xF0F0F0F0E1LL;    // Adres nadawania
const uint64_t odbierz = 0xF0F0F0F0D2LL;  // Adres odbioru

RF24 radio(CE_PIN, CSN_PIN);

int message = 1;

void setup() {
  sensors.begin();
  Serial.begin(9600);
  Serial.println("Arduino Ready");


  pinMode(trigPin1, OUTPUT);  
	pinMode(echoPin1, INPUT);  
  pinMode(trigPin2, OUTPUT);  
	pinMode(echoPin2, INPUT);  


  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    while (1)
      ;
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(100);  // Jeden kanał dla obu operacji
  Servo1.attach(6);
  Silnik.attach(5,1000,2000);
  Zaneta.attach(3);
  Zaneta.write(0);
}

void loop() {
  // Odbieranie
  radio.openReadingPipe(1, odbierz);  // Ustaw adres do odbierania
  radio.startListening();             // Przełącz na tryb odbierania

  if (radio.available()) {
    while (radio.available()) {
    radio.read(&sterowanieData, sizeof(sterowanieData));  // Odczytaj dane
   }
    Serial.print("Kierunek: ");
    Serial.println(sterowanieData.lr);
    Serial.print("Prdkosc: ");
    Serial.println(sterowanieData.speed);
     if(sterowanieData.lr>85 && sterowanieData.lr<95){
      Servo1.write(90);
    }
    else{
    Servo1.write(sterowanieData.lr);
    }
    if (sterowanieData.speed > 80){
      Silnik.write(92);
    }
    else if (sterowanieData.speed < 30){
      Silnik.write(30);
    }
    else{
      Silnik.write(sterowanieData.speed);
    }


    Serial.print("zaneta: ");
    Serial.println(sterowanieData.zaneta);
    if(sterowanieData.zaneta){
      Zaneta.write(0);
    }
    else{
      Zaneta.write(180);
    }
    //-----------------------po odbiorze przechodzimy do mierzenia
    sensors.requestTemperatures();

    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);

    durationl = pulseIn(echoPin1, HIGH);
    distancel = (durationl*.0343)/2;
    Serial.print("Distance L: ");
    Serial.println(distancel);


    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);

    durationr = pulseIn(echoPin2, HIGH);
    distancer = (durationr*.0343)/2;
    Serial.print("Distance R: ");
    Serial.println(distancer);
    //print the temperature in Celsius
    Serial.print("Temperature: ");
    Serial.print(sensors.getTempCByIndex(0));

    Serial.println("C");
    Serial.println();
    // Po odbiorze przechodzimy do nadawania
    radio.stopListening();         // Przełącz na tryb nadawania
    radio.openWritingPipe(nadaj);  // Ustaw adres do nadawania
    data.temperatura=sensors.getTempCByIndex(0);
    data.lewa=distancel;
    data.prawa=distancer;
    data.predkosc=map(sterowanieData.speed,0,90,100,0);
    data.kierunek=sterowanieData.lr;
    data.zaneta=sterowanieData.zaneta;

    bool result = radio.write(&data, sizeof(data));  // Wyślij dane


   
  }
  delay(100);  // Krótkie opóźnienie, aby zapobiec przeciążeniu pętli
}
