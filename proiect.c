#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TABLE_SIZE 10

typedef struct
{
    int i;
    int j;
}POSITION;



typedef struct 
{
    POSITION start;
    POSITION end;
}SHIP;


typedef struct 
{
    SHIP carrier;
    SHIP battleship;
    SHIP cruiser;
    SHIP submarine;
    SHIP destroyer;
    /*
        fiecare nava va retine pozitia de incepere si de sfarsit 
    */
}SHIPS;

typedef struct 
{
    uint16_t size;
    uint16_t** table;
    int16_t id;
    /*
        valorile din tabel vor fi:
        - 0 daca inca nu s a tras in acea casuta
        - 1 daca avem barca acolo
        - 2 daca avem acolo o barca si s a tras in ea
        - 3 daca avem acolo barca moarta
        - 4 ne arata ca a fost lovit patratul si nu a lovit nimic
        id stocheaza numarul jucatorului(max 2)
    */
   SHIPS player_ships;
   //stocheaza coordonatele fiecarei nave in parte
}PLAYER;


typedef enum 
{
    ERR_ok, 
    INIT_err,
    POZ_tken,
    DEAD_Ship

}ERR_CODE;

void clear_buffer()
{
    while(getchar() != '\n');
}


ERR_CODE player_init(PLAYER* player, int player_id)
{
    player->size = TABLE_SIZE;
    player->id = player_id;
    if((player->table = (uint16_t**)malloc(player->size * sizeof(uint16_t*))) == NULL)
    {
        return INIT_err;
    }

    for(int i = 0; i < player->size; i++)
    {
        if((player->table[i] = (uint16_t*) malloc(player->size * sizeof(uint16_t))) == NULL)
        {
            return INIT_err;
        }

    }

    for(int i = 0; i < player->size; i++)
    {
        for(int j = 0; j < player->size; j++)
            {
                player->table[i][j] = 0;
            }
    }

    return ERR_ok;

}

