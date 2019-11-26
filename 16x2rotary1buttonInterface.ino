
/*You will see the angular displacement of the rotary encoder printed on Serial Monitor. 
 * 
if turn the rotary encoder 
  {clockwise, the angular displacement is increased;
  counterclockwise, the displacement is decreased.}
  
 If press switch on rotary encoder, 
      reading will return to zero.
      
      */
 
//Email: nBED@gmx.com
//Website: www.n-bed.com
//October, 2019 
//the Mos Fett
//#include <WiFi.h>
//#include <ThingSpeak.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <Unistep2.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Unistep2 stepperX(2,3,4,5, 4096, 1000);
//Unistep2 stepperY(6,7,8,9, 4096, 1000);


const int clkPin= 2; //the clk attach to pin2
const int dtPin= 3; //the dt attach to pin3
const int swPin= 4 ;//the number of the button
const int BtnPin = 5;
const int ButtonPin= 5 ;
int menuB;
int change;
const uint8_t menuAMAX = 5;
const uint8_t menuBMAX = 5;

 const char* selecttxt = "Select *button*";
 const char* menutxt[5]= {"Manual Control","AI Training","AI Run modes","Motor Control","Settings"};
 //const char* menusubtxt[5]= {"Select *button* ","Back","Start","Stop","<disabled>"};
 
const char* manualtxt[5]= {"Motor 1 Input", "Motor 2 Input", "Max/Min Angle","Go Home", "Back"};
//                                 |           |
//                                 V-----------
const char* motoritxt[5]= {"Input: ", "Steps:", "Min Angle:", "Max Angle","Please Wait"}; //0 and 1


const char* aitxt[5]= {"Run","unused","unused","unused","Back"};
const char* mctrltxt[5]= {"unused","unused","unused","unused","Back"};
const char* settxt[5]= {"Contrast","Max Angle","Min Angle","Change Speed","Back"};

typedef struct
{ int menuL; 
  unsigned long timerM;
  int menuV,subV,selV,tempV;
}menust, *pmenust;

//typedef struct
//{ bool layerON; 
//  uint8_t menuL,submL;
//}fetbox, *pfetbox';


typedef struct
{ bool latchF,writeF ;   //latch status flag, write flag
  uint8_t shbuf;
}motorout, *pmotorout;

typedef struct
{  unsigned byte[2]; //motor stepp sequences (shifted final out);
   bool motorEN,mF;
   motorout MOUT;
}motorc, *pmotorc;


typedef struct
{ menust MENU; 
  motorc MCTRL;
}fetbox, *pfetbox;

fetbox FBOX;
int ladder;


unsigned long buttonT,selectT=0;
     
uint8_t menuA,menustate;

bool select=false;
bool ignoreturn=false;
int buttonCASE,buttonB=0;

float mot1a,mot2a;
int mot1s,mot2s;
int inpmot;
int machinestate;
 //static 
 int oldA = HIGH; //set the oldA as HIGH
 // static
  int oldB = HIGH; //set the oldB as HIGH
  int oldchange;
  unsigned long refresht;
// menu messes up when scrolled out of range
// you need to double check encoder value is being reset when over max or under 0

void setup()
{ pinMode(BtnPin,INPUT);//initialize the buttonPin as an intput
  lcd.init();   
  lcd.init();        // initialize the lcd 
  // Print a message to the LCD. 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("mFETbox");
  lcd.setCursor(0,1);
  lcd.print("initializing....");
  
  //set clkPin,dePin,swPin as INPUT
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
  Serial.begin(9600); // initialize serial communications at 9600 bps
  lcd.clear();
  
  FBOX.MENU.timerM = millis();
  selectT = millis();
  refresht=millis();
  buttonT = millis();
   menuF();
}

void loop()
{ 

// stepperX.run();
// stepperY.run();
 buttonREAD();
 dialIN();   

 if (oldchange != change or select == true )
  {   lcd.clear();
      menuF();  //took out of dialIN
     // refresht = millis();
  };
  
}
void dialIN()
  { 
    oldchange=change;
    change = getEncoderTurn();
   FBOX.MENU.tempV = FBOX.MENU.tempV + change;
//  if (ignoreturn == false)  
             //  if(FBOX.MENU.menuL== 0)
               //  { 
                  if (FBOX.MENU.tempV>5)
                     { FBOX.MENU.tempV=0;
                     //  change=0;
                     };  
               //  };                                       
//                if (FBOX.MENU.menuL==1)
//                 { if (FBOX.MENU.tempV>menuBMAX)
//                     {  FBOX.MENU.tempV=0;
//                    //    change=0;
//                     }; 
//                  };
                
                if (FBOX.MENU.tempV<0)
                      {   FBOX.MENU.tempV=4;
                        //  change=0;
                      };
};
      

