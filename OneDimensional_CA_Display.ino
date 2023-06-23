#define CA_SIZE 32
#define RULE 30

int ca[CA_SIZE];

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  // Initialize the first generation
  for (int i = 0; i < CA_SIZE; i++) {
    ca[i] = 0;
  }
  ca[CA_SIZE / 2] = 1; // Single seed in the middle
}

int rules(int a, int b, int c) {
  return ((RULE >> (a * 4 + b * 2 + c)) & 1);
}

void draw() {
  int nextGen[CA_SIZE];

  // Display current generation in Serial Monitor
  for (int i = 0; i < CA_SIZE; i++) {
    Serial.print(ca[i] ? '1' : '0');
  }
  Serial.println();

  // Calculate next generation
  for (int i = 1; i < CA_SIZE - 1; i++) {
    nextGen[i] = rules(ca[i-1], ca[i], ca[i+1]);
  }
  
  // Copy the next generation into the current one
  for (int i = 1; i < CA_SIZE - 1; i++) {
    ca[i] = nextGen[i];
  }

  // Display current generation as decimal in Serial Plotter
  int decimalValue = 0;
  for (int i = 0; i < CA_SIZE; i++) {
    decimalValue = decimalValue * 2 + ca[i];
  }
  Serial.println(decimalValue);
}

void loop() {
  draw();
  delay(500);
}
