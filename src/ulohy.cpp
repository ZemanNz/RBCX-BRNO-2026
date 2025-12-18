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
    while(rkServosGetPosition(1) < 75){
        delay(10);
    }
}
void zasun_zhazovadlo(){
    rkServosSetPosition(1, 0);
    while(rkServosGetPosition(1) > 5){
        delay(10);
    }
}

void srovnani(){ // nejak vyuzit tlacitka a ultrazvuky.....

}

//////////////////////////////////////////////////////////////////////////////////////////////////

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

}