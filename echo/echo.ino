int n;
int i;

byte buffer_tx[64];
byte buffer_rx[64];

void setup() {
  pinMode(13, OUTPUT); // set up to blink LED
}

void loop() {
  n = RawHID.recv(buffer_rx, 0);
  if (n > 0) {
    for (i = 0; i < 64; i++) {
      buffer_tx[i] = buffer_rx[i];
    }
    RawHID.send(buffer_tx, 0);
  }
}
