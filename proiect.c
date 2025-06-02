#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "gameinit.h"
#include "positions.h"
#include "computer.h"



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

void startup_messages(int* pl_num)
{
    printf("\n\n===== Welcome to BATTLESHIPS =====\n\n");
    printf("--A game where you have to sink your enemy's fleet--\n");
    printf("--To start, write a number below to select an option--\n");
    printf("--Options--\n[ 0 ] Play against bot\n[ 1 ] Play against player\n[ 2 ]Quit\n");

    int selector = -1;

    while(selector > 2 || selector < 0)
    {
        scanf("%d", &selector);
        if(fflush(stdin) == 0)
        {

            //printf("OK\n");
        }
           
        if(selector > 2 || selector < 0)
        {
            printf("Invalid Option. Please try again\n");
        }
    }

    switch(selector)
    {
        case 0:
        {
            printf("\n--You selected the option: Play against bot--\n");
            *pl_num = 2;
            break;
        }
        case 1:
        {
            printf("\n--You selected the option: Play against player--\n");
            *pl_num = 2;
            break;
        }
        case 2:
        {
            printf("\n--You selected to quit the game. Good bye--\n");
            exit(0);
            break;
        }
    }


}

void show_players(PLAYER* player, int player_number)
{
    for(int i = 0; i < player_number; i++)
    {
        show_player(&player[i],i%2);
    }   
}



