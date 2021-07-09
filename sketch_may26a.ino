//    Roll과 Pitch 각도 구하기 실습  》                                               */
//  가속도 센서만 가지고 롤(Roll)과 피치(Pitch)를 구하는 예제입니다.                          
//  본 코드는 드론과 같은 움직임이 심한 경우가 아닌 비교적 정적인 상태에서 안정된 값을 얻을 수 있으며,
//  복잡한 외부라이브러리를 사용하지 않아 코드가 심플해서 파악후 적용하기가 쉽습니다.
/* 아래 코드관련 실습에 대한 설명과 회로도 및 자료는 https://rasino.tistory.com/ 에 있습니다    */

#include<Wire.h>
#include<SoftwareSerial.h>

#define BT_RXD 8
#define BT_TXD 7
#define Button 9 // 버튼을 눌러 센서 값 받아들일때 사용

SoftwareSerial BTserial(BT_RXD, BT_TXD);

unsigned long time_previous, time_current;
const int MPU_ADDR = 0x68;    // I2C통신을 위한 MPU6050의 주소
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;   // 가속도(Acceleration)와 자이로(Gyro)
double angleAcX;
double angleAcY;
double TX_angleAcX; //블투로 보낼 x각도 변화 값 지정
double TX_angleAcY; // 블투로 보낼 y각도 변화 값 지정
//double angleAcZ;
const double RADIAN_TO_DEGREE = 180 / 3.14159;
const int interval = 500;
 
void setup() {
  pinMode(Button, INPUT_PULLUP);
  if(digitalRead(Button) == LOW){ 
 
  initSensor();
  BTserial.begin(9600);
  Serial.begin(9600);
  time_previous = 0;
  delay(200);
  }
}


void loop() {
  time_current = millis();
  if(time_current-time_previous>interval){
    
  getAngleXY();
  TX_angleAcX = angleAcX; // 블루투스로 보낼 X축 각도값
  TX_angleAcY = angleAcY; // 블루투스로 보낼 Y축 각도값
  Serial.print("Angle x : ");
  Serial.print(angleAcX);
  Serial.print("\t\t Angle y : ");
  Serial.println(angleAcY);
  /* Serial.print("\t\t Angle Z : ");
  Serial.println(angleAcZ);*/ 
  BTserial.print(TX_angleAcX);
  BTserial.print(",");
  BTserial.print(TX_angleAcY);
  BTserial.print(",");
  BTserial.println();
  time_previous = time_current;
}

double getAngleXY() {
  getData();  
  // 삼각함수를 이용한 롤(Roll)의 각도 구하기 
  angleAcX = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2)));
  angleAcX *= RADIAN_TO_DEGREE;
  // 삼각함수를 이용한 피치(Pitch)의 각도 구하기
  angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
  angleAcY *= RADIAN_TO_DEGREE;
  //angleAcZ값(Yaw)은 아래의 삼각함수 공식은 있으나, 가속도 센서만 이용해서는 원하는 데이터를 얻을 수 없어 생략
  /*angleAcZ = atan(sqrt(pow(AcX, 2) + pow(AcY, 2)) / AcZ );
  angleAcZ *= RADIAN_TO_DEGREE;*/
}

void initSensor() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);   // I2C 통신용 어드레스(주소)
  Wire.write(0x6B);    // MPU6050과 통신을 시작하기 위해서는 0x6B번지에    
  Wire.write(0);
  Wire.endTransmission(true);
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);   // AcX 레지스터 위치(주소)를 지칭합니다
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);  // AcX 주소 이후의 14byte의 데이터를 요청
  AcX = Wire.read() << 8 | Wire.read(); //두 개의 나뉘어진 바이트를 하나로 이어 붙여서 각 변수에 저장
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}
