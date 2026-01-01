#include "robotka.h"
#include "ulohy.h"

int zadek_od_stredu = 105; // v mm (10.5 cm)
int predek_od_stredu = 100; // v mm (10 cm)
int jedno_pole = 300; // v mm (30 cm)
int od_steny_na_stred_pole = jedno_pole/2 - zadek_od_stredu; // 150 - 105 = 45 mm (4.5 cm)




void otevrit_klepeto(){
    rkServosSetPosition(4, 88);
}
void zavrit_klepeto(){
    rkServosSetPosition(4, -41);
}

void vysun_zhazovadlo(){//musi byt blokujici ... ceka nez dosahne stupne....
    rkServosSetPosition(1,-90);
    delay(200);
}
void zasun_zhazovadlo(){
    rkServosSetPosition(1, -55);
    delay(200);
}

uint16_t levy_ir(){
    return rkIrRight();
}
uint16_t pravy_ir(){
    return rkIrLeft();
}
void srovnani(){

}

void srovnej_se_v_pravo(){
    orient_to_wall(true, []() -> uint32_t { return rk_laser_measure("laser"); },
                             []() -> uint32_t { return rkUltraMeasure(2); }, -23);
}

void srovnej_se_v_levo(){
    orient_to_wall(false, []() -> uint32_t { return rkUltraMeasure(4); },
                             []() -> uint32_t { return rkUltraMeasure(3); },-10);
}   

bool cervena(){
    float r, g, b;
    if (rkColorSensorGetRGB("front", &r, &g, &b)) {
        printf("red: %f, green: %f, blue: %f\n", r, g, b);
        delay(10);
        if (r > g && r > b && r > 130)
        {
            printf("RED\n");
            rkLedRed(true);
            return true;
        }
        else {
            return false;
        }
    } else {
        Serial.println("Sensor 'front' not found.");
        return false;
    }
}

void sledovani_cary() {
    while (rkUltraMeasure(1) > 100){
        // Použijeme pomocné funkce, které správně mapují senzory
        uint16_t pravy_senzor = pravy_ir(); 
        uint16_t levy_senzor = levy_ir();

        // Pro debugování můžeme hodnoty tisknout
        Serial.print("PRAVY: "); Serial.println(pravy_senzor);
        Serial.print("LEVY: "); Serial.println(levy_senzor);
        Serial.println();

        int zakladni_rychlost = 30; // Základní rychlost (%)

        int rychlost_levy_motor = map(levy_senzor, 0, 3200, 0, 50);
        int rychlost_pravy_motor = map(pravy_senzor, 0, 3200, 0, 50);


        // Nastavení rychlosti motorů
        rkMotorsSetSpeed(rychlost_levy_motor, rychlost_pravy_motor);

        delay(10); // Krátká pauza pro stabilizaci
    }
    // Po ukončení smyčky (překážka je blízko) zastavíme motory
    rkMotorsSetSpeed(0, 0);
}