void player_delete(PLAYER* player)
{
    for(int i = 0; i < player->size; i++)
    {
        free(player->table[i]);
    }

    free(player->table);
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

            printf("OK\n");
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

ERR_CODE init_players(PLAYER* player, int player_number)
{
    for(int i = 0; i < player_number; i++)
    {
        if(player_init(&player[i],i) != ERR_ok)
        {
            return INIT_err;
        }
    }   
    return ERR_ok;
}

void show_players(PLAYER* player, int player_number)
{
    for(int i = 0; i < player_number; i++)
    {
        show_player(&player[i],i%2);
    }   
}

void delete_players(PLAYER* player, int player_number)
{
    for(int i = 0; i < player_number; i++)
    {
        player_delete(&player[i]);
    }   
}

void get_position(int* poz_i, int* poz_j)
{
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

        if(fgets(poz,4,stdin) == NULL)
        {
            printf("--There was an error when reading your position. Please try again--\n");
            t = 2;
        }
        else if((sscanf(poz,"%c%d", &c, &temp) != 2))
        {
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
    int delta_i = position->end.i - position->start.i;
    //int delta_j = position->end_j - position->start_j;

    if(delta_i == 0)
    {
        for(int j = position->start.j; j < position->end.j;j++)
        {
            if(player->table[position->start.i][j] != 0)
                printf("%d ", j);
                //return POZ_tken;
        }
    }
    else
    {
        for(int i = position->start.i; i < position->end.i;i++)
        {
            if(player->table[i][position->start.j] != 0)
                return POZ_tken;
        }
    }

    return ERR_ok;
}

void place_ship(PLAYER* player, SHIP* position)
{
    if(position->start.i == position->end.i)
    {
        for(int j = position->start.j; j <= position->end.j;j++)
        {
            player->table[position->start.i][j] = 1;    
        }
    }
    else
    {
        for(int i = position->start.i; i <= position->end.i;i++)
        {
            player->table[i][position->start.j] = 1;
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
                        place_ship(player, &temp);
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
                            place_ship(player,&temp);
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
                            place_ship(player,&temp);
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
                            place_ship(player,&temp);
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
                            place_ship(player,&temp);
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

void random_ship(PLAYER* enemy,SHIP* temp, int ship_size)
{
   
    int direction = rand() % 4;
    temp->start.i = rand() % 10;
    temp->start.j= rand() % 10;

    int ship_is_placeable = 1;
            // 1 - daca nava nu poate fi pozitionata si trebe o alta poz initiala; 0 - daca se poate aseza
    int repeat = 0;

    while(ship_is_placeable != 0 && repeat <= 3)
        {
            switch(direction)
            {
                case 0:
                {
                    temp->end.j = temp->start.j;
                    temp->end.i = temp->start.i - ship_size + 1;
                    if(temp->end.i >= 0 || temp->end.i <= 9/*&& ceva */ )
                    {
                        ship_is_placeable = 0;
                    }
                    else
                    {
                        repeat++;

                    }
                    break;
                }
                case 1:
                {
                    temp->end.i = temp->start.i;
                    temp->end.j = temp->start.j + ship_size - 1;
                    if(temp->end.j >= 0 || temp->end.j <= 9)
                    {
                        ship_is_placeable = 0;
                    }
                    
                    break;
                }
                case 2:
                {
                    temp->end.j = temp->start.j;
                    temp->end.i = temp->start.i + ship_size - 1;
                    if(temp->end.i >= 0 || temp->end.i <= 9)
                    {
                        ship_is_placeable = 0;
                    }
                    break;
                }
                case 3:
                {
                    temp->end.i = temp->start.i;
                    temp->end.j = temp->start.j - ship_size + 1;
                    if(temp->end.j >= 0 || temp->end.j <= 9)
                    {
                        ship_is_placeable = 0;
                    }
                    break;
                }
            }
        }
            

}

void random_ships(PLAYER* enemy)
{


}




void game_msg_start()
{
    printf("--Now that you selected your fleet's position, let's start the game itself--\n");
    printf("--The correct way of selecting the position that you want to hit is by typing the cell's coordonates. The correct format is:\"A2\"--\n");
    printf("--After every hit that you selected, you will be able to see your and your enemy's hits--\n");
    printf("--Here is a little head's up of what you will encounter on the board--\n");
    printf("-- -O- Represents an tile that has not been hit yet--\n");
    printf("-- -+- Represents that an ship is at that location and has been hit--\n");
    printf("-- -#- Represents a dead ship--/n");
    printf("-- -*- Represents a tile that hs been selected--/n");
} 

ERR_CODE dead_ship(PLAYER* player,SHIP* ship)
{
    int code = 1;
    //code  = 1 daca ship is dead else 0 if ship alive
    
    if(ship->start.i == ship->end.i)
    {
        for(int j = ship->start.j; j <= ship->end.j;j++)
        {
            if(player->table[ship->start.i][j] == 1)
            {
                code = 0;
            }    
        }
        if(code == 1)
        {
            //the ship is dead
            for(int j = ship->start.j; j <= ship->end.j;j++)
                player->table[ship->start.i][j] = 3;
                return DEAD_Ship;
        }
    }
    else
    {
        for(int i = ship->start.i; i <= ship->end.i;i++)
        {
            if(player->table[i][ship->start.j] == 1)
            {
                code = 0;
            }
            if(code == 1)
            {
                //the ship is dead
                for(int i = ship->start.i; i <= ship->end.i;i++)
                    player->table[i][ship->start.j] = 3;
                    return DEAD_Ship;

            }
        }
    }
    return ERR_ok;
}

void start_game(PLAYER* player, PLAYER* enemy)
{
    game_msg_start();
    srand(time(NULL));
    int game_going = 1;
    SHIP position;

    while(game_going == 1)
    {
        printf("--Here are the 2 boards. Select the tile you want to hit on your enemy's board--\n");
        printf("\n--It's your turn.--\n");

        clear_buffer();
        get_position(position.start.i, position.start.j);

        if(enemy->table[position.start.i][position.start.j] == 0)
        {
            enemy->table[position.start.i][position.start.j] = 4;
            printf("--That was a miss--\n");
        }
        else
        {
            enemy->table[position.start.i][position.start.j] = 2;
            printf("--Well done!. You hit a ship. Keep going!--\n");

        }

        printf("\n--It's your enemy's turn--\n");





    }
    


}



int main(void)
{
    srand(time(NULL));
    int player_number = 0;
    startup_messages(&player_number);
    //printf("Player number:%d\n", player_number);

    PLAYER player[player_number];
    //printf("%lu\n", sizeof(player));
    
    
    if(init_players(player, player_number) != ERR_ok)
    {
        printf("Eroare alocare player\n");
        exit(0);
    }

    ship_placement(&player[0]);
    //game_msg_start();



    //show_players(player, player_number);
    
    //delete_players(player, player_number);
  
    printf("totul ok\n");
    return 0;
}