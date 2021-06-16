#include "chars.h"

//values platers
//cazador
int dmg_estocada_cazador = 1000;
int sangrado_estocada_cazador = 500;
int dmg_cc_cazador = 3000;
//medico
int valor_curar_medic = 2000;
int min_rayo_luz_medic = 500;
int max_rayo_luz_medic = 3000;
//hacker
int dmg_ddos_hacker = 1500;
int dmg_fb_hacker = 10000;
// values entitys
// jagruz
int dmg_ruzgar_jagruz = 1000;
int proba_ruzgar_jagruz = 50;
int dmg_coletazo_jagruz = 500;
int proba_coletazo_jagruz = 50;
// ruzalos
int dmg_salto_ruzalos = 1500;
int proba_salto_ruzalos = 40;
int dmg_espina_intoxicado_ruzalos = 500;
int dmg_espina_ruzalos = 400;
int duracion_intox_ruzalos = 3;
int proba_espina_ruzalos = 60;
//ruiz
int proba_copia_ruiz = 40;
int proba_repro_ruiz = 20;
int proba_rm_ruiz = 40;
int rm_multiplier_ruiz = 100;
// Crea un nuevo entity en el heap
// @param [char*] nombre: nombre del jugador
// @param [bool] party_leader: indica si es o no party leader
// @param [char*] type: clase del jugador o tipo de monstruo, debe ser como en el enunciado
// @param [bool] is_player: indica si es un juigador o monstruo
// @return [entity*] retorna el puntero al nuevo entity 
entity* new_entity(bool party_leader, int client_socket, bool is_player, char *name, char* type)
{
    entity* new_entity = malloc(sizeof(entity));
    if (is_player)
    {
        new_entity->jugador = malloc(sizeof(entity));
        new_entity->jugador->client_socket = client_socket;
        new_entity->monstruo = NULL;
        new_entity->is_player = is_player;
        new_entity->jugador->intoxicated = false;
        new_entity->jugador->turns_intoxicated = 0;
        new_entity->jugador->party_leader = party_leader;
        new_entity->jugador->last_used_distraer = false;
        
        new_entity->jugador->nombre=name;
        new_entity->type= type;
    }
    else
    {
        new_entity->monstruo = malloc(sizeof(entity));
        new_entity->jugador = malloc(sizeof(entity));
        new_entity->is_player = false;
        new_entity->jugador->nombre = type;
        if (!strcmp(type, "Great JagRuz")){
            new_entity->vida_max = 10000;
            new_entity->type = type;} 
        else if (!strcmp(type, "Ruzalos")){
            new_entity->vida_max = 20000;
            new_entity->type = type;} 
        else if (!strcmp(type, "Ruiz, el Gemelo Malvado del Profesor Ruz")){
            new_entity->vida_max = 25000;
            new_entity->type = type;}
        new_entity->alive = true;
        new_entity->monstruo->used_salto = false;
    }
    new_entity->alive = true;
    new_entity->dmg_per_turn = 0;
    new_entity->vida = new_entity->vida_max;
    new_entity->dmg_modifier = 1;
    new_entity->dmg_recieved_modifier = 1;
    new_entity->times_sangrado = 0;
    new_entity->times_fb = 0;
    return new_entity;
}
//funcion para agregar el nombre al jugador
void name_entity_player(entity* player, char* name){
    player->jugador->nombre = name;
}
//funcion para agregar el type al jugador
void type_entity_player(entity* player, int type){
    if (type==1) {
        player->vida_max = 5000;
        player->type = "Cazador";
        player->jugador->ability1_name = "Estocada";
        player->jugador->ability2_name = "Corte Cruzado";
        player->jugador->ability3_name = "Distraer";
    } else if (type==2) {
        player->vida_max = 3000;
        player->type = "Médico";
        player->jugador->ability2_name = "Destello Regenerador";
        player->jugador->ability3_name = "Descarga Vital";
        player->jugador->ability1_name = "Curar";
    } else if (type==3){
        player->vida_max = 2500;
        player->type = "Hacker";
        player->jugador->ability1_name = "Inyección SQL";
        player->jugador->ability2_name = "Ataque DDOS";
        player->jugador->ability3_name = "Fuerza Bruta";
    }
    player->vida = player->vida_max;
}

