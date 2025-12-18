#include "robotka.h"
#include "ulohy.h"

byte Bbutton1 = 34;
byte Bbutton2 = 35;


void trap() {
    Serial.println("trap\n");
    while (1)
        ;
}
void test_batery(){
    printf("batery percent: %u\n", rkBatteryPercent());
    printf("batery percent: %u\n", rkBatteryVoltageMv()/1000);
}
void configurating(){
    Serial.begin(115200);
    rkConfig cfg;
    rkSetup(cfg);
    static const uint8_t TCS_SDA_pin = 21;
    static const uint8_t TCS_SCL_pin = 22;


    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
    pinMode(TCS_SDA_pin, PULLUP);
    pinMode(TCS_SCL_pin, PULLUP);
    Wire1.begin(TCS_SDA_pin, TCS_SCL_pin, 100000);
    if (!tcs.begin(TCS34725_ADDRESS, &Wire1)) {
        printf("Can not connect to the RGB sensor");
        trap();
    }
    printf("Starting main loop\n");
    //start tlacitko pro kalibraci klepet

    rkLedBlue(false);
    rkLedGreen(false);
    rkLedYellow(false);
    rkLedRed(false);
    test_batery();
}

enum RobotButton {
    NONE,
    UP_VYHREJ,
    OFF_MEDVED,
    DOWN_KULICKY,
    RIGHT_BLUDISTE,
    LEFT_SLALOM,
    ON_SPRINT,
    BUTTON1_KOMBINACE1,
    BUTTON2_KOMBINACE2
};

RobotButton getPressed() {
    if (rkButtonUp()) return UP_VYHREJ;
    if (rkButtonOff()) return OFF_MEDVED;
    if (rkButtonDown()) return DOWN_KULICKY;
    if (rkButtonRight()) return RIGHT_BLUDISTE;
    if (rkButtonLeft()) return LEFT_SLALOM;
    if (rkButtonOn()) return ON_SPRINT;
    if (digitalRead(Bbutton1) == LOW) return BUTTON1_KOMBINACE1;
    if (digitalRead(Bbutton2) == LOW) return BUTTON2_KOMBINACE2;
    return NONE;
}

void setup() {
    configurating();
    
    while(true) {
        switch(getPressed()) {
            case UP_VYHREJ:
                
                break;

            case OFF_MEDVED:
               
                break;

            case DOWN_KULICKY:
                
                break;
                
            case RIGHT_BLUDISTE:
                
                break;
                
            case LEFT_SLALOM:
                
                break;
                
            case ON_SPRINT:
                
                break;
                
            case BUTTON1_KOMBINACE1:
                
                break;
                
            case BUTTON2_KOMBINACE2:
                
                break;
        }
        delay(50);
    }
}
