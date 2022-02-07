#include <Servo.h>

#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

int leftMotor1 = 6;
int leftMotor2 = 9;
int rightMotor1 = 10;
int rightMotor2 = 11;


Servo myServo;  //舵机

int inputPin = 12; // 定义超声波信号接收接口
int outputPin = 13; // 定义超声波信号发出接口

void setup() {
  // put your setup code here, to run once:
  //串口初始化
  Serial.begin(9600);
  //舵机引脚初始化
  myServo.attach(3);
  //测速引脚初始化
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  //pinMode(leftPWM, OUTPUT);
  //pinMode(rightPWM, OUTPUT);
  //超声波控制引脚初始化
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  avoidance();
  /*int i;
    // put your main code here, to run repeatedly:
    if (Serial.available())
    { i = Serial.read();
    switch (i)
    { case'7': {
          avoidance();
          break;
        }
      case'8': {
          digitalWrite(leftMotor1, LOW);
          digitalWrite(leftMotor2, LOW);
          digitalWrite(rightMotor1, LOW);
          digitalWrite(rightMotor2, LOW);
          break;
        }
        //case'3':{turnRight();break;}
        //case'4':{_stop();break;}
        //case'5':{back();break;}

    }
    }*/

}
void motorRun(int cmd, int value)
{
  //analogWrite(leftPWM, value);  //设置PWM输出，即设置速度
  //analogWrite(rightPWM, value);
  switch (cmd) {
    case FORWARD:
      Serial.println("FORWARD"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    case BACKWARD:
      Serial.println("BACKWARD"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case TURNLEFT:
      Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case TURNRIGHT:
      Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    default:
      Serial.println("STOP"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}
void avoidance()
{
  int pos;
  int dis[3];//距离
  motorRun(FORWARD, 200);
  myServo.write(90);
  dis[1] = getDistance(); //中间

  if (dis[1] < 30)
  {
    motorRun(STOP, 0);
    myServo.write(90);
    dis[1] = getDistance(); //中间
    delay(800);
    myServo.write(170);
    delay(500);
    dis[2] = getDistance(); //左边
    delay(100);
    myServo.write(10);
    delay(600);
    dis[0] = getDistance(); //右边
    delay(100);
    myServo.write(90);
    delay(500);
    if ((dis[0] < dis[2]) && (dis[2] > 30)) //右边距离障碍的距离比左边近
    {
      //左转
      motorRun(TURNLEFT, 250);
      delay(500);
    }
    else if ((dis[0] > dis[2]) && (dis[0] > 30)) //右边距离障碍的距离比左边远
    {
      //右转
      motorRun(TURNRIGHT, 250);
      delay(500);
    }
    else
    {
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
    }
  }
}
int getDistance()
{
  digitalWrite(outputPin, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW); // 保持发出超声波信号接口低电平
  int distance = pulseIn(inputPin, HIGH); // 读出脉冲时间
  distance = distance / 58; // 将脉冲时间转化为距离（单位：厘米）
  Serial.println(distance); //输出距离值
  if (distance >= 50)
  {
    //如果距离小于50厘米返回数据
    return 50;
  }//如果距离小于50厘米小灯熄灭
  else
    return distance;
}
