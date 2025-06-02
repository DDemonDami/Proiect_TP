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
    DEAD_Ship,
    NOT_dead

}ERR_CODE;

ERR_CODE player_init(PLAYER*, int); // initializeaza jucatorii cu un id

void player_delete(PLAYER*); //elibereaza memoria alocata pt jucatori

void delete_players(PLAYER*, int); // sterge un numar de jucatori

ERR_CODE init_players(PLAYER*, int); // initializeaza un numar de jucatori

void clear_buffer(); // folosita pt a distruge orice caract ce nu ar trebui sa fie in buffer



