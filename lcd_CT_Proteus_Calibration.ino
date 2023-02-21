#include <LiquidCrystal.h>             // Library for Crystal
#include <SD.h>
#include <SPI.h>

LiquidCrystal lcd( 8,  9, 4, 5, 6, 7); // Initial config for MODEL_02

int menu = 1;
int sub_menu = 1;
int aut = 1;
bool ext = false;
int a = 0;
int step = 0;
int inc = 0;
int ctrl = 3;
int gate =0;

File FileManual;
File FileAuto;

void setup(){
  lcd.begin(16, 2);
  updateMenu();

  Serial.begin(9600);
    if (SD.begin()){
      Serial.println("SD card is ready to use.");
    } else{
      Serial.println("SD card initialization failed");
    return;
    }
}

void loop(){
  a = analogRead (A0);
  while(a>600){
    if(checkButton()=='D'){
      menu++;
      updateMenu();
      delay(100);
      while (checkButton()=='D');
    }
    else if(checkButton()=='U'){
      menu--;
      updateMenu();
      delay(100);
      while (checkButton()=='U');
    }
    else if(checkButton()=='S'){
      Exec();
      updateMenu();
      delay(100);
      while (checkButton()=='S');
    }
  }
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Auto");
      lcd.setCursor(0, 1);
      lcd.print(" Manual");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Auto");
      lcd.setCursor(0, 1);
      lcd.print(">Manual");
      break;
    case 3:
      menu = 2;
      break;
  }
}

void Exec(){
  switch(menu){
    case 1:
      Auto();
      //lcd.setCursor(0, 0);  
      //lcd.print("Auto selected");
      //while(checkButton()!="S");
      //delay(1000);
      break;
    case 2:
      //Manual();
      //lcd.setCursor(0, 0);
      //lcd.print("Manual selected");
      Manual();
      //while(checkButton()!="S");
      //delay(100);
      break;
  }
}

void Manual(){
  ext = false;
  //lcd.clear();
  //lcd.setCursor(0, 1);
  //lcd.print("Press up/down");
  //delay(100);
  sub_menu=1;
  updateManual();
  delay(200);
  while(!ext){
    if(checkButton()=='D'){
      sub_menu++;
      updateManual();
      delay(100);
      while (checkButton()=='D');
    }
    else if(checkButton()=='U'){
      sub_menu--;
      updateManual();
      delay(100);
      while (checkButton()=='U');
    }
    else if(checkButton()=='S'){
      ExecManual();
      updateManual();
      while (checkButton()=='S');
    }
  }
}

void updateManual() {
  switch (sub_menu) {
    case 0:
      sub_menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Incr.<");
      lcd.setCursor(9, 0);
      lcd.print("(0~255)");
      lcd.setCursor(0, 1);
      lcd.print(" Start");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Incr.");
      lcd.setCursor(0, 1);
      lcd.print(">Start");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">Exit");
      break;
    case 4:
      sub_menu = 3;
      break;
  }
}

void ExecManual(){
  switch(sub_menu){
    case 1:
      SetIncreament();
      break;
    case 2:
      StartManual();
      //lcd.clear();
      //lcd.setCursor(0, 0);
      //lcd.print("Starting...");
      //while(checkButton()!="S");
      delay(1000);
      break;
    case 3:
      ext = true;
      updateMenu();
      //lcd.clear();
      //lcd.setCursor(0, 0);
      //lcd.print("Exiting...");
      //delay(200);
      break;
  }
}

void SetIncreament(){
  lcd.clear();
  lcd.print(">Incr.");
  lcd.setCursor(13, 0);
  lcd.print("</>");
  while(checkButton()!='D'){
  if(checkButton()=='R'){
    inc++;
    if(inc>255){inc = 255;}
    lcd.clear();
    lcd.print(">Incr.<");
    lcd.setCursor(13, 0);
    lcd.print(inc);
    lcd.setCursor(0, 1);
    lcd.print(" Start");
    delay(100);
  }
  else if(checkButton()=='L'){
    inc--;
    if(inc<0){inc = 0;}
    lcd.clear();
    lcd.print(">Incr.<");
    lcd.setCursor(13, 0);
    lcd.print(inc);
    lcd.setCursor(0, 1);
    lcd.print(" Start");
    delay(100);
  }
  }
}

