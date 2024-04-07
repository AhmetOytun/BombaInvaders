#include "constants.hpp"
#include <raylib.h>
#include "game.hpp"
#include "string.h"

std::string FormatWithLeadingZeros(int number, int width){
    std::string numberText = std::to_string(number);
    int zeros = width - numberText.length();
    return (std::string(zeros, '0') + numberText);
}

int main(){
    InitWindow(WINDOW_WIDTH + UI_OFFSET, WINDOW_HEIGHT + 2 * UI_OFFSET, TITLE);
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf",64,0,0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    SetTargetFPS(FPS);

    Game game;

    while(!WindowShouldClose()){
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(GREY);
        DrawRectangleRoundedLines({3,3,794,794},0.18f,20,2,YELLOW);
        DrawLineEx({20,730},{781,730},3,YELLOW);

        if(game.run){
            DrawTextEx(font,"LEVEL 01",{570,740},34,2,YELLOW);
        }else{
            DrawTextEx(font,"GAME OVER",{570,740},34,2,YELLOW);
        }

        float x = 50.0;

        for(int i = 0; i<game.lives; i++){
            DrawTextureV(spaceshipImage,{x,745},WHITE);
            x += 50;
        }

        DrawTextEx(font,"SCORE ",{300,740},34,2,YELLOW);
        std::string scoreText = FormatWithLeadingZeros(game.score,5);
        DrawTextEx(font,scoreText.c_str(),{400,740},34,2,YELLOW);

        std::string highScoreText = FormatWithLeadingZeros(game.highScore,5);
        DrawTextEx(font,"HI-SCORE ",{40,10},34,2,YELLOW);
        DrawTextEx(font,highScoreText.c_str(),{200,10},34,2,YELLOW);


        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
    return 0;
}