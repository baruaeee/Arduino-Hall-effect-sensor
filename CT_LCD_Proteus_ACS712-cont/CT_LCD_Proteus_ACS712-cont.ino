#include <LiquidCrystal.h>             // Library for LCD
#include <SD.h>                        // Library for SD Card
#include <SPI.h>                       // Library for Serial Peripheral Interface

LiquidCrystal lcd( 8,  9, 4, 5, 6, 7); // Initial pin config for LCD

int menu = 1;         // to count and nevigate main menu
int sub_menu = 1;     // to count and nevigate main submenu
bool ext = false;     // to exit from submenu
int a = 0;            // to store the value of analog input A0 to detect push buttons
float step = 0;         // initialize variable for set increament for Auto functions
int inc = 0;          // initialize variable for set increament/decreament for Manual functions
int ctrl = 3;         // Define control pin = Digital(PWM) output pin 3
int gate =0;          // Initialize variable for gate voltage
float Imax = 0.0;

File FileManual;      // define a variable to open/close file for Manual operation read/write
File FileAuto;        // define a variable to open/close file for Auto operation read/write

void setup(){
  pinMode(10, INPUT); //must be set as INPUT for LCD Backlight control
  lcd.begin(16, 2);   // define LCD size (columns and rows)
  updateMenu();       // call this function to display main menu at startup

  Serial.begin(9600); // initialize serial interface
    if (SD.begin(2)){
      Serial.println("SD card is ready to use.");
    } else{
      Serial.println("SD card initialization failed");
    return;
    }
}

void loop(){
  a = analogRead (A0);              // read push button status
  while(a>600){                     // for the current setup if not button pressed A0 reading > 600
    if(checkButton()=='D'){         // call checkButton() function to detect/compare push button
      menu++;                       // Down button will move the selector one step down
      updateMenu();                 // show the main menu after moving selector
      delay(200);                   // wait 100 milliseconds
      while (checkButton()=='D');   // freeze the loop unless any other button pressed
    }
    else if(checkButton()=='U'){
      menu--;                       // Up button will move the selector one step up
      updateMenu();
      delay(200);
      while (checkButton()=='U');
    }
    else if(checkButton()=='S'){
      Exec();                       // if Select button pressed it will call Exec() function
      updateMenu();                 // After execuation return to the lats menu
      delay(200);
      while (checkButton()=='S');
    }
  }
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;               // force the selector to position 1 if value on 'menu' = 0
      break;
    case 1:
      lcd.clear();            // erase the whole LCD
      lcd.print(">Set Imax");     // set the selector to position 1
      if(Imax>0){
        lcd.setCursor(13, 0);
        lcd.print(Imax);
      }
      lcd.setCursor(0, 1);    // select second row to write
      lcd.print(" Auto");
      break;
    case 2:
      lcd.clear();            // erase the whole LCD
      lcd.print(" Set Imax");
      if(Imax>0){
        lcd.setCursor(13, 0);
        lcd.print(Imax);
      }
      lcd.setCursor(0, 1);    // select second row to write
      lcd.print(">Auto");     // set the selector to position 2
      break;
    case 3:
      lcd.clear();
      lcd.print(" Auto");
      lcd.setCursor(0, 1);
      lcd.print(">Manual");   // set the selector to position 3
      break;
    case 4:
      menu = 3;               // as there are only 3 options if menu increamented to 4 it will freeze to second option
      break;
  }
}

void Exec(){                  // just call function corresponing to value of menu
  switch(menu){
    case 1:
      SetImax();
      break;
    case 2:
      Auto();
      break;
    case 3:
      Manual();
      break;
  }
}

