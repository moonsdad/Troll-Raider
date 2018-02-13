/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * map.c -- Map Utility Functions.                                            *
 ******************************************************************************/

#include <math.h>
#include <curses.h>
#include "map.h"
#include "item.h"

//TODO: make map symbols configable
const char WALL   = '#';
const char FLOOR  = '.';
const char DOOR   = '+';
const char USTAIR = '>';
const char DSTAIR = '<';

/* Macro Functions */
#define squared(x) (x * x)


/******************************************************************************
 * FUNCTION:    biggest                                                       *
 * ARGUMENTS:   double  a   -- The first value to compare.                    *
 *              double  b   -- The second value to compare.                   *
 * RETURNS:     double      -- The largest of a and b, or zero if the same.   *
 ******************************************************************************/
#define biggest( a, b ) (( a == b )? 0.0 : ( a > b )? a : b)


/******************************************************************************
 * FUNCTION:    smallest                                                      *
 * ARGUMENTS:   int     a   -- The first value to compare.                    *
 *              int     b   -- The second value to compare.                   *
 * RETURNS:     int         -- a iff it is smaller than b.                    *
 ******************************************************************************/
#define smallest( a, b ) (( a < b )? a : b)


/******************************************************************************
 * FUNCTION:    dist                                                          *
 * ARGUMENTS:   COORD     a  -- First Point                                   *
 *              COORD     b  -- Second Point                                  *
 * RETURNS:     double      -- distance between point a and b                 *
 ******************************************************************************/
static double dist( COORD a, COORD b )
{
    return sqrt( squared( a.rowy - b.rowy ) + squared( a.colx - b.colx ) );
}/* End dist Func */


/******************************************************************************
 * FUNCTION:    set_loc             Initialize a location's type              *
 * ARGUMENTS:   LOC * spot          -- The spot to initialize                 *
 *              char  type          -- The type for this spot                 *
 ******************************************************************************/
void set_loc( LOC * spot, char type  )
{
    spot->litter = NULL;
    spot->mon = NULL;
    spot->icon = type;
    spot->is_trap = false;
}/* End set_loc Func */


/******************************************************************************
 * FUNCTION:    fill                Fill an area with a certain LOC type      *
 * ARGUMENTS:   LEVEL *  l          -- The level with an area to fill         *
 *              char     t          -- The LOC type                           *
 *              COORD    d,c        -- Two opposite corners defining the area *
 * RETURNS:     bool                Fails if area exeeds bounds of a map.     *
 * NOTE: XXX d and c use to stand for door and corner.                        *
 ******************************************************************************/
bool fill( LEVEL * l, char t, COORD d, COORD c )
{
    int i, j, s = smallest( d.colx, c.colx ),
        b[2] = { biggest( d.rowy, c.rowy ), biggest( d.colx, c.colx ) };
    /* NOTE: Biggest returns zero if it's arguments are the same. */

    for( i = smallest( d.rowy, c.rowy ); i < b[0]; i++ )
        for( j = s; j < b[1]; j++ )
            if( ( i > MAX_ROW )||( j > MAX_COL ) ) return false;
            else set_loc( &l->map[i][j], t );
    return true;
}/* End fill Func */


/******************************************************************************
 * FUNCTION:    init_lv                    Initialize Level                   *
 * ARGUMENTS:   LEVEL *  l          -- The level being initialized            *
 *              short    t          -- the type of level                      *
 ******************************************************************************/
void init_lv( LEVEL * l, short t )
{
    int r, c;

    /* Keep Track of Current Level and Position in curlv array */
    l->type = t;

    l->is_new = true; /* Initially Unexplored */

    /* Set All map flags Initially in Case Some Slip Through */
    for( r = 0; r < MAX_ROW; r++ ) for( c = 0; c < MAX_COL; c++ )
        /* Border is always wall *///TODO: This could be done much more efficiently
        if( ( r == 0 )||( r == (MAX_ROW-1) )
          ||( c == 0 )||( c == (MAX_COL-1) ) )
            set_loc( &l->map[r][c], WALL );
        else /* Interior is floors for now */
            set_loc( &l->map[r][c], FLOOR );
}/* End init_lv Func */


/******************************************************************************
 * FUNCTION:    get_map_icon         Get the map icon to display for a LOC    *
 * ARGUMENTS:   LOC     here      -- The location in question.                *
 * RETURNS:     char              -- The symobl to display for this location. *
 ******************************************************************************/
char get_map_icon( LOC here )
{

    if( here.litter != NULL ) return '&';//tempfortest
    if( here.mon != NULL )    return 'o';//tempfortest
                              return here.icon;
}/* end get_map_icon func */


/******************************************************************************
 * FUNCTION:    draw_map                                                      *
 * ARGUMENTS:   LEVEL * curlv   -- The Level to draw.                         *
 ******************************************************************************/
void draw_map( LEVEL * curlv )
{
    int r, c;
    for( r = 0; r < MAX_ROW; r++ ) for( c = 0; c < MAX_COL; c++ )
        mvaddch( r,c, get_map_icon( curlv->map[r][c] ) );
}/* End draw_map Func */


/******************************************************************************
 * FUNCTION:    buildgen                -- Generate building interiors        *
 * ARGUMENTS:   LEVEL *  outside        -- The exterior level                 *
 *              LEVEL *  inside         -- The generated interior level       *
 ******************************************************************************/
