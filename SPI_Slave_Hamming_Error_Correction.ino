#include <SPI.h>

#define SPI_MISO_PIN 22

void setup() {
  // Setup SPI Slave
  pinMode(SPI_MISO_PIN, OUTPUT);
  SPCR |= _BV(SPE);

  // Initialize TRNG
  MCLK->APBCMASK.bit.TRNG_ = 1;
  TRNG->CTRLA.bit.ENABLE = 1;
}

uint32_t get_trng() {
  while ((TRNG->INTFLAG.reg & TRNG_INTFLAG_MASK) == 0);
  return (TRNG->DATA.reg);
}

uint8_t decode_hamming(uint8_t encodedData) {
  uint8_t p0 = (encodedData >> 4) & 0x1;
  uint8_t p1 = (encodedData >> 5) & 0x1;
  uint8_t p2 = (encodedData >> 6) & 0x1;

  uint8_t d0 = encodedData & 0x1;
  uint8_t d1 = (encodedData >> 1) & 0x1;
  uint8_t d2 = (encodedData >> 2) & 0x1;
  uint8_t d3 = (encodedData >> 3) & 0x1;

  uint8_t e0 = p0 ^ d0 ^ d1 ^ d3;
  uint8_t e1 = p1 ^ d0 ^ d2 ^ d3;
  uint8_t e2 = p2 ^ d1 ^ d2 ^ d3;

  uint8_t errorPosition = (e2 << 2) | (e1 << 1) | e0;

  if (errorPosition) {
    encodedData ^= (1 << (errorPosition - 1));
  }

  return (d3 << 3) | (d2 << 2) | (d1 << 1) | d0;
}

void loop() {
  if (SPDR != 0) { // check if data is received via SPI
    // Extract encoded data
    uint8_t receivedData = SPDR;

    // Decode and correct data using Hamming(7,4) code
    uint8_t decodedData = decode_hamming(receivedData);

    // Here decodedData can be used or sent back to master if needed
  }
  delay(10);
}
