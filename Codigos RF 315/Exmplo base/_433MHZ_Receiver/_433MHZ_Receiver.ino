#include <VirtualWire.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>

#define TFT_CS     10
#define TFT_RST    8                      
#define TFT_DC     9
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!

const int receive_pin = 12;
char temperatureChar[10];
char humidityChar[10];


struct package
{
  float temperature = 0.0;
  float humidity = 0.0;
};


typedef struct package Package;
Package data;


void setup()
{
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK); 
    printUI();
    delay(1000);

    // Initialise the IO and ISR
    vw_set_rx_pin(receive_pin);
    vw_setup(500);	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[sizeof(data)];
    uint8_t buflen = sizeof(data);

if (vw_have_message())  // Is there a packet for us? 
  {
    vw_get_message(buf, &buflen);
    memcpy(&data,&buf,buflen);

    Serial.print("\nPackage:");
    Serial.print(data.temperature);
    String temperatureString = String(data.temperature,1);
    temperatureString.toCharArray(temperatureChar,10);
    tft.fillRect(10,20,80,30,ST7735_BLACK);
    printText(temperatureChar, ST7735_WHITE,10,20,3);

    String humidityString = String(data.humidity,1);
    humidityString.toCharArray(humidityChar,10);
    tft.fillRect(10,95,80,100,ST7735_BLACK);
    printText(humidityChar, ST7735_WHITE,10,95,3);

    Serial.print("\n");
    Serial.println(data.humidity);
  }
}

void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}

void printUI()
{
    printText("TEMPERATURE", ST7735_GREEN,30,5,1);  // Temperature Static Text
    printText("o", ST7735_WHITE,90,13,2);
    printText("C", ST7735_WHITE,105,20,3);

    printText("HUMIDITY", ST7735_BLUE,30,80,1);  // Temperature Static Text
    printText("%", ST7735_WHITE,90,95,3);
}

