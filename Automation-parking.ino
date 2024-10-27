
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80); // port server

const char* ssid = "Redmi Note 10S"; // nama wifi
const char* password = "87654321"; // password wifi

Servo servo1;
Servo servo2;

const int servo1Pin = 2;
const int servo2Pin = 15;

const int sensorTrigPin1 = 14;
const int sensorEchoPin1 = 26;


const int sensorTrigPin2 = 12;
const int sensorEchoPin2 = 27;

int posMasuk;
int posKeluar;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  pinMode(sensorTrigPin1, OUTPUT);
  pinMode(sensorEchoPin1, INPUT);
  pinMode(sensorTrigPin2, OUTPUT);
  pinMode(sensorEchoPin2, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi...");

  while (WiFi.status() != WL_CONNECTED){
    Serial.println("Wrong Connected!!");
    delay(500);
  }
  Serial.println("Wifi Connected!!");
  Serial.println(WiFi.localIP());

  server.on("/gerbangMasuk",HTTP_POST, gerbangMasuk);
  server.on("/gerbangKeluar",HTTP_POST, gerbangKeluar);

  server.begin();
  Serial.println("Server started...");

  Serial.println("Setup ESP32! Finished");

}

void loop() {
  server.handleClient();
}

long bacaJarak(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);          
  delayMicroseconds(2);                 
  digitalWrite(trigPin, HIGH);          
  delayMicroseconds(10);                
  digitalWrite(trigPin, LOW);           
  
  long duration = pulseIn(echoPin, HIGH); 
  long distance = duration * 0.034 / 2;   
  return distance;                        
}


void gerbangMasuk(){
  for (posMasuk = 90; posMasuk >= 0; posMasuk -= 1) {
    servo1.write(posMasuk);
    delay(15);
  }
  while (bacaJarak(sensorTrigPin1, sensorEchoPin1) > 10) {
      delay(100); 
    }
  while (bacaJarak(sensorTrigPin1, sensorEchoPin1) < 10) {
      delay(100); 
    }
  tutupGerbangMasuk();
  server.sendHeader("Gerbang Masuk Control","*");
  server.send(200,"text/plain","Buka_Gerbang_Masuk_Success");
}

void tutupGerbangMasuk(){
  for (posMasuk = 0; posMasuk <= 90; posMasuk += 1) {
    servo1.write(posMasuk);
    delay(15);
  }
}

void gerbangKeluar(){
  for (posKeluar = 90; posKeluar >= 0; posKeluar -= 1) {
    servo2.write(posKeluar);
    delay(15);
  }
  while (bacaJarak(sensorTrigPin2, sensorEchoPin2) > 10) {
      delay(100); 
    }
  while (bacaJarak(sensorTrigPin2, sensorEchoPin2) < 10) {
      delay(100); 
    }
  tutupGerbangKeluar();
  server.sendHeader("Gerbang Keluar Control","*");
  server.send(200,"text/plain","Buka_Gerbang_Keluar_Success");

}

void tutupGerbangKeluar(){
  for (posKeluar = 0; posKeluar <= 90; posKeluar += 1) {
    servo2.write(posKeluar);
    delay(15);
  }
}