void ship_placement(PLAYER* player)
{
    printf("\n\n--Now that the game started, it's time to place our ships.--\n");
    printf("--Each ship has a diffrent lenght. You must select a starting point and an end point for you ship.--\n");
    printf("--You can select a square by typing the coordonates of that square in the terminal. A valid square pozition has this format: \"A2\"--\n");
    printf("--Here is how your fleet looks until now:--\n\n");
    show_player(&player[0],0);

    printf("--Here are the ships that you have to place:--\n--Carrier -1- --\n--Battleship -2- --\n--Cruiser -3- --\n--Submarine -4- --\n--Destroyer -5- --\n");
    printf("--You can select the ship that you want to place by simply typing the number coresponding to the ship--\n");
    printf("--Start placing your ships!--\n");

    int ship_selector = 0;
    int remaining_ships = 5;
    //enum ship_names fleet;
    int reset = 1; // va fi setat 0 daca jucatorul nu vrea ca sa modifice pozitiile navelor
    int reset_value = 0;

    //git remaining_ships = 1;  //am sa il folosesc daor pentru a testa butonul de reset ships

    
    while(reset != 0 || remaining_ships != 0)
    {   
        
        while(remaining_ships != 0)
        {
            printf("--Select the ship that you want to place--\n");
            scanf("%d", &ship_selector);
           // printf("\n\nRemainig ships:%d\n\n",remaining_ships);
        
            show_player(&player[0],0);
            switch(ship_selector)
            {
                case 1:
                {
                    printf("--You selected the Carrier. This ship needs to have a lenght of 5--\n");
                    int ok = 1;  // 0 daca ii bine, 1 daca nu ii bine
                    while(ok == 1)
                    {
                        SHIP temp;
                        clear_buffer();

                        if(reset_value != 0)
                        {
                            printf("-- The last position of your ship has been deleted. Please selecat a new position--\n");
                            ship_delete(player, &player->player_ships.carrier);
                            //remaining_ships++;
                        }
                        printf("\n\nRemainig ships:%d\n\n",remaining_ships);

                        printf("--Please select the starting position--\n");
                        get_position(&temp.start.i, &temp.start.j);

                        printf("--Please select the ending position--\n");
                        get_position(&temp.end.i, &temp.end.j);
                        /*
                        int delta_i = temp.end_i - temp.start_i + 1;
                        int delta_j = temp.end_j - temp.start_j + 1;
                        //trebuie sa verific ca pozitiile astea nu sunt ocupate de alte nave
                        */

                        //printf("%d\n%d\n", delta_i, delta_j);
                        if((((temp.start.i == temp.end.i) && ((temp.end.j - temp.start.j + 1) == 5)) || ((temp.start.j == temp.end.j) && ((temp.end.i - temp.start.i + 1) == 5))) && (check_position(player, &temp) == ERR_ok))
                        {
                            //printf("OK\n");
                            
                        printf("--The position was saved succesfully--\n");
                        place_ship(player, &temp,0);
                        player->player_ships.carrier = temp;
                        remaining_ships--;
                        printf("\n\nRemainig ships:%d\n\n",remaining_ships);
                        ok = 0;
                        
                        }
                        else
                        {
                            printf("--The pozition you selected is not valid. Please try again\n");
                        }
                
                    }
                    break;
                }
                case 2:
                {
                    printf("--You selected the Battleship. This ship needs to have a lenght of 4--\n");
                    int ok = 1;  // 0 daca ii bine, 1 daca nu ii bine
                    while(ok == 1)
                    {
                        SHIP temp;
                        clear_buffer();

                        if(reset_value != 0)
                        {
                            printf("-- The last position of your ship has been deleted. Please selecat a new position--\n");
                            ship_delete(player, &player->player_ships.battleship);
                        }


                        printf("--Please select the starting position--\n");
                        get_position(&temp.start.i, &temp.start.j);

                        printf("--Please select the ending position--\n");
                        get_position(&temp.end.i, &temp.end.j);

                        if((((temp.start.i == temp.end.i) && ((temp.end.j - temp.start.j + 1) == 4)) || ((temp.start.j == temp.end.j) && ((temp.end.i - temp.start.i + 1) == 4))) && (check_position(player, &temp) == ERR_ok))
                        {
                            printf("--The position was saved succesfully--\n");
                            place_ship(player,&temp,0);
                            player->player_ships.battleship = temp;
                            remaining_ships--;
                            ok = 0;
                        }
                        else
                        {
                            printf("--The pozition you selected is not valid. Please try again\n");
                        }
                
                    }
                    break;
                }
                case 3:
                {
                    printf("--You selected the Cruiser. This ship needs to have a lenght of 3--\n");
                    int ok = 1;  // 0 daca ii bine, 1 daca nu ii bine
                    while(ok == 1)
                    {
                        SHIP temp;
                        clear_buffer();

                        if(reset_value != 0)
                        {
                            printf("-- The last position of your ship has been deleted. Please selecat a new position--\n");
                            ship_delete(player, &player->player_ships.cruiser);
                        }


                        printf("--Please select the starting position--\n");
                        get_position(&temp.start.i, &temp.start.j);

                        printf("--Please select the ending position--\n");
                        get_position(&temp.end.i, &temp.end.j);

                        if((((temp.start.i == temp.end.i) && ((temp.end.j - temp.start.j + 1) == 3)) || ((temp.start.j == temp.end.j) && ((temp.end.i - temp.start.i + 1) == 3))) && (check_position(player, &temp) == ERR_ok))
                        {
                            printf("--The position was saved succesfully--\n");
                            place_ship(player,&temp,0);
                            player->player_ships.cruiser = temp;
                            remaining_ships--;
                            ok = 0;
                        }
                        else
                        {
                            printf("--The pozition you selected is not valid. Please try again\n");
                        }
                
                    }
                    break;
                }
                case 4:
                {
                    printf("--You selected the Submarine. This ship needs to have a lenght of 3--\n");
                    int ok = 1;  // 0 daca ii bine, 1 daca nu ii bine
                    while(ok == 1)
                    {
                        SHIP temp;
                        clear_buffer();

                        if(reset_value != 0)
                        {
                            printf("-- The last position of your ship has been deleted. Please selecat a new position--\n");
                            ship_delete(player, &player->player_ships.submarine);
                        }


                        printf("--Please select the starting position--\n");
                        get_position(&temp.start.i, &temp.start.j);

                        printf("--Please select the ending position--\n");
                        get_position(&temp.end.i, &temp.end.j);


                        if((((temp.start.i == temp.end.i) && ((temp.end.j - temp.start.j + 1) == 3)) || ((temp.start.j == temp.end.j) && ((temp.end.i - temp.start.i + 1) == 3))) && (check_position(player, &temp) == ERR_ok))
                        {
                            printf("--The position was saved succesfully--\n");
                            place_ship(player,&temp,0);
                            player->player_ships.submarine = temp;
                            remaining_ships--;
                            ok = 0;
                        }
                        else
                        {
                            printf("--The pozition you selected is not valid. Please try again\n");
                        }
                
                    }   
                    break;
                }
                case 5:
                {
                    printf("--You selected the Destroyer. This ship needs to have a lenght of 2--\n");
                    int ok = 1;  // 0 daca ii bine, 1 daca nu ii bine
                    while(ok == 1)
                    {
                        SHIP temp;
                        clear_buffer();

                        if(reset_value != 0)
                        {
                            printf("-- The last position of your ship has been deleted. Please selecat a new position--\n");
                            ship_delete(player, &player->player_ships.destroyer);
                        }


                        printf("--Please select the starting position--\n");
                        get_position(&temp.start.i, &temp.start.j);

                        printf("--Please select the ending position--\n");
                        get_position(&temp.end.i, &temp.end.j);

                        if((((temp.start.i == temp.end.i) && ((temp.end.j - temp.start.j + 1) == 2)) || ((temp.start.j == temp.end.j) && ((temp.end.i - temp.start.i + 1) == 2))) && (check_position(player, &temp) == ERR_ok))
                        {
                            printf("--The position was saved succesfully--\n");
                            place_ship(player,&temp,0);
                            player->player_ships.destroyer = temp;
                            remaining_ships--;
                            ok = 0;
                        }
                        else
                        {
                            printf("--The pozition you selected is not valid. Please try again\n");
                        }
                
                    }
                    break;
                }
                default:
                {
                    printf("--The ship you selected is not valid. Please select another ship--\n");
                }

            }
        printf("--Here is how your fleet looks like until now--\n");
        show_player(&player[0],0);

    }
    
    printf("--Do you want to modify the positions of your ships?--\n");
    printf("--Type 1 for Yes / 0 for No--\n");
    
    scanf("%d", &reset_value);
    //printf("%d", reset_value);
    if(reset_value == 0)
    {
        reset = 0;
    }
    else
    {
    //printf("\n\nRemainig ships:%d\n\n",remaining_ships);
    remaining_ships = 1;
    }
}
    

}


