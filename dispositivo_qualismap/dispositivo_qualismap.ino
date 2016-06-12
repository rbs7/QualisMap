/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include<Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Variaveis do MCU-6050
const int MPU=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int AcXm,AcYm,AcZm,Tmpm,GyXm,GyYm,GyZm;
int count, N = 20;
unsigned long tempoamostra;
int x, i;
int uplim = -8000;
int dwlim = 8000;
int Gyro;
char aux[7];

//Variaveis da WiFi
const char* ssid     = "EF2";
const char* password = "*****";
const char* host = "data.sparkfun.com";
const char* streamId   = "RMqJbj6LzAFpXRKDrdzY";
const char* privateKey = "lzpW7X5Jrbi0ZXkp6DM5";

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
  tempoamostra = millis();
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
/*
  Serial.print("#S|LOGFILE|["); Serial.print(itoa(AcX, aux, 10));
  Serial.print("&"); Serial.print(itoa(AcY, aux, 10));
  Serial.print("&"); Serial.print(itoa(AcZ, aux, 10));
  //Serial.print(" | Tmp = "); Serial.println(Tmp/340.00+36.53);
  
  Serial.print("&"); Serial.print(itoa(GyX, aux, 10));
  Serial.print("&"); Serial.print(itoa(GyY, aux, 10));
  Serial.print("&"); Serial.print(itoa(GyZ, aux, 10));
  Serial.println("]#");
  */

  Gyro = sqrt(GyX*GyX + GyY*GyY);
  if (Gyro > 3500) {
    Serial.print("Gyro = "); Serial.println(Gyro);
  }
  x = 100*(Gyro-dwlim)/(uplim-dwlim);
  x = x < 0 ? 0 : x;
  x = x > 100 ? 100 : x;
  
  Serial.print("|");
  for (i = 0; i <= 100; i++) {
    if (i == x) {
      Serial.print("*");
    } else {
      Serial.print(" ");
    }
  }
  Serial.println("|");
  
  

/*
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());
  
  if (millis() - tempoamostra > 100 && count>0) {
    AcXm/=count;
    AcYm/=count;
    AcZm/=count;
    Tmpm/=count;
    GyXm/=count;
    GyYm/=count;
    GyZm/=count;

    Serial.print("AcX = "); Serial.print(AcXm);
    Serial.print(" | AcY = "); Serial.print(AcYm);
    Serial.print(" | AcZ = "); Serial.print(AcZm);
    //Serial.print(" | Tmp = "); Serial.println(Tmpm/340.00+36.53);
    
    Serial.print(" | GyX = "); Serial.print(GyXm);
    Serial.print(" | GyY = "); Serial.print(GyYm);
    Serial.print(" | GyZ = "); Serial.println(GyZm);

    


    
    if(gps.location.isValid() && gps.hdop.isValid() && gps.hdop.value() <= 500) {
      Serial.print("Connecting to ");
      Serial.println(host);
      
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      
      
      // We now create a URI for the request
      String url = "/input/";
      url += streamId;
      url += "?private_key=";
      url += privateKey;
      url += "&acx=";
      url += AcXm;
      url += "&acy=";
      url += AcYm;
      url += "&acz=";
      url += AcYm;
      //url += "&tmp=";
      //url += Tmpm;
      url += "&gyx=";
      url += GyXm;
      url += "&gyy=";
      url += GyYm;
      url += "&gyz=";
      url += GyZm;
      url += "&lat=";
      url += gps.location.lat();
      url += "&long=";
      url += gps.location.lng();
      url += "&date=";
      url += gps.date.value();
      url += "&time=";
      url += gps.time.value();
      url += "&hdop=";
      url += gps.hdop.value();
      
      Serial.print("Requesting URL: ");
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
      Serial.println("closing connection");
  

    }

    

    AcXm=0;
    AcYm=0;
    AcZm=0;
    Tmpm=0;
    GyXm=0;
    GyYm=0;
    GyZm=0;
    count = 0;
    tempoamostra = millis();
    
  } else {
    AcXm+=AcX;
    AcYm+=AcY;
    AcZm+=AcZ;
    Tmpm+=Tmp;
    GyXm+=GyX;
    GyYm+=GyY;
    GyZm+=GyZ;
    count++;
  }




  

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
  }

  else if (gps.hdop.isUpdated())
  {
    Serial.print(F("HDOP       "));
    Serial.print(F("Value="));
    Serial.println(gps.hdop.value());
  }

  */
  delay(50);
}
