#include <SPI.h>

#define SPI_CS_PIN 10

void setup() {
  SPI.begin();
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, HIGH);
  
  // Initialize TRNG
  MCLK->APBCMASK.bit.TRNG_ = 1;
  TRNG->CTRLA.bit.ENABLE = 1;
}

uint32_t get_trng() {
  while ((TRNG->INTFLAG.reg & TRNG_INTFLAG_MASK) == 0);
  return (TRNG->DATA.reg);
}

void loop() {
  uint32_t seed = get_trng();  // get TRNG seed
  uint8_t dataToSend = 0xAB;   // example data byte to send

  // Send data via SPI
  digitalWrite(SPI_CS_PIN, LOW);
  SPI.transfer(seed >> 24);
  SPI.transfer(seed >> 16);
  SPI.transfer(seed >> 8);
  SPI.transfer(seed);
  SPI.transfer(dataToSend);
  digitalWrite(SPI_CS_PIN, HIGH);
  
  delay(2000); // wait for 2 seconds
}
