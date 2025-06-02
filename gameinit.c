#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "gameinit.h"
#include "positions.h"
#include "computer.h"


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

void delete_players(PLAYER* player, int player_number)
{
    for(int i = 0; i < player_number; i++)
    {
        player_delete(&player[i]);
    }   
}

void clear_buffer()
{
    char c;
    while((c = getchar()) == '\n');
    ungetc(c,stdin);
}