void Auto(){
  ext = false;
  sub_menu=1;
  updateAuto();
  delay(200);
  while(!ext){
    if(checkButton()=='D'){
      sub_menu++;
      updateManual();
      delay(100);
      while (checkButton()=='D');
    }
    else if(checkButton()=='U'){
      sub_menu--;
      updateManual();
      delay(100);
      while (checkButton()=='U');
    }
    else if(checkButton()=='S'){
      ExecAuto();
      updateAuto();
      while (checkButton()=='S');
    }
  }
}

void updateAuto() {
  switch (sub_menu) {
    case 0:
      sub_menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Step<");
      lcd.setCursor(9, 0);
      lcd.print("(0~255)");
      lcd.setCursor(0, 1);
      lcd.print(" Start");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Step");
      lcd.setCursor(0, 1);
      lcd.print(">Start");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">Exit");
      break;
    case 4:
      sub_menu = 3;
      break;
  }
}

void ExecAuto(){
  switch(sub_menu){
    case 1:
      SetStep();
      break;
    case 2:
      StartAuto();
      delay(1000);
      break;
    case 3:
      ext = true;
      updateMenu();
      break;
  }
}

void SetStep(){
  step = 0;
  lcd.clear();
  lcd.print(">Step");
  lcd.setCursor(13, 0);
  lcd.print("</>");
  while(checkButton()!='D'){
  if(checkButton()=='R'){
    step++;
    if(step>255){step = 255;}
    lcd.clear();
    lcd.print(">Step<");
    lcd.setCursor(13, 0);
    lcd.print(step);
    lcd.setCursor(0, 1);
    lcd.print(" Start");
    delay(100);
  }
  else if(checkButton()=='L'){
    step--;
    if(step<0){step = 0;}
    lcd.clear();
    lcd.print(">step<");
    lcd.setCursor(13, 0);
    lcd.print(step);
    lcd.setCursor(0, 1);
    lcd.print(" Start");
    delay(100);
  }
  }
}

void StartAuto(){
  char key = ' ';
  float U2=(float)analogRead(A1)*5/1023;
  float U3=(float)analogRead(A2)*5/1023;
  float I2=9.9*(U2-2.5);
  float I3=9.9*(U3-2.5);
  lcd.clear();
  while(key != 'S'){
    lcd.setCursor(0,0);
    lcd.print("Gate:"+ String((float)gate*5*4.1/255));
    lcd.setCursor(0,1);
    lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
    lcd.setCursor(11,0);
    lcd.print("Sl-Ex");
    delay(100);

    SD.remove("Auto.txt");
    FileAuto = SD.open("Auto.txt", FILE_WRITE);
    if(FileAuto){
      FileAuto.println("Gate, I2, I3");
    }
    else {
      Serial.println("error opening Auto.txt");
      SD.remove("Auto.txt");  
    }
	
	  int x = 1;
	  while(x==1){
		lcd.clear();
		gate=gate+step;
		analogWrite(ctrl, gate);
    delay(100);
    float U2=(float)analogRead(A1)*5/1023;
    float U3=(float)analogRead(A2)*5/1023;
    float I2=9.9*(U2-2.5);
    float I3=9.9*(U3-2.5);
    if(I2>10.00){x=0;}
		delay(100);        
    lcd.setCursor(0,0);
    lcd.print("Gate: "+ String((float)gate*5*4.1/255));
    delay(100);
    lcd.setCursor(0,1);
    lcd.print("I2:"+ String(I2) + " I3:" + String(I3));

    if(FileAuto){
      FileAuto.println(String((float)gate*5*4.1/255)+ ", "+ String(I2) + ", " + String(I3));
    }
    else {
      Serial.println("error opening Auto.txt");
      FileAuto.close();
    }    
    
    delay(2000);
    if(255%step!=0 && gate==(255-255%step)){
      gate=255-step;
      }
    else if(gate==255){x=0;}
		else if(checkButton()=='S'){key = 'S';}
	}
	gate=0;
  analogWrite(ctrl, gate);
	lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
  FileAuto.close();
  Serial.println("Writing completed on Auto.txt");
  key = 'S';
  }
}