void SetImax(){
  ext = false;
  lcd.clear();
  lcd.print("<Imax(0~10)>");
  lcd.setCursor(12, 0);
  lcd.print(Imax);
  lcd.setCursor(0, 1);
  lcd.print("Down => Exit");
  while(!ext){
    if(checkButton()=='R' & Imax<20.0){
      Imax = Imax + 1.0;
      lcd.setCursor(12, 0);
      lcd.print("  ");
      lcd.setCursor(12, 0);
      lcd.print(Imax);
      while(checkButton()=='R');
      }
    else if(checkButton()=='L' & Imax>0.0){
      Imax = Imax - 1.0;
      lcd.setCursor(12, 0);
      lcd.print("  ");
      lcd.setCursor(12, 0);
      lcd.print(Imax);
      while(checkButton()=='L');
      }
    else if(checkButton()=='D')
      ext = true;
    }
  }

void Manual(){
  ext = false;                    // for exiting from while loop
  sub_menu=1;                     // force sub_menu to 1 for update manual function
  updateManual();                 // show the Menu inside Manual
  delay(200);
  while(!ext){                    // as long as ext is false while loop can't break
    if(checkButton()=='D'){
      sub_menu++;
      updateManual();
      delay(200);
      while (checkButton()=='D');
    }
    else if(checkButton()=='U'){
      sub_menu--;
      updateManual();
      delay(200);
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
      lcd.setCursor(9, 0);      // set cursor at the right side of the 1st row
      lcd.print("(0~255)");     // to remind the user to put value between 0~255
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
      delay(500);
      break;
    case 3:
      ext = true;
      updateMenu();
      break;
  }
}

void SetIncreament(){           // increament for manual function
  lcd.clear();
  lcd.print(">Incr.");
  lcd.setCursor(13, 0);
  lcd.print("</>");
  while(checkButton()!='D'){    // after setting the value user has to go down to start operation
  if(checkButton()=='R'){
    inc++;
    if(inc>255){inc = 255;}     // Increament value can not be greater then 255
    lcd.clear();
    lcd.print(">Incr.<");
    lcd.setCursor(13, 0);
    lcd.print(inc);
    lcd.setCursor(0, 1);
    lcd.print(" Start");
    delay(200);
  }
  else if(checkButton()=='L'){
    inc--;
    if(inc<0){inc = 0;}         // Decreament value can not be less then 0
    lcd.clear();
    lcd.print(">Incr.<");
    lcd.setCursor(13, 0);
    lcd.print(inc);
    lcd.setCursor(0, 1);
    lcd.print(" Start");
    delay(200);
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
      updateAuto();
      delay(100);
      while (checkButton()=='D');
    }
    else if(checkButton()=='U'){
      sub_menu--;
      updateAuto();
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
      lcd.print("(0~" + String(Imax));
      lcd.setCursor(0, 1);
      lcd.print(" Start");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Step");
      lcd.setCursor(13, 0);
      lcd.print(step);
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
    step = step + 0.5;
    if(step>Imax){step = Imax;}
    lcd.clear();
    lcd.print(">Step<");
    lcd.setCursor(13, 0);
    lcd.print(step);
    lcd.setCursor(0, 1);
    lcd.print(" Start");
    delay(200);
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
    delay(200);
  }
  }
}

void getCurrent1(){
  U1=(float)analogRead(A1)*5/1024;
  I1=10*(U2-2.5);
  return I1;
}

void getCurrent2(){
  float U2=(float)analogRead(A2)*5/1024;
  float I2=10*(U3-2.5);
  return I2;
}

void StartAuto(){
  gate = 0;
  float I = 0;
  float U1 = 0;
  float U2 = 0;
  float I1 = 0;
  float I2 = 0;
  SD.remove("Auto.txt");                                // remove the 'Auto.txt' file before writing newly
  FileAuto = SD.open("Auto.txt", FILE_WRITE);           // create 'Auto.txt' file
  if(FileAuto){
    FileAuto.println("Gate, I2, I3");                   // write header
  }
  else {
    Serial.println("error opening Auto.txt");
    SD.remove("Auto.txt");
  }
  while(checkButton() != 'D'){
    I = I + step;
    while(getCurrent1()!=I | getCurrent2()!=I){
      gate++;
      
    }
  }
  
		
		gate=gate+step;
		analogWrite(ctrl, gate);
    delay(200);
    float U2=(float)analogRead(A1)*5/1024;                // this is the 10-bit analog value of analog input
    float U3=(float)analogRead(A2)*5/1024;
    float I2=10*(U2-2.5);
    float I3=10*(U3-2.5);
    if(I2>(float)Imax){x=0;}                                    // exit loop when current is greater then 10 ampere
		delay(100);
    lcd.clear();   
    lcd.setCursor(0,0);
    lcd.print("Gate: "+ String((float)gate*5/256));
    //delay(100);
    lcd.setCursor(0,1);
    //lcd.print("U2:"+ String(U2) + " U3:" + String(U3));
    lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
    if(FileAuto){
      FileAuto.println(String((float)gate*5/256)+ ", "+ String(I2) + ", " + String(I3));
    }
    else {
      Serial.println("error opening Auto.txt");
      FileAuto.close();
    }    
    
    delay(500);                                        // this delay to get stable value of the reading
    if(165%step!=0 && gate==(255-165%step)){            // to keep last gate value equal to 255 
      gate=255-step;
      }
    else if(gate==255){x=0;}                            // exit loop after last gate value
		else if(checkButton()=='D'){key = 'D';}             // exit operation if 'S' button pushed
    //analogWrite(ctrl, 0);
    delay(500);
	}
	gate=0;                                               // reset gate value
  analogWrite(ctrl, gate);                              // write gate value to control pin
	lcd.print("I2:"+ String(U2) + " U3:" + String(U3));
  //lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
  FileAuto.close();
  Serial.println("Writing completed on Auto.txt");
  key = 'D';
  }
}

