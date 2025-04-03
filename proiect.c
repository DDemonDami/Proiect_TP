#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 10


typedef struct 
{
    int start_i;
    int start_j;
    int end_i;
    int end_j;
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
        id stocheaza numarul jucatorului(max 2)
    */
   SHIPS player_ships;
   //stocheaza coordonatele fiecarei nave in parte
}PLAYER;


typedef enum 
{
    ERR_ok, 
    INIT_err,
    POZ_tken
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

void show_player(PLAYER* player)
{
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
                        printf("X");
                        break;
                    }
                    case 2:
                    {
                        printf("*");
                        break;
                    }
                    case 3:
                    {
                        printf("#");
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
        show_player(&player[i]);
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
            printf("Eroare citire\n");
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
                printf("Eroare convertire litera\n");
                t = 3;
            }   
                else if(temp < 0 || temp > 9)
                {
                    printf("Eroare convertire cifra\n");
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
    int delta_i = position->end_i - position->start_i;
    //int delta_j = position->end_j - position->start_j;

    if(delta_i == 0)
    {
        for(int j = position->start_j; j < position->end_j;j++)
        {
            if(player->table[position->start_i][j] != 0)
                printf("%d ", j);
                //return POZ_tken;
        }
    }
    else
    {
        for(int i = position->start_i; i < position->end_i;i++)
        {
            if(player->table[i][position->start_j] != 0)
                return POZ_tken;
        }
    }

    return ERR_ok;
}

void place_ship(PLAYER* player, SHIP* position)
{
    if(position->start_i == position->end_i)
    {
        for(int j = position->start_j; j <= position->end_j;j++)
        {
            player->table[position->start_i][j] = 1;    
        }
    }
    else
    {
        for(int i = position->start_i; i <= position->end_i;i++)
        {
            player->table[i][position->start_j] = 1;
        }
    }


}

void ship_placement(PLAYER* player)
{
    printf("\n\n--Now that the game started, it's time to place our ships.--\n");
    printf("--Each ship has a diffrent lenght. You must select a starting point and an end point for you ship.--\n");
    printf("--You can select a square by typing the coordonates of that square in the terminal. A valid square pozition has this format: \"A2\"--\n");
    printf("--Here is how your fleet looks until now:--\n\n");
    show_player(&player[0]);
    printf("--Here are the ships that you have to place:--\n--Carrier -1- --\n--Battleship -2- --\n--Cruiser -3- --\n--Submarine -4- --\n--Destroyer -5- --\n");
    printf("--You can select the ship that you want to place by simply typing the number coresponding to the ship--\n");
    printf("--Start placing your ships!--\n");

    int ship_selector = 0;
    int remaining_ships = 5;
    //enum ship_names fleet;

    while(remaining_ships != 0)
    {
        printf("--Select the ship that you want to place--\n");
        scanf("%d", &ship_selector);
        

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
                    printf("--Please select the starting position--\n");
                    get_position(&temp.start_i, &temp.start_j);

                    printf("--Please select the ending position--\n");
                    get_position(&temp.end_i, &temp.end_j);
                    /*
                    int delta_i = temp.end_i - temp.start_i + 1;
                    int delta_j = temp.end_j - temp.start_j + 1;
                    //trebuie sa verific ca pozitiile astea nu sunt ocupate de alte nave
                    */

                    //printf("%d\n%d\n", delta_i, delta_j);
                    if((((temp.start_i == temp.end_i) && ((temp.end_j - temp.start_j + 1) == 5)) || ((temp.start_j == temp.end_j) && ((temp.end_i - temp.start_i + 1) == 5))) && (check_position(player, &temp) == ERR_ok))
                    {
                        //printf("OK\n");
                       
                        printf("--The position was saved succesfully--\n");
                        place_ship(player, &temp);
                        player->player_ships.carrier = temp;
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
            case 2:
            {
                printf("--You selected the Battleship. This ship needs to have a lenght of 4--\n");
                int ok = 1;  // 0 daca ii bine, 1 daca nu ii bine
                while(ok == 1)
                {
                    SHIP temp;
                    clear_buffer();

                    printf("--Please select the starting position--\n");
                    get_position(&temp.start_i, &temp.start_j);

                    printf("--Please select the ending position--\n");
                    get_position(&temp.end_i, &temp.end_j);

                    if((((temp.start_i == temp.end_i) && ((temp.end_j - temp.start_j + 1) == 4)) || ((temp.start_j == temp.end_j) && ((temp.end_i - temp.start_i + 1) == 4))) && (check_position(player, &temp) == ERR_ok))
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

                    printf("--Please select the starting position--\n");
                    get_position(&temp.start_i, &temp.start_j);

                    printf("--Please select the ending position--\n");
                    get_position(&temp.end_i, &temp.end_j);

                    if((((temp.start_i == temp.end_i) && ((temp.end_j - temp.start_j + 1) == 3)) || ((temp.start_j == temp.end_j) && ((temp.end_i - temp.start_i + 1) == 3))) && (check_position(player, &temp) == ERR_ok))
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

                    printf("--Please select the starting position--\n");
                    get_position(&temp.start_i, &temp.start_j);

                    printf("--Please select the ending position--\n");
                    get_position(&temp.end_i, &temp.end_j);

                    if((((temp.start_i == temp.end_i) && ((temp.end_j - temp.start_j + 1) == 3)) || ((temp.start_j == temp.end_j) && ((temp.end_i - temp.start_i + 1) == 3))) && (check_position(player, &temp) == ERR_ok))
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

                    printf("--Please select the starting position--\n");
                    get_position(&temp.start_i, &temp.start_j);

                    printf("--Please select the ending position--\n");
                    get_position(&temp.end_i, &temp.end_j);

                    if((((temp.start_i == temp.end_i) && ((temp.end_j - temp.start_j + 1) == 2)) || ((temp.start_j == temp.end_j) && ((temp.end_i - temp.start_i + 1) == 2))) && (check_position(player, &temp) == ERR_ok))
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
        show_player(&player[0]);
    }


}
 


int main(void)
{
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


    //show_players(player, player_number);
    
    //delete_players(player, player_number);
  
    printf("totul ok\n");
    return 0;
}