void StartManual(){
  char key = ' ';
  float U2=(float)analogRead(A1)*5/1023;
  float U3=(float)analogRead(A2)*5/1023;
  float I2=9.9*(U2-2.5);
  float I3=9.9*(U3-2.5);
  lcd.clear();
  SD.remove("Manual.txt");
  FileManual = SD.open("Manual.txt", FILE_WRITE);
  if(FileManual){
    FileManual.println("Gate, I2, I3");
  }
  else {
    Serial.println("error opening Manual.txt");
    SD.remove("Manual.txt");  
  }
  while(key != 'S'){
    lcd.setCursor(0,0);
    lcd.print("Gate:"+ String((float)gate*5*4.1/255));
    //lcd.setCursor(9,0);
    //lcd.print("Sl-Ex");
    //delay(100);
    lcd.setCursor(0,1);
    lcd.print("I2:"+ String(I2) + "  I3:" + String(I3));
    lcd.setCursor(11,0);
    lcd.print("  </>");
    delay(100);
    if(checkButton()=='R'){
      gate=gate+inc;
      if(gate >= 255){gate = 255;}
      analogWrite(ctrl, gate);
      delay(100);
      U2=(float)analogRead(A1)*5/1023;
      U3=(float)analogRead(A2)*5/1023;
      I2=9.9*(U2-2.5);
      I3=9.9*(U3-2.5);
      if(I2>10 || I3>10){
        gate=0;
        analogWrite(ctrl, gate);
        FileManual.close();
        Serial.println("Writing completed on Manual.txt");
        key = 'S';}
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Gate:"+ String((float)gate*5*4.1/255));
      lcd.setCursor(11,0);
      lcd.print("Sl-Ex");
      lcd.setCursor(0,1);
      lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
      if(FileManual){
        FileManual.println(String(gate)+ ", "+ String(I2) + ", " + String(I3));
      }
      else {
        Serial.println("error opening Manual.txt");
        FileManual.close();
      }
      delay(100);
      //while(checkButton()=='R');
    }
    else if(checkButton()=='L'){
      gate=gate-inc;
      if(gate <= 0){gate = 0;}
      analogWrite(ctrl, gate);
      delay(100);
      U2=(float)analogRead(A1)*5/1023;
      U3=(float)analogRead(A2)*5/1023;
      I2=9.9*(U2-2.5);
      I3=9.9*(U3-2.5);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Gate:"+ String((float)gate*5*4.1/255));
      lcd.setCursor(11,0);
      lcd.print("Sl-Ex");
      lcd.setCursor(0,1);
      lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
      if(FileManual){
        FileManual.println(String((float)gate*5*4.1/255)+ ", "+ String(I2) + ", " + String(I3));
      }
      else {
        Serial.println("error opening Manual.txt");
        FileManual.close();
      }
      delay(100);
      //while(checkButton()=='L');
    }
    else if(checkButton()=='S'){
      gate=0;
      analogWrite(ctrl, gate);
	    lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
      FileManual.close();
      Serial.println("Writing completed on Manual.txt");
      key = 'S';
    }
  }
}

char checkButton() {
  int x = analogRead (A0);
  char buttonPressed;
  if (x < 25) {
    buttonPressed = 'R';
  } else if (x>55 && x < 80) {
    buttonPressed = 'U';
  } else if (x>170 && x < 200){
    buttonPressed = 'D';
  } else if (x>330 && x < 350){
    buttonPressed = 'L';
  } else if (x>570 && x < 590){
    buttonPressed = 'S';
  } else {
    buttonPressed = ' ';
  }
  return buttonPressed;
}