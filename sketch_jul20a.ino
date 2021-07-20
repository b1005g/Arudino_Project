#include <ArduinoBLE.h >
#include <Arduino_LSM9DS1.h >

BLEService DeviceInformation("180A");
BLEFloatCharacteristic Weight("2A98", BLERead | BLENotify);
BLEShortCharacteristic WeightMeasurement("2A9D", BLERead | BLENotify);

float acc_x, acc_y, acc_z;
float acc_pitc, acc_roll;
short acc_pit, acc_rol;

long previousMillis = 0;

void setup() {
Serial.begin(9600);
if (!IMU.begin()) { //LSM6DS3센서 시작
Serial.println("Sensor load failed!");
while (1);
}
if (!BLE.begin()) {
Serial.println("starting BLE failed!");
while (1);
}

// set advertised local name and service UUID:
BLE.setLocalName("getAngle");
BLE.setAdvertisedService(DeviceInformation);
DeviceInformation.addCharacteristic(Weight);
DeviceInformation.addCharacteristic(WeightMeasurement);
BLE.addService(DeviceInformation);

Weight.writeValue(0);
WeightMeasurement.writeValue(0);

// start advertising
BLE.advertise();
Serial.println("BLE get angle peripheral");
}

void updateAngle() {
if (IMU.accelerationAvailable()) {
IMU.readAcceleration(acc_x, acc_y, acc_z);
//각도 계산(라디안을 각도로 180/PI = 57.27755)
acc_pitc = RAD_TO_DEG * atan(acc_x / sqrt(acc_y *acc_y + acc_z *acc_z));
acc_roll = RAD_TO_DEG * atan(acc_y / sqrt(acc_x *acc_x + acc_z *acc_z));

acc_pit = acc_pitc * 100;
acc_rol = acc_roll * 100;

Serial.print("pit = "); // print it
Serial.print(acc_pit);
Serial.print(", rol = "); // print it
Serial.println(acc_rol);

Weight.writeValue(acc_pit); // and update the battery level characteristic
WeightMeasurement.writeValue(acc_rol);
}
}

void loop() {
// listen for BLE peripherals to connect:
BLEDevice central = BLE.central();

// if a central is connected to peripheral:
if (central) {
Serial.print("Connected to central: ");
Serial.println(central.address());
// while the central is still connected to peripheral:
while (central.connected()) {
long currentMillis = millis();
if (currentMillis - previousMillis >= 50) {
previousMillis = currentMillis;
updateAngle();
}
Serial.print("print : ");
Serial.println(acc_pit);
delay(523);
}
Serial.print("Disconnected from central : ");
Serial.println(central.address());
}
}