int getEncoderTurn(void)
{
 // static int oldA = HIGH; //set the oldA as HIGH
 // static int oldB = HIGH; //set the oldB as HIGH
  int result = 0;
  int newA = digitalRead(dtPin);//read the value of clkPin to newA
  int newB = digitalRead(clkPin);//read the value of dtPin to newB
  if (newA != oldA || newB != oldB) //if the value of clkPin or the dtPin has changed
    {// lcd.clear();  // something has changed
      if (oldA == HIGH && newA == LOW)
        {result = (oldB * 2 - 1);   
           };     
     //  menuF();  
     }; 
  oldA = newA;
  oldB = newB;
  return result;
};

void buttonREAD()
{ 
if (millis() - FBOX.MENU.timerM >80)
  {
    if (0 == digitalRead(BtnPin))   
         {  select=true;  
            selectT=millis();                    
         };
  };
};

void eventHandler()
{
 
};

void menuF()
{ 
                                       if (select==true)
                                          { lcd.clear();   
                                            if( FBOX.MENU.tempV != 4)
                                              { //FBOX.MENU.subV=FBOX.MENU.tempV;
                                                FBOX.MENU.menuL++; 
                                                }else{  if(FBOX.MENU.tempV == 4 and FBOX.MENU.menuL >0 )\
                                                          FBOX.MENU.menuL--;   
                                                      };             
                                             select=false; 
                                             change=0;
                                             FBOX.MENU.timerM=millis();
                                          };
 switch (FBOX.MENU.menuL)
  {  
    case 0:
                                lcd.setCursor(0,0); 
                                lcd.print(menutxt[FBOX.MENU.tempV]);
                                lcd.setCursor(0,1);
                                lcd.print(selecttxt);      
                                break;
     case 1:                    lcd.setCursor(0,0);
                                lcd.print(manualtxt[FBOX.MENU.tempV]);
                                lcd.setCursor(0,1);
                                lcd.print(selecttxt);                           
                                break;
      case 2:                    lcd.setCursor(0,0);
                                 lcd.print(motoritxt[FBOX.MENU.tempV]);
                                 lcd.setCursor(0,1);
                                 lcd.print("Press select");
                                 break;
      case 3:                    machinestate = (FBOX.MENU.subV*10 + FBOX.MENU.selV);
                                 lcd.setCursor(0,0);
                                 lcd.print("Motor1 angle");
                                 lcd.setCursor(0,1);
                                 lcd.print(FBOX.MENU.tempV);
                                 break;                                                            
  };

};











//const char* manualtxt[5]= {"Motor 1 Input", "Motor 2 Input", "Max/Min Angle","Go Home", "Back"};
//const char* motoritxt[5]= {"Clockwise", "C-Clockwise", "Switch Motor", "Position","Back"}; //0 and 1
//const char* aittext[5]= {"Run training","unused","unused","unused","Back"};
//const char* airtxt[5]= {"Run","unused","unused","unused","Back"};
//const char* mctrltxt[5]= {"unused","unused","unused","unused","Back"};
//const char* settxt[5]= {"Contrast","Max Angle","Min Angle","Change Speed","Back"};

//OLD MENU DISPLAYT CODE (BRUTE FORCE IT VIA SWITCH CASE)
  //  if (menuL==0)
  //    {  FBOX.MENU.menuV=FBOX.MENU.tempV;
  //       menuL=1;  
    //  }; 
