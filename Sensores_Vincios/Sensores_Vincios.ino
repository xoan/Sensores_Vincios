#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int orange_led = 2;
int green_led = 3;
int blue_led = 4;

float temp = 0;
int lum = 0;
int humid = 0;

unsigned long millis_ts = 0;
unsigned long interval_ts = 60000; // thingspeak interval

unsigned long millis_lcd = 0;
int interval_lcd = 5000; // lcd interval

int page_lcd = 0;

void setup() {
  pinMode(orange_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);

  digitalWrite(orange_led, HIGH);
  digitalWrite(green_led, HIGH);
  digitalWrite(blue_led, HIGH);

  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("CEIP Vincios");
}

void loop() {
  temp = analogRead(A0)*0.488;
  lum = map(analogRead(A1), 0, 1023, 100, 0);
  humid = map(analogRead(A2), 0, 1023, 100, 0);

  if (humid < 33) {
    digitalWrite(orange_led, HIGH);
    digitalWrite(green_led, LOW);
    digitalWrite(blue_led, LOW);
  } else if (humid >= 33 && humid < 66) {
    digitalWrite(orange_led, LOW);
    digitalWrite(green_led, HIGH);
    digitalWrite(blue_led, LOW);
  } else {
    digitalWrite(orange_led, LOW);
    digitalWrite(green_led, LOW);
    digitalWrite(blue_led, HIGH);
  }
  
  unsigned long millis_current = millis();

  if (millis_current - millis_lcd >= interval_lcd) {
    millis_lcd = millis_current;   
    lcd.clear();
    if (page_lcd > 2) page_lcd = 0;

    if (page_lcd == 0) {
      lcd.print("TEMPERATURA");
      lcd.setCursor(0,1);
      lcd.print(round(temp));
      lcd.print(" ");
      lcd.print(char(223));
      lcd.print("C");
    } else if (page_lcd == 1) {
      lcd.print("LUMINOSIDADE");
      lcd.setCursor(0,1);
      lcd.print(lum);
      lcd.print(" %");
    } else {
      lcd.print("HUMIDADE");
      lcd.setCursor(0,1);
      lcd.print(humid);
      lcd.print(" %");
    }

    page_lcd++;
  }
 
  if (millis_current - millis_ts >= interval_ts) {
    millis_ts = millis_current;   

    Serial.print("field1=");
    Serial.print(temp);
    Serial.print("&field2=");
    Serial.print(lum);
    Serial.print("&field3=");
    Serial.println(humid);
  }
}