entity* new_monster(int type){
    entity* new_entity = malloc(sizeof(entity));
    new_entity->monstruo = malloc(sizeof(entity));
    new_entity->jugador = malloc(sizeof(entity));
    new_entity->is_player = false;
    if (type==1){
        new_entity->vida_max = 10000;
        new_entity->type ="Great JagRuz";
        new_entity->jugador->nombre = "Great JagRuz";
    }
       
    else if (type== 2){
        new_entity->vida_max = 20000;
        new_entity->type = "Ruzalos";
        new_entity->jugador->nombre = "Ruzalos";
    } 
    else if (type==3){
        new_entity->vida_max = 25000;
        new_entity->type = "Ruiz, el Gemelo Malvado del Profesor Ruz";
        new_entity->jugador->nombre = "Ruiz, el Gemelo Malvado del Profesor Ruz";
    }
    new_entity->alive = true;
    new_entity->monstruo->used_salto = false;
    new_entity->dmg_per_turn = 0;
    new_entity->vida = new_entity->vida_max;
    new_entity->dmg_modifier = 1;
    new_entity->dmg_recieved_modifier = 1;
    new_entity->times_sangrado = 0;
    new_entity->times_fb = 0;
    return new_entity;
}

// Función usada para que una entity pueda usar una habilidad
// @param [entity*] user: jugador que esta usando la habilidad
// @param [entity*] target: el monstruo que esta siendo atacado
// @param [char*] ability: nombre de la habilidad, debe ser como sale en el enunciado
// @param [entity**] entitys: array de entitys que estan jugando
// @param [int] amt_of_entitys: número de jugadores activos
// @return [bool] true si la habilidad se uso, false si la habilidad fallo
bool use_ability(entity* user, entity* target, char* ability, entity** entitys, int amt_of_entitys, juego* game)
{
    if (strcmp(user->jugador->ability1_name, ability) && strcmp(user->jugador->ability2_name, ability) && strcmp(user->jugador->ability3_name, ability))
    {
        return false;
    }
    if (!strcmp(user->type, "Cazador"))
    {
        return use_ability_cazador(user, target, ability, entitys, amt_of_entitys, game);
    }
    if (!strcmp(user->type, "Médico"))
    {
        return use_ability_medico(user, target, ability, entitys, amt_of_entitys, game);
    }
    if (!strcmp(user->type, "Hacker"))
    {
        return use_ability_hacker(user, target, ability, game);
    }
    return false;
}

// Cazador
bool use_ability_cazador(entity* user, entity *target, char* ability, entity** entitys, int amt_of_entitys, juego* game)
{
    if (!strcmp(ability, user->jugador->ability1_name)) // estocada
    {
        return use_estocada(user, target, game);
    } 
    else if (!strcmp(ability, user->jugador->ability2_name)) // cotre cruzado
    {
        return use_corte_cruzado(user, target, game);
    }
    else // distraer
    {
        return use_distraer(entitys, user, amt_of_entitys, game);
    }
}

bool use_estocada(entity* user, entity* target, juego* game)
{
    printf("%s uso esotcada\n", user->jugador->nombre);
    dmg_entity(user, target, dmg_estocada_cazador);
    if (target->alive)
    {
        if (target->times_sangrado < 3)
        {
            target->times_sangrado++;
            target->dmg_per_turn += sangrado_estocada_cazador;
        }
    }
    acualize_game_state(game);
    return true;
}

bool use_corte_cruzado(entity* user, entity* target, juego* game)
{
    printf("%s uso cc\n", user->jugador->nombre);
    dmg_entity(user, target, dmg_cc_cazador);
    acualize_game_state(game);
    return true;
}

