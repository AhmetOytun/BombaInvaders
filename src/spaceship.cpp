#include "spaceship.hpp"
#include "constants.hpp"
#include <iostream>

Spaceship::Spaceship(){
    image = LoadTexture("Graphics/spaceship.png");
    position.x = GetScreenWidth() / 2 - image.width / 2;
    position.y = GetScreenHeight() - image.height - 100;
    lastFireTime = 0.0;
}
Spaceship::~Spaceship(){
    UnloadTexture(image);
}

void Spaceship::Draw(){
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft(){
    position.x -= SPACESHIP_SPEED;
    if(position.x <= 0 + 50){
        position.x = 0 + 50;
    }
}

void Spaceship::MoveRight(){
    position.x += SPACESHIP_SPEED;
    if(position.x >= GetScreenWidth() - image.width - 50){
        position.x = GetScreenWidth() - image.width - 50;
    }
}

void Spaceship::FireLaser(){
    if(GetTime()-lastFireTime>=LASER_FIRE_DELAY){
        lasers.push_back(Laser({position.x + image.width/2 - LASER_WIDTH/2,position.y},6));
        lastFireTime = GetTime();
    }
}

Rectangle Spaceship::getRect(){
    return {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset(){
    position.x = GetScreenWidth() / 2 - image.width / 2;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear();
}