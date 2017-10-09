// compile with rawhid, make sure correct device is selected
// note that arduino is little endian, so we need to do the swap to big if using the number on the device

//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val ) 
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap short
int16_t swap_int16( int16_t val ) 
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}

//! Byte swap int
int32_t swap_int32( int32_t val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

const int ledPin = 13;
byte buffer_tx[64]; // transfer
byte buffer_rx[64]; // receive
int n; // any data in?
int i; // loop counter

char chr;
int nt;
long lng;
float flt;
uint8_t *array; // used for float

byte comp; // first byte in packet

float float_swap(float value){
       long temp =  swap_int32(*(unsigned int*)&value);
       return *(float*)&temp;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //See if there's new data
  n = RawHID.recv(buffer_rx, 0);
  // if there's new data, blink the LED
  // and do something with the data
  if (n > 0)
  {
    digitalWrite(ledPin, HIGH);
    // how to interpret the data
    // float, works
    comp = buffer_rx[0];
    if (comp == 'f') {
      unsigned char b[] = {buffer_rx[1], buffer_rx[2], buffer_rx[3], buffer_rx[4]};
      memcpy(&flt, &b, sizeof(flt));
      // practice arithmetic on float -- need to swap bytes, do addition, then swap back
      flt = float_swap(flt);
      flt += 3.0;
      flt = float_swap(flt);
      buffer_tx[0] = 'f';
      array = reinterpret_cast<uint8_t*>(&flt);
      buffer_tx[1] = array[0];
      buffer_tx[2] = array[1];
      buffer_tx[3] = array[2];
      buffer_tx[4] = array[3];
      for (i = 5; i < 64; i++) {
        buffer_tx[i] = 0;
      }
    } 
    // char, works
    else if (comp == 'c') 
    {
      chr = buffer_rx[1];
      buffer_tx[0] = 'c';
      buffer_tx[1] = chr;
      for (i = 2; i < 64; i++) {
        buffer_tx[i] = 0;
      }
    }
    // int, seems off
    else if (comp == 'i')
    {
      nt = (buffer_rx[1] << 8 | buffer_rx[2]);
      // arithmetic, then convert back
      nt += 3;
      buffer_tx[0] = 'i';
      buffer_tx[1] = highByte(nt);
      buffer_tx[2] = lowByte(nt);
      for (i = 3; i < 64; i++) {
        buffer_tx[i] = 0;
      }
    }
    // long, definitely off
    else if (comp == 'l')
    {
      lng = (long)(buffer_rx[1] << 24 | buffer_rx[2] << 16 | buffer_rx[3] << 8 | buffer_rx[4]);
      lng *= 10;
      buffer_tx[0] = 'l';
      buffer_tx[1] = (int)((lng >> 24) & 0xFF);
      buffer_tx[2] = (int)((lng >> 16) & 0xFF);
      buffer_tx[3] = (int)((lng >> 8) & 0xFF);
      buffer_tx[4] = (int)(lng & 0xFF);
      for (i = 5; i < 64; i++) {
        buffer_tx[i] = 0;
      }
    }
    // echo the entire packet back
    else if (comp == 'e')
    {
      for (i = 0; i < 64; i++) {
        buffer_tx[i] = buffer_rx[i];
      }
    }
    // char not recognized, send back a few bytes as a hint
    else
    {
      buffer_tx[0] = 'x';
      buffer_tx[1] = 'x';
      buffer_tx[2] = 'x';
      buffer_tx[3] = buffer_rx[0]; // the offending command
      buffer_tx[4] = buffer_rx[1];
      buffer_tx[5] = buffer_rx[2];
      buffer_tx[6] = buffer_rx[3];
      buffer_tx[7] = highByte(n);
      buffer_tx[8] = lowByte(n);
      for (i = 9; i < 64; i++) {
        buffer_tx[i] = 0;
      }
    }
    
    RawHID.send(buffer_tx, 10);
    delay(400);
    digitalWrite(ledPin, LOW);
  } // end if (n > 0)
  
} // end loop()
