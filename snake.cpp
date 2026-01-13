#include <raylib.h>
#include<iostream>
#include<cmath>
#include<deque>
#include<raymath.h>
using namespace std;
int cellsize=30;
int cellcount=25;
Color green={173,204,96,255};
    Color darkGreen={43,51,24,255};
Color lightPink={255,182,192,255};
int offset=75;
double lastUpdateTime=0;//used to keep track of time at which last update of the snake occured

bool ElementInDeque(Vector2 element,const deque<Vector2>& dq){
    for(unsigned int i=0;i<dq.size();i++){
        if(Vector2Equals(dq[i],element)){//element exists in deque
        return true;}
    }
    return false;
}

bool eventTriggered(double interval){//interval is the time gap ex:snake moves every 0.2 secondsthats 5moves per second so it tells how often something is allowed to happen
    double currentTime=GetTime();//seconds passed since the game started 
    if(currentTime-lastUpdateTime>=interval){//it checks has the at least interval seconds are passed since last time?
        lastUpdateTime=currentTime;//updates the time 
        return true;//yes you can do event  now
    }
    return false;//no you have to wait
}
//create snake
class Snake{
    public:
    deque<Vector2> body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 direction={1,0};
    bool addSegment=false;
    
    void Draw(){
        for(int i=0;i<body.size();i++){
            int x=body[i].x;
            int y=body[i].y;
            Rectangle segment = Rectangle{
                offset + (float)x * cellsize,
                offset + (float)y * cellsize,
                (float)cellsize,
                (float)cellsize
            };
            
            DrawRectangleRounded(segment,0.2f,6,darkGreen);

            // DrawRectangle(x*cellsize,y*cellsize,cellsize,cellsize,darkGreen);
        }

    }
   
    void Update(){
        body.push_front(Vector2Add(body[0],direction));
        if(addSegment==true){
             

            addSegment=false;
        }else{
            body.pop_back();
        }

      //new coordinates for snakes
    }
    void Reset(){
        body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        direction={1,0};
    }

};
class Food{
    public:
    Vector2 position;
    void Draw(){
        int cx=offset+position.x*cellsize+cellsize/2;
        int cy=offset+position.y*cellsize+cellsize/2;
        float time=GetTime();
        float pulse=sin(time*3)*3;
       
        //pink petals
        DrawCircle(cx,cy-10-pulse,6,PINK);//top
        DrawCircle(cx,cy+10+pulse,6,PINK);//bottom
        DrawCircle(cx-10-pulse,cy,6,PINK);//left

        DrawCircle(cx+10+pulse,cy,6,PINK);//right
        //center(light pink)
        DrawCircle(cx,cy,6,lightPink);
        




        


    }
    
    Vector2 GenerateRandomCell(){
        float x=GetRandomValue(0,cellcount-1);
        float y=GetRandomValue(0,cellcount-1);
        return Vector2{(float)x,(float)y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody){//in parenthesis we need to check the position of  food exists in the  cells
     //it make sure that food image is not generated is not in the snake body
        Vector2 position=GenerateRandomCell();
        while(ElementInDeque(position,snakeBody)){
            position=GenerateRandomCell();

        }
        return position;
        

    }


};
class Game{

    public:
        Snake snake =Snake();
        Food food;
        bool running=true;
        int score=0;
        Game(){
            food.position=food.GenerateRandomPos(snake.body);
        }
        void Draw(){
            food.Draw();
            snake.Draw();
        }
        void Update(){
            if(running){

            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
        CheckCollisionWithTail();}
        }
        void CheckCollisionWithFood(){
            if(Vector2Equals(snake.body[0],food.position)){
                food.position=food.GenerateRandomPos(snake.body);
                snake.addSegment=true;
                score++;
            }

        }
        void CheckCollisionWithEdges(){
            if(snake.body[0].x<0|| snake.body[0].x>=cellcount){
               GameOver(); 
            }
            if(snake.body[0].y>=cellcount|| snake.body[0].y<0){
                GameOver();
            }
        }
        void GameOver(){
            snake.Reset();
            food.position=food.GenerateRandomPos(snake.body);
            score=0;
            running=false;
        }
        void CheckCollisionWithTail(){
            deque<Vector2> headlessBody=snake.body;
            headlessBody.pop_front();
            if(ElementInDeque(snake.body[0],headlessBody)){
                GameOver();
            }
        }
      

};


int main()
{
    cout<<"starting the game.."<<endl;
    Game game=Game();
    

    InitWindow(2*offset+cellsize*cellcount,2*cellsize*cellcount,"Retro Snake");//to create window we use Initwindow() it takes three arguments  width,heightand window
    SetTargetFPS(60);//how fast your game loop runs so in this code 60 frames per second this line shold be written
    // Food food=Food();
    // Snake snake=Snake();
    // food.position=food.GenerateRandomPos();
    
    while(WindowShouldClose()==false){
        BeginDrawing();

       if(eventTriggered(0.2)){
        game.Update();
       }
       if(IsKeyPressed(KEY_UP)&&game.snake.direction.y!=1){//to check key up is pressed
        game.snake.direction={0,-1};//we decrease the y coordinate
       }
       if(IsKeyPressed(KEY_DOWN)&&game.snake.direction.y!=-1){
        game.snake.direction={0,1};
       }
       if(IsKeyPressed(KEY_LEFT)&&game.snake.direction.x!=1){
        game.snake.direction={-1,0};

       }
       if(IsKeyPressed(KEY_RIGHT)&& game.snake.direction.x!=-1){
        game.snake.direction={1,0};
       }

       if(!game.running&&IsKeyPressed(KEY_SPACE)){
        game.running=true;
        lastUpdateTime=GetTime();
       }

        //background  
        ClearBackground(green); 
        DrawRectangleLinesEx(Rectangle{(float)offset-5,(float)offset-5,(float)cellsize*cellcount+10,(float)cellsize*cellcount+10},5,darkGreen);
        DrawText("Retro Snake",offset-5,20,40,darkGreen);
        DrawText(TextFormat("%i",game.score),offset-5,offset+cellsize*cellcount+10,40,darkGreen);
        game.Draw();

        if(!game.running){
            DrawText("Game Over",260,350,40,RED);
            DrawText("Press SPACE to restart",210,400,20,DARKGREEN);
        }






        



        EndDrawing();
    }

   CloseWindow(); 
    return 0;
}
