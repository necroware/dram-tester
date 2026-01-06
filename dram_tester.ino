// 4164/41256 DRAM tester for Arduino Nano by Necroware
// License: CC-BY-SA-4.0

const uint8_t JP1 = A4;
const uint8_t G_LED = A2;
const uint8_t R_LED = A3;
const uint8_t PIN_RAS = A1;
const uint8_t PIN_CAS = A0;
const uint8_t PIN_WE = 13;
const uint8_t PIN_DO = 11;
const uint8_t PIN_DI = 12;

void set_address(uint16_t value) {
  value &= 0b111111111;
  PORTD = (PORTD & 0b00000011) | (value << 2);
  PORTB = (PORTB & 0b11111000) | (value >> 6);
}

void reset_pins() {
  digitalWrite(PIN_WE, HIGH);
  digitalWrite(PIN_CAS, HIGH);
  digitalWrite(PIN_RAS, HIGH);
}

void dram_write(int row, int col, int val) {
  set_address(row);
  digitalWrite(PIN_RAS, LOW);
  digitalWrite(PIN_WE, LOW);
  digitalWrite(PIN_DI, val);
  set_address(col);
  digitalWrite(PIN_CAS, LOW);
  digitalRead(PIN_DO);
  reset_pins();
}

int dram_read(int row, int col) {
  set_address(row);
  digitalWrite(PIN_RAS, LOW);
  set_address(col);
  digitalWrite(PIN_CAS, LOW);
  const auto val = digitalRead(PIN_DO);
  reset_pins();
  return val;
}

void set_leds(bool state) {
  digitalWrite(G_LED, state);
  digitalWrite(R_LED, !state);
}

template <typename T>
bool test(T&& generator) {
  const auto width = digitalRead(JP1) ? 64u : 256u;

  using operation_t = bool (*)(uint16_t, uint16_t, uint8_t);
  const auto iterate = [&](const operation_t& operation) {
    for (auto row = 0u; row < width; row++) {
      for (auto col = 0u; col < width; col++) {
        const auto pattern = generator(row, col) & 0x1;
        if (!operation(row, col, pattern)) {
          return false;
        }
      }
      // make it blink every 4th row
      if ((row & 0x3) == 0) {
        set_leds(row & 0x4);
      }
    }
    return true;
  };

  iterate([](uint16_t row, uint16_t col, uint8_t pattern) {
    dram_write(row, col, pattern);
    return true;
  });

  return iterate([](uint16_t row, uint16_t col, uint8_t pattern) {
    return dram_read(row, col) == pattern;
  });
}

void setup() {
  // Setup address pins
  DDRD |= 0b11111100;  // pins 2–7 as outputs
  DDRB |= 0b00000111;  // pins 8–10 as outputs

  pinMode(G_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(PIN_RAS, OUTPUT);
  pinMode(PIN_CAS, OUTPUT);
  pinMode(PIN_WE, OUTPUT);
  pinMode(PIN_DI, OUTPUT);
  pinMode(PIN_DO, INPUT);
  pinMode(JP1, INPUT_PULLUP);

  reset_pins();
}

void loop() {
  noInterrupts();
  set_leds(
      // checkmate
      test([](uint16_t row, uint16_t col) { return row + col; }) &&
      // inverted checkmate
      test([](uint16_t row, uint16_t col) { return ~(row + col); }) &&
      // all 1's
      test([](uint16_t row, uint16_t col) { return 1; }) &&
      // all 0's
      test([](uint16_t row, uint16_t col) { return 0; }));
  interrupts();

  while (true) {
    delay(1000);
  }
}
