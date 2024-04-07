#include "mysteryship.hpp"
#include "constants.hpp"

MysteryShip::MysteryShip(){
    image = LoadTexture("Graphics/mystery.png");
    alive = false;
}
MysteryShip::~MysteryShip(){
    UnloadTexture(image);
}

void MysteryShip::Spawn(){
    position.y = 20;
    int side = GetRandomValue(0,1);

    if(side == 0){
        position.x = 0;
        speed = MYSTERYSHIP_SPEED;
    }else{
        position.x = GetScreenWidth() - image.width;
        speed = -MYSTERYSHIP_SPEED;
    }

    alive = true;
}

void MysteryShip::Update(){
    if(alive){
        position.x += speed;
        if(position.x + image.width < 0 || position.x > GetScreenWidth()){
            alive = false;
        }
    }
}

void MysteryShip::Draw(){
    if(alive){
        DrawTextureV(image,position,WHITE);
    }
}

Rectangle MysteryShip::getRect(){
    if(alive){
        return {position.x, position.y, float(image.width), float(image.height)};
    }else{
        return {position.x,position.y,0,0};
    }
}