bool use_distraer(entity **entitys, entity* user, int amt_of_entitys, juego* game)
{
    printf("%s uso distraer\n", user->jugador->nombre);
    for (int i = 0; i < amt_of_entitys; i++)
    {
        entitys[i]->jugador->last_used_distraer = false;
    }
    user->jugador->last_used_distraer = true;
    acualize_game_state(game);
    return true;
}

// Médico
bool use_ability_medico(entity* user, entity* target, char* ability, entity** entitys, int amt_of_entitys, juego* game)
{
    if (!strcmp(ability, user->jugador->ability1_name)) // curar
    {
        return use_curar(user, target, game);
    }
    else if (!strcmp(ability, user->jugador->ability2_name)) // destello ragenerador
    {
        return use_destello(user, target, entitys, amt_of_entitys, game);       
    }
    else  // descarga vital
    {
        return use_vital(user, target, game);
    }
}


bool use_curar(entity* user, entity* target, juego* game)
{
    printf("%s uso curar\n", user->jugador->nombre);
    heal_entity(user, target, valor_curar_medic);
    acualize_game_state(game);
    return true;
}

bool use_destello(entity* user, entity* target, entity** entitys, int amt_of_entitys, juego* game)
{
    printf("%s uso destello\n", user->jugador->nombre);
    int dagno = (rand() % (min_rayo_luz_medic - max_rayo_luz_medic + 1)) + min_rayo_luz_medic;
    int target_entity_index = rand() % amt_of_entitys;
    entity* target_entity = entitys[target_entity_index];
    heal_entity(user, target_entity, dagno);
    dmg_entity(user, target, dagno);
    acualize_game_state(game);
    return true;
}

bool use_vital(entity* user, entity* target, juego* game)
{
    printf("%s uso vital\n", user->jugador->nombre);
    int dagno = 2 * (user->vida_max - user->vida);
    dmg_entity(user, target, dagno);
    acualize_game_state(game);
    return true;
}

bool use_ability_hacker(entity* user, entity* target, char* ability, juego* game)
{
    if (!strcmp(ability, user->jugador->ability1_name)) // inyeccion sql
    {
        printf("%s uso sql\n", user->jugador->nombre);
        return use_sql(target, game);
    }
    else if (!strcmp(ability, user->jugador->ability2_name)) // ddos
    {
        return use_ddos(user, target, game);
    }
    else // fuerza bruta
    {
        return use_fb(user, target, game);
    }
}

bool use_sql(entity* target, juego* game)
{
    target->dmg_modifier *= 2;
    acualize_game_state(game);
    return true;
}

bool use_ddos(entity* user, entity* target, juego* game)
{
    printf("%s uso ddos\n", user->jugador->nombre);
    dmg_entity(user, target, dmg_ddos_hacker);
    acualize_game_state(game);
    return true;
}

bool use_fb(entity* user, entity* target, juego* game)
{
    printf("%s uso fb\n", user->jugador->nombre);
    if (user->times_fb < 3)
    {
        user->times_fb++;
    }
    else
    {
        dmg_entity(user, target, dmg_fb_hacker);
        user->times_fb = 0;
    }
    acualize_game_state(game);
    return true;
}

