#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ezButton.h>
// Piny dla CE i CSN
#define CE_PIN 9
#define CSN_PIN 10
#define SW_PIN 2  // Arduino pin connected to SW  pin

ezButton button(SW_PIN);

struct Sterowanie {
  int speed = 0;  
  int lr = 0;
  bool zaneta = true;
};
Sterowanie sterowanieData;
// Adres komunikacyjny
const uint64_t pipe = 0xF0F0F0F0D2LL;

RF24 radio(CE_PIN, CSN_PIN);

int message = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino Ready");

  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    while (1)
      ;
  }
  button.setDebounceTime(50);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(100);

  radio.openWritingPipe(pipe);  // Adres do wysyłania
  radio.stopListening();        // Przełącz na tryb nadawania
}

void loop() {
  button.loop();  // MUST call the loop() function first
  bool result = radio.write(&sterowanieData, sizeof(sterowanieData));
  if (button.isPressed()) {
    sterowanieData.zaneta = false;
  }
  Serial.print("Sent: LR:  ");
  Serial.print(sterowanieData.lr);
  Serial.print("  SPEED:  ");
  Serial.print(sterowanieData.speed);
  Serial.print("  ZANETA:  ");
  Serial.println(sterowanieData.zaneta);
  if (result) {
  
  } else {
    Serial.println("Message failed to send");
  }
  sterowanieData.speed = map(analogRead(A5), 0, 1023, 90, 0);
  sterowanieData.lr = map(analogRead(  A1), 0, 1023, 0, 180);
  delay(1500);  // Opóźnienie między kolejnymi wysyłkami
}