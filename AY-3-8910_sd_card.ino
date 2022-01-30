// Reading a SNG song file and playing it through an AY-3-8910 programmable sound generator

// Used in the video https://www.youtube.com/watch?v=srmNbi9yQNU
// Sérgio Vieira 2022

#include <SD.h>

const int SD_CARD_SELECT_PIN = 10;
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
  DDRD = 0xff;
  // Set pins 0 to 7 to output LOW
  PORTD = 0;

  set_mode_inactive();

  // Reset the AY-3-8910
  digitalWrite(RESET_PIN, LOW);
  delay(1);
  digitalWrite(RESET_PIN, HIGH);

  // Initialize the SD Card interface
  SD.begin(SD_CARD_SELECT_PIN);
}

void loop() {
  // Open the SNG file
  File file = SD.open("output.sng");

  if (file)
  {
    // Read header (could/should check it)
    file.read();
    file.read();
    file.read();
    file.read();
    file.read();

    unsigned char reg = 0;
    unsigned char value = 0;

    while (reg != 0xff)
    {
      reg = file.read();
      value = file.read();

      if (reg < 16)
      {
        write_register(reg, value);
      }
      else if (reg == 16)
      {
        delay(20 * (value + 1));
      }
    }

    // We've reached the end of the SNG file, so let's close it
    file.close();
  }
}

