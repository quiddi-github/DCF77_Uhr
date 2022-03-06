#include "DCF77.h"
#include "Time.h"
#include <LiquidCrystal.h>
#include "DHT.h" //DHT Bibliothek laden

#define DCF_PIN 2           // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0    // Interrupt number associated with pin
#define PIN_LED 13
#define DHTINNENPIN 9 //Der Sensor wird an PIN 9 angeschlossen
#define DHTTYPEINNEN DHT11    
char* Wochentag[8] = {"xx","So","Mo","Di","Mi","Do","Fr","Sa"};
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT dhtinnen(DHTINNENPIN, DHTTYPEINNEN);
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
  dhtinnen.begin(); 
  Serial.println("Waiting for DCF77 time ... ");
  Serial.println("It will take at least 2 minutes until a first update can be processed.");
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
  lcd.print("Maunzi jagd");
  lcd.setCursor(0, 1);
  lcd.print("Katzen!");
}

void loop() 
{
unsigned long aktuelleZeit = millis();
AusgabeLCDDisplay( aktuelleZeit);

CheckDCFUpdate ( aktuelleZeit); //Update DCF-Time and Print it with the Serial Monitor

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

// Temperatursensor kalibrieren
float TempKalib (float TempGemessen)
{ return (TempGemessen - ((0.04739336 * (TempGemessen))+0.78151659));
 }

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
int runden (float Temperatur)
{ int IntTemperatur = Temperatur; 
if (abs((Temperatur-IntTemperatur))>= 0.5 &&  Temperatur > 0) //Aufrunden bei positiven Werten
{IntTemperatur++;
  }
if (abs((Temperatur-IntTemperatur))>= 0.5 &&  Temperatur < 0) //Aufrunden bei negativen Werten
{IntTemperatur--;
  }  
  return IntTemperatur;
  }
void AusgabeLCDDisplay(unsigned long aktuelleZeit)
{if(lastSecond != second())
{
  if (lastSecond=59)
    {lcd.clear();
    }
    
  lastSecond = second();
  
  lcd.setCursor(0, 0);
  lcd.print(Wochentag[weekday()]);
  lcd.print(" ");
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
  lcd.print(year());
  lcd.setCursor(12, 0); //
  lcd.print("W");
  lcd.print(ymdToWeekNumber (year(), month(), day()));
  //lcd.print(dhtinnen.readTemperature());//
  lcd.setCursor(15, 0);
  lcd.print("S");
  lcd.setCursor(11, 1); 
  lcd.print(TempKalib (dhtinnen.readTemperature()) );//dhtinnen.readTemperature());
  lcd.setCursor(15, 1); 
  lcd.print("C");
  //lcd.print(" ");
if(((aktuelleZeit-lastDcfUpdate)>200000) || (lastDcfUpdate == 0))
{
    lcd.setCursor(15, 0);// Lösche letztes Zeichen, wenn das DCF-Update zu weit zurück liegt.
    lcd.print(" ");
  
  if (year() == 1970) //Wenn er das erste mal eingesteckt wird, zeige Suche Maunzi
  {
  lcd.setCursor(0, 1);
  lcd.print("Suche Maunzi    ");
  }
}// Ende: if(((aktuelleZeit-lastDcfUpdate)>200000)...
}// Ende: if(lastSecond != second())
  }//Ende: void AusgabeLCDDisplay

    void CheckDCFUpdate (unsigned long aktuelleZeit)
{
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
  }// Ende: void CheckDCFUpdate

// Anfang: Kalenderwoche berechnen
int ymdToWeekNumber (int y, int m, int d) {
  // reject out-of-range dates
  if ((y < 1901)||(y > 2099)) return 0;
  if ((m < 1)||(m > 12)) return 0;
  if ((d < 1)||(d > 31)) return 0;
  // take advantage of 28-year cycle
  while (y >= 1929) y -= 28;
  // compute adjustment for dates within the year
  //     If Jan. 1 falls on: Mo Tu We Th Fr Sa Su
  // then the adjustment is:  6  7  8  9  3  4  5
  int adj = (y-1873) + ((y-1873)>>2);
  while (adj > 9) adj -= 7;
  // compute day of the year (in range 1-366)
  int doy = d;
  for (int i=1; i<m; i++) doy += (30 + ((0x15AA >> i) & 1));
  if (m > 2) doy -= ((y & 3) ? 2 : 1);
  // compute the adjusted day number
  int dnum = adj + doy;
  // compute week number
  int wknum = dnum >> 3;
  dnum -= ((wknum << 3) - wknum);
  while (dnum >= 7) {
    dnum -= 7;
    wknum++;
  }
  // check for boundary conditions
  if (wknum < 1) {
    // last week of the previous year
    // check to see whether that year had 52 or 53 weeks
    // re-compute adjustment, this time for previous year
    adj = (y-1874) + ((y-1874)>>2);
    while (adj > 9) adj -= 7;
    // all years beginning on Thursday have 53 weeks
    if (adj==9) return 53;
    // leap years beginning on Wednesday have 53 weeks
    if ((adj==8) && ((y&3)==1)) return 53;
    // other years have 52 weeks
    return 52;
  }
  if (wknum > 52) {
    // check to see whether week 53 exists in this year
    // all years beginning on Thursday have 53 weeks
    if (adj==9) return 53;
    // leap years beginning on Wednesday have 53 weeks
    if ((adj==8) && ((y&3)==0)) return 53;
    // other years have 52 weeks
    return 1;
  }
  return wknum;
}
//Ende: Kalenderwoche berechnen

  
    
