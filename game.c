#include <raylib.h>
#include <raymath.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "gameinit.h"
#include "positions.h"
#include "computer.h"

#define LENGTH 1080
#define WIDTH 720

//functie folosita pentru a pune culorile ca sa stim daca am lovit sau nu o nava
void show_square(PLAYER* player, Rectangle hit[10][10])
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < player->size; j++)
            {
                switch(player->table[i][j])
                {
                    case 0:
                    {
                        DrawRectangleRounded(hit[j][i],0.05,0,BLANK);
                        break;
                    }
                    case 2:
                    {
                        DrawRectangleRounded(hit[j][i],0.05,0,ORANGE);
                        break;
                    }
                    case 3:
                    {
                        DrawRectangleRounded(hit[j][i],0.05,0,RED);
                        break;
                    }
                    case 4:
                    {
                        DrawRectangleRounded(hit[j][i],0.05,0,GRAY);
                        break;
                    }

                }
            }
    }
}

void show_player(PLAYER* player, int chk_enemy)
{
    //chk_enemy va lua 1 daca vrem sa afisam inamicul,altfel 0
    if(player->id == 0)
    {
        printf("\n===YOU===\n");
    }
    else
    {
        printf("\n===ENEMY===\n");
    }

    printf("#=========================#\n");
    printf("| # | A B C D E F G H I J |\n");
    printf("#---#---------------------#\n");

    for(int i = 0; i < player->size; i++)
    {
        printf("| %d | ", i);
        for(int j = 0; j < player->size; j++)
            {
                //printf(" ");
                switch(player->table[i][j])
                {
                    case 0:
                    {
                        printf("O");
                        break;
                    }
                    case 1:
                    {
                        if(chk_enemy == 0)
                        {
                            printf("X");
                        }
                        else
                        {
                            printf("O");
                        }
                        break;
                    }
                    case 2:
                    {
                        printf("+");
                        break;
                    }
                    case 3:
                    {
                        printf("#");
                        break;
                    }
                    case 4:
                    {
                        printf("*");
                        break;
                    }

                }
                printf(" ");

            }
            printf("|\n");
    }
    printf("#===#=====================#\n");
}


