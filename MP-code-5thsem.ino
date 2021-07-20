//code by Siddhartha.S.Singh and Md.Omer Ali
//Date=28/1/21
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define DHT11Pin D3
#define DHTType DHT11
//OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//MLX90614
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

DHT dht(DHT11Pin,DHTType); //import dht lib
BlynkTimer timer; //blynk timer
float humi;
float tempC;
float tempF;

//Blynk auth
char auth[] = "5grXWjJZOPYI1q4hw-onU3uOC3maP6PB";
char ssid[] = "OnePlus_Nord";
char pass[] = "Okok1234";


//mlx
float objectTemp, stemp; // object temperature
float threshold= 5.6 ;

//OLED define
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup() {
  Serial.begin(9600);
  //Blynk auth call
  Blynk.begin(auth, ssid, pass);
  //For DHT11
  dht.begin();
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  //For OLED I2C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display(); //Display logo
  delay(1000); 
  display.clearDisplay();
}

void loop() {
 delay(1000);
  Blynk.run();
 timer.run();
 humi = dht.readHumidity();
 tempC = dht.readTemperature();
 tempF = dht.readTemperature(true);

//print of dht data on oled dynamically
 Serial.print("Humidity:");
 Serial.print(humi,0);
 Serial.print("%");
 Serial.print(" Temperature:");
 Serial.print(tempC,1);
 Serial.print("C ~ ");
 Serial.print(tempF,1);
 Serial.println("F");

 display.clearDisplay();
 oledDisplayHeader();
 

 oledDisplay(3,5,28,humi,"%");
 oledDisplay(2,70,16,tempC,"C");
 oledDisplay(2,70,44,tempF,"F");
 
 display.display(); 

 delay(10000);

  //print on screne to place hand
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("PLACE YOUR HAND NEAR SENSOR");
  display.display();

 delay(5000);

  objectTemp = threshold + mlx.readObjectTempC() ;  
  // print to OLED of mlx
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("Temp:" + String(objectTemp) + "C");
  display.display(); 

 delay(10000);       
}
void oledDisplayHeader(){
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0, 0);
 display.print("Humidity");
 display.setCursor(60, 0);
 display.print("Temperature");
}
void oledDisplay(int size, int x,int y, float value, String unit){
 int charLen=12;
 int xo=x+charLen*3.2;
 int xunit=x+charLen*3.6;
 int xval = x; 
 display.setTextSize(size);
 display.setTextColor(WHITE);
 
 if (unit=="%"){
   display.setCursor(x, y);
   display.print(value,0);
   display.print(unit);
 } else {
   if (value>99){
    xval=x;
   } else {
    xval=x+charLen;
   }
