#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <MHZ19_uart.h>
#include <Button2.h>

#define BUTTON_1        35
#define BUTTON_2        0
#define TFT_BL          4  // Display backlight control pin

TFT_eSPI tft = TFT_eSPI();  // Invoke library
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);
int tft_rotation = 3;

const int rx_pin = 26;  //Serial rx pin no
const int tx_pin = 27; //Serial tx pin no

MHZ19_uart mhz19;

int fnt=4;
int idx_shift = 23;
void query_mhz19()
{
  int co2ppm = mhz19.getPPM();
  int temp = mhz19.getTemperature();

  Serial.print("co2: "); Serial.print(co2ppm);
  Serial.print("    temp: "); Serial.println(temp);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(0, 0);
  tft.setTextFont(fnt);
  tft.setTextSize(2);
  if (co2ppm < 600)
    tft.setTextColor(TFT_GREEN);
  else if (co2ppm < 1000)
    tft.setTextColor(TFT_YELLOW);
  else
    tft.setTextColor(TFT_RED);

  int big_fnt_height = tft.fontHeight();
  tft.printf("CO");
  tft.setTextSize(1);
  int small_fnt_height = tft.fontHeight();
  idx_shift = big_fnt_height - small_fnt_height - 5;
  tft.setCursor(tft.getCursorX(), tft.getCursorY() + idx_shift);
  tft.printf("2");
  tft.setTextSize(2);
  tft.setCursor(tft.getCursorX(), tft.getCursorY() - idx_shift);
  tft.printf(": %d\n", co2ppm);
  tft.setTextColor(TFT_SKYBLUE);
  tft.printf("Temp: %d\n", temp-3);
}


void button_init()
{
    btn1.setPressedHandler([](Button2 & b)
    {
      tft_rotation = (tft_rotation + 1) % 4;
      tft.setRotation(tft_rotation);
      Serial.printf("Screen rotation: %d\n", tft_rotation);
      query_mhz19();
    });

    btn2.setPressedHandler([](Button2 & b)
    {
        Serial.printf("Zero point calibration\n", fnt);
        mhz19.calibrateZero();
    });
}

/*----------------------------------------------------------
    MH-Z19 CO2 sensor  setup
  ----------------------------------------------------------*/
void setup()
{

  Serial.begin(115200);
  mhz19.begin(rx_pin, tx_pin);
  mhz19.setAutoCalibration(false);
  button_init();
  tft.init();
  tft.setRotation(tft_rotation);

  int n = 10;
  while(mhz19.isWarming() && --n > 0)
  {
    Serial.print("MH-Z19 now warming up...  status:");Serial.println(mhz19.getStatus());
    delay(1000);
  }
}

/*----------------------------------------------------------
    MH-Z19 CO2 sensor  loop
  ----------------------------------------------------------*/
int zzz = 0;
void loop() 
{

  if (zzz-- == 0)
  {
    query_mhz19();
    zzz = 5000000;
  }
  btn1.loop();
  btn2.loop();
}