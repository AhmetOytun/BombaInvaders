#include "laser.hpp"
#include "constants.hpp"

Laser::Laser(Vector2 position, int speed){
    this->position = position;
    this->speed = speed;
    active = true;
}

void Laser::Draw(){
    if(active){
        DrawRectangleV(position, {LASER_WIDTH, LASER_HEIGHT}, YELLOW);
    }
}

void Laser::Update(){
    position.y -= speed;
    if(active){
        if(position.y > GetScreenHeight() || position.y < 0){
            active = false;
        }
    }
}

Laser::~Laser(){
    // Destructor
}

Rectangle Laser::getRect(){
    Rectangle rect;
    
    rect.x = position.x;
    rect.y = position.y;
    rect.width = LASER_WIDTH;
    rect.height = LASER_HEIGHT;

    return rect;
}