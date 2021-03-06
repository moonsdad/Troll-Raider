/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 ******************************************************************************/
#pragma once

#include "rmhv_stdlib.h"

#define CANCEL      -1
#define NOT_PLACED  99

/* get_cmd return values *//* get_subi_cmd return values */
#define NO_ACTION    0
#define NORTH        1
#define SOUTH        2
#define EAST         3
#define WEST         4
#define NORTH_EAST   5
#define NORTH_WEST   6
#define SOUTH_EAST   7
#define SOUTH_WEST   8
#define WAIT         9
#define QUIT         10
#define SUB_FIN      10
#define INVENTORY    11
#define EQUIPMENT    12
#define DROP_ITEM    13
#define REMOVE_ITEM  14
#define DESTROY_ITEM 15
#define PICK_UP      16

/* Character Abilities */
#define STR         0 /*Strength*/
#define AGI         1 /*Agility*/
#define MEN         2 /*Mental*/
#define CON         3 /*Constitution*/
#define CHA         4 /*Charisma*/
#define LUC         5 /*Luck*/
#define MAX_STATS   6
#define MAX_STAT_VAL 10

/* Item Statistics */
#define TO_HIT          0
#define TO_DAM          1
#define MAX_ITEM_STATS  2

#define ITEM_DAT "dat/item.dat"
#define MAX_ITEM_NAME_LEN   10
#define MAX_LV_ITEM         10

/* Equipment Types */
#define MELEE           0
#define REACH           1
#define RANGE1          2
#define RANGE2          3
#define SHIELD          4
#define TYPHAT          5
#define TYPARM          6
#define MAX_ITEM_TYPES  7

/* NPCs */
#define MON_DAT "dat/human.dat"
#define MAX_NAME_LEN 11
#define MAX_NPC      45
#define NPC_TTROLL    0
#define HUMAN_INCT    1
#define HUMAN_COMN    2
#define HUMAN_HERO    3
#define NPC_TYPE_QT   4

/* Equipment Slots */
#define WEP         0 /*Weapon*//* Main Hand */
#define OFF         1 /*Off-Hand (shield or weapon)*/
#define ARM         2 /*Armor*/
#define HAT         3 /*Head*/
#define MAX_SLOTS   4

#define MAX_HOLD 10

/* Map/LV Size Definitions */
#define MAX_ROW 20
#define MAX_COL 60

#define EAST_WEST   0
#define NORTH_SOUTH 1

/* Current Level Array Access Definitions */
#define HVILLAGE    0 /* Human Village */
#define IN_HHUTS    1 /* Inside Human Huts */
#define CASL_UP2    2 /* Castles 3rd Upper Level *///current "highest" level
#define CASL_UP1    3 /* Castles 2nd Upper Level */
#define CASL_UP0    4 /* Castles 1st Upper Level */
#define CASL_GRD    5 /* Castles Ground Level */
#define CASL_DN0    6 /* Castles 1st Lower Level */
#define CASL_DN1    7 /* Castles 2nd Lower Level */
#define CASL_DN2    8 /* Castles 3rd Lower Level *///current "lowest" level
#define MAX_MAPS    9 /* TOTAL NUMBER OF MAPS PER GAME */

/******************************************************************************
 *                            -----------                                     *
 *                           / CASL_UPX /       Player starts in HVILLAGE     *
 *                           -----------        which is paired with IN_HHUTS *
 *                                  ^                                         *
 *                                 ...          Player may leave town or      *
 *                                  v               decend into CASL_DN0.     *
 *                            -----------                                     *
 *                           / CASL_GRD /       The CASL_DNX levels           *
 *    ----------- <---------------------            are underground.          *
 *   / HVILLAGE /                   ^           CASL_UPX levels are above.    *
 *   -----------                    v                                         *
 *        ^                    -----------      From the CASL_GRD level       *
 *        +-----------------> / CASL_DN0 /        the player may exit back    *
 *        v                  -----------          to the HVILLAGE.            *
 *    -----------                   ^                                         *
 *   / IN_HHUTS /                  ...                                        *
 *   -----------                    v                                         *
 *                            -----------                                     *
 *                           / CASL_DNX /                                     *
 *                           -----------                                      *
 ******************************************************************************/


typedef struct {
    char name[MAX_ITEM_NAME_LEN + 1];
    int stats[MAX_ITEM_STATS];
    int worth;
    bool is_2handed;
    unsigned short int type;
} ITEM;

typedef struct {
    char name[MAX_NAME_LEN+1];
    int stats[MAX_STATS];
    ITEM * inventory[MAX_HOLD];
    ITEM * equip[MAX_SLOTS];
    int hp[2]; /* [0]MAX [1]CUR */
    float explv; /* lv is int, exp is decimal */
    int money, food;
    int locr, locc, maplv;
    unsigned is_main  : 1;
    unsigned is_alive : 1;
    unsigned is_human : 1;
    unsigned is_awake : 1;
} PLAYER;

typedef struct {
    int rowy;
    int colx;
} COORD;

typedef struct {
    COORD a;
    COORD b;
} RECT;

typedef struct {
    ITEM * litter;
    PLAYER * mon;
    char icon;
    bool is_trap;
} LOC;

typedef struct level_struct {
    LOC map[MAX_ROW][MAX_COL];
    int type; /* Current Level Array Index */
    bool is_new; /* Unexplored */
} LEVEL;
