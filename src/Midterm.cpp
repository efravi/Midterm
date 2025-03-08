/* 
 * Project Midterm
 * Author: Efrain Vila
 * Date:3.6.25
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_BME280.h"
#include "Adafruit_SSD1306.h"
#include "neopixel.h"
#include "IoTClassroom_CNM.h"
#include "animation.h"

const int rgbcycle[] = {red, green, yellow};

SYSTEM_MODE(MANUAL);

const int OLED_RESET = -1;

const int PIXELCOUNT = 3;
int pixNum;
const int PIXDELAY = 100;

const int BBUTTON = D5;
const int JBUTTON = D4;
const int EBUTTON = D3;
int inputValueB;
int inputValueJ;
int inputValueE;
bool buttonState = FALSE;

Adafruit_NeoPixel pixel ( PIXELCOUNT , SPI1 , WS2812B );
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BME280 bme;

Button brian (BBUTTON);
Button jamie (JBUTTON);
Button ej (EBUTTON);

TCPClient WeMoClient;

int const hexAddress = 0x76;
byte status;
float tempC;
float pressPA;
float humidRH;
int currentTime;
int lastSecond;
float tempF;
float pressHg;
int humBright;
bool lunchScreen;
bool switchWemo;

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
// SerialLogHandler logHandler(LOG_LEVEL_INFO);

// setup() runs once, when the device is first turned on
void setup() { 
  
WiFi.on();
WiFi.clearCredentials();
WiFi.setCredentials("IoTNetwork");

WiFi.connect(); 
  
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  pixel.begin();
  pixel.setBrightness (100);
  pixel.setPixelColor (0, red);
  pixel.setPixelColor (1, red);
  pixel.setPixelColor (2, red);
  pixel.show();

  status = bme.begin (hexAddress);

  // Put initialization like pinMode and begin functions here
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  tempC = bme.readTemperature();
  pressPA = bme.readPressure ();
  humidRH = bme.readHumidity ();
  currentTime = millis();
  tempF = tempC * 1.8 +32;
  pressHg = pressPA * .0002953;
  humBright = humidRH * 4.25 - 85;

  if((currentTime - lastSecond)>1000){
    lastSecond = millis();
    //Serial.printf("temp %.0f\n", tempF);
    //Serial.printf("pressure %.0f\n", pressHg);
    //Serial.printf("humidity %.0f\n", humidRH);
  }

// pixNum = 1.5 * tempF -75;

  if (brian.isClicked()){
    inputValueB++;
    inputValueB = inputValueB%3;
    pixel.setPixelColor (0, rgbcycle[inputValueB]);
    pixel.show();
    Serial.printf("%i\n",inputValueB);
  }

  if (jamie.isClicked()){
    inputValueJ++;
    inputValueJ = inputValueJ%3;
    pixel.setPixelColor (1, rgbcycle[inputValueJ]);
    pixel.show();
    Serial.printf("%i\n",inputValueJ);
  }

  if (ej.isClicked()){
    inputValueE++;
    inputValueE = inputValueE%3;
    pixel.setPixelColor (2, rgbcycle[inputValueE]);
    pixel.show();
    Serial.printf("%i\n",inputValueE);
  }
  if((inputValueB != 1) && (inputValueJ != 1) && (inputValueE != 1)){
  Serial.printf("No lights are green, it's lunch\n");  
  
  // if((currentTime - lastSecond)>500){
  //   lastSecond = millis();
  display.drawBitmap(0, 0,  bitmap_iwpyei, 128, 64, 1);
  display.display();
  
  // if((currentTime - lastSecond)>500){
  //   lastSecond = millis();
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap_eipn6g, 128, 64, 1);
  display.display();
  

  // if((currentTime - lastSecond)>1000){
  //   lastSecond = millis();
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap_8mzo2w, 128, 64, 1);
  display.display();
  

  // if((currentTime - lastSecond)>1000){
  //   lastSecond = millis();
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap_aqehn2i, 128, 64, 1);
  display.display();
  

  // if((currentTime - lastSecond)>1000){
  //   lastSecond = millis();
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap_a1hcb, 128, 64, 1);
  display.display();
  display.clearDisplay();

    wemoWrite(2, HIGH);
    setHue(3, 255, HueBlue, 255 , 255);
}
else {
  Serial.printf("At least one light is green, class is on\n");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf("CLASS IS ON\nTemp: %.0f\nPressure: %.0f\nHumidity: %.0f", tempF, pressHg, humidRH);
  //display.printf("Temp: %f\nPressure: %f\nHumidity %f", tempf, pressure,)
  display.display();
  display.clearDisplay();

  wemoWrite(2, LOW);
  setHue(3, 255, HueGreen, 255 , 255);
}
}
