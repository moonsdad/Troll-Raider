/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * cwin.c -- Curses Window Functions.                                         *
 ******************************************************************************/

#include "cwin.h"


WINDOW  * display_btm;



/******************************************************************************
 * FUNCTION:    wsay                                                          *
 * ARGUMENTS:   WINDOW * where   -- The window to which we will write         *
 *              char   * string  -- The string which will be written          *
 ******************************************************************************/
void wsay( WINDOW * where, const char * string )
{
    int i = 0;

    /* Move all lines up */
    wmove( where, 0, 0 );
    if( wdeleteln(where) == ERR ) exit( Error( "wdeleteln err", FAIL ) );
    /* print string at end */
    else while( string[i] != '\0' ) {
        wmove( where, BTM_SUB_ROWS-1, i );
        if( winsch( where, string[i++] ) == ERR )
            exit( Error( "message insert display", string[--i] ) );
    }/* End string !terminated While */

    wrefresh(where);
}/* End wsay Func */


/******************************************************************************
 * FUNCTION:    vsay                                                          *
 * ARGUMENTS:   const char * fmt  -- The format string.                       *
 *              ...               -- A variable quantity of arguments.        *
 * Writes a formatted string to the bottom display.                           *
 ******************************************************************************/
 #define BUFFER_SIZE 80
void vsay( const char * fmt, ... )
{
    char buf[BUFFER_SIZE];
    va_list valist;

    va_start(valist, fmt);
    vsnprintf( buf, BUFFER_SIZE, fmt, valist );
    va_end(valist);
    wsay( display_btm, buf );

}/* End vsay Func */


/******************************************************************************
 * FUNCTION:    init_display_right                                            *
 * ARGUMENTS:   WINDOW   *     d    -- Reference to the window to initialize  *
 *              PLAYER   *     p    -- The player whose statistics to display *
 *              unsigned long  turn    -- What Turn is it?                    *
 * RETURNS:     bool                                                          *
 * NOTE:        d expects the right window.                                   *
 ******************************************************************************/
bool init_display_right( WINDOW * d, PLAYER * p, unsigned long turn )
{
    int i;
    static const char* dflav[] = {
        /* for each dungeon lv an arbitrary 5 letter name */
                             "Vault",
                             "Jails",
                             "Depth"
                         };

    wmove( d, 1, 1 );
    wprintw( d, " %s  LV %.2f   $%d\n\n  HP %d/%d\n",
        p->name, p->explv, p->money, p->hp[1], p->hp[0] );//[0]MAX//[1]CUR
    for( i = 0; i < MAX_STATS; i++ ) {
        wprintw( d, "\n" );
        switch(i) {
            case STR:   wprintw(d,"  STR ");   break;
            case AGI:   wprintw(d,"  AGI ");   break;
            case MEN:   wprintw(d,"  MEN ");   break;
            case CON:   wprintw(d,"  CON ");   break;
            case CHA:   wprintw(d,"  CHA ");   break;
            case LUC:   wprintw(d,"  LUC ");   break;
            default:    return false;
        }/* End i Switch */
        wprintw( d, "%d", p->stats[i] );
    }/* End MAX_STATS i For */
    wprintw( d, "\n\n  Food: %d\n  Family Saved: %d ", p->food, 0 ); //TODO: s->family );

    /* Initialize i for Flavor */
    i = rng(3);//XXX

    switch(p->maplv) {
//        case TVILLAGE:    wprintw(d,"\n\n Troll Village ");    break;
        case HVILLAGE:    wprintw(d,"\n\n Human Village ");    break;
        case IN_HHUTS:    wprintw(d,"\n\n In a Hut      ");    break;
        case CASL_GRD:    wprintw(d,"\n\n Castle Ground ");    break;
        case CASL_UP0:    wprintw(d,"\n\n Tower 1st LV  ");    break;
        case CASL_UP1:    wprintw(d,"\n\n Tower 2nd LV  ");    break;
        case CASL_UP2:    wprintw(d,"\n\n Tower 3rd LV  ");    break;
        case CASL_DN0:    case CASL_DN1:    case CASL_DN2:
            wprintw(d,"\n\n Dungeon %s ", dflav[i] );       break;
        default:        wprintw(d,"\n\n ERROR ");           break;
    }/*end switch*/
    wprintw( d, "\n  Turn: %d\n", turn );
    box(d,'|','-');//
    return true;
}/* end init_display_right func*/



/************************************EOF***************************************/