void srovnej_na_caru() {
    Serial.println("Srovnávám se na čáru (sekvenčně)...");

    // NASTAVENÍ
    int rychlost = 10;       // Požadovaná rychlost otáčení
    int prah_cerne = 1000;     // Pod tímto číslem je černá (uprav dle kalibrace)
    unsigned long start_time = millis(); // Časovač pro timeout

    // 1. Spustíme otáčení DOLEVA
    // Levý motor couvá (-), pravý jede dopředu (+)
    rkMotorsSetSpeed(-rychlost, rychlost);

    // KROK A: Čekáme, až LEVÝ senzor UVIDÍ černou čáru
    // (Pokud už na ní stojí, tento cyklus se přeskočí nebo proběhne hned)
    while (levy_ir() > prah_cerne) {
        if (millis() - start_time > 100000) { rkMotorsSetSpeed(0,0); return; } // Timeout
        Serial.print("IR levý: "); Serial.println(rkIrRight());
        Serial.print("IR pravý: "); Serial.println(rkIrLeft());
        delay(30);
        // Stále se točíme...
    }

    Serial.println("Levý senzor vidí čáru.");

    // KROK B: Čekáme, až LEVÝ senzor PŘEJEDE čáru (uvidí zase bílou)
    // Tím zajistíme, že čára je teď vpravo od levého senzoru
    while (levy_ir() < prah_cerne) {
        if (millis() - start_time > 100000) { rkMotorsSetSpeed(0,0); return; } // Timeout
        Serial.print("IR levý: "); Serial.println(rkIrRight());
        Serial.print("IR pravý: "); Serial.println(rkIrLeft());
        delay(30);
        // Stále se točíme...
    }

    Serial.println("Levý senzor přejel čáru (je na bílé).");

    // KROK C: Pokračujeme v točení, dokud PRAVÝ senzor nenarazí na okraj čáry
    while (pravy_ir() > prah_cerne + 500) {
        if (millis() - start_time > 100000) { rkMotorsSetSpeed(0,0); return; } // Timeout
        Serial.print("IR levý: "); Serial.println(rkIrRight());
        Serial.print("IR pravý: "); Serial.println(rkIrLeft());
        delay(30);
        // Stále se točíme...
    }

    // HOTOVO: Pravý senzor narazil na čáru.
    // Teď by měla být čára mezi senzory (levý je na bílé, pravý na hraně černé).
    rkMotorsSetSpeed(0, 0); 
    Serial.println("Srovnáno! Zastavuji.");

    // while(true){
    //     delay(1000);
    //     Serial.print("IR levý: "); Serial.println(rkIrRight());
    //     Serial.print("IR pravý: "); Serial.println(rkIrLeft());

    // }
}



//////////////////////////////////////////////////////////////////////////////////////////////////

// Assuming sensor 2 is for left
bool is_free_right() {
    return rk_laser_measure("laser") > 200;
}

bool is_free_front() {
    return rkUltraMeasure(1) > 200;
}

bool is_free_left() {
    return rkUltraMeasure(4) > 200;
}

