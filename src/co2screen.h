#ifndef __CO2SCREEN__
#define __CO2SCREEN__

class CO2Screen: public TFT_eSPI
{
    public:
    CO2Screen(int font = 4):
        TFT_eSPI(),
        m_font(font)
    {
        init();
        setRotation(3);
        setTextFont(m_font);
    }
    void Clear(int color = TFT_BLACK)
    {
        fillScreen(color);
        setCursor(0, 0);
        setTextFont(m_font);
        setTextSize(1);

    }

    void PrintCo2(int co2ppm, int idx_shift = 0)
    {
        if (co2ppm < 600)
            setTextColor(TFT_GREEN);
        else if (co2ppm < 1000)
            setTextColor(TFT_YELLOW);
        else
            setTextColor(TFT_RED);
        setTextSize(textsize + 1);
        int big_fnt_height = fontHeight();
        printf("CO");

        setTextSize(textsize - 1);
        int small_fnt_height = fontHeight();
        idx_shift = big_fnt_height - small_fnt_height - idx_shift;
        setCursor(getCursorX(), getCursorY() + idx_shift);
        printf("2");
        setTextSize(textsize+1);
        setCursor(getCursorX(), getCursorY() - idx_shift);
        printf(": %d\n", co2ppm);
        setTextSize(textsize - 1);
    }
    void PrintTemp(int temp, int temp_shift = -3)
    {
        setTextSize(textsize + 1);
        temp += temp_shift;
        setTextColor(TFT_SKYBLUE);
        if (temp < 18)
            setTextColor(TFT_SKYBLUE);
        else if (temp < 24)
            setTextColor(TFT_YELLOW);
        else
            setTextColor(TFT_ORANGE);
        printf("Temp: %d\n", temp);
        setTextSize(textsize - 1);
    }

    private:
        int m_font;
};

#endif

