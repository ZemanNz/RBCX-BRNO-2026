#include "robotka.h"
#include "ulohy.h"

void otevrit_klepeto(){
    rkServosSetPosition(4, 88);
}
void zavrit_klepeto(){
    rkServosSetPosition(4, -41);
}

void vysun_zhazovadlo(){//musi byt blokujici ... ceka nez dosahne stupne....
    rkServosSetPosition(1,80);
    delay(200);
}
void zasun_zhazovadlo(){
    rkServosSetPosition(1, 0);
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
        if (r > g && r > b && r > 170)
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
bool is_free_right(int threshold) {
    return rkUltraMeasure(1) > threshold;
}

bool is_free_front(int threshold) {
    return rkUltraMeasure(3) > threshold;
}

bool is_free_left(int threshold) {
    return rkUltraMeasure(2) > threshold;
}

void sprint(int distance){
    forward_acc(distance, 60);
}
void slalom(bool right){
    forward(300,60);
    turn_on_spot_right(90, 50);
    forward(150,60);
    radius_left(90, 180, 60);
    radius_right(90, 184, 60);
    back_buttons(30);
    forward(84,30);
    turn_on_spot_left(90, 50);
    back_buttons(30);
    forward(84,30);
    turn_on_spot_left(90, 50);
    forward(300,50);
}
void medved(){
    otevrit_klepeto();
    delay(10);
    forward(450,70);
    delay(10);
    radius_right(75, 90, 70);
    delay(10);
    forward(450,70);
    delay(10);
    back_buttons(45);
    delay(10);
    zavrit_klepeto();
    delay(10);
    forward(89,30);
    delay(10);
    turn_on_spot_right(90,50);
    delay(10);
    back_buttons(30);
    delay(10);
    forward(89,30);
    delay(10);
    forward(300,70);
    delay(10);
    turn_on_spot_right(90,50);
    delay(10);
    forward(300,70);

}
void kulicky(){
    forward(90,30);
    vysun_zhazovadlo();
    zasun_zhazovadlo();
    forward(300,50);
    vysun_zhazovadlo();
    zasun_zhazovadlo();
    forward(280,50);
    vysun_zhazovadlo();
    zasun_zhazovadlo();
    backward(50, 30);
    turn_on_spot_left(90, 50);
    srovnani();
    turn_on_spot_right(90, 50);
    forward(300,60);

}
void bludiste(){
    forward(300,70);
    for(int i=0; i< 5; i++){
        if(is_free_right()){// v pravo je volno
            turn_on_spot_right(90, 50);
            delay(10);
            forward(300,70);
            delay(10);
        }
        else if(is_free_front()){// ve predu je volno
            forward(300,70);
            delay(10);
        }
        else{ // vlevo
            turn_on_spot_left(90, 50);
            if(is_free_front()){
                srovnani(); 
                turn_on_spot_right(90, 50); 
            }
            else{
                srovnani();
            }
        }
        delay(100);
    }
    while(!cervena()){ 
        if(is_free_right()){// v pravo je volno
            turn_on_spot_right(90, 50);
            forward(300,50);
        }
        else if(is_free_front()){// ve predu je volno
            forward(300,70);
        }
        else{ // vlevo
            turn_on_spot_left(90, 50);
            if(is_free_front()){
                srovnani(); 
                turn_on_spot_right(90, 50); 
            }
            else{
                srovnani();
            }
        }
        delay(100);
    }
}

