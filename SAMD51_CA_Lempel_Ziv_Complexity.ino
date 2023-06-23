const int CA_SIZE = 32;  // size of the cellular automaton array
const int CA_ITERATIONS = 1000; // number of CA iterations

void setup() {
  Serial.begin(115200);
  while (!Serial) ;  // Wait for Serial Monitor to open
  Serial.println("1D Cellular Automaton with TRNG seed and Lempel-Ziv complexity measurement\n");
  MCLK->APBCMASK.bit.TRNG_ = 1;  // enable clock  (APBC clock is already enabled)
  TRNG->CTRLA.bit.ENABLE = 1;    // enable the TRNG
}

uint32_t get_trng() {
  while ((TRNG->INTFLAG.reg & TRNG_INTFLAG_MASK) == 0) ;  // Busy loop waiting for next RN to be created
  return (TRNG->DATA.reg);   // return the new number.
}

int lempel_ziv_complexity(bool sequence[], int size) {
  int complexity = 1;
  int substr_length = 1;
  int i = 1;

  while (i < size) {
    bool found = false;

    for (int j = 0; j < i; j++) {
      bool match = true;

      for (int k = 0; k < substr_length; k++) {
        if (j + k >= i || sequence[j + k] != sequence[i + k]) {
          match = false;
          break;
        }
      }

      if (match) {
        found = true;
        break;
      }
    }

    if (found) {
      i += substr_length;
      substr_length = 1;
    } else {
      substr_length++;
      if (i + substr_length > size) {
        complexity++;
        break;
      }
    }
  }

  return complexity;
}

void loop() {
  bool ca[CA_SIZE] = {0};
  bool ca_sequence[CA_SIZE * CA_ITERATIONS] = {0};
  uint32_t seed = get_trng();

  // Seed the CA with the TRNG
  for (int i = 0; i < CA_SIZE; i++) {
    ca[i] = (seed >> i) & 1;
  }

  // Iterate the CA
  for (int i = 0; i < CA_ITERATIONS; i++) {
    bool new_ca[CA_SIZE];
    for (int j = 1; j < CA_SIZE - 1; j++) {
      new_ca[j] = ca[j - 1] ^ (ca[j] || ca[j + 1]); // Rule 30
    }
    memcpy(ca, new_ca, sizeof(ca));

    // Store the state to ca_sequence for LZ complexity measurement
    memcpy(ca_sequence + i * CA_SIZE, ca, sizeof(ca));
  }

  // Measure Lempel-Ziv complexity
  int complexity = lempel_ziv_complexity(ca_sequence, CA_SIZE * CA_ITERATIONS);
  Serial.print("Lempel-Ziv complexity: ");
  Serial.println(complexity);

  delay(5000);  // Optional: add a delay between each measurement for readability
}
