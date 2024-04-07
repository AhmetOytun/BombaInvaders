#include "game.hpp"
#include <iostream>
#include "constants.hpp"
#include <fstream>

Game::Game(){
    music = LoadMusicStream("Sounds/Level1.wav");
    explosion = LoadSound("Sounds/Explosion_03.wav");
    hit = LoadSound("Sounds/Hit_03.wav");
    shoot = LoadSound("Sounds/Shoot_00.wav");
    gameover = LoadSound("Sounds/GameOver.wav");
    highscore = LoadSound("Sounds/NewHiScore.wav");
    InitGame();
}

Game::~Game(){
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosion);
    UnloadSound(hit);
    UnloadSound(shoot);
    UnloadSound(gameover);
    UnloadSound(highscore);
}

void Game::Update(){
    if(run){
        double currentTime = GetTime();
    
        if(currentTime - lastTimeMysteryShipSpawned >= mysteryShipSpawnTime){
            mysteryShip.Spawn();
            lastTimeMysteryShipSpawned = currentTime;
            mysteryShipSpawnTime = MYSTERYSHIP_SPAWN_TIME_DELAY + GetRandomValue(MYSTERYSHIP_MIN_SPAWN_TIME,MYSTERYSHIP_MAX_SPAWN_TIME);
        }

        for(auto& laser: spaceship.lasers){
            laser.Update();
        }

        MoveAliens();

        AlienShootLaser();

        for(auto& laser: alienlasers){
            laser.Update();
        }

        DeleteInactiveLasers();

        mysteryShip.Update();
        checkForCollisions();
    }else{
        if(IsKeyDown(KEY_ENTER)){
            Reset();
            InitGame();
        }
    }
}

void Game::Draw(){
    spaceship.Draw();
    mysteryShip.Draw();

    for(auto& laser: spaceship.lasers){
        laser.Draw();
    }

    for(auto& obstacle: obstacles){
        obstacle.Draw();
    }
    
    for(auto& alien: aliens){
        alien.Draw();
    }

    for(auto& laser: alienlasers){
        laser.Draw();
    }
}

void Game::HandleInput(){
    if(run){
        if(IsKeyDown(KEY_LEFT)){
        spaceship.MoveLeft();
        }
        if(IsKeyDown(KEY_RIGHT)){
            spaceship.MoveRight();
        }
        if(IsKeyPressed(KEY_SPACE)){
            PlaySound(shoot);
            spaceship.FireLaser();
        }
    }
}

void Game::DeleteInactiveLasers(){
    for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();){
        if(!it -> active){
            it = spaceship.lasers.erase(it);
        }else{
            ++ it;
        }
    }

    for(auto it = alienlasers.begin(); it != alienlasers.end();){
        if(!it -> active){
            it = alienlasers.erase(it);
        }else{
            ++ it;
        }
    }
}

std::vector <Obstacle> Game::CreateObstacles(){
    int ObstacleWidth =Obstacle::grid[0].size() * 3; 
    float GapWidth = (GetScreenWidth() - (ObstacleWidth * 4)) / 5;

    for(int i = 0; i<4; i++){
        float offsetX = (i + 1) * GapWidth + (i * ObstacleWidth);
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight()-200)}));
    }

    return obstacles;
}

std::vector<Alien> Game::CreateAliens(){
    std::vector<Alien> aliens;
    int offsetX = (GetScreenWidth() - (ALIEN_COLS * ALIEN_SIZE)) / 2;
    int offsetY = 50;
    for(int row = 0; row<ALIEN_ROWS; row++){
        for(int col = 0; col<ALIEN_COLS; col++){
            int alienType;

            if(row == 0){
                alienType = 3;
            }else if(row >= 1 && row <= 2){
                alienType = 2;
            }else{
                alienType = 1;
            }

            float x = offsetX + col * ALIEN_SIZE;
            float y = offsetY + row * ALIEN_SIZE;
            aliens.push_back(Alien(alienType,{x,y}));
        }
    }

    return aliens;
}
void Game::MoveAliens(){
    for(auto& alien: aliens){
        if(alien.position.x + alien.alienImages[alien.type-1].width > GetScreenWidth()){
            AliensDirection = -1;
            MoveDownAliens(ALIENS_MOVE_DOWN_DISTANCE);
        }else if(alien.position.x < 0){
            AliensDirection = 1;
            MoveDownAliens(ALIENS_MOVE_DOWN_DISTANCE);
        }
        alien.Update(AliensDirection);
    }
}

