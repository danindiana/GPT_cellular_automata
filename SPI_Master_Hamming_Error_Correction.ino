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

uint8_t encode_hamming(uint8_t data) {
  uint8_t d0 = data & 0x1;
  uint8_t d1 = (data >> 1) & 0x1;
  uint8_t d2 = (data >> 2) & 0x1;
  uint8_t d3 = (data >> 3) & 0x1;

  uint8_t p0 = d0 ^ d1 ^ d3;
  uint8_t p1 = d0 ^ d2 ^ d3;
  uint8_t p2 = d1 ^ d2 ^ d3;

  return (p2 << 6) | (p1 << 5) | (p0 << 4) | (d3 << 3) | (d2 << 2) | (d1 << 1) | d0;
}

void loop() {
  uint8_t dataToSend = 0xA; // example 4-bit data

  // Encode data using Hamming(7,4) code
  uint8_t encodedData = encode_hamming(dataToSend);

  // Send encoded data via SPI
  digitalWrite(SPI_CS_PIN, LOW);
  SPI.transfer(encodedData);
  digitalWrite(SPI_CS_PIN, HIGH);
  
  delay(2000); // wait for 2 seconds
}