int main(void)
{
    srand(time(NULL));
    PLAYER player[2];
    init_players(player, 2);
    random_ships(&player[1]);
    //show_player(&player[1], 0);
    POSITION poz;
    int hit = 0;
    POSITION rand_hit;
    POSITION fst_hit;
    int dir = -1;
    int correct_dir = 0;
    int dead_ships_player = 0; // stocam cate nave sunt distruse de la fiecare jucator
    int dead_ships_enemy = 0;
    int clean = 0;
     

    int Playerturn = 0;

    InitWindow(LENGTH,WIDTH,"BattleShips");
    SetTargetFPS(90);
    ShowCursor();

    Vector2 mousePoz = {0,0};

    //dimensiuni butoane si background START
    Rectangle Title = {(LENGTH/60) * 10, (WIDTH/60) * 7, (LENGTH/60) * 40, WIDTH/60 * 10};
    Rectangle but1 = {(LENGTH/60) * 20, (WIDTH/60) * 32, (LENGTH/60) * 20, WIDTH/60 * 8};
    Rectangle but2 = {(LENGTH/60) * 25, (WIDTH/60) * 43, (LENGTH/60) * 10, WIDTH/60 * 5};  
    Rectangle but3 = {(LENGTH/60) * 41, (WIDTH/60) * 43, (LENGTH/60) * 10, WIDTH/60 * 5};  
    Rectangle PlayerTurnBack = {(LENGTH/60) * 41, (WIDTH/60) * 43, (LENGTH/60) * 10, WIDTH/60 * 5};  
    Rectangle PlayerTextBack = {(LENGTH/60) * 24, (WIDTH/60) * 13, (LENGTH/60) * 10, WIDTH/60 * 5};  
    //Rectangle EnemyTextBack = {(LENGTH/60) * 41, (WIDTH/60) * 43, (LENGTH/60) * 10, WIDTH/60 * 5};  
    Rectangle Text1 = {(LENGTH/60) * 40, (WIDTH/60) * 50, (LENGTH/60) * 20, WIDTH/60 * 5};

    Rectangle hitPlayer[10][10];
    Rectangle hitEnemy[10][10];
    float widthHit = 50.4;


    //play arena table size (idk cum le am facut dar is ok spre surprinderea mea)
    Rectangle TablePlayer = {(LENGTH/60), (WIDTH/60) * 16, (LENGTH/60) * 28, (LENGTH/60) * 28};
    Rectangle TableEnemy = {(LENGTH/60) * 31, (WIDTH/60) * 2 , (LENGTH/60) * 28, (LENGTH/60) * 28};

    Rectangle TablePlayer2 = {(LENGTH/60) * 2, (WIDTH/60) * 8, (LENGTH/60) * 28, (LENGTH/60) * 28};
    
    int isPlaced[5];
    for(int i = 0; i< 5;i++)
    {
        isPlaced[i] = 0;
    }

    SHIP ships[5];

    //dimensiuni nave care le vom pune pe tabla
    int xCruiser = (LENGTH/60) * (30 + 29/2 - 3);
    int yCruiser = (WIDTH/60) * 5;
    Vector2 pozCruiser = {xCruiser, yCruiser};
    int lengthCruiser = 3 * 50.4 - 4;
    int widthShip = 50.4 - 4;
    int rotateCruiser = 0;
    Rectangle Cruiser = {xCruiser, yCruiser, lengthCruiser, widthShip};
   
    int xDestroyer = (LENGTH/60) * (30 + 29/2 - 1);
    int yDestroyer = (WIDTH/60) * 12;
    int lengthDestroyer = 2 * 50.4 - 4;
    int rotateDestroyer = 0;
    Rectangle Destroyer = {xDestroyer, yDestroyer, lengthDestroyer, widthShip};
    
    int xSubmarine = (LENGTH/60) * (30 + 29/2 - 3);
    int ySubmarine = (WIDTH/60) * 19;
    int lengthSubmarine = 3 * 50.4 - 4;
    int rotateSubmarine = 0;
    Rectangle Submarine = {xSubmarine, ySubmarine, lengthSubmarine, widthShip};

    int xBattleship = (LENGTH/60) * (30 + 29/2 - 4);
    int yBattleship = (WIDTH/60) * 26;
    int lengthBattleship = 4 * 50.4 - 4;
    int rotateBattleship = 0;
    Rectangle Battleship = {xBattleship, yBattleship, lengthBattleship, widthShip};

    int xCarrier =(LENGTH/60) * (30 + 29/2 - 5);
    int yCarrier = (WIDTH/60) * 33;
    int lengthCarrier = 5 * 50.4 - 4;
    int rotateCarrier = 0;
    Rectangle Carrier = {xCarrier, yCarrier, lengthCarrier, widthShip};
   

    Vector2 mLast = {0,0};

    // culori care le am folosit in examen
    Color TABLE_BACK = {185, 213, 228, 192};
    Color TEXT = {11,44,74,255};
    Color HIT = {222, 216, 91, 255};
    Color TEXT2 = {11,44,74,128};
    
    
    int window = 1; //il folosim ca sa stim care ecran sa il afisam si sa putem naviga intre ecrane
    int closeWindow = 0; // folosit pt a iesi din joc
    int select = 0; // folosit pentru a vedea daca deja avem o nava care o mutam
 
    while(WindowShouldClose() == false && closeWindow == 0)
    {
        //preluam pozitia mouse-ului la fiecare iteratie de bucla
        mousePoz = GetMousePosition();

        //play arena
         BeginDrawing();
            ClearBackground(RAYWHITE);

           switch(window)
           {
                case 1:
                {
                    DrawRectangleRounded(Title,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("BATTLESHIPS"),(LENGTH/60) * 14 - 6, (WIDTH/60) * 9, 80, TEXT);
                    DrawRectangleRounded(but1,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("START"),(LENGTH/60) * 24 + 2, (WIDTH/60) * 34 - 5, 60, TEXT);
                    DrawRectangleRounded(but2,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("EXIT"),(LENGTH/60) * 27 + 5, (WIDTH/60) * 44, 40, TEXT);

                    // va urma aici ca sa verific fiecare buton 

                    if(CheckCollisionPointRec(mousePoz, but1))
                    {
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                            window = 2;
                    }

                     if(CheckCollisionPointRec(mousePoz, but2))
                    {
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                            closeWindow = 1;
                    }

                    break;
                }
                case 2:
                {   
                    int collision = 0;
                    // in urmatoare sectiune aflam pe care nava am apasat ca sa o putem muta
                    if(CheckCollisionPointRec(mousePoz, Cruiser) && collision == 0 && select == 0)
                    {
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            select = 1;
                            collision = 1;
                        }
                    }


                     if(CheckCollisionPointRec(mousePoz, Destroyer) && collision == 0)
                    {
                        //printf("Sunt aici/n");
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            select = 2;
                            collision = 1;
                        }
                    }

                    
                     if(CheckCollisionPointRec(mousePoz, Submarine) && collision == 0)
                    {
                        //printf("Sunt aici/n");
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            select = 3; 
                            collision = 1;  
                        }
                    }

                     if(CheckCollisionPointRec(mousePoz, Battleship) && collision == 0)
                    {
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            select = 4;
                            collision = 1;
                        }
                    }

                     if(CheckCollisionPointRec(mousePoz, Carrier) && collision == 0)
                    {
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            select = 5;
                            collision = 1;
                        }
                    }

                    //aici mutam nava, o rotim si preluam pozitia ei daca se afla pe tabla de joc
                    switch(select)
                    {
                        case 1:
                        {

                            if(IsKeyPressed(KEY_R) && rotateCruiser == 0)
                            {
                                //Rectangle Cruiser = {xCruiser, yCruiser, lengthCruiser, widthShip};
                                Cruiser.width = widthShip;
                                Cruiser.height = lengthCruiser;
                                rotateCruiser = 1;
                            }
                            else if(IsKeyPressed(KEY_R) && rotateCruiser == 1)
                            {
                                Cruiser.width = lengthCruiser;
                                Cruiser.height = widthShip;
                                rotateCruiser = 0;
                            }   
                            Cruiser.x = Cruiser.x + (mousePoz.x - mLast.x);
                            Cruiser.y = Cruiser.y + (mousePoz.y - mLast.y);


                            if(CheckCollisionRecs(TablePlayer2, Cruiser))
                            {
                                
                                ships[select-1].start.i = (Cruiser.y - (WIDTH/60) * 8) / 50.4;
                                ships[select-1].start.j = (Cruiser.x - (LENGTH/60) * 2) / 50.4;
                                if(rotateCruiser == 0)
                                {
                                    // pe orizontala
                                    ships[select-1].end.i = ships[select-1].start.i;
                                    ships[select-1].end.j = ships[select-1].start.j + 2;
                                }
                                else
                                {
                                    //pe verticala
                                    ships[select-1].end.i = ships[select-1].start.i + 2;
                                    ships[select-1].end.j = ships[select-1].start.j;
                                }

                                if(check_position(&player[0], &ships[select - 1]) == ERR_ok)
                                {
                                    place_ship(&player[0], &ships[select - 1], 0);
                                    isPlaced[select - 1] = 1;
                                    
                                }
                               
                            }
                            printf("Cruiser Start I:%d J:%d\n",  ships[select-1].start.i, ships[select-1].start.j);
                            printf("Cruiser END I:%d J:%d\n",  ships[select-1].end.i, ships[select-1].end.j);

                           
                            break;
                        }

                        case 2:
                        {
                            if(IsKeyPressed(KEY_R) && rotateDestroyer == 0)
                            {
                                //Rectangle Cruiser = {xCruiser, yCruiser, lengthCruiser, widthShip};
                                Destroyer.width = widthShip;
                                Destroyer.height = lengthDestroyer;
                                rotateDestroyer = 1;
                            }
                            else if(IsKeyPressed(KEY_R) && rotateDestroyer == 1)
                            {
                                Destroyer.width = lengthDestroyer;
                                Destroyer.height = widthShip;
                                rotateDestroyer = 0;
                            }
                            Destroyer.x = Destroyer.x + (mousePoz.x - mLast.x);
                            Destroyer.y = Destroyer.y + (mousePoz.y - mLast.y);

                             if(CheckCollisionRecs(TablePlayer2, Destroyer))
                            {
                                ships[select-1].start.i = (Destroyer.y - (WIDTH/60) * 8) / 50.4;
                                ships[select-1].start.j = (Destroyer.x - (LENGTH/60) * 2) / 50.4;
                                if(rotateDestroyer == 0)
                                {
                                    // pe orizontala
                                    ships[select-1].end.i = ships[select-1].start.i;
                                    ships[select-1].end.j = ships[select-1].start.j + 1;
                                }
                                else
                                {
                                    //pe verticala
                                    ships[select-1].end.i = ships[select-1].start.i + 1;
                                    ships[select-1].end.j = ships[select-1].start.j;
                                }

                                 if(check_position(&player[0], &ships[select - 1]) == ERR_ok)
                                {
                                    place_ship(&player[0], &ships[select - 1], 0);
                                    isPlaced[select - 1] = 1;
                                }
                            }
                           
                            printf("Destroyer I:%d J:%d\n", ships[select-1].start.i, ships[select-1].start.j);
                            break;
                        }
                        case 3:
                        {
                            if(IsKeyPressed(KEY_R) && rotateSubmarine == 0)
                            {
                                //Rectangle Cruiser = {xCruiser, yCruiser, lengthCruiser, widthShip};
                                Submarine.width = widthShip;
                                Submarine.height = lengthSubmarine;
                                rotateSubmarine = 1;
                            }
                            else if(IsKeyPressed(KEY_R) && rotateSubmarine == 1)
                            {
                                Submarine.width = lengthSubmarine;
                                Submarine.height = widthShip;
                                rotateSubmarine = 0;
                            }
                            
                            Submarine.x = Submarine.x + (mousePoz.x - mLast.x);
                            Submarine.y = Submarine.y + (mousePoz.y - mLast.y);

                            if(CheckCollisionRecs(TablePlayer2, Submarine))
                            {
                                ships[select-1].start.i = (Submarine.y - (WIDTH/60) * 8) / 50.4;
                                ships[select-1].start.j = (Submarine.x - (LENGTH/60) * 2) / 50.4;
                                if(rotateSubmarine == 0)
                                {
                                    // pe orizontala
                                    ships[select-1].end.i = ships[select-1].start.i;
                                    ships[select-1].end.j = ships[select-1].start.j + 2;
                                }
                                else
                                {
                                    //pe verticala
                                    ships[select-1].end.i = ships[select-1].start.i + 2;
                                    ships[select-1].end.j = ships[select-1].start.j;
                                }

                                if(check_position(&player[0], &ships[select - 1]) == ERR_ok)
                                {
                                    place_ship(&player[0], &ships[select - 1], 0);
                                    isPlaced[select - 1] = 1;
                                }
                            }
                            
                            printf("Submarine I:%d J:%d\n", ships[select-1].start.i, ships[select-1].start.j);
                            break;
                        }

                        case 4:
                        {   
                             if(IsKeyPressed(KEY_R) && rotateBattleship == 0)
                            {
                                //Rectangle Cruiser = {xCruiser, yCruiser, lengthCruiser, widthShip};
                                Battleship.width = widthShip;
                                Battleship.height = lengthBattleship;
                                rotateBattleship = 1;
                            }
                            else if(IsKeyPressed(KEY_R) && rotateBattleship == 1)
                            {
                                Battleship.width = lengthBattleship;
                                Battleship.height = widthShip;
                                rotateBattleship = 0;
                            }
                            
                            Battleship.x = Battleship.x + (mousePoz.x - mLast.x);
                            Battleship.y = Battleship.y + (mousePoz.y - mLast.y);

                            if(CheckCollisionRecs(TablePlayer2, Battleship))
                            {
                                ships[select-1].start.i = (Battleship.y - (WIDTH/60) * 8) / 50.4;
                                ships[select-1].start.j = (Battleship.x - (LENGTH/60) * 2) / 50.4;
                                if(rotateBattleship == 0)
                                {
                                    // pe orizontala
                                    ships[select-1].end.i = ships[select-1].start.i;
                                    ships[select-1].end.j = ships[select-1].start.j + 3;
                                }
                                else
                                {
                                    //pe verticala
                                    ships[select-1].end.i = ships[select-1].start.i + 3;
                                    ships[select-1].end.j = ships[select-1].start.j;
                                }

                                 if(check_position(&player[0], &ships[select - 1]) == ERR_ok)
                                {
                                    place_ship(&player[0], &ships[select - 1], 0);
                                    isPlaced[select - 1] = 1;
                                }
                            }
                          
                            printf("battleship I:%d J:%d\n",  ships[select-1].start.i,  ships[select-1].start.j);
                            break;
                        }
                        case 5:
                        {
                            if(IsKeyPressed(KEY_R) && rotateCarrier == 0)
                            {
                                //Rectangle Cruiser = {xCruiser, yCruiser, lengthCruiser, widthShip};
                                Carrier.width = widthShip;
                                Carrier.height = lengthCarrier;
                                rotateCarrier = 1;
                            }
                            else if(IsKeyPressed(KEY_R) && rotateCarrier == 1)
                            {
                                Carrier.width = lengthCarrier;
                                Carrier.height = widthShip;
                                rotateCarrier = 0;
                            }

                            Carrier.x = Carrier.x + (mousePoz.x - mLast.x);
                            Carrier.y = Carrier.y + (mousePoz.y - mLast.y);

                             if(CheckCollisionRecs(TablePlayer2, Carrier))
                            {
                                ships[select-1].start.i = (Carrier.y - (WIDTH/60) * 8) / 50.4;
                                ships[select-1].start.j = (Carrier.x - (LENGTH/60) * 2) / 50.4;
                                if(rotateCarrier == 0)
                                {
                                    // pe orizontala
                                    ships[select-1].end.i = ships[select-1].start.i;
                                    ships[select-1].end.j = ships[select-1].start.j + 4;
                                }
                                else
                                {
                                    //pe verticala
                                    ships[select-1].end.i = ships[select-1].start.i + 4;
                                    ships[select-1].end.j = ships[select-1].start.j;
                                }
                                 if(check_position(&player[0], &ships[select - 1]) == ERR_ok)
                                {
                                    place_ship(&player[0], &ships[select - 1], 0);
                                    isPlaced[select - 1] = 1;
                                }
                            }

                            
                            printf("Carrier I:%d J:%d\n", ships[select-1].start.i,   ships[select-1].start.j);
                            break;
                        }
                    }

                    //verificam ca toate navele se afla pe tabla de joc ca sa putem trece pe urmatoarea pagina
                    int check = 0;
                    for(int i = 0; i< 5; i++)
                    {
                        if(isPlaced[i] == 0)
                        check = 1;
                    }

                    if(check == 0)
                    {
                        //buton de trecut pe pagina urmatoare
                        DrawRectangleRounded(but3,0.05,0,TABLE_BACK);
                        DrawText(TextFormat("NEXT"),(LENGTH/60) * 43 - 1, (WIDTH/60) * 44, 40, TEXT);

                        if(CheckCollisionPointRec(mousePoz, but3))
                        {
                            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                            {
                                window = 3;
                            }
                        }

                    }

                    select = 0;
                    //Rectangle Text1 = {(LENGTH/60) * 40, (WIDTH/60) * 3, (LENGTH/60) * 15, WIDTH/60 * 5};

                    //desenat tabla unde vom pune navele
                    DrawRectangleRounded(TablePlayer2,0.05,0,TABLE_BACK);
                    DrawRectangleRounded(Text1,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("[R] to rotate"),(LENGTH/60) * 41 - 1, (WIDTH/60) * 51, 40, TEXT);
                    //liniile tablei de joc
                    for(int i = 1; i <= 9; i++)
                    {   
                        DrawLine((LENGTH/60) * 2 + i* 50.4 - 1 ,(WIDTH/60) * 8, (LENGTH/60) * 2 + i * 50.4 - 1, (LENGTH/60) * 28 + (WIDTH/60) * 8 ,TEXT);
                        DrawLine((LENGTH/60) * 2 + i* 50.4, (WIDTH/60) * 8, (LENGTH/60) * 2 + i * 50.4, (LENGTH/60) * 28 +(WIDTH/60) * 8 ,TEXT);
                        DrawLine((LENGTH/60) * 2 + i* 50.4 + 1, (WIDTH/60) * 8, (LENGTH/60) * 2 + i * 50.4 + 1, (LENGTH/60) * 28 +(WIDTH/60) * 8 ,TEXT);
                        
                        DrawLine((LENGTH/60) * 2, (WIDTH/60) * 8 - 1 + i* 50.4, (LENGTH/60) * 30,(WIDTH/60) * 8 + i*50.4 - 1, TEXT);
                        DrawLine((LENGTH/60) * 2, (WIDTH/60) * 8 + i* 50.4, LENGTH/60 * 30, (WIDTH/60) * 8 + i*50.4, TEXT);
                        DrawLine((LENGTH/60) * 2, (WIDTH/60) * 8 + 1 + i* 50.4, LENGTH/60 * 30, (WIDTH/60) * 8 + i*50.4 + 1, TEXT);

                    }

                   //desenat navele
                   
                    DrawRectangleRounded(Cruiser,0.55,0,TEXT);
                    DrawRectangleRounded(Destroyer,0.55,0,TEXT);
                    DrawRectangleRounded(Submarine,0.55,0,TEXT);
                    DrawRectangleRounded(Battleship,0.55,0,TEXT);
                    DrawRectangleRounded(Carrier,0.55,0,TEXT);

                    break;
                }

                case 3:
                {
                    //aici stergem orice nave care au fost amplasate gresit pe tabla de joc cum le am tras pe tabla
                    if(clean == 0)
                    {
                        for(int i = 0; i < 10; i++)
                        {
                            for(int j = 0; j< 10; j++)
                            {
                                player[0].table[i][j] = 0;
                            }
                        }
                        for(int cv = 0; cv < 5; cv++)
                        {
                            place_ship(&player[0], &ships[cv], 0);
                        }
                        clean = 1;
                        show_player(&player[0], 0);
                    }
                    
                    // amplasam navele deja aranjate in matricea din dreapta
                    for(int i = 0; i < 5; i++)
                    {
                        switch(i)
                        {   
                            case 0:
                            {
                                Cruiser.x = LENGTH/60 * 31  + ships[i].start.j * 50.4;
                                Cruiser.y =(WIDTH/60) * 2  + ships[i].start.i * 50.4;
                                break;
                            } 
                            case 1:
                            {

                                Destroyer.x = LENGTH/60 * 31  + ships[i].start.j * 50.4;
                                Destroyer.y = (WIDTH/60) * 2  + ships[i].start.i * 50.4;
                                break;
                            }
                            case 2:
                            {
                                Submarine.x = LENGTH/60 * 31  + ships[i].start.j * 50.4;
                                Submarine.y = (WIDTH/60) * 2  + ships[i].start.i * 50.4;
                                break;
                            }
                            case 3:
                            {
                                Battleship.x = LENGTH/60 * 31  + ships[i].start.j * 50.4;
                                Battleship.y = (WIDTH/60) * 2  + ships[i].start.i * 50.4;
                                break;
                            }
                             case 4:
                            {
                                Carrier.x = LENGTH/60 * 31  + ships[i].start.j * 50.4;
                                Carrier.y = (WIDTH/60) * 2  + ships[i].start.i * 50.4;
                                break;
                            }
                        }

                    }
                    //partea de joc efectiv
                    // player's turn
                    if(Playerturn == 0)
                    {
                    if(CheckCollisionPointRec(mousePoz, TablePlayer))
                    {
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        {
                            int xMouse = (mousePoz.x - (LENGTH/60)) / 50.4;
                            int yMouse = (mousePoz.y - (WIDTH/60) * 16) / 50.4;

                            //printf("X MOUSE:%d Y MOUSE: %d\n", xMouse, yMouse);

                            hitPlayer[xMouse][yMouse].x = (LENGTH/60) + xMouse * 50.4;
                            hitPlayer[xMouse][yMouse].y = (WIDTH/60) * 16 + yMouse * 50.4;
                            hitPlayer[xMouse][yMouse].width = widthHit;
                            hitPlayer[xMouse][yMouse].height = widthHit;

                            if(player[1].table[yMouse][xMouse] == 1)
                            {
                               // printf("Am ajuns aici\n");
                                player[1].table[yMouse][xMouse] = 2;
                                Playerturn = 1;
                                //DrawRectangleRounded(hitPlayer[xMouse][yMouse],0.05,0,HIT);
                            }
                            else if(player[1].table[yMouse][xMouse] == 0)
                            {
                                // am ratat si vom 
                                player[1].table[yMouse][xMouse] = 4;
                                Playerturn = 1;
                                //DrawRectangleRounded(hitPlayer[xMouse][yMouse],0.05,0,GRAY);
                            }
                        }
                    }
                }
                else
                {//calcularea pozitiei care o va lovi calculatorul
                    POSITION poz;
                    poz.i = rand() % 10;
                    poz.j = rand() % 10;
                    
                    if(hit == 0)
                    {
                        while(player[0].table[poz.i][poz.j] != 1 && player[0].table[poz.i][poz.j] != 0)
                        {
                            poz.i = rand() % 10;
                            poz.j = rand() % 10;
                        }
                    }
                    else
                    {
                        int counter = 0;
                        // printf("\nAm intrat in ramura de calc a poz urm\n\n");
                        do{
                            //printf("Suntem in do while\n\n");
                            if(correct_dir == 1)
                            {
                                dir = dir;
                            }
                            else if(dir == -1)
                            {
                                dir = rand() % 4;
                            }

                            if(counter > 0)
                            {
                               dir = (dir + 1) % 4;
                            }

                            switch(dir)
                            {
                                case 0:
                                {
                                    poz.j = rand_hit.j;
                                    poz.i = rand_hit.i + 1;
                                    break;
                                }
                                case 1:
                                {
                                    poz.i = rand_hit.i;
                                    poz.j = rand_hit.j - 1;
                                    break;
                                }
                                case 2:
                                {
                                    poz.j = rand_hit.j;
                                    poz.i = rand_hit.i - 1;
                                    break;
                                }
                                case 3:
                                {
                                    poz.i = rand_hit.i;
                                    poz.j = rand_hit.j + 1;
                                    break;
                                }
                            }
                            counter ++;
                        }while((poz.i < 0 || poz.i >= 10) || (poz.j < 0 || poz.j >= 10));
                    }

                
                    if(player[0].table[poz.i][poz.j] == 0)
                    {
                        player[0].table[poz.i][poz.j] = 4;
                        //printf("--The enemy missed your fleet! Keep going.--\n");
                        if(dir != -1)
                        {
                            dir = (dir + 1) % 4;
                            rand_hit = fst_hit;
                        }
                    }

                    else if(player[0].table[poz.i][poz.j] == 1)
                    {
                        player[0].table[poz.i][poz.j] = 2;
                        // printf("--The enemy hit one of your ships! Keep going.--\n");
                        hit = 1;
                        rand_hit = poz;
                        if(dir != -1)
                        {
                            correct_dir = 1;
                        }
                        else
                        {
                            fst_hit = poz;
                            rand_hit = fst_hit;
                        }
                    }
                    else if(player[0].table[poz.i][poz.j] == 2 || player[0].table[poz.i][poz.j] == 4)
                    {
                        dir = (dir + 1) % 4; 
                        rand_hit = fst_hit;
                    }
                    
                    printf("RANDOM I: %d J: %d\n", poz.i, poz.j);
                    printf("DIRECTION:%d HIT:%d CORRECT DIR:%d\n", dir, hit, correct_dir);
                    hitEnemy[poz.j][poz.i].x = (LENGTH/60) * 31 + poz.j * 50.4;
                    hitEnemy[poz.j][poz.i].y = (WIDTH/60) * 2 + poz.i * 50.4;
                    hitEnemy[poz.j][poz.i].width = widthHit;
                    hitEnemy[poz.j][poz.i].height = widthHit;
                    Playerturn = 0;
                   
                }

                int cnt = 0;
                for(int i = 0; i< 5; i++)
                {
                    if(chk_dead_ship(&player[0], &ships[i]) == DEAD_Ship)
                    {
                        cnt = 1;
                    }
                }
                if(cnt == 1)
                {
                    dead_ships_player++;
                    correct_dir = 0;
                    dir = -1;
                    hit = 0;
                }

       
                if(chk_dead_ships(&player[1]) == 1)
                {
                    dead_ships_enemy++;

                }

                DrawRectangleRounded(TablePlayer,0.05,0,TABLE_BACK);
                DrawRectangleRounded(TableEnemy,0.05,0,TABLE_BACK);
                show_square(&player[1], hitPlayer);
                show_square(&player[0], hitEnemy);

                for(int i = 1; i <= 9; i++)
                {   
                    DrawLine(LENGTH/60 + i* 50.4 - 1 , (WIDTH/60) * 16, LENGTH/60 + i * 50.4 - 1, (LENGTH/60) * 28 +(WIDTH/60) * 16 ,TEXT);
                    DrawLine(LENGTH/60 + i* 50.4, (WIDTH/60) * 16, LENGTH/60 + i * 50.4, (LENGTH/60) * 28 +(WIDTH/60) * 16 ,TEXT);
                    DrawLine(LENGTH/60 + i* 50.4 + 1, (WIDTH/60) * 16, LENGTH/60 + i * 50.4 + 1, (LENGTH/60) * 28 +(WIDTH/60) * 16 ,TEXT);
                    
                    DrawLine((LENGTH/60), (WIDTH/60) * 16 - 1 + i* 50.4, LENGTH/60 * 29, WIDTH/60 * 16 + i*50.4 - 1, TEXT);
                    DrawLine((LENGTH/60), (WIDTH/60) * 16 + i* 50.4, LENGTH/60 * 29, WIDTH/60 * 16 + i*50.4, TEXT);
                    DrawLine((LENGTH/60), (WIDTH/60) * 16 + 1 + i* 50.4, LENGTH/60 * 29, WIDTH/60 * 16 + i*50.4 + 1, TEXT);

                    DrawLine((LENGTH/60) * 31 + 1 + i* 50.4, (WIDTH/60) * 2, LENGTH/60 * 31 + 1 + i * 50.4, (LENGTH/60) * 28 +(WIDTH/60) * 2 ,TEXT);
                    DrawLine((LENGTH/60) * 31 + i* 50.4, (WIDTH/60) * 2, LENGTH/60 * 31  + i * 50.4, (LENGTH/60) * 28 +(WIDTH/60) * 2 ,TEXT);
                    DrawLine((LENGTH/60) * 31 - 1 + i* 50.4, (WIDTH/60) * 2, LENGTH/60 * 31 -1 + i * 50.4, (LENGTH/60) * 28 +(WIDTH/60) * 2 ,TEXT);

                    DrawLine((LENGTH/60) * 31, (WIDTH/60) * 2 + i* 50.4 - 1, LENGTH/60 * 59, WIDTH/60 * 2 + i*50.4 - 1, TEXT);
                    DrawLine((LENGTH/60) * 31, (WIDTH/60) * 2 + i* 50.4, LENGTH/60 * 59, WIDTH/60 * 2 + i*50.4, TEXT);
                    DrawLine((LENGTH/60) * 31, (WIDTH/60) * 2 + i* 50.4 + 1, LENGTH/60 * 59, WIDTH/60 * 2 + i*50.4 + 1, TEXT);
                }

                DrawRectangleRounded(Cruiser,0.55,0,TEXT2);
                DrawRectangleRounded(Destroyer,0.55,0,TEXT2);
                DrawRectangleRounded(Carrier,0.55,0,TEXT2);
                DrawRectangleRounded(Battleship,0.55,0,TEXT2);
                DrawRectangleRounded(Submarine,0.55,0,TEXT2);



                if(dead_ships_enemy == 5)
                {
                    dead_ships_player = 0;
                    DrawRectangleRounded(Title,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("YOU WON!"),(LENGTH/60) * 19 - 6, (WIDTH/60) * 9, 80, TEXT);
                    DrawRectangleRounded(but2,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("EXIT"),(LENGTH/60) * 27 + 5, (WIDTH/60) * 44, 40, TEXT);

                    if(CheckCollisionPointRec(mousePoz, but2))
                    {
                        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                            closeWindow = 1;
                    }
                }
                else if(dead_ships_player == 5)
                {
                    dead_ships_enemy = 0;
                    DrawRectangleRounded(Title,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("YOU LOST!"),(LENGTH/60) * 19 - 6, (WIDTH/60) * 9, 80, TEXT);
                    DrawRectangleRounded(but2,0.05,0,TABLE_BACK);
                    DrawText(TextFormat("EXIT"),(LENGTH/60) * 27 + 5, (WIDTH/60) * 44, 40, TEXT);
                    
                    if(CheckCollisionPointRec(mousePoz, but2))
                        {
                            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                                closeWindow = 1;
                        }
                }

                    
                break;
                }

            }
        mLast = mousePoz;

        EndDrawing();
    }
    delete_players(player, 2);
    CloseWindow();
    return 0;
}