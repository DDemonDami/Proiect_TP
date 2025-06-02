#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "gameinit.h"
#include "positions.h"
#include "computer.h"


void random_ship(PLAYER* enemy, SHIP* temp, int ship_size)
{
   
    int direction = rand() % 4;
  
    //int direction = 3;
    temp->start.i = rand() % 10;
    temp->start.j= rand() % 10;

    //printf("Start i: %d, Start j: %d, Direction: %d\n", temp->start.i, temp->start.j, direction);

    int ship_is_placeable = 1;
            // 1 - daca nava nu poate fi pozitionata si trebe o alta poz initiala; 0 - daca se poate aseza
    int repeat = 0;
    // ii folosit pt a vedea ca nava sa nu intre in bucla infinita 
while(ship_is_placeable == 1)
{
    while(ship_is_placeable == 1 && repeat <= 3)
        {
            //printf("Am intrat in while\n");
            switch(direction)
            {
                case 0:
                {
                    temp->end.j = temp->start.j;
                    temp->end.i = temp->start.i - ship_size + 1;
                   // printf("End i: %d, End j: %d. direction :%d\n", temp->end.i, temp->end.j, direction);

                    if((temp->end.i >= 0 && temp->end.i <= 9) && (check_position(enemy, temp) == ERR_ok))
                    {
                        //place_ship(enemy, temp);
                        //printf("The ship is placed\n");
                        ship_is_placeable = 0;
                    }
                    else
                    {
                        repeat++;
                        direction = (direction + 1) % 4;

                    }
                    //printf("ii ok 2\n");
                    break;
                }
                case 1:
                {
                    temp->end.i = temp->start.i;
                    temp->end.j = temp->start.j + ship_size - 1;

                   /// printf("Interval: %d, Ii ok:%d\n",(temp->end.j >= 0 && temp->end.j <= 9),(check_position(enemy, temp) == ERR_ok));
                    //printf("End i: %d, End j: %d. direction :%d\n", temp->end.i, temp->end.j, direction);

                    //printf("%d\n", (temp->end.j >= 0 && temp->end.j <= 9) && (check_position(enemy, temp) == ERR_ok));
                    if((temp->end.j >= 0 && temp->end.j <= 9) && (check_position(enemy, temp) == ERR_ok))
                    {
                        ship_is_placeable = 0;
                       // place_ship(enemy, temp);
                      //  printf("ii ok 2\n");
                    }
                    else
                    {
                        repeat++;
                        //printf("Trecem la urm directie\n");
                        direction = (direction + 1) % 4;
                    }
                    //printf("ii ok 2\n");
                    break;
                }
                case 2:
                {
                    temp->end.j = temp->start.j;
                    temp->end.i = temp->start.i + ship_size - 1;
                    //printf("End i: %d, End j: %d. direction :%d\n", temp->end.i, temp->end.j, direction);
                    if((temp->end.i >= 0 && temp->end.i <= 9) && (check_position(enemy, temp) == ERR_ok))
                    {
                        ship_is_placeable = 0;
                       // place_ship(enemy, temp);
                       // printf("ii ok 2\n");
                    }
                    else
                    {
                        repeat++;
                       // printf("Trecem la urm directie\n");
                        direction = (direction + 1) % 4;
                    }
                    
                    break;
                }
                case 3:
                {
                    temp->end.i = temp->start.i;
                    temp->end.j = temp->start.j - ship_size + 1; 
                    //printf("End i: %d, End j: %d. direction :%d\n", temp->end.i, temp->end.j, direction);

                    if((temp->end.j >= 0 && temp->end.j <= 9) && (check_position(enemy, temp) == ERR_ok))   
                    {
                        ship_is_placeable = 0;
                        //place_ship(enemy, temp);
                        //printf("ii ok 2\n");
                    }
                    else
                    {
                        repeat++;
                        direction = (direction + 1) % 4;
                    }
                    
                    break;
                }
               
            }
             if(ship_is_placeable == 0)
                {
                    place_ship(enemy,temp,0);
                }
    }
   
    if(ship_is_placeable == 1)
    {
        temp->start.i = rand() % 10;
        temp->start.j = rand() % 10;
        direction = rand() % 4;
        repeat = 0;
    }      

}
//printf("End i: %d, End j: %d\n", temp->end.i, temp->end.j);

}

void random_ships(PLAYER* enemy)
{


    random_ship(enemy, &enemy->player_ships.carrier, 5);
    //show_player(enemy,0);
    random_ship(enemy, &enemy->player_ships.battleship, 4);
    //show_player(enemy,0);
    random_ship(enemy, &enemy->player_ships.cruiser, 3);
    //show_player(enemy,0);
    random_ship(enemy, &enemy->player_ships.submarine, 3);
    //show_player(enemy,0);
    random_ship(enemy, &enemy->player_ships.destroyer, 2);
    //show_player(enemy,0);

}