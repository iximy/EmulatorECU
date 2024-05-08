


#include "HCDisplay.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define LEDPIN 3

HCDisplay HCDisplay;   


#define X_RAW_MIN 157
#define X_RAW_MAX 850
#define Y_RAW_MIN 105
#define Y_RAW_MAX 866


unsigned int XMin = 65535, YMin = 65535, XMax = 0, YMax = 0;
int x = 0, y = 0,a=1;


void setup() 
{



   pinMode(LEDPIN, OUTPUT);
   digitalWrite(LEDPIN, 1);
   

   
    cli();
    TCCR1A = 0;  
    TCCR1B = 0;

    OCR1A = 781; 

    TCCR1B |= (1 << WGM12);  
    TCCR1B |= (1 << CS10); 
    TCCR1B |= (1 << CS12);

    TIMSK1 |= (1 << OCIE1A); 
    sei(); 


    
  Serial.begin(9600);
  
  HCDisplay.Init();   

  HCDisplay.TouchInit(X_RAW_MIN, X_RAW_MAX, Y_RAW_MIN, Y_RAW_MAX, TOUCH_0_NORMAL); //Initialise the touch sensor

  HCDisplay.Circle ( 70, 175, 50,OUTLINE,2) ;
HCDisplay.Circle ( 250, 175, 50,OUTLINE,2) ;

 HCDisplay.Pos(60, 145);
     HCDisplay.SetFont (LCDLarge_52ptFont) ;
   HCDisplay.Print("-");
   HCDisplay.Pos(240, 145);
     HCDisplay.SetFont (LCDLarge_52ptFont) ;
   HCDisplay.Print("+");

   HCDisplay.Pos((HCDisplay.ResX() / 2) - 70, HCDisplay.ResY() / 5);
     HCDisplay.Print(7812/OCR1A,false);
       HCDisplay.Print("  RPM   ");
}


void loop() 
{

  byte Ave = 0;
  unsigned long AveX = 0, AveY = 0; 

 
  while(HCDisplay.Pressed() && Ave < 100)
  {
    AveX += HCDisplay.ReadRaw(TOUCH_AXIS_X);
    AveY += HCDisplay.ReadRaw(TOUCH_AXIS_Y);

    Ave++;
  }



  if(Ave == 100)
  {
 
    AveX /= 100;
    AveY /= 100;

  
    if(AveX < XMin)
      XMin = AveX;

    if(AveX > XMax)
      XMax = AveX;

    if(AveY < YMin)
      YMin = AveY;

    if(AveY > YMax)
      YMax = AveY;

    HCDisplay.TouchInit(140, 800, 140, 800, TOUCH_0_NORMAL);


    HCDisplay.SetFG(0,0,0);


    
    HCDisplay.ReadTouch();
    x = HCDisplay.GetCoord(TOUCH_AXIS_X);
    y = HCDisplay.GetCoord(TOUCH_AXIS_Y);
    
    HCDisplay.SetFG(255,255,255);




    HCDisplay.Pos((HCDisplay.ResX() / 2) - 70, HCDisplay.ResY() / 5);
     HCDisplay.SetFont (LCDLarge_52ptFont) ;





    if(x < 150){
    


        cli();  
    TCCR1A = 0;  
    TCCR1B = 0;

    if(OCR1A>20){
    OCR1A = OCR1A+10; 
 }
 else{
  OCR1A = OCR1A+1; 
  }
    TCCR1B |= (1 << WGM12); 
    TCCR1B |= (1 << CS10); 
    TCCR1B |= (1 << CS12);

    TIMSK1 |= (1 << OCIE1A); 
    sei();

    

    }
    
    if(x > 150)
     {
     


        cli(); 
    TCCR1A = 0; 
    TCCR1B = 0;
 if(OCR1A>20){
    OCR1A = OCR1A-10;
 }
 else{
  OCR1A = OCR1A-1; 
  }
    TCCR1B |= (1 << WGM12);  
    TCCR1B |= (1 << CS10); 
    TCCR1B |= (1 << CS12);

    TIMSK1 |= (1 << OCIE1A);  
    sei(); 

    

     }
    HCDisplay.Print(7812/OCR1A,false);
       HCDisplay.Print("  RPM   ");
  }


}
ISR(TIMER1_COMPA_vect)
{
  
  if (a<117){
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    }
    else{
      digitalWrite(LEDPIN, 1);
      }
if (a>119){
    a=0;
    }
      
    a=a+1;
}
