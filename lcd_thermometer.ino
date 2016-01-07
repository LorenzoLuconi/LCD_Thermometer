#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int BACK_LIGHT_PIN = 13;
const int TEMP_PIN = 0;
const int BUTTON_PIN = 7;

const int TIME_ON = 10000;

typedef struct {
  float temp;
  float humidity;
} Temp;

void setup() {
  Serial.begin(9600);
  
  pinMode(BACK_LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print( "Thermometer ...");

  displayOn();
  delay(2000);
  displayOff();
}

void loop() {

  int buttonState = digitalRead(BUTTON_PIN);
  
  if ( buttonState == HIGH) {
    Temp t = readTemp();
  
    lcdWrite(t);
    serialWrite(t);
    
    displayOn();
    
    delay(TIME_ON);
  } else {
    displayOff();
  }
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
    lcd.setCursor( 0, 0 );
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
    lcd.display();
    digitalWrite(BACK_LIGHT_PIN, HIGH);
}

void displayOff() {
    lcd.noDisplay();
    digitalWrite(BACK_LIGHT_PIN, LOW);
}