void buildgen( LEVEL * outside, LEVEL * inside )
{
    int r, c, i, j;
    bool found_first;
    bool done;

    /* copy/invert ouside map to inside map */
    for( r = 1; r < (MAX_ROW -1); r++ ) for( c = 1; c < (MAX_COL -1); c++ )
        if( outside->map[r][c].icon == WALL )
            set_loc( &inside->map[r][c], FLOOR );
        else if( outside->map[r][c].icon == FLOOR )
            set_loc( &inside->map[r][c], WALL );
        else if( outside->map[r][c].icon == DOOR ) {
            //Both a door and exit to lv
            set_loc( &inside->map[r][c], FLOOR );
            //second found wall should usually be made exit door
            found_first = false;
            done  = false;
            for( i = r-1; i <= r+1; i++ )
                if( done  == true ) break;
                else for( j = c-1; j <= c+1; j++ )
                    if( inside->map[i][j].icon == WALL ) {
                        if( found_first == false ) found_first = true;
                        else{
                            set_loc( &inside->map[i][j], DOOR );
                            done  = true;
                            break;
                        }/* end place exit door else */
                    }/* end is_wall if */
        }/* end is_trap if */

    /* TODO: Generate sub buildings attached to main buildings with tunnels */

}/* end buildgen */


/******************************************************************************
 * FUNCTION:    towngen             -- Generate the town level.               *
 * ARGUMENTS:   LEVEL * l           -- The level.                             *
 *              unsigned short n    -- qt of buildings to generate.           *
 * RETURNS:     bool                -- Generation successfull                 *
 * Attempts to generate buildings MIN_HUT_DIST appart, however they may still *
 * overlap effectively generating less than n buildings. This makes them more *
 * interesting though, so it is permitted.                                    *
 ******************************************************************************/
bool towngen( LEVEL * l, unsigned short n )
{
    unsigned short i, j, z;
    int dvert, dhorz;
    COORD hutspot[MAX_HUTS];
    RECT room[MAX_HUTS];

    assert( n > 0 );
    assert( n <= MAX_HUTS );

    /* Create n Huts */
    for( i = 0; i < n; i++ ) {

        while( true ) {
            /* Find centers of Buildings */
            hutspot[i].rowy =  ( 2 + MIN_HUT_HGT ) + rng( MAX_ROW - ( 2 + MIN_HUT_HGT ) );
            hutspot[i].colx =  ( 2 + MIN_HUT_WID ) + rng( MAX_COL - ( 2 + MIN_HUT_WID ) );
            if( i > 0 ) for( z = 0; z < i; z++ )
                if( dist( hutspot[z], hutspot[i] ) < MIN_HUT_DIST ) continue;

            /* Expand Building Dimensions */
            dvert = MIN_HUT_HGT + rng(MAX_HUT_HGT);
            dhorz = MIN_HUT_WID + rng(MAX_HUT_WID);

            room[i].a.rowy = hutspot[i].rowy - dvert;
            room[i].a.colx = hutspot[i].colx - dhorz;
            room[i].b.rowy = hutspot[i].rowy + dvert;
            room[i].b.colx = hutspot[i].colx + dhorz;

            /* Don't Bump the Outer Wall */
            if( ( room[i].a.rowy < 2  )||( room[i].a.colx < 2 ) ) continue;
            if( room[i].b.rowy > ( MAX_ROW - 2 ) ) continue;
            if( room[i].b.colx > ( MAX_COL - 2 ) ) continue;

            break;
        }/*end !done while */

        /* Fill Building With Wall */
        if( fill_wall( l, room[i].a, room[i].b ) == false )
            return( Error( "Failed to Fill Building Walls", i ) );

        /* Place Doors */ //inline with center//TODO:+-rng(dvert||dhorz -1)
        for( j = 0, z = rng(4) ; j < 4; j++ ) { /* Try Up to Each Direction NSEW *///TODO: Perhaps try only once
            if( ++z > WEST ) z = NORTH;
            switch( z ) {
                case NORTH:
                    hutspot[i].rowy = room[i].a.rowy;
                    if( l->map[hutspot[i].rowy-1][hutspot[i].colx].icon == WALL )
                        continue;
                    break;
                case SOUTH:
                    hutspot[i].rowy = room[i].b.rowy - 1;
                    if( l->map[room[i].b.rowy][hutspot[i].colx].icon == WALL )
                        continue;
                    break;
                case EAST:
                    hutspot[i].colx = room[i].b.colx - 1;
                    if( l->map[hutspot[i].rowy-1][room[i].b.colx].icon == WALL )
                        continue;
                    break;
                case WEST:
                    hutspot[i].colx = room[i].a.colx;
                    if( l->map[hutspot[i].rowy][hutspot[i].colx-1].icon == WALL )
                        continue;
                    break;
            }/* End cardinal direction Switch */

            /* Set Building Enterance Flags */
            set_loc( &l->map[hutspot[i].rowy][hutspot[i].colx], DOOR );
            break;
        }/* End NSEW For */
    }/* End n For */

    /* TODO: Place NPCs */


    return true;
}/* End towngen Func */



/************************************EOF***************************************/
