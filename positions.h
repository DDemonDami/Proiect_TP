void get_position(int*, int*); // functie pt a citi de la tastatura o pozitie

ERR_CODE check_position(PLAYER*, SHIP*); // folosita pentru a verifica daca nava este pusa corect

void place_ship(PLAYER*, SHIP*, int); // functie folosita pentru a pune o nava in tabel, sau pentru a pune o nava moarta in tabel

void ship_delete(PLAYER*, SHIP*); // functie folosita pentru a putea sterge o nava pusa gresit

ERR_CODE chk_dead_ship(PLAYER*, SHIP*); //verifica daca nava unui jucator este ditrusa

int chk_dead_ships(PLAYER* player); //verifica daca una din navele jucatorului a fost distrusa si returneaza 1