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
// values monsters
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


// Crea un nuevo player en el heap
// @param [char*] nombre: nombre del jugador
// @param [bool] party_leader: indica si es o no party leader
// @param [char*] class: clase del jugador, debe ser como en el enunciado
// @return [player*] retorna el puntero al nuevo player 
player* new_player(char* nombre, bool party_leader, char* class)
{
    player* new_jugador = malloc(sizeof(player));
    if (!strcmp(class, "Cazador")) {
        new_jugador->vida_max = 5000;
        new_jugador->class = class;
        new_jugador->ability1_name = "Estocada";
        new_jugador->ability2_name = "Corte Cruzado";
        new_jugador->ability3_name = "Distraer";
    } else if (!strcmp(class, "Médico")) {
        new_jugador->vida_max = 3000;
        new_jugador->class = class;
        new_jugador->ability1_name = "Curar";
        new_jugador->ability2_name = "Destello Regenerador";
        new_jugador->ability3_name = "Descarga Vital";
    } else if (!strcmp(class, "Hacker")){
        new_jugador->vida_max = 2500;
        new_jugador->class = class;
        new_jugador->ability1_name = "Inyección SQL";
        new_jugador->ability2_name = "Ataque DDOS";
        new_jugador->ability3_name = "Fuerza Bruta";
    }
    new_jugador->alive = true;
    new_jugador->intoxicated = false;
    new_jugador->dmg_per_turn = 0;
    new_jugador->turns_intoxicated = 0;
    new_jugador->vida = new_jugador->vida_max;
    new_jugador->nombre = nombre;
    new_jugador->party_leader = party_leader;
    new_jugador->dmg_modifier = 1;
    new_jugador->dmg_recieved_modifier = 1;
    new_jugador->last_used_distraer = false;
    new_jugador->times_fb = 0;
    return new_jugador;
}

// Función usada para que un player pueda usar una habilidad
// @param [player] user: jugadore que esta usando la habilidad
// @param [monster] target: el monstruo que esta siendo atacado
// @param [player] target_p: cuando el target es otro jugador
// @param [char*] ability: nombre de la habilidad, debe ser como sale en el enunciado
// @param [player*] players: array de players que estan jugando
// @param [int] amt_of_players: número de jugadores activos
// @return [bool] true si la habilidad se uso, false si la habilidad fallo
bool use_ability(player* user, monster* target, player* target_p, char* ability, player** players, int amt_of_players)
{
    if (strcmp(user->ability1_name, ability) && strcmp(user->ability2_name, ability) && strcmp(user->ability3_name, ability))
    {
        return false;
    }
    if (!strcmp(user->class, "Cazador"))
    {
        return use_ability_cazador(user, target, ability, players, amt_of_players);
    }
    if (!strcmp(user->class, "Médico"))
    {
        return use_ability_medico(user, target, target_p, ability, players, amt_of_players);
    }
    if (!strcmp(user->class, "Hacker"))
    {
        return use_ability_hacker(user, target, target_p, ability);
    }
    return false;
}

// Cazador
bool use_ability_cazador(player* user, monster *target, char* ability, player** players, int amt_of_players)
{
    if (!strcmp(ability, user->ability1_name)) // estocada
    {
        return use_estocada(target);
    } 
    else if (!strcmp(ability, user->ability2_name)) // cotre cruzado
    {
        return use_corte_cruzado(user, target);
    }
    else // distraer
    {
        return use_distraer(players, user, amt_of_players);
    }
}

bool use_estocada(monster* target)
{
    if (target->vida > dmg_estocada_cazador)
    {
        target->vida -= dmg_estocada_cazador;
        if (target->times_sangrado < 3)
        {
            target->times_sangrado++;
            target->side_effect_dmg += sangrado_estocada_cazador;
        }
        return true;
    } else 
    {
        target->alive = false;
        return true;
    }
}

bool use_corte_cruzado(player* user, monster* target)
{
    dmg_monster(user, target, dmg_cc_cazador);
    return true;
}

bool use_distraer(player **players, player* user, int amt_of_players)
{
    for (int i = 0; i < amt_of_players; i++)
    {
        players[i]->last_used_distraer = false;
    }
    user->last_used_distraer = true;
    return true;
}

