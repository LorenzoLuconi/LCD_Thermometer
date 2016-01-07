#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int BACK_LIGHT_PIN = 13;
const int TEMP_PIN = 0;
const int BUTTON_PIN = 7;

const int DISPLAY_ON_INTERVAL = 10000;
const int TEMP_READ_INTERVAL = 30000;

typedef struct temperature {
  float temp;
  float humidity;
} Temp;

Temp minTemp = { 2000.0, 2000.0 };
Temp maxTemp = { 0, 0 };
Temp actualTemp;
bool first = true;

int buttonState = LOW;
int prevButtonState = LOW;

long tempReadTime = 0;
long displayOnTime = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(BACK_LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  lcd.begin(16,2);
}

void loop() {
  if ( tempReadTime == 0 || tempReadTime + TEMP_READ_INTERVAL < millis()) {

    actualTemp = readTemp();
    serialWrite(actualTemp);
    
    minTemp.temp = min(minTemp.temp, actualTemp.temp);
    minTemp.humidity = min(minTemp.humidity, actualTemp.humidity);

    maxTemp.temp = max(maxTemp.temp, actualTemp.temp);
    maxTemp.humidity = max(maxTemp.humidity, actualTemp.humidity);

    Serial.println("Min:");
    serialWrite(minTemp);
    Serial.println("Max:");
    serialWrite(maxTemp);

    tempReadTime = millis();
  }

  int buttonState = digitalRead(BUTTON_PIN);

  if ( first || buttonState == HIGH ) {
    first = false;
    lcdWrite(actualTemp);
    displayOn();
  }
  
  displayOff();
}

Temp readTemp() {
  int val_Adc = analogRead(TEMP_PIN);
  float voltage = (val_Adc /1024.0) * 5.0;
  //float temp = ;
  
  return Temp { (voltage - .5) * 100, 0 };
}

void lcdWrite(Temp t) {
    lcd.clear();
    
    // Temperature
    lcd.setCursor( 0, 0 );
    lcd.print("Temp.: ");
    lcd.print( t.temp, 1 );
    lcd.print( ' ' );
    lcd.print( (char) 223 );
    lcd.print( 'C' );

    // Humidity
    lcd.setCursor( 0, 1 );
    lcd.print("Hum.: ");
    lcd.print( t.humidity, 1 );
    lcd.print( '%' );
}

void serialWrite(Temp t) {
  Serial.print("Temperature: ");
  Serial.print(t.temp, 2);
  Serial.print("        Humidity:");
  Serial.println(t.humidity, 0);
}

void displayOn() {
  displayOnTime = millis();
  lcd.display();
  digitalWrite(BACK_LIGHT_PIN, HIGH);
}

void displayOff() {
  if ( displayOnTime + DISPLAY_ON_INTERVAL < millis() ) {
    lcd.noDisplay();
    digitalWrite(BACK_LIGHT_PIN, LOW);
  }
}


