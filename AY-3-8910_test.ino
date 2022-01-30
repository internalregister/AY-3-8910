// Interfacing the Arduino with an AY-3-8910 and playing tones in its channel A
// Used in the video https://www.youtube.com/watch?v=srmNbi9yQNU
// Sérgio Vieira 2022

const int RESET_PIN = 8;
const int BC1_PIN = A5;
const int BDIR_PIN = A4;

void set_mode_inactive()
{
  digitalWrite(BC1_PIN, LOW);
  digitalWrite(BDIR_PIN, LOW);
}

void set_mode_latch()
{
  digitalWrite(BC1_PIN, HIGH);
  digitalWrite(BDIR_PIN, HIGH);
}

void set_mode_write()
{
  digitalWrite(BC1_PIN, LOW);
  digitalWrite(BDIR_PIN, HIGH);
}

void write_register(char reg, char value)
{
  set_mode_latch();  
  PORTD = reg;  
  set_mode_inactive();  
  set_mode_write();  
  PORTD = value;  
  set_mode_inactive();
}

void setup() {
  // Set up Timer 1 to output a 2 MHZ clock signal in Pin 9
  TCCR1A = bit(COM1A0);
  TCCR1B = bit(WGM12) | bit(CS10);
  OCR1A = 3;
  pinMode(9, OUTPUT);

  pinMode(RESET_PIN, OUTPUT);
  pinMode(BC1_PIN, OUTPUT);
  pinMode(BDIR_PIN, OUTPUT);

  // Set pins 0 to 7 to output
  DDRD = 0xFF;
  // Set pins 0 to 7 to output LOW
  PORTD = 0x00;

  set_mode_inactive();

  // Reset the AY-3-8910
  digitalWrite(RESET_PIN, LOW);
  delay(1);
  digitalWrite(RESET_PIN, HIGH);

  // Enable only the Tone Generator on Channel A
  write_register(7, 0b00111110);
  
  // Set the amplitude (volume) to maximum on Channel A
  write_register(8, 0b00001111);
}

void loop() {
  // Change the Tone Period for Channel A every 500ms

  write_register(0, 223);
  write_register(1, 1);

  delay(500);
  
  write_register(0, 170);
  write_register(1, 1);

  delay(500);
  
  write_register(0, 123);
  write_register(1, 1);

  delay(500);
  
  write_register(0, 102);
  write_register(1, 1);

  delay(500);
  
  write_register(0, 63);
  write_register(1, 1);

  delay(500);
  
  write_register(0, 28);
  write_register(1, 1);

  delay(500);
  
  write_register(0, 253);
  write_register(1, 0);

  delay(500);
}