// Médico
bool use_ability_medico(player* user, monster* target, player* target_p, char* ability, player** players, int amt_of_players)
{
    if (!strcmp(ability, user->ability1_name)) // curar
    {
        heal_player(user, target_p, valor_curar_medic);
        return true;
    }
    else if (!strcmp(ability, user->ability2_name)) // destello ragenerador
    {
        int dagno = (rand() % (min_rayo_luz_medic - max_rayo_luz_medic + 1)) + min_rayo_luz_medic;
        int target_player_index = rand() % amt_of_players;
        player* target_player = players[target_player_index];
        heal_player(user, target_player, dagno);
        dmg_monster(user, target, dagno);
        return true;        
    }
    else  // descarga vital
    {
        int dagno = 2 * (user->vida_max - user->vida);
        dmg_monster(user, target, dagno);
        return true;
    }
}

bool use_ability_hacker(player* user, monster* target, player* target_p, char* ability)
{
    if (!strcmp(ability, user->ability1_name)) // inyeccion sql
    {
        target_p->dmg_modifier *= 2;
        return true;
    }
    else if (!strcmp(ability, user->ability2_name)) // ddos
    {
        dmg_monster(user, target, dmg_ddos_hacker);
        return true;
    }
    else // fuerza bruta
    {
        if (user->times_fb < 3)
        {
            user->times_fb++;
        }
        else
        {
            dmg_monster(user, target, dmg_fb_hacker);
            user->times_fb = 0;
        }
        return true;
    }
}

// Función usada para que un monstruo use una habilidad
// @param [monster] user: monstruo que ataca
// @param [player**] players: array de players que estan jugando
// @param [int*] rounds: puntero a la cantidad de rondas desde que se uso la habilidad sudo rm -rf
// @param [int] amt_of_players: número de jugadores activos
// @return [bool] true si la habilidad se uso, false si la habilidad fallo
bool monster_use_ability(monster* user, player** players, int amt_of_players, int* rounds)
{
    player* target;
    bool assigned_target = false;
    srand ( time(NULL) );
    for (int i = 0; i < amt_of_players; i++)
    {
        if (players[i]->last_used_distraer)
        {
            target = players[i];
            assigned_target = true;
            players[i]->last_used_distraer = false;
        }
    }
    if (!assigned_target)
    {
        int player_index = rand() % amt_of_players;
        target = players[player_index];
    }   
    int prob = rand() % 100; 
    if (!strcmp(user->type, "Great JagRuz"))
    {
        if (prob < proba_ruzgar_jagruz) // ruzgar
        {
            dmg_player(target, dmg_ruzgar_jagruz);
            return true;
        }
        else // coletazo
        {
            for (int i = 0; i < amt_of_players; i++)
            {
                dmg_player(players[i], dmg_coletazo_jagruz);
            }
            return true;
        }
    }
    else if (!strcmp(user->type, "Ruzalos"))
    {
        printf("%d\n", prob);
        if (prob < proba_salto_ruzalos) // salto
        {
            printf("Uso salto");
            if (user->used_salto)
            {
                printf("pero fallo\n");
                user->used_salto = false;
                return false;
            }
            else
            {
                printf("\n");
                dmg_player(target, dmg_salto_ruzalos);
                user->used_salto = false;
                return true;
            }  
        }
        else // espina venenoza
        {            
            if (target->intoxicated)
            {
                dmg_player(target, dmg_espina_intoxicado_ruzalos);
                return true;
            }
            else
            {
                target->intoxicated = true;
                target->dmg_per_turn += dmg_espina_ruzalos;
                target->turns_intoxicated = duracion_intox_ruzalos;
                return true;
            }
        }
    }
    else
    {
        if (prob < proba_copia_ruiz)
        {
            return true;
        }
        else if (prob >= proba_copia_ruiz && prob < proba_copia_ruiz + proba_repro_ruiz)
        {
            target->dmg_recieved_modifier *= 1.5;
            target->dmg_modifier *= 0.5;
            return true;
        }
        else // 
        {
            int dmg = *rounds * 100;
            printf("%d\n", dmg);
            *rounds = 0;
            for (int i = 0; i < amt_of_players; i++)
            {
                dmg_player(players[i], dmg);   
            }
            return true;
        }
    }
}

void dmg_monster(player* attacker, monster* target, int dmg)
{
    int real_dmg = dmg * attacker->dmg_modifier;
    if (target->vida > real_dmg)
    {
        target->vida -= real_dmg;
    } 
    else 
    {
        target->alive = false;
        target->vida = 0;
    }
}

void dmg_player(player* target, int dmg)
{
    int real_dmg = dmg * target->dmg_recieved_modifier;
    if (target->vida > real_dmg)
    {
        target->vida -= real_dmg;
    } 
    else 
    {
        target->alive = false;
        target->vida = 0;
    }
}

void heal_player(player* user, player* target, int amt)
{
    int real_amt = amt * user->dmg_modifier;
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