void sprint_m_d(){
    
    // Na začátku změříme vzdálenost k oběma stěnám
    int first_distance_left = rkUltraMeasure(4);
    int first_distance_right = rk_laser_measure("laser");

    // Nastavení P-regulátoru a rychlosti
    const int base_speed = 60; // Základní rychlost
    const float Kp = 0.5;      // Proporční konstanta (laditelná hodnota)

    int rychlost_levy_motor = base_speed;
    int rychlost_pravy_motor = base_speed;


    // Smyčka běží, dokud je před námi volno
    while(rkUltraMeasure(1) > (od_steny_na_stred_pole + 20)){ // + 20 kvuli setrvacnosti
        // Aktuální měření
        int current_distance_left = rkUltraMeasure(4);
        int current_distance_right = rk_laser_measure("laser");

        if(current_distance_right < 200 && current_distance_right> 10 && (abs(current_distance_right - first_distance_right) < abs(current_distance_left - first_distance_left))){
            // Výpočet chyby
            int error_right = current_distance_right - first_distance_right;

            // Výpočet korekce
            int correction = Kp * error_right;

            // Úprava rychlosti motorů
            rychlost_levy_motor = base_speed + correction;
            rychlost_pravy_motor = base_speed - correction;
        }
        else if(current_distance_left < 200 && current_distance_left > 10){
            // Výpočet chyby
            int error_left = current_distance_left - first_distance_left;

            // Výpočet korekce
            int correction = Kp * error_left;
            // Úprava rychlosti motorů
            rychlost_levy_motor = base_speed - correction;
            rychlost_pravy_motor = base_speed + correction;
        }
        else{
            rychlost_levy_motor = base_speed;
            rychlost_pravy_motor = base_speed;
        }

        // Odeslání příkazu motorům
        rkMotorsSetSpeed(rychlost_levy_motor, rychlost_pravy_motor);

        // Malá pauza pro plynulost
        delay(20);
    }

    // Po skončení smyčky zastavíme
    rkMotorsSetSpeed(0, 0);

    srovnej_se_v_levo();
    delay(100);
    turn_on_spot_right(90, 50);
    delay(100);
    back_buttons(40);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);
    turn_on_spot_right(90, 50);
    delay(100);
    back_buttons(40);
    delay(100);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);
    turn_on_spot_left(90, 50);
}
void sprint_cara(){
    forward_acc(150,60);
    turn_on_spot_right(50, 50);
    srovnej_na_caru();
    int a = 0;
    int casovac = 100;
    while (true){
        uint16_t pravy_senzor = pravy_ir(); 
        uint16_t levy_senzor = levy_ir();

        Serial.print("PRAVY: "); Serial.println(pravy_senzor);
        Serial.print("LEVY: "); Serial.println(levy_senzor);
        Serial.println();

        int zakladni_rychlost = 45; // Základní rychlost (%)

        int rychlost_levy_motor = map(levy_senzor, 0, 3800, 0, zakladni_rychlost);
        int rychlost_pravy_motor = map(pravy_senzor, 0, 3800, 0, zakladni_rychlost);

        rkMotorsSetSpeed(rychlost_levy_motor, rychlost_pravy_motor);
        delay(10); // Krátká pauza pro stabilizaci
        if(a > casovac){
            if(cervena()){
                break;
            }
            a = 0;
            casovac = 15;
        }
        a++;
    }

    rkMotorsSetSpeed(0, 0);

    srovnej_se_v_levo();
    delay(100);
    turn_on_spot_right(90, 50);
    delay(100);
    back_buttons(40);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);
    turn_on_spot_right(90, 50);
    delay(100);
    back_buttons(40);
    delay(100);
    forward_acc(od_steny_na_stred_pole, 50);
    delay(100);
    turn_on_spot_left(90, 50);
}
void slalom(bool right){
    forward_acc(150,60);
    turn_on_spot_right(50, 50);
    srovnej_na_caru();
    int a = 0;
    byte b= 0;
    int casovac = 300;
    while (true){
        uint16_t pravy_senzor = pravy_ir(); 
        uint16_t levy_senzor = levy_ir();

        Serial.print("PRAVY: "); Serial.println(pravy_senzor);
        Serial.print("LEVY: "); Serial.println(levy_senzor);
        Serial.println();

        int zakladni_rychlost = 25; // Základní rychlost (%)

        int rychlost_levy_motor = map(levy_senzor, 0, 3800, 0, zakladni_rychlost);
        int rychlost_pravy_motor = map(pravy_senzor, 0, 3800, 0, zakladni_rychlost);

        rkMotorsSetSpeed(rychlost_levy_motor, rychlost_pravy_motor);
        delay(10); // Krátká pauza pro stabilizaci
        if(a > casovac){
            if(cervena()){
                break;
            }
            a = 0;
            casovac = 15;
        }
        a++;
        if (pravy_senzor > 3700 && levy_senzor > 3700){///tohle ve v2 nebude
            b++;
            Serial.println(b);
            if(b > 15){
                rkMotorsSetSpeed(0, 0);
                srovnej_se_v_levo();
                forward_acc(50,60);
                delay(100);
                b=0;
            }
        }
        else{
            b = 0;
        }//posem --- mozna nahradit necim kdyby stratil caru
    }
    rkMotorsSetSpeed(0, 0);
    delay(100);
    srovnej_se_v_levo();
    delay(100);
    back_buttons(40);
    delay(100);
    forward_acc(od_steny_na_stred_pole , 50);
    turn_on_spot_right(90, 50);
    delay(100);
    back_buttons(40);
    delay(100);
    forward_acc(od_steny_na_stred_pole , 50);
    turn_on_spot_left(90, 50);
    delay(100);
    forward_acc(jedno_pole, 50);
    delay(100);
    srovnej_se_v_levo();
    delay(100);
    
}
void medved(){
    otevrit_klepeto();
    delay(100);
    forward(420,70);
    delay(100);
    radius_right(70, 90, 50);
    delay(100);
    forward(jedno_pole,70);
    delay(10);
    srovnej_se_v_pravo();
    delay(10);
    back_buttons(od_steny_na_stred_pole);
    delay(10);
    zavrit_klepeto();
    delay(10);
    forward_acc(od_steny_na_stred_pole,50);
    delay(100);
    turn_on_spot_right(90,50);
    delay(10);
    back_buttons(30);
    delay(10);
    forward_acc(jedno_pole + od_steny_na_stred_pole, 70); // 345
    delay(10);
    turn_on_spot_left(90,50);
    delay(10);
    forward_acc(jedno_pole,70);
    srovnej_se_v_pravo();
    delay(10);
}
void kulicky(){
    int cekani = 1500;
    forward_acc(30,30);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();
    forward_acc(jedno_pole,50);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();
    forward_acc(jedno_pole,50);
    vysun_zhazovadlo();
    delay(cekani);
    zasun_zhazovadlo();
    backward(od_steny_na_stred_pole, 30);
    turn_on_spot_left(90, 50);
    srovnej_se_v_pravo();
    delay(100);
    back_buttons(od_steny_na_stred_pole);
    delay(100);
    forward_acc(od_steny_na_stred_pole + jedno_pole, 50);

}
void bludiste(){
    forward_acc(jedno_pole,40);
    for(int i=0; i< 5; i++){
        if(is_free_right()){// v pravo je volno
            if(is_free_left()){
                turn_on_spot_right(90, 50);
                delay(100);
                forward_acc(jedno_pole,40);
                delay(100);
            }
            else{
                srovnej_se_v_levo();
                turn_on_spot_right(90, 50);
                delay(100);
                back_buttons(40);
                delay(100);
                forward_acc(od_steny_na_stred_pole + jedno_pole, 40); // 345
                delay(100);
            }
        }
        else if(is_free_front()){// ve predu je volno
            srovnej_se_v_pravo();
            forward_acc(jedno_pole,40);
            delay(100);
        }
        else if(is_free_left()){ // vlevo
            srovnej_se_v_pravo();
            turn_on_spot_left(90, 50);
            delay(100);
            forward_acc(jedno_pole,40);
            delay(100);             
        }
        else{
            turn_on_spot_left(180, 50);
            back_buttons(40);
            delay(100);
            forward_acc(od_steny_na_stred_pole + jedno_pole, 40); // 345
            delay(100);
        }
        
        delay(200);
    }
    while(!cervena()){ 
        rkBuzzerSet(true);
        delay(200);
        rkBuzzerSet(false);
        delay(100);
        if(is_free_right()){// v pravo je volno
            if(is_free_left()){
                turn_on_spot_right(90, 50);
                delay(100);
                forward_acc(jedno_pole,40);
                delay(100);
            }
            else{
                srovnej_se_v_levo();
                turn_on_spot_right(90, 50);
                delay(100);
                back_buttons(40);
                delay(100);
                forward_acc(od_steny_na_stred_pole + jedno_pole, 40); // 345
                delay(100);
            }
        }
        else if(is_free_front()){// ve predu je volno
            srovnej_se_v_pravo();
            forward_acc(jedno_pole,40);
            delay(100);
        }
        else if(is_free_left()){ // vlevo
            srovnej_se_v_pravo();
            turn_on_spot_left(90, 50);
            delay(100);
            forward_acc(jedno_pole,40);
            delay(100);             
        }
        else{
            turn_on_spot_left(180, 50);
            back_buttons(40);
            delay(100);
            forward_acc(od_steny_na_stred_pole + jedno_pole, 40); // 345
            delay(100);
        }
        
        delay(200);
    }
}