void Game::MoveDownAliens(int distance){
    for(auto& alien: aliens){
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser(){
    double currentTime = GetTime();
    if(currentTime - lastTimeAlienFired >= ALIEN_SHOOT_INTERVAL && !aliens.empty()){
        int randomIndex = GetRandomValue(0,aliens.size()-1);
        Alien& alien = aliens[randomIndex];
        alienlasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type-1].width/2,alien.position.y + alien.alienImages[alien.type-1].height},-6));
        lastTimeAlienFired = currentTime;
    }
}

void Game::checkForCollisions(){
    for(auto& laser: spaceship.lasers){
        auto it = aliens.begin();
        while(it!= aliens.end()){
            if(CheckCollisionRecs(it->getRect(),laser.getRect())){
                if(it->type == 1){
                    score += 100;
                }else if(it->type == 2){
                    score += 200;
                }else if(it->type == 3){
                    score += 300;
                }
                PlaySound(explosion);
                checkForHighScore();
                it = aliens.erase(it);
                laser.active = false;
            }else{
                ++it;
            }
        }
        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it!= obstacle.blocks.end()){
                if(CheckCollisionRecs(it->getRect(),laser.getRect())){
                    PlaySound(hit);
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }else{
                    ++it;
                }
            }
        }
        if(CheckCollisionRecs(mysteryShip.getRect(),laser.getRect())){
            score += 500;
            PlaySound(explosion);
            checkForHighScore();
            mysteryShip.alive = false;
            laser.active = false;
        }
    }

    for(auto& laser: alienlasers){
        if(CheckCollisionRecs(laser.getRect(),spaceship.getRect())){
            PlaySound(hit);
            laser.active = false;
            lives--;
            if(lives == 0){
                GameOver();
            }
        }
        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it!= obstacle.blocks.end()){
                if(CheckCollisionRecs(it->getRect(),laser.getRect())){
                    it = obstacle.blocks.erase(it);
                    PlaySound(hit);
                    laser.active = false;
                }else{
                    ++it;
                }
            }
        }
    }

    for(auto& alien: aliens){
        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it!= obstacle.blocks.end()){
                if(CheckCollisionRecs(it->getRect(),alien.getRect())){
                    it = obstacle.blocks.erase(it);
                }else{
                    ++it;
                }
            }
        }
        if(CheckCollisionRecs(alien.getRect(),spaceship.getRect())){
            PlaySound(hit);
            lives = 0;
            GameOver();
        }
    }
}

void Game::GameOver(){
    StopMusicStream(music);
    PlaySound(gameover);
    run = false;
}

void Game::InitGame(){
    PlayMusicStream(music);
    score = 0;
    highScore = loadHighScoreFromFile();
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    AliensDirection = 1;
    lastTimeAlienFired = 0.0;
    lastTimeMysteryShipSpawned = GetTime();
    lives = 3;
    mysteryShipSpawnTime = GetRandomValue(MYSTERYSHIP_MIN_SPAWN_TIME,MYSTERYSHIP_MAX_SPAWN_TIME);
    highscorechanged = false;
    run = true;
}

void Game::Reset(){
    StopSound(gameover);
    StopSound(explosion);
    StopSound(hit);
    StopSound(shoot);
    StopSound(highscore);
    spaceship.Reset();
    aliens.clear();
    alienlasers.clear();
    obstacles.clear();
    mysteryShip.alive = false;
}

void Game::checkForHighScore(){
    if(score > highScore){
        if(highscorechanged == false){
            PlaySound(highscore);
            highscorechanged = true;
            std::cout << "New Highscore: " << score << std::endl;
        }
        highScore = score;
        saveHighScoreToFile(highScore);
    }
}

void Game::saveHighScoreToFile(int highscore){
    std::ofstream highscoreFile("highscore.HS");
    if(highscoreFile.is_open()){
        highscoreFile << highscore;
        highscoreFile.close();
    }else{
        std::cout << "Unable to save highscore to file" << std::endl;
    }
}

int Game::loadHighScoreFromFile(){
    std::ifstream highscoreFile("highscore.HS");
    if(highscoreFile.is_open()){
        int highscore = 0;
        highscoreFile >> highscore;
        highscoreFile.close();
        return highscore;
    }else{
        std::cout << "Unable to load highscore from file" << std::endl;
        return 0;
    }
}