/*
void StartAuto(){
  char key = ' ';                                        // for exiting from while loop
  gate = 0;
  float U2=(float)analogRead(A1)*5/1024;                  // Analog input(10-bit), 0~5 volt = 0~1023
  float U3=(float)analogRead(A2)*5/1024;
  float I2=10*(U2-2.5);                                  // this is an approximate formula to convert the voltage to equivalent current
  float I3=10*(U3-2.5);
  lcd.clear();
  while(key != 'D'){                                      // unless pressing 'S', while loop continues
    lcd.setCursor(0,0);
    lcd.print("Gate:"+ String((float)gate*5/256));        // to show current gate voltage
    lcd.setCursor(0,1);
    lcd.print("I2:"+ String(U2) + " U3:" + String(U3));
    //lcd.print("I2:"+ String(I2) + " I3:" + String(I3));   // to show the correspinding current
    lcd.setCursor(11, 0);
    lcd.print("Sl-Ex");                                   // to remind pressing select to exit anytime
    delay(100);

    SD.remove("Auto.txt");                                // remove the 'Auto.txt' file before writing newly
    FileAuto = SD.open("Auto.txt", FILE_WRITE);           // create 'Auto.txt' file
    if(FileAuto){
      FileAuto.println("Gate, I2, I3");                   // write header
    }
    else {
      Serial.println("error opening Auto.txt");
      SD.remove("Auto.txt");
    }
	
	  int x = 1;                                            // using this variable to exit when gate value out of range
	  while(x==1){
		
		gate=gate+step;
		analogWrite(ctrl, gate);
    delay(200);
    float U2=(float)analogRead(A1)*5/1024;                // this is the 10-bit analog value of analog input
    float U3=(float)analogRead(A2)*5/1024;
    float I2=10*(U2-2.5);
    float I3=10*(U3-2.5);
    if(I2>(float)Imax){x=0;}                                    // exit loop when current is greater then 10 ampere
		delay(100);
    lcd.clear();   
    lcd.setCursor(0,0);
    lcd.print("Gate: "+ String((float)gate*5/256));
    //delay(100);
    lcd.setCursor(0,1);
    //lcd.print("U2:"+ String(U2) + " U3:" + String(U3));
    lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
    if(FileAuto){
      FileAuto.println(String((float)gate*5/256)+ ", "+ String(I2) + ", " + String(I3));
    }
    else {
      Serial.println("error opening Auto.txt");
      FileAuto.close();
    }    
    
    delay(500);                                        // this delay to get stable value of the reading
    if(165%step!=0 && gate==(255-165%step)){            // to keep last gate value equal to 255 
      gate=255-step;
      }
    else if(gate==255){x=0;}                            // exit loop after last gate value
		else if(checkButton()=='D'){key = 'D';}             // exit operation if 'S' button pushed
    //analogWrite(ctrl, 0);
    delay(500);
	}
	gate=0;                                               // reset gate value
  analogWrite(ctrl, gate);                              // write gate value to control pin
	lcd.print("I2:"+ String(U2) + " U3:" + String(U3));
  //lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
  FileAuto.close();
  Serial.println("Writing completed on Auto.txt");
  key = 'D';
  }
}

*/
void StartManual(){
  char key = ' ';
  gate = 90;
  float U2=(float)analogRead(A1)*5/1024;
  float U3=(float)analogRead(A2)*5/1024;
  float I2=10*(U2-2.5);
  float I3=10*(U3-2.5);
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
  while(key != 'D'){
    lcd.setCursor(0,0);
    lcd.print("Gate:"+ String((float)gate*5/256));
    lcd.setCursor(0,1);
    lcd.print("I2:"+ String(I2) + "  I3:" + String(I3));
    lcd.setCursor(11,0);
    lcd.print("Dn/Ex");
    delay(100);
    if(checkButton()=='R'){
      gate=gate+inc;
      if(gate >= 255){gate = 255;}
      analogWrite(ctrl, gate);
      delay(100);
      U2=(float)analogRead(A1)*5/1024;
      U3=(float)analogRead(A2)*5/1024;
      I2=10*(U2-2.5);
      I3=10*(U3-2.5);
      if(I2>(float)Imax || I3>(float)Imax){
        gate=0;
        analogWrite(ctrl, gate);
        FileManual.close();
        Serial.println("Writing completed on Manual.txt");
        key = 'D';}
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Gate:"+ String((float)gate*5/256));
      lcd.setCursor(11,0);
      lcd.print("Dwn-Ex");
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
      analogWrite(ctrl, 0);
    }
    else if(checkButton()=='L'){
      gate=gate-inc;
      if(gate <= 90){gate = 90;}
      analogWrite(ctrl, gate);
      delay(100);
      U2=(float)analogRead(A1)*5/1024;
      U3=(float)analogRead(A2)*5/1024;
      I2=10*(U2-2.5);
      I3=10*(U3-2.5);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Gate:"+ String((float)gate*5/256));
      lcd.setCursor(11,0);
      lcd.print("Sl-Ex");
      lcd.setCursor(0,1);
      lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
      if(FileManual){
        FileManual.println(String((float)gate*5/256)+ ", "+ String(I2) + ", " + String(I3));
      }
      else {
        Serial.println("error opening Manual.txt");
        FileManual.close();
      }
      delay(100);
      analogWrite(ctrl, 0);
    }
    else if(checkButton()=='D'){
      gate=0;
      analogWrite(ctrl, gate);
	    lcd.print("I2:"+ String(I2) + " I3:" + String(I3));
      FileManual.close();
      Serial.println("Writing completed on Manual.txt");
      key = 'D';
    }
  }
}
/* for LCD Keypad Shield D1 Robot
char checkButton() {
  int x = analogRead (A0);
  char buttonPressed;
  if (x < 40) {
    buttonPressed = 'R';
  } else if (x>120 && x < 150) {
    buttonPressed = 'U';
  } else if (x>290 && x < 315){
    buttonPressed = 'D';
  } else if (x>450 && x < 500){
    buttonPressed = 'L';
  } else if (x>700 && x < 750){
    buttonPressed = 'S';
  } else {
    buttonPressed = ' ';
  }
  return buttonPressed;
}
*/

// for Proteus Simulation
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