void game_msg_start()
{
    printf("--Now that you selected your fleet's position, let's start the game itself--\n");
    printf("--The correct way of selecting the position that you want to hit is by typing the cell's coordonates. The correct format is:\"A2\"--\n");
    printf("--After every hit that you selected, you will be able to see your and your enemy's hits--\n");
    printf("--Here is a little head's up of what you will encounter on the board--\n");
    printf("-- -O- Represents an tile that has not been hit yet--\n");
    printf("-- -+- Represents that an ship is at that location and has been hit--\n");
    printf("-- -#- Represents a dead ship--\n");
    printf("-- -*- Represents a tile that hs been selected--\n");
}



int start_game(PLAYER* player, PLAYER* enemy)
{
    system("clear");
    game_msg_start();
   // random_ships(player);
    random_ships(enemy);
  //  srand(time(NULL));
   // show_player(enemy,0);

    int dead_ships_player = 0; // stocam cate nave sunt distruse de la fiecare jucator
    int dead_ships_enemy = 0;

    int game_going = 1;
    POSITION position;
    int hit = 0;
    POSITION rand_hit;
    POSITION fst_hit;
    int dir = -1;
    int correct_dir = 0;
    //correct_dir ia 1 daca ii  corect directia

    while(game_going == 1)
    {
        
        printf("\n--Here are the 2 boards. Select the tile you want to hit on your enemy's board--\n");
        show_players(player,2);

        printf("\n--It's your turn.--\n");
        
        // trebuie sa fac verificarea ca sa nu se selecteze aceeasi casuta de 2 ori
           // clear_buffer();
            get_position(&position.i, &position.j);
            //clear_buffer();


        //printf("\n\n POZITIE TAST %d %d\n\n", position.i, position.j);
    

        if(enemy->table[position.i][position.j] == 0)
        {
            enemy->table[position.i][position.j] = 4;
            printf("--That was a miss--\n");
        }
        else
        {
            enemy->table[position.i][position.j] = 2;
            printf("--Well done!. You hit a ship. Keep going!--\n");

        }

        printf("\n--It's your enemy's turn--\n");

       // printf("\n\nCorrect dir; dir; hit; %d %d %d \n\n", correct_dir, dir, hit);

        POSITION poz;
        poz.i = rand() % 10;
        poz.j = rand() % 10;

        if(hit == 0)
        {
            while(player->table[poz.i][poz.j] != 1 && player->table[poz.i][poz.j] != 0)
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
        //printf("\n\nRAND poz : %d %d\n\n", poz.i, poz.j);

        if(player->table[poz.i][poz.j] == 0)
        {
            player->table[poz.i][poz.j] = 4;
            printf("--The enemy missed your fleet! Keep going.--\n");
            if(dir != -1)
            {
                dir = (dir + 1) % 4;
                rand_hit = fst_hit;
            }
        }
        else
        {
            player->table[poz.i][poz.j] = 2;
            printf("--The enemy hit one of your ships! Keep going.--\n");
            hit = 1;
            rand_hit = poz;
            if(dir != -1)
            {
                correct_dir = 1;
            }
            else
            {
                fst_hit = poz;
            }

        }

       // printf("\n\nCorrect dir; dir; hit; %d %d %d \n\n", correct_dir, dir, hit);

        if(chk_dead_ships(player) == 1)
        {
            dead_ships_player++;
            correct_dir = 0;
            dir = -1;
            hit = 0;
            printf("Nu am intrat aici din greseala\n\n");

        }

        if(chk_dead_ships(enemy) == 1)
        {
            dead_ships_enemy++;
            
        }
        
        //printf("\nNR PLAYER | NR CALC  %d | %d\n\n", dead_ships_player, dead_ships_enemy);

        if(dead_ships_enemy == 5 || dead_ships_player == 5)
        {
            game_going = 0;
        }

    }

    if(dead_ships_enemy == 5)
    {
        return 1;
    }
    else if(dead_ships_player == 5)
    {
        return 0;
    }
    

    return 0;
}



int main(void)
{
    srand(time(NULL));
    int player_number = 0;
    startup_messages(&player_number);
    printf("Player number:%d\n", player_number);


    PLAYER player[player_number];
    //printf("%lu\n", sizeof(player));
    
    
    if(init_players(player, player_number) != ERR_ok)
    {
        printf("Eroare alocare player\n");
        exit(0);
    }
   
    ship_placement(&player[0]);

    if(start_game(&player[0], &player[1]) == 1)
    {
        printf("--Yaay! You won. Well Done.--\n");
    }
    else
    {
        printf("--You lost, but you can always play again and have a chance to win--\n");
    }

    
    //game_msg_start();

    
   // show_players(player, 2);
    //show_player(&player[1],0);
    delete_players(player, player_number);
  
    printf("totul ok\n");
    return 0;
}