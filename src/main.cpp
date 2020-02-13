#define USE_WifWaf

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#ifdef USE_WifWaf
#include <MHZ19.h>
#else
#include <MHZ19_uart.h>
#endif
#include <Button2.h>
#include "./co2screen.h"


#define BUTTON_1        35
#define BUTTON_2        0
#define TFT_BL          4  // Display backlight control pin

CO2Screen tft = CO2Screen(4);  // Invoke library
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);
int tft_rotation = 3;
int screen = 1;
int max_screen = 2;

const int rx_pin = 26;  //Serial rx pin no
const int tx_pin = 27; //Serial tx pin no

#ifdef USE_WifWaf
HardwareSerial hserial(1);
MHZ19 mhz19;
#else
MHZ19_uart mhz19;
#endif

int idx_shift = 23;
void draw_screen()
{
  int co2ppm =
#ifdef USE_WifWaf
  mhz19.getCO2(false);
#else
  mhz19.getPPM();
#endif
  int temp = mhz19.getTemperature();

  tft.Clear();
  switch(screen)
  {
      case 1: // main screen - CO2 + temp
      {
          tft.PrintCo2(co2ppm);
          tft.PrintTemp(temp);
          break;
      }
      case 2:
      {
          tft.setTextFont(2);
          tft.PrintCo2(co2ppm);
          tft.PrintTemp(temp);
#ifdef USE_WifWaf
          tft.setTextSize(1);
          tft.setTextColor(TFT_WHITE);
          tft.printf("CO2 unlim  : %d\n", mhz19.getCO2(true));
          tft.printf("CO2 raw    : %.2f\n", mhz19.getCO2Raw());
          tft.printf("CO2 backgr : %d\n", mhz19.getBackgroundCO2());
          tft.printf("Range      : 0..%d\n", mhz19.getRange());
#endif

      }
  }
#ifdef USE_WifWaf
#endif
}


void button_init()
{
    btn1.setPressedHandler([](Button2 & b)
    {
      if (++screen > max_screen) screen = 1;
      draw_screen();
    });

    btn2.setPressedHandler([](Button2 & b)
    {
        mhz19.calibrateZero();
    });
}

/*----------------------------------------------------------
    MH-Z19 CO2 sensor  setup
  ----------------------------------------------------------*/
void setup()
{

  Serial.begin(115200);
#ifdef USE_WifWaf
  hserial.begin(9600, SERIAL_8N1, rx_pin, tx_pin);
  mhz19.begin(hserial);
#else
  mhz19.begin(rx_pin, tx_pin);
  mhz19.setAutoCalibration(false);
#endif
  button_init();

#ifndef USE_WifWaf
  int n = 10;
  while(mhz19.isWarming() && --n > 0)
  {
    Serial.print("MH-Z19 now warming up...  status:");Serial.println(mhz19.getStatus());
    delay(1000);
  }
#endif
}

/*----------------------------------------------------------
    MH-Z19 CO2 sensor  loop
  ----------------------------------------------------------*/
int zzz = 0;
void loop() 
{

  if (zzz-- == 0)
  {
    draw_screen();
    zzz = 5000000;
  }
  btn1.loop();
  btn2.loop();
}