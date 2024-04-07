#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        void checkForCollisions();
        int lives;
        void GameOver();
        bool run;
        int score;
        int highScore;
        Music music;
    private:
        Sound gameover;
        Sound explosion;
        Sound shoot;
        Sound hit;
        Sound highscore;
        bool highscorechanged;
        void DeleteInactiveLasers();
        Spaceship spaceship;
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        int AliensDirection;
        std::vector<Laser> alienlasers;
        float lastTimeAlienFired;
        MysteryShip mysteryShip;
        float mysteryShipSpawnTime;
        float lastTimeMysteryShipSpawned;
        void Reset();
        void InitGame();
        void checkForHighScore();
        void saveHighScoreToFile(int highscore);
        int loadHighScoreFromFile();
};