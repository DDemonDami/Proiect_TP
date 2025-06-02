#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "gameinit.h"
#include "positions.h"
#include "computer.h"


void get_position(int* poz_i, int* poz_j)
{
   // printf("\n\nam intrat aici\n\n");
    clear_buffer();
    char poz[4];
    char c; 
    int temp;

    //fgets(poz,4,stdin);
    int t = 1; //1 - nu a citit nimic; 2 - a citit eronat; 3 - eroare convertire; 0 - a citit si convertit corect
    // int ok = 1; //folosit doar pentru a nu afisa mesajul de eroare prima data
    while(t != 0)
    {//format pozitie: A4
        //clear_buffer();
        //sterge toate caracterele care au ramas in buffer nefolosite pentu a nu da eroare la inceput
        //getchar();
        if(fgets(poz,3,stdin) == NULL)
        {
            printf("|%s|",poz);
            printf("--There was an error when reading your position. Please try again--\n");
            t = 2;
        }
        else if((sscanf(poz,"%c%d", &c, &temp) != 2))
        {
            
            printf("|%s|\n",poz);

            printf("--Wrong position format. Try again--\n");
            t = 2;
        }
        if(islower(c))
        {
            c = toupper(c);
        }

        if((c < 'A' || c > 'J'))
            {
                printf("--The letter that you selected is not valid. Please try again--\n");
                t = 3;
            }   
                else if(temp < 0 || temp > 9)
                {
                    printf("--The number you selected is not valid. Please try again--\n");
                    t = 3;
                }
                else
                {
                    t = 0;
                }   
    }
    
    *poz_j = (int)(c - 'A');
    *poz_i = temp;
    //ok = 0;

}


ERR_CODE check_position(PLAYER* player, SHIP* position)
{
    if(position->start.i == position->end.i)
    {
        if(position->start.j < position->end.j)
        {
            for(int j = position->start.j; j <= position->end.j;j++)
            {
                if(player->table[position->start.i][j] != 0)
                //printf("%d ", j);
                    return POZ_tken;
            }
        }
        else
        {
            
            for(int j = position->start.j; j >= position->end.j; j--)
            {
                if(player->table[position->start.i][j] != 0)
                    return POZ_tken;
            }
            
        }
    }
    else
    {
        
        if(position->start.i < position->end.i)
        {
            for(int i = position->start.i; i <= position->end.i;i++)
            {
                if(player->table[i][position->start.j] != 0)
                    return POZ_tken;
            }
        }
        else
        {
           
            for(int i = position->start.i; i >= position->end.i;i--)
            {
                if(player->table[i][position->start.j] != 0)
                    return POZ_tken;
            }
        }
    }

    return ERR_ok;
}


void place_ship(PLAYER* player, SHIP* position, int cv)
{
    //cv va fi fol pt a putea pune acea nava moarta in tabel, ia 1 daca nava ii moarta, altfel 0
    //printf("Am intrat aici pt test\n");
    if(position->start.i == position->end.i)
    {
        if(position->start.j < position->end.j)
        {
            for(int j = position->start.j; j <= position->end.j;j++)
            {
                if(cv == 0)
                {
                    player->table[position->start.i][j] = 1;
                }
                else
                {
                     player->table[position->start.i][j] = 3;
                }    
            }
        }
        else
        {
            //printf("Am intrat aici pt dir 3\n");
            for(int j = position->start.j; j >= position->end.j; j--)
            {
               if(cv == 0)
               {
                    player->table[position->start.i][j] = 1;
               }
               else
               {
                player->table[position->start.i][j] = 3;
               }
               
            }
            //printf("Am iesit din if ul pt dir 3\n");
        }
    }
    else
    {
        //printf("Am intrat bine in else\n");
        if(position->start.i < position->end.i)
        {
            for(int i = position->start.i; i <= position->end.i;i++)
            {
                if(cv == 0)
                {
                    player->table[i][position->start.j] = 1;
                }
               else
                {
                    player->table[i][position->start.j] = 3;
                }
            }
        }
        else
        {
           // printf("Am intrat aici pt directia 0\n");
            for(int i = position->start.i; i >= position->end.i;i--)
            {
               // printf("%d\n", i);
               if(cv == 0)
               {
                player->table[i][position->start.j] = 1;                
               }
               else
               {
                player->table[i][position->start.j] = 3;
               }
               
            }
        }
    }


}


void ship_delete(PLAYER* player, SHIP* position)
{
    if(position->start.i == position->end.i)
    {
        for(int j = position->start.j; j <= position->end.j;j++)
        {
            player->table[position->start.i][j] = 0;    
        }
    }
    else
    {
        for(int i = position->start.i; i <= position->end.i;i++)
        {
            player->table[i][position->start.j] = 0;
        }
    }

}

ERR_CODE chk_dead_ship(PLAYER* player, SHIP* position)
{
    int dead = 1;

     if(position->start.i == position->end.i)
    {
        if(position->start.j < position->end.j)
        {
            for(int j = position->start.j; j <= position->end.j;j++)
            {
                if(player->table[position->start.i][j] != 2)
                    dead = 0;
            }
        }
        else
        {
            //printf("Am intrat aici pt dir 3\n");
            for(int j = position->start.j; j >= position->end.j; j--)
            {
                if(player->table[position->start.i][j] != 2)
                    dead = 0;
            }
           
        }
    }
    else
    {
        
        if(position->start.i < position->end.i)
        {
            for(int i = position->start.i; i <= position->end.i;i++)
            {
                if(player->table[i][position->start.j] != 2)
                    dead = 0;
            }
        }
        else
        {
            for(int i = position->start.i; i >= position->end.i;i--)
            {
              if(player->table[i][position->start.j] != 2)
                dead = 0;
            }
        }
    }
    
    if(dead == 1)
    {
        place_ship(player, position,1);
        return DEAD_Ship;
    }
    else
    {
        return NOT_dead;
    }
}



int chk_dead_ships(PLAYER* player)
{
    int nr = 0;
    for(int i = 0; i < 5; i++)
    {
        switch(i)
        {
            case 0:
            {   
                if(chk_dead_ship(player, &player->player_ships.battleship) == DEAD_Ship)
                {
                    place_ship(player, &player->player_ships.battleship,1);
                    nr++;
                }
                break;
            }
            case 1:
            {
                 if(chk_dead_ship(player, &player->player_ships.carrier) == DEAD_Ship)
                {
                    place_ship(player, &player->player_ships.carrier,1);
                    nr++;
                }
                break;
            }
            case 2:
            {
                 if(chk_dead_ship(player, &player->player_ships.cruiser) == DEAD_Ship)
                {
                    place_ship(player, &player->player_ships.cruiser,1);
                    nr++;
                }
                break;
            }
            case 3:
            {
                 if(chk_dead_ship(player, &player->player_ships.submarine) == DEAD_Ship)
                {
                    place_ship(player, &player->player_ships.submarine,1);
                    nr++;
                }
                break;
            }
           case 4:
            {
                 if(chk_dead_ship(player, &player->player_ships.destroyer) == DEAD_Ship)
                {
                    place_ship(player, &player->player_ships.destroyer,1);
                    nr++;
                }
                break;
            }
        }
        //nr++;
    }
    return nr;

}