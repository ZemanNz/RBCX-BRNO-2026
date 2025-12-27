#include "robotka.h"
#include "ulohy.h"

byte Bbutton1 = 34;
byte Bbutton2 = 35;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

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


    pinMode(TCS_SDA_pin, PULLUP);
    pinMode(TCS_SCL_pin, PULLUP);
    Wire1.begin(TCS_SDA_pin, TCS_SCL_pin, 100000);
    Wire1.setTimeOut(1); // from example

    // Initialize the color sensor with a unique name and the I2C bus
    rkColorSensorInit("front", Wire1, tcs);
    
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
        rkLedBlue(false);
        rkLedGreen(false);
        rkLedYellow(false);
        rkLedRed(false);

        switch(getPressed()) {
            case UP_VYHREJ:
                rkLedGreen(true); // Zelená pro výhru
                delay(10000);
                // Kombinace úkolů pro "vyhrej"
                sprint(2100); 
                delay(10000);
                slalom(true); 
                delay(10000);
                bludiste();
                delay(10000);
                medved();
                delay(10000);
                kulicky();
                break;

            case OFF_MEDVED:
                rkLedRed(true); // Červená pro medvěda
                delay(10000);
                medved();
                break;

            case DOWN_KULICKY:
                rkLedBlue(true); // Modrá pro kuličky
                delay(10000);
                kulicky();
                break;
                
            case RIGHT_BLUDISTE:
                rkLedYellow(true); // Žlutá pro bludiště
                delay(10000);
                bludiste();
                break;
                
            case LEFT_SLALOM:
                rkLedRed(true);
                rkLedYellow(true); // Oranžová pro slalom
                delay(10000);
                slalom(false);
                break;
                
            case ON_SPRINT:
                rkLedBlue(true);
                rkLedRed(true); // Fialová pro sprint
                delay(10000);
                sprint(2000);
                break;
                
            case BUTTON1_KOMBINACE1:
                rkLedGreen(true);
                rkLedYellow(true); // Zeleno-žlutá pro kombinaci 1
                delay(10000);
                // Zde může být akce pro kombinaci 1
                break;
                
            case BUTTON2_KOMBINACE2:
                rkLedRed(true);
                rkLedGreen(true);
                rkLedBlue(true);
                rkLedYellow(true); // Bílá (všechny barvy) pro kombinaci 2
                delay(10000);
                // Zde může být akce pro kombinaci 2
                break;
            
            case NONE:
                break;
        }
        delay(50);
    }
}
