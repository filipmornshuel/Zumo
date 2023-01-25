/*
Autors: Filip Slavkovic and Diego Fantino
Version: 1.10 
Short description: A Zumo's who can fight with others Zumo's. 
Code works with a setup and a loop method, in the loop method is a switch-case with three cases.
*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;

Zumo32U4LCD lcd;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
int zustand = 0;

// my setup code here, to run once:
void setup() {
  
  buttonA.waitForButton();
  
  /* Configuration 1: * - 3 line sensors (1, 3, 5) * - 3 proximity sensors (left, front, right) */
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
  

  calibrateLineSensors();
}

//CalibrateLineSensors yellow LED
void calibrateLineSensors() {  
  // To indicate we are in calibration mode, turn on the yellow LED
  // and print "Line cal" on the LCD.
  ledYellow(1);
  lcd.clear();
  lcd.print(F("Line cal"));
  for (uint16_t i = 0; i < 400; i++) {
    lcd.gotoXY(0, 1);
    lcd.print(i);
    lineSensors.calibrate();
  }
  ledYellow(0);
  lcd.clear();
}

uint16_t lineSensorValues[3] = { 0, 0, 0 };


//Loop method
void loop() {
  lineSensors.readCalibrated(lineSensorValues);
  proxSensors.read();
  

  //Switch case with 3 cases
  switch(zustand){
    
    //case 1 - drive and attack
    case 0:
      motors.setSpeeds(300, 300);
      if(proxSensors.countsRightWithRightLeds() > 4){
        motors.setSpeeds(400,50);
      }else if(proxSensors.countsLeftWithLeftLeds() > 4){
        motors.setSpeeds(50,400);
      }else if(proxSensors.countsFrontWithLeftLeds() > 4){
        motors.setSpeeds(150,400);
      }else if(proxSensors.countsFrontWithRightLeds() > 4){
        motors.setSpeeds(400,150);
      } 

      //For to check what sensors detect
      for (int l: lineSensorValues) {
        if (l < 500) {
          if(proxSensors.countsRightWithRightLeds() > 4){
            motors.setSpeeds(400,50);
         }else if(proxSensors.countsLeftWithLeftLeds() > 4){
            motors.setSpeeds(50,400);
         }else if(proxSensors.countsFrontWithLeftLeds() > 4){
            motors.setSpeeds(150,400);
          }else if(proxSensors.countsFrontWithRightLeds() > 4){
            motors.setSpeeds(400,150);
         }else{
            zustand = 1;
         }
          zustand = 1;
          
        } 
      }
     
      break;
    
    // case 2 - to back up 
    case 1:
      motors.setSpeeds(-400,-400); //Links rechts
      delay(500);
        if(proxSensors.countsRightWithRightLeds() > 4){
          motors.setSpeeds(400,50);
        }else if(proxSensors.countsLeftWithLeftLeds() > 4){
          motors.setSpeeds(50,400);
        }else if(proxSensors.countsFrontWithLeftLeds() > 4){
          motors.setSpeeds(150,400);
        }else if(proxSensors.countsFrontWithRightLeds() > 4){
          motors.setSpeeds(400,150);
        } else{
          zustand = 2;
        }
      zustand = 2;
      break;

    // case 3 - going left or going right
    case 2:
      motors.setSpeeds(400,-400);
      delay(500);
        if(proxSensors.countsRightWithRightLeds() > 4){
          motors.setSpeeds(400,50);
        }else if(proxSensors.countsLeftWithLeftLeds() > 4){
          motors.setSpeeds(50,400);
        }else if(proxSensors.countsFrontWithLeftLeds() > 4){
          motors.setSpeeds(150,400);
        }else if(proxSensors.countsFrontWithRightLeds() > 4){
          motors.setSpeeds(400,150);
        }else{
          zustand = 0;
        }
      zustand = 0;
      break;
  }
}