// Función usada para que un monstruo use una habilidad
// @param [entity*] user: monstruo que ataca
// @param [entity**] entitys: array de entitys que estan jugando
// @param [int*] rounds: puntero a la cantidad de rondas desde que se uso la habilidad sudo rm -rf
// @param [int] amt_of_entitys: número de jugadores activos
// @return [bool] true si la habilidad se uso, false si la habilidad fallo
bool entity_use_ability(entity* user, entity** entitys, int amt_of_entitys, int* rounds, juego* game)
{
    entity* target = NULL;
    srand ( time(NULL) );
    for (int i = 0; i < amt_of_entitys; i++)
    {
        if (entitys[i]->jugador->last_used_distraer)
        {
            target = entitys[i];
            entitys[i]->jugador->last_used_distraer = false;
        }
    }
    if (!target)
    {
        int entity_index = rand() % amt_of_entitys;
        target = entitys[entity_index];
    }
    int prob = rand() % 100; 
    if (!strcmp(user->type, "Great JagRuz"))
    {
        if (prob < proba_ruzgar_jagruz) // ruzgar
        {
            printf("%s uso ruzgar\n", user->type);
            dmg_entity(user, target, dmg_ruzgar_jagruz);
            acualize_game_state(game);
            return true;
        }
        else // coletazo
        {
            for (int i = 0; i < amt_of_entitys; i++)
            {
                dmg_entity(user, entitys[i], dmg_coletazo_jagruz);
            }
            printf("%s uso coletazo\n", user->type);
            acualize_game_state(game);
            return true;
        }
    }
    else if (!strcmp(user->type, "Ruzalos"))
    {
        if (prob < proba_salto_ruzalos) // salto
        {
            printf("%s uso salto", user->type);
            if (user->monstruo->used_salto)
            {
                printf("pero fallo\n");
                user->monstruo->used_salto = false;
                acualize_game_state(game);
                return false;
            }
            else
            {
                printf("\n");
                dmg_entity(user, target, dmg_salto_ruzalos);
                user->monstruo->used_salto = false;
                acualize_game_state(game);
                return true;
            }  
        }
        else // espina venenoza
        {   
            printf("%s uso espina venenoza\n", user->type);         
            if (target->jugador->intoxicated)
            {
                dmg_entity(user, target, dmg_espina_intoxicado_ruzalos);
                acualize_game_state(game);
                return true;
            }
            else
            {
                target->jugador->intoxicated = true;
                target->dmg_per_turn += dmg_espina_ruzalos;
                target->jugador->turns_intoxicated = duracion_intox_ruzalos;
                acualize_game_state(game);
                return true;
            }
        }
    }
    else
    {
        if (prob < proba_copia_ruiz)
        {
            printf("%s uso copia\n", user->type);
            int ability = rand() % 9;
            switch (ability)
            {
            case 0:
                return use_estocada(user, target, game);
            case 1:
                return use_corte_cruzado(user, target, game);
            case 2:
                return use_distraer(entitys, user, amt_of_entitys, game);
            case 3:
                return use_curar(user, target, game);
            case 4:
                return use_destello(user, target, entitys, amt_of_entitys, game);
            case 5:
                return use_vital(user, target, game);
            case 6:
                return use_ddos(user, target, game);
            case 7:
                return use_fb(user, target, game);
            case 8:
                printf("%s uso sql\n", user->type);
                return use_sql(target, game);
            }
            return false;
        }
        else if (prob >= proba_copia_ruiz && prob < proba_copia_ruiz + proba_repro_ruiz)
        {
            printf("%s uso reprobado\n", user->type);
            target->dmg_recieved_modifier *= 1.5;
            target->dmg_modifier *= 0.5;
            acualize_game_state(game);
            return true;
        }
        else // 
        {
            printf("%s uso rm\n", user->type);
            int dmg = *rounds * 100;
            *rounds = 0;
            for (int i = 0; i < amt_of_entitys; i++)
            {
                dmg_entity(user, entitys[i], dmg);   
            }
            acualize_game_state(game);
            return true;
        }
    }
}

void dmg_entity(entity* attacker, entity* target, int dmg)
{
    int real_dmg = dmg * attacker->dmg_modifier;
    if (target->is_player)
    {
        printf("%s hizo %d de daño a %s\n",attacker->type, real_dmg, target->jugador->nombre);
    }
    else
    {
        printf("%s hizo %d de daño a %s\n",attacker->jugador->nombre, real_dmg, target->type);
    }
    
    if (target->vida > real_dmg)
    {
        target->vida -= real_dmg;
    } 
    else 
    {
        target->alive = false;
        target->vida = 0;
        if(target->is_player){
            printf("%s ha muerto!\n", target->jugador->nombre);
        }
        else {
            printf("%s ha muerto!\n", target->type);
        }
    }
}

