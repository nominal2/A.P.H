//a0:b7:65:68:5d:d2
#include <PS4Controller.h>

const int trigPin1 = 2;
const int echoPin1 = 4;

const int trigPin2 = 18;
const int echoPin2 = 5;

#define RXD2 16
#define TXD2 17

#define SOUND_SPEED 0.034

long duration1;
float distanceCm1;

long duration2;
float distanceCm2;

#define ENCAL 34
#define ENCBL 35
volatile int posil = 0;
#define ENCAR 33 
#define ENCBR 32 
#define pwm1 27
#define dir1 14
#define pwm2 25
#define dir2 26
#define x_axis 13
#define y_axis 12
volatile int posir = 0;
volatile int x_c =0;
volatile int y_c = 0;
String joy;
int left;
int right;
String self;
void setup() {
Serial.begin(115200);
Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT); 
pinMode(trigPin2, OUTPUT); 
pinMode(echoPin2, INPUT); 
pinMode(ENCAL,INPUT);
pinMode(ENCBL,INPUT);
pinMode(ENCAR,INPUT);
pinMode(ENCBR,INPUT);
pinMode(x_axis,INPUT);
pinMode(y_axis,INPUT);
pinMode(dir1,OUTPUT);
pinMode(dir2,OUTPUT);
pinMode(pwm1,OUTPUT);
pinMode(pwm2,OUTPUT);
PS4.attach(notify);
PS4.begin();
attachInterrupt(digitalPinToInterrupt(ENCAL),readEncoderl,RISING);
attachInterrupt(digitalPinToInterrupt(ENCAR),readEncoderr,RISING);
}

void loop() {
 while (Serial2.available()) {
    String datain = Serial2.readStringUntil('\n');
    datain.trim();
    self = datain.substring(0,1);
    joy = datain.substring(2,3);
    Serial.print(self);
    Serial.print("-");
    Serial.println(joy); 
  }
//  Serial.print(posil);
//  Serial.print(" ");
//  Serial.println(posir);
int x = map(analogRead(x_axis),0,4095, -255,255);
int y = map(analogRead(y_axis),0,4095, -255,255);
if(x_c < 10 and x_c> -10)
  x_c = 0;
if(y_c < 10 and y_c > -10)
  y_c = 0;
x_c = map(x_c,-128,128, -255,255);
y_c = map(y_c,-128,128, -255,255);
if(x < 30 and x> -30)
    x = 0;
  if(y < 30 and y > -30)  
    y = 0;

if (joy.toInt() == 1)
{
  right = y_c + x_c;
  left = y_c - x_c;
}
else
{
   right = y + x;
   left = y - x;
}
//Serial.print(distanceCm1);
//Serial.print(" ");
//Serial.println(distanceCm2);

drive(left,right);
//Serial.print(x_c);
//Serial.print(" ");
//Serial.println(y_c);
//readD1();
//readD2();
}

void readEncoderl(){
  int b = digitalRead(ENCBL);
  if(b > 0){
    posil++;
  }
  else{
    posil--;
  }
}


void readEncoderr(){
  int b = digitalRead(ENCBR);
  if(b > 0){
    posir++;
  }
  else{
    posir--;
  }
}


void drive(int speed_a , int speed_b)
{
   speed_a = constrain(speed_a, -255,255);
   speed_b = constrain(speed_b, -255,255);

   if ( speed_a < 0 ) {
    digitalWrite(dir1, HIGH);
    analogWrite(pwm1, abs(speed_a));
  } 
  else if ( speed_a > 0 ) {
    digitalWrite(dir1, LOW);
    analogWrite(pwm1, abs(speed_a));
  } 
  else {
     analogWrite(pwm1, 0);

  }

   if ( speed_b == 0 ) {
    analogWrite(pwm2, 0);

  } else if (speed_b > 0 ) {
    digitalWrite(dir2, LOW);
    analogWrite(pwm2, abs(speed_b));
  } else {
    digitalWrite(dir2, HIGH);
    analogWrite(pwm2, abs(speed_b));
  }

  Serial.print(speed_a);
  Serial.print(" ");
  Serial.println(speed_b);
}


void notify()
{
  x_c = PS4.LStickX();
  y_c = PS4.LStickY();
}


void readD1()
{
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  
  duration1 = pulseIn(echoPin1, HIGH);

  distanceCm1 = duration1 * SOUND_SPEED/2;
 
}


void readD2()
{
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  
  duration2 = pulseIn(echoPin2, HIGH);

  distanceCm2 = duration2 * SOUND_SPEED/2;

}
