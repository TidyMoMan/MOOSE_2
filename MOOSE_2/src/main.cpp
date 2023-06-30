#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

int delayVal = 0;
int pitch = 0;
int lowSwitch = 8;
int selSwitch = 17;
int rightButton = 11;
int leftButton = 10;
int buzzer = 16;
boolean silent;

String modes[] = {"Zapper", "Flashl", "Music ", "EvlKey", "RndChr"};

const int rs = 4, en = 6,  d4 = 0, d5 = 1, d6 = 2, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define LED_PIN     5
#define LED_COUNT  16
#define BRIGHTNESS 5 //max 255

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void rainbow(int loops, int interval){
  for(int k = 0; k < loops; k++){
    for(int i = 0; i < 16; i++){
        strip.setPixelColor(i, random(0, 60), 0, random(0, 60));
        strip.show();
        delay(interval);
      }
      for(int i = 0; i < 16; i++){
        strip.setPixelColor(i, 0, 0, 0);
        strip.show();
        delay(interval);
      }
  } 
  strip.clear();
  strip.show();
}

void fullColor(int red, int green, int blue, int delayms){
    for(int i = 0; i < 16; i++){
        strip.setPixelColor(i, red, green, blue);
        strip.show();
        delay(delayms);
    }
}

void zapper(){
  lcd.clear();
  boolean bounce = true;
  int dutyOn = 20;
  int dutyOff = 200;
  while(digitalRead(selSwitch)==LOW){

    if(digitalRead(rightButton)==HIGH && bounce == true){
      bounce = false;
      lcd.clear();
      lcd.print("firing...");
      tone(buzzer, 750, 200);
      fullColor(45, 0, 0, 15);
      fullColor(0, 0, 0, 15);
      digitalWrite(12, HIGH);
      delay(dutyOn);
      digitalWrite(12 , LOW);
      delay(dutyOff);
      digitalWrite(18, HIGH);
      delay(dutyOn);
      digitalWrite(18, LOW);
      delay(dutyOff);
      digitalWrite(9, HIGH);
      delay(dutyOn);
      digitalWrite(9, LOW);
      delay(dutyOff);
      }else{
        if(digitalRead(lowSwitch) == HIGH){
          dutyOn = map(analogRead(26), 0, 1024, 15, 200);
          lcd.setCursor(0, 0);
          lcd.print("intMs: "); lcd.print(dutyOff); lcd.print("    ");
          lcd.setCursor(0, 1);
          lcd.print("lenMs: "); lcd.print(dutyOn); lcd.print(" <<");
        }else{
          dutyOff = map(analogRead(26), 0, 1024, 80, 1000);
          lcd.setCursor(0, 0);
          lcd.print("intMs: "); lcd.print(dutyOff); lcd.print(" <<");
          lcd.setCursor(0, 1);
          lcd.print("lenMs: "); lcd.print(dutyOn); lcd.print("    ");
        }
        bounce = true;
        delay(300);
      }
  }

}

void flashLight(){
  lcd.clear();
  lcd.setCursor(0, 0);
  boolean bounce = false;
  int red = 50, green = 50, blue = 50, editing = 0;
  while(digitalRead(selSwitch)==LOW){

    //DISCO MODE!!!!!
    while(digitalRead(lowSwitch)==HIGH){
      fullColor(random(0, 255), random(0, 255), random(0, 255), 25);
    }

    if(digitalRead(rightButton)==HIGH && editing < 2 && bounce == false){editing++; bounce = true;}
    else if(digitalRead(leftButton)==HIGH && editing > -1 && bounce == false){editing-=1;bounce = true;}
    else{bounce = false;}

    if(editing == 0){
      red = map(analogRead(26), 0, 1024, 0, 255);
    }else if(editing == 1){
      green = map(analogRead(26), 0, 1024, 0, 255);
    }else if(editing == 2){
      blue = map(analogRead(26), 0, 1024, 0, 255);
    }else{editing = 2;}
      lcd.print("R "); lcd.print(red); lcd.print(" G "); lcd.print(green); lcd.setCursor(0, 1);
      lcd.print("B "); lcd.print(blue); lcd.print(" E: "); lcd.print(editing);

      fullColor(red, green, blue, 0);
      delay(100);
      lcd.setCursor(0, 0);
  }
  lcd.write("flashlight: off");
  fullColor(0, 0, 0, 20);
}

void music(){
  lcd.clear();
  while(digitalRead(selSwitch)==LOW){
    pitch = map(analogRead(26), 0, 1024, 500, 1000);  
    delay(100);
    tone(buzzer, 600);
    delay(100);
  }
}

void randomCharacters(int lines){
  lcd.setCursor(0, 1);
  for(int r = 0; r < lines; r++){
      for(int i = 0; i < 16; i++){
      lcd.setCursor(i, 0);
      lcd.write(random(0, 2000));
      delay(30);
    }
    for(int i = 0; i < 16; i++){
      lcd.setCursor(i, 1);
      lcd.write(random(0, 2000));
      delay(30);
    }
  }
  lcd.noAutoscroll();
}

void setup() {
  lcd.begin(16, 2);
  strip.begin();
  strip.show();   
  delay(10);
  lcd.print("MOOSE II START");

  pinMode(A0, INPUT);
  pinMode(selSwitch, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(lowSwitch, INPUT);

  pinMode(16, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);


  pinMode(13, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(9, OUTPUT);
 

  rainbow(2, 20);
  tone(buzzer, 600, 50);
  delay(60);
  tone(buzzer, 750, 100);

  lcd.clear();
  strip.clear();
  strip.show();   
  
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  int kVal = analogRead(26);
  int kValrel = map(kVal, 0, 1024, 0, modes->length());

  lcd.print(modes[kValrel]); lcd.print(" <- "); lcd.print(kVal);
  lcd.setCursor(0, 1);

  if(digitalRead(selSwitch) == LOW){

    String selected = modes[kValrel];

    if(selected == "Flashl"){
      flashLight();
    }else if(selected == "Zapper"){
      zapper();
    }else if(selected == "RndChr"){
      randomCharacters(1);
    }else if(selected == "Music"){
      lcd.print("music b!");
      music();
    }else if(selected == "EvlKey"){

    }

  }else{
    lcd.setCursor(0, 1); lcd.print(modes[kValrel+1]);
    strip.clear();
    strip.show();
  }
  delay(500);
}