void heal_entity(entity* user, entity* target, int amt)
{
    int real_amt = amt * user->dmg_modifier;
    printf("curo %d de vida\n", real_amt);
    if (target->alive)
    {
        if (target->vida_max - target->vida < real_amt)
        {
            target->vida += real_amt;
        }
        else
        {
            target->vida = target->vida_max;
        }
    }
}

void free_entity(entity* entidad)
{
    if (entidad->is_player)
    {
        free(entidad->jugador->nombre);
        free(entidad->jugador);
    }
    else
    {
        free(entidad->jugador);
        free(entidad->monstruo);
    }
    free(entidad);
}

juego* init_game(entity** players, entity* monster, int amt_of_players)
{
    juego *new_game = malloc(sizeof(juego));
    new_game->amt_of_players = amt_of_players;
    new_game->battle_going = false;
    new_game->monster = monster;
    new_game->players = players;
    new_game->rounds = malloc(sizeof(4));
    new_game->rounds_rm = malloc(sizeof(4));
    *new_game->rounds = 1;
    *new_game->rounds_rm = 1;
    new_game->game_state = malloc(sizeof(char)*1000);
    acualize_game_state(new_game);
    return new_game;
}

void acualize_game_state(juego* juego)
{
    char* life_displayer = "%s [%s]:  %d/%d\n";
    char* rondas = "RONDA: %d\n";
    char rounds[30];
    sprintf(rounds, rondas, *juego->rounds);
    
    strcpy(juego->game_state, "===================== SITUACIÓN ACTUAL =====================\n");
    strcat(juego->game_state, rounds);
    for (int i = 0; i < juego->amt_of_players; i++)
    {
        char player_status[500]; 
        sprintf(player_status, life_displayer, juego->players[i]->jugador->nombre, juego->players[i]->type, juego->players[i]->vida, juego->players[i]->vida_max);
        strcat(juego->game_state,player_status);
    }
    char monster_status[500];
    sprintf(monster_status, life_displayer, juego->monster->type, "monstruo", juego->monster->vida, juego->monster->vida_max);
    strcat(juego->game_state, monster_status);
    strcat(juego->game_state, "============================================================\n");
}


// Función para pasar de turno
// @return [bool] retorna true si el combate sigue y false si el combate se acaba
bool pasar_turno(entity** players, entity* target, int* rondas, int* rondas_since_rm, int amt_players, juego* game)
{
    *rondas += 1;
    *rondas_since_rm += 1;
    bool someone_alive = false;
    for (int i = 0; i < amt_players; i++)
    {
        actualize_entity(players[i]);
        if (!someone_alive)
        {
            someone_alive = players[i]->alive;
        }
    }
    actualize_entity(target);
    if (!target->alive || !someone_alive)
    {
        return false;
    }
    acualize_game_state(game);
    return true;
}

void actualize_entity(entity* entidad)
{
    entity* nature = malloc(sizeof(entity));
    nature->type = "Tiempo";
    nature->jugador = malloc(sizeof(player));
    nature->jugador->nombre = "Tiempo";
    nature->dmg_modifier = 1;
    dmg_entity(nature, entidad, entidad->dmg_per_turn);
    if (entidad->alive)
    {
        if (entidad->is_player && entidad->jugador->intoxicated)
        {
            entidad->jugador->turns_intoxicated--;
            if (entidad->jugador->turns_intoxicated == 0)
            {
                entidad->jugador->intoxicated = false;
                entidad->dmg_per_turn -= 500; // TODO: Cambiar por variable cuando solucione el tema de importar variables globales
            } 
        }
    }
    free(nature->jugador);
    free(nature);
}

void free_game(juego* game)
{
    free(game->rounds);
    free(game->rounds_rm);
    free(game->game_state);
    for (int i = 0; i < game->amt_of_players; i++)
    {
        free(game->players[i]->jugador);
        free(game->players[i]->monstruo);
        free(game->players[i]);
    }
    free(game->monster->monstruo);
    free(game->monster->jugador);
    free(game->monster);
    free(game->players);
    free(game);
}