//          switch (FBOX.MENU.menuV)
//     
//              {
//                case 0: lcd.setCursor(0,0);
//                        lcd.print("SETTINGS");
//                        lcd.setCursor(0,1);
//                        lcd.print("menulayer 1");          
//                        break;
//                case 1: lcd.setCursor(0,0);
//                        lcd.print("Robot joint 1");
//                        lcd.setCursor(0,1);
//                        lcd.print("menulayer 1"); 
//                        break;
//                case 2: lcd.setCursor(0,0);
//                        lcd.print("Robot joint 2");
//                        lcd.setCursor(0,1);
//                        lcd.print("menulayer 1"); 
//                        break;
//                case 3: lcd.setCursor(0,0);
//                        lcd.print("RUN AI cntrl");
//                        lcd.setCursor(0,1);
//                        lcd.print("menulayer 1"); 
//                        break;
//                case 4: lcd.setCursor(0,0);
//                        lcd.print("Stop");
//                        lcd.setCursor(0,1);
//                        lcd.print("menulayer 1"); 
//                        break;
//                };
//      }
//  };
//  
//       if (menuL==1)
//      {         
//       if  
//         switch (menuA)
//    {  menuA=FBOX.MENU.tempV
//    
//    //case 0: lcd.setCursor(0,0);
//  
//  for (int i=0;i<menuAMAX;i++)
//   { if (FBOX.MENU.tempV == i)
//         {   lcd.print(menutxt[i]);
//            lcd.setCursor(0,1);
//            lcd.print(menutxt[1]"); 
//         };  
//   }                    
//            break;
//    case 1: lcd.setCursor(0,0);
//            lcd.print("Robot joint 1");
//            lcd.setCursor(0,1);
//            lcd.print("menulayer 1"); 
//            break;
//    case 2: lcd.setCursor(0,0);
//            lcd.print("Robot joint 2");
//            lcd.setCursor(0,1);
//            lcd.print("menulayer 1"); 
//            break;
//    case 3: lcd.setCursor(0,0);
//            lcd.print("RUN AI cntrl");
//            lcd.setCursor(0,1);
//            lcd.print("menulayer 1"); 
//            break;
//    case 4: lcd.setCursor(0,0);
//            lcd.print("Stop");
//            lcd.setCursor(0,1);
//            lcd.print("menulayer 1"); 
//            break;
//      };
//      }
//
//};

// hrtr
// from switch (FBOX.MENU.menuL)                                                             
//   case 1:                          if (select==true)
//                                    { lcd.clear(); 
//                                            if(FBOX.MENU.tempV ==4 ) 
//                                              { FBOX.MENU.menuL=0; 
//                                                FBOX.MENU.tempV=0;
//                                                select=false; 
//                                              }; 
//                                            if (FBOX.MENU.menuV==0 and FBOX.MENU.tempV <=1)
//                                              { ignoreturn=!ignoreturn;
//                                                  
//                                              }
//                                           select=false; 
//                                    }; 
//                                                 switch (FBOX.MENU.menuV) 
//                                          {
//                                            case 0: 
//                                            
//                 //     const char* manualtxt[5]= {"Motor 1", "Motor 2", "Position","Go Home", "Back"};
//
//                                                    lcd.setCursor(0,0);
//                                                    lcd.print(manualtxt[FBOX.MENU.tempV]);
//                                                    lcd.setCursor(0,1);
//                                                    lcd.print(menusubtxt[0]);   
//                                                    
//                                                    if (FBOX.MENU.tempV==0 and ignoreturn == true)
//                                                      {  stepperX.moveTo(25*FBOX.MENU.tempV); 
//                                                        lcd.setCursor(0,1);
//                                                         lcd.print("* Clockwise turn dial *");        
//                                                      };
//                                                    if (FBOX.MENU.tempV==1 and ignoreturn = true)
//                                                       {
//                                                        stepperX.moveTo(-25*FBOX.MENU.tempV) ;  
//                                                        lcd.setCursor(0,1);  
//                                                        lcd.print("* C-lockwise turn dial *");  
//
//                                                       };                
//                                                    break;
//                                                    
//                                            case 1: lcd.setCursor(0,0);
//                                                    lcd.print(aittxt[FBOX.MENU.tempV]);
//                                                    lcd.setCursor(0,1);
//                                                    lcd.print(menusubtxt[0]);                                                      
// 
//                                                    if (FBOX.MENU.tempV==0 and ignoreturn == true)
//                                                      {  stepperY.moveTo(25*FBOX.MENU.tempV); 
//                                                        lcd.setCursor(0,1);
//                                                         lcd.print("* Clockwise turn dial *");        
//                                                      };
//                                                    if (FBOX.MENU.tempV==1 and ignoreturn == true)
//                                                       {
//                                                        stepperY.moveTo(-25*FBOX.MENU.tempV) ;  
//                                                        lcd.setCursor(0,1);  
//                                                        lcd.print("* C-lockwise turn dial *");  
//    };                                
//                                                    break;
//                                            case 2:  lcd.setCursor(0,0);
//                                                    lcd.print(airtxt[FBOX.MENU.tempV]);
//                                                    lcd.setCursor(0,1);
//                                                    //lcd.print(menusubtxt[0]);    
//                                                    break;
//                                            case 3:  lcd.setCursor(0,0);
//                                                    lcd.print(mctrltxt[FBOX.MENU.tempV]);
//                                                    lcd.setCursor(0,1);
//                                                    //lcd.print(menusubtxt[0]);    
//                                                    break;
//                                            case 4:  lcd.setCursor(0,0);
//                                                    lcd.print(settxt[FBOX.MENU.tempV]);
//                                                    lcd.setCursor(0,1);
//                                                    //lcd.print(menusubtxt[0]);    
//                                                    break;
//                                               };
//                           break;
//      };
