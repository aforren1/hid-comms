int n;
int i;

const int led_pin = 13;

uint8_t buffer_tx[64];
uint8_t buffer_rx[64];

void setup() {
  pinMode(led_pin, OUTPUT); // set up to blink LED
}

void loop() {
  n = RawHID.recv(buffer_rx, 0);
  if (n > 0) {
    digitalWrite(led_pin, HIGH);
    for (i = 0; i < 64; i++) {
      buffer_tx[i] = buffer_rx[i];
    }
    RawHID.send(buffer_tx, 0);
    delay(200);
    digitalWrite(led_pin, LOW);
  }
}
