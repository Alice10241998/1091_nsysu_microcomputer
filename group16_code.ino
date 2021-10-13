#define Rwheela 1 //右馬達反轉
#define Rwheelb 0 //右馬達正轉
#define Rwheels 5 //右馬達轉速
#define Lwheela 2 //左馬達反轉
#define Lwheelb 3 //左馬達正轉
#define Lwheels 6 //左馬達轉速
#define R_trig 13
#define R_echo 12
#define L_trig 9
#define L_echo 8
#define buzzer 4

#include <SoftwareSerial.h>

double durationR, durationL, cmR=0, cmL=0;
const int leftSpeed  = 650;    //左輪轉速
const int rightSpeed = 405;    //右輪轉速
int state;

void setup()
{
  pinMode(Rwheela,OUTPUT);
  pinMode(Rwheelb,OUTPUT);
  pinMode(Lwheela,OUTPUT);
  pinMode(Lwheelb,OUTPUT);
  
  pinMode(Rwheels,OUTPUT);
  pinMode(Lwheels,OUTPUT);
  
  pinMode(R_trig,OUTPUT);
  pinMode(R_echo,INPUT);
  
  pinMode(L_trig,OUTPUT);
  pinMode(L_echo,INPUT);

  pinMode(buzzer,OUTPUT);

  state = 0;
  
  carstop();
}

void loop()
{
  double R=distanceR();
  delay(100);
  double L=distanceL();
  if((R<25 || L<25) && state != 4)
    voice();
  state = detect(R, L, state);
}
//B=0 F=1 L=2 R=3 S=4
int detect(double r, double l, int state)
{
    switch(state){
      case 0: //detect distance
        backward();
        //if(r<25 || l<25)
        if(r<17 || l<17)
            return 4;
        return 0;
      case 1:
        break;
      case 2: //turnleft
        turnLeft();
        r=distanceR();
        delay(100);
        l=distanceL();
        if(l-r<5 && (l<11 || r<11))
          return 4;
        else
          return 0;
      case 3: //turnright
        turnRight();
        r=distanceR();
        delay(100);
        l=distanceL();
        if(r-l<5 && (l<11 || r<11))
          return 4;
        else
          return 0;
      case 4: //left or right
        carstop();
        if(r-l>6)
          return 3;
        
        else if (l-r>6)
          return 2;
        
        else 
          return 4;
    }
}

void voice()
{
    for(int i=0;i<2;i++)
    {
      tone(buzzer, 1000);
      delay(15);
      tone(buzzer, 500);
      delay(15);
      noTone(buzzer);
     }
}


double distanceR()
{
  digitalWrite(R_trig,LOW);
  delayMicroseconds(5);
  digitalWrite(R_trig,HIGH);
  delayMicroseconds(10);
  
  pinMode(R_echo, INPUT);
  durationR = pulseIn(R_echo,HIGH);
  cmR = (durationR/2)/29.1;
  return cmR;
}

double distanceL()
{
  digitalWrite(L_trig,LOW);
  delayMicroseconds(5);
  digitalWrite(L_trig,HIGH);
  delayMicroseconds(10);
  
  pinMode(L_echo, INPUT);
  durationL = pulseIn(L_echo,HIGH);
  cmL = (durationL/2)/29.1;
  return cmL;
}



//後退
void backward()
{
  //右馬達
  digitalWrite(Rwheela,HIGH);
  digitalWrite(Rwheelb,LOW);
  analogWrite(Rwheels,rightSpeed);
  //左馬達
  digitalWrite(Lwheela,HIGH);
  digitalWrite(Lwheelb,LOW);
  analogWrite(Lwheels,leftSpeed);
  
 }

//前進
void forward()
{
  //右馬達
  digitalWrite(Rwheela,LOW);
  digitalWrite(Rwheelb,HIGH);
  analogWrite(Rwheels,rightSpeed);
  //左馬達
  digitalWrite(Lwheela,LOW);
  digitalWrite(Lwheelb,HIGH);
  analogWrite(Lwheels,leftSpeed);
 }

//左轉彎
void turnLeft()
{
  //右馬達
  digitalWrite(Rwheela,LOW);
  digitalWrite(Rwheelb,LOW);
  analogWrite(Rwheels,rightSpeed);
  //左馬達
  digitalWrite(Lwheela,HIGH);
  digitalWrite(Lwheelb,LOW);
  analogWrite(Lwheels,leftSpeed);
 }

//右轉彎
void turnRight()
{
  //右馬達
  digitalWrite(Rwheela,HIGH);
  digitalWrite(Rwheels,LOW);
  analogWrite(Rwheels,rightSpeed);
  //左馬達
  digitalWrite(Lwheela,LOW);
  digitalWrite(Lwheelb,LOW);
  analogWrite(Lwheels,leftSpeed);
}

//停止
void carstop()
{
  //右馬達
  digitalWrite(Rwheela,LOW);
  digitalWrite(Rwheelb,LOW);
  //analogWrite(Rwheels,rightSpeed);
  //左馬達
  digitalWrite(Lwheela,LOW);
  digitalWrite(Lwheelb,LOW);
  //analogWrite(Lwheels,leftSpeed);
}
