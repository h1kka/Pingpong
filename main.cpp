#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

class Ball{
public:
float x,y;
int speed_x, speed_y;
int radius; 

void Draw(){
    DrawCircle(x, y, radius, Yellow);
}

void Update(int paddle_x, int paddle_y, int paddle_width, int paddle_height){
    x += speed_x;
    y += speed_y;
 
    if(y + radius >= GetScreenHeight() || y - radius <= 0){
        speed_y *= -1;
    }
    if(x + radius >= GetScreenWidth()){
        player_score++;
        ResetBall();
    }
    if(x - radius <= 0){
        cpu_score++;
        ResetBall();
    }
}

void ResetBall(){
    y = GetScreenHeight() / 2;
    x = GetScreenWidth() / 2;

    int speed_choices[2] = {-1, 1};
    speed_x = speed_choices[GetRandomValue(0,1)] * speed_x;
    speed_y = speed_choices[GetRandomValue(0,1)] * speed_y;
}
};

class Paddle{
protected:

void LimitMovement(){
    if(y <= 0 ){
        y = 0;
    }
    if(y + height >= GetScreenHeight()){
        y = GetScreenHeight() - height;
    }
}

public:
float x,y;
float width, height;
int speed;

void Draw(){
    DrawRectangleRounded(Rectangle{x, y, width, height}, 1, 0, WHITE);
}

void Update(){
    if(IsKeyDown(KEY_W)){
        y = y - speed;
    }

    if(IsKeyDown(KEY_S)){
        y = y + speed;
    }

    LimitMovement();
}
};

class CpuPaddle: public Paddle{
public:

void Update(){
    if(IsKeyDown(KEY_UP)){
        y = y - speed;
    }
    if(IsKeyDown(KEY_DOWN)){
        y = y + speed; 
    }

    LimitMovement();
}
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main(){
    cout << "Hello world, my name is Danil" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Пинг понг");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;
    
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = GetScreenWidth() - cpu.width - 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while(WindowShouldClose() == false){
        BeginDrawing();
        
        //Updating
        ball.Update(player.x, player.y, player.width, player.height);
        player.Update();
        cpu.Update();

        //Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }

        //Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);

        ball.Draw();
        player.Draw();
        cpu.Draw();

        DrawText(TextFormat("%i", player_score), screen_width / 4, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu_score), 3*screen_width / 4, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}