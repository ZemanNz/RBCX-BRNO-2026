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

void sprint(int distance){
    forward_acc(distance/2, 60);
    srovnej_se_v_pravo();
    forward_acc(distance/2, 60);
}
void slalom(bool right){
    forward_acc(270,60);
    turn_on_spot_right(90, 50);
    forward_acc(140,60);
    delay(1000);
    radius_left(70, 180, 60);
    delay(1000);
    radius_right(70, 180, 60);
    delay(1000);
    back_buttons(60);
    forward_acc(od_steny_na_stred_pole,30);
    turn_on_spot_right(90, 50);
    back_buttons(40);
    forward_acc(od_steny_na_stred_pole,30);
    turn_on_spot_left(90, 50);
    srovnej_se_v_levo();
    forward_acc(jedno_pole,50);
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

