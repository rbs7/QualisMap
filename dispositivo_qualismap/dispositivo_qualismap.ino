/*
 *  This code sends data via HTTP GET requests to our server.
 *
 *  QualisMap device code
 */

#include <ESP8266WiFi.h>
#include<Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

int max (int a, int b) {
  return a > b ? a : b;
}

//Variaveis do MCU-6050
const int MPU=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int qualidade = 0;

//Variaveis da WiFi
const char* ssid     = "CINGUESTS";
const char* password = "acessocin";
const char* host = "172.22.78.206"; //Server Address

//Variaveis do GPS
static const int RXPin = 13, TXPin = 15;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

void setup(){
  Wire.begin(12, 14); // sda, scl
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Serial.begin(115200);
  ss.begin(GPSBaud);
  
  delay(10);
  Serial.println("Iniciando dispositivo QualisMap...");
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");  
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());
  
  int GyXY = sqrt(GyX*GyX + GyY*GyY);
  if (GyXY > 3500) {
    qualidade = max(qualidade,1);
  }
  if (GyXY > 5500) {
    qualidade = max(qualidade,2);
  }
  if (GyXY > 7500) {
    qualidade = max(qualidade,3);
  }

  Serial.print("Qualidade = "); Serial.println(qualidade);
    

  /*
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.println(AcZ);
  //Serial.print(" | Tmp = "); Serial.println(Tmpm/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print("GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  */

  if (gps.hdop.isValid() && gps.hdop.value() > 500) qualidade = 0;

  if(qualidade > 0 && gps.location.isValid() && gps.hdop.isValid() && gps.hdop.value() <= 500) {
    
    
    Serial.print("Conectando a ");
    Serial.println(host);
    
    
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("conexao falhou");
      return;
    }
    
    // We now create a URI for the request
    String url = "/php/receiver_v2.php?";
    url += "latitude=";
    url += String(gps.location.lat(),6);
    url += "&longitude=";
    url += String(gps.location.lng(),6);
    url += "&data=";
    url += gps.date.value();
    url += "&hora=";
    url += gps.time.value();
    url += "&classificacao=";
    if(qualidade==1) url+="RE";
    else if(qualidade==2) url+="PR";
    else url+="NR";
    qualidade = 0;
   
    Serial.print("Requisitando URL: ");
    Serial.println(url);
    
    
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    Serial.println();
    Serial.println("fechando conexao");

    delay(5000);
  }





  /*
  if (gps.location.isUpdated())
  {
    
    Serial.print(F("LOCATION   "));
    Serial.print(F(" Lat="));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long="));
    Serial.println(gps.location.lng(), 6);
    
  }

  else if (gps.date.isUpdated())
  {
    
    Serial.print(F("DATE       "));
    Serial.print(F("Raw="));
    Serial.println(gps.date.value());
    
  }

  else if (gps.time.isUpdated())
  {
    
    Serial.print(F("TIME       "));
    Serial.print(F("Raw="));
    Serial.println(gps.time.value());
    
  }*/

  else if (gps.hdop.isValid())
  {
    
    Serial.print(F("HDOP       "));
    Serial.print(F("Value="));
    Serial.println(gps.hdop.value());
    
  }
  
  
}
