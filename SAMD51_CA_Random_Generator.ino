const int CA_SIZE = 32;  // size of the cellular automaton array

void setup() {
  Serial.begin(115200);
  while (!Serial) ;  // Wait for Serial Monitor to open
  Serial.println("1D Cellular Automaton with TRNG seed\n");
  MCLK->APBCMASK.bit.TRNG_ = 1;  // enable clock  (APBC clock is already enabled)
  TRNG->CTRLA.bit.ENABLE = 1;    // enable the TRNG
}

uint32_t get_trng() {
  while ((TRNG->INTFLAG.reg & TRNG_INTFLAG_MASK) == 0) ;  // Busy loop waiting for next RN to be created
  return (TRNG->DATA.reg);   // return the new number.
}

void loop() {
  bool ca[CA_SIZE] = {0};
  uint32_t seed = get_trng();

  // Seed the CA with the TRNG
  for (int i = 0; i < CA_SIZE; i++) {
    ca[i] = (seed >> i) & 1;
  }

  // Iterate the CA and generate numbers
  for (int i = 0; i < 50; i++) {
    bool new_ca[CA_SIZE];
    for (int j = 1; j < CA_SIZE - 1; j++) {
      new_ca[j] = ca[j - 1] ^ (ca[j] || ca[j + 1]); // Rule 30
    }
    memcpy(ca, new_ca, sizeof(ca));

    // Convert the CA state to a number
    uint32_t number = 0;
    for (int j = 0; j < CA_SIZE; j++) {
      number |= (ca[j] << j);
    }

    // Print the generated number
    Serial.println(number);
  }

  delay(1000);  // Optional: add a short delay between each group of 50 numbers for readability
}
