#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int BACK_LIGHT = 13;
const int TEMP = 0;
const int BUTTON = 7;

const int TIME_ON = 10000;

void setup() {
  Serial.begin(9600);
  
  pinMode(BACK_LIGHT, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(BACK_LIGHT, HIGH);

  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor( 0, 0 );
  lcd.print( "--- INIT ---");
  
  delay(2000);
  
  lcd.noDisplay();
  digitalWrite(BACK_LIGHT, LOW);
}

void loop() {

  int buttonState = digitalRead(BUTTON);
  
  if ( buttonState == HIGH) {
    lcd.display();
    digitalWrite(BACK_LIGHT, HIGH);
    lcd.setCursor( 0, 0 );
    lcd.print( "Temperatura:");
    
    int val_Adc = analogRead(TEMP);
    //Converte il valore letto in tensione
    float voltage = (val_Adc /1024.0) * 5.0;
    //Converte il valore di tensione in temperatura
    float temp = (voltage - .5) * 100;
    // Trasmette sulla seriale il valore della tensione letto
    Serial.print(voltage,4); 
    Serial.print(" volts    ");
    // Trasmette sulla seriale il valore della temperatura
    Serial.print(temp,2);  
    Serial.println(" gradi celsius");

    lcd.setCursor( 3, 1 );
    // scrive la temperatura
    lcd.print( temp, 1 );
    // lascia uno spazio
    lcd.print( ' ' );
    // Scrive '°'
    lcd.print( (char) 223 );
    //Scrive il carattere "C"
    lcd.print( 'C' );

    delay(TIME_ON);
  } else {
    lcd.noDisplay();
    digitalWrite(BACK_LIGHT, LOW);
    lcd.clear();
  }
}
