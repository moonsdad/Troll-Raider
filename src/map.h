/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * map.h -- Map Utility Functions.                                            *
 ******************************************************************************/
 #pragma once

#include "types.h"

/* Map Generation Macros */
#define ACTIVE_LOCATION    curlv[pc.maplv].map[r][c]


/******************************************************************************
 * FUNCTION:    set_loc
 * ARGUMENTS:   char  t
 *              LOC * s
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool set_loc( char t, LOC * s );


/******************************************************************************
 * FUNCTION:    fill
 * ARGUMENTS:   char     t
 *              LEVEL *  l
 *              COORD    d
 *              COORD    c
 * RETURNS:     int
 * WARNING:
 * NOTE://map, door row, door col, corner row, corner column
 ******************************************************************************/
bool fill( char t, LEVEL * l, COORD d, COORD c );
#define fill_wall(x,y,z) fill('w', x,y,z)
#define fill_floor(x,y,z) fill('.', x,y,z)


/******************************************************************************
 * FUNCTION:    init_lv
 * ARGUMENTS:   LEVEL *  l
 *              short    t
 * WARNING:
 * NOTE:
 ******************************************************************************/
void init_lv( LEVEL * l, short t );

/******************************************************************************
 * FUNCTION:    get_map_icon
 * ARGUMENTS:   LOC here
 * RETURNS:     char
 * WARNING:
 * NOTE:
 ******************************************************************************/
char get_map_icon( LOC here );


/******************************************************************************
 * FUNCTION:    draw_map                                                      *
 * ARGUMENTS:   LEVEL * curlv   -- The Level to draw.                         *
 ******************************************************************************/
void draw_map( LEVEL * curlv );


/******************************************************************************
 * FUNCTION:    buildgen
 * ARGUMENTS:   LEVEL *  outside
 *              LEVEL *  inside
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool buildgen( LEVEL * outside, LEVEL * inside );


/******************************************************************************
 * FUNCTION:    towngen             -- Generate the town level.               *
 * ARGUMENTS:   LEVEL * l           -- The level.                             *
 *              unsigned short n    -- qt of buildings to generate.           *
 * RETURNS:     bool                -- Generation successfull                 *
 * Attempts to generate buildings MIN_HUT_DIST appart, however they may still *
 * overlap effectively generating less than n buildings. This makes them more *
 * interesting though, so it is permitted.                                    *
 ******************************************************************************/
bool towngen( LEVEL * l, unsigned short n );



/************************************EOF***************************************/
