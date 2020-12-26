#include "DCF77.h"
#include "Time.h"
#include <LiquidCrystal.h>

#define DCF_PIN 2           // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0    // Interrupt number associated with pin
#define PIN_LED 13
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//time_t time;
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT, false);
// wurde ein gueltiges Signal gefunden
bool g_bDCFTimeFound = false;
unsigned long lastDcfRun = 0;
unsigned long lastDcfUpdate = 0;
unsigned int lastSecond = 0;

void setup() 
{
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600); 
  DCF.Start();
  Serial.println("Waiting for DCF77 time ... ");
  Serial.println("It will take at least 2 minutes until a first update can be processed.");
    lcd.begin(16, 2);
  lcd.print("Maunzi jagd Katzen!");
}

void loop() 
{
unsigned long aktuelleZeit = millis();

if(lastSecond != second())
{
  lastSecond = second();
  lcd.setCursor(0, 0);
  lcd.print(hour());
  lcd.print(":");
  lcd.print(minute());
  lcd.print(":");
  lcd.print(second());
  //lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(day());
  lcd.print(".");
  lcd.print(month());
  lcd.print(".");
  lcd.print(year()-2000); 
  lcd.setCursor(15, 1);
  lcd.print("S");
if(((aktuelleZeit-lastDcfUpdate)>200000) || (lastDcfUpdate == 0))
{
  if (year() == 1970)
  {
  lcd.setCursor(0, 1);
  lcd.print("Suche Maunzi    ");
  }
}
    
}


if (((aktuelleZeit -lastDcfRun) > 5000) || ((aktuelleZeit -lastDcfRun) < 0))
{
  digitalWrite(PIN_LED, HIGH);
  delay(50);
  digitalWrite(PIN_LED, LOW);

  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime != 0)
  {
    Serial.println("Time is updated");
    setTime(DCFtime);
    g_bDCFTimeFound = true;
    lastDcfUpdate = aktuelleZeit;
  }
  
  // die Uhrzeit wurde gesetzt, also LED nach kurzer Zeit ein
  if (g_bDCFTimeFound)
  {
    delay(50);
    digitalWrite(PIN_LED, HIGH);
  } 
  digitalClockDisplay(); 
  lastDcfRun = aktuelleZeit;
}
}//End loop

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
