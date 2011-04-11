/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Dystopia Mud improvements copyright (C) 2000, 2001 by Brian Graversen  *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * The core of the automap system was taken from the snippet by Dingo
 * which can be accessed at : http://www.vidler.clara.net/automap.html
 *
 * A few minor extensions to spiff it up was made by Jobo.
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "merc.h"

/* The map itself */
struct map_type map[MAPX + 1][MAPY + 1];

/* Take care of some repetitive code for later */
void get_exit_dir( int dir, int *x, int *y, int xorig, int yorig )
{
    /* Get the next coord based on direction */
    switch( dir )
    {
        case 0:                                             /* North */
            *x = xorig;
            *y = yorig - 1;
            break;
        case 1:                                             /* East */
            *x = xorig + 1;
            *y = yorig;
            break;
        case 2:                                             /* South */
            *x = xorig;
            *y = yorig + 1;
            break;
        case 3:                                             /* West */
            *x = xorig - 1;
            *y = yorig;
            break;
        default:
            *x = -1;
            *y = -1;
            break;
    }
}

/* Clear one map coord */
void clear_coord( int x, int y )
{
    map[x][y].tegn = ' ';
    map[x][y].vnum = 0;
    map[x][y].depth = 0;
    map[x][y].info = 0;
    map[x][y].can_see = TRUE;
}

/* Clear all exits for one room */
void clear_room( int x, int y )
{
    int dir, exitx, exity;

    /* Cycle through the four directions */
    for( dir = 0; dir < 4; dir++ )
    {
        /* Find next coord in this direction */
        get_exit_dir( dir, &exitx, &exity, x, y );

        /* If coord is valid, clear it */
        if ( !BOUNDARY( exitx, exity ) ) clear_coord( exitx, exity );
    }
}

/* This function is recursive, ie it calls itself */
void map_exits(CHAR_DATA *ch, ROOM_INDEX_DATA *pRoom, int x, int y, int depth)
{
    static char map_chars [4] = "|-|-";
    int door;
    int exitx = 0, exity = 0;
    int roomx = 0, roomy = 0;
    char buf[200];                                          // bugs
    EXIT_DATA *pExit;

    /* Setup this coord as a room */
    switch(pRoom->sector_type)
    {
        case SECT_CITY:
        case SECT_INSIDE:
        case SECT_UNUSED:
            map[x][y].tegn = 'O';
            break;
        case SECT_FIELD:
        case SECT_FOREST:
        case SECT_HILLS:
            map[x][y].tegn = '*';
            break;
        case SECT_MOUNTAIN:
            map[x][y].tegn = '@';
            break;
        case SECT_WATER_SWIM:
        case SECT_WATER_NOSWIM:
            map[x][y].tegn = '=';
            break;
        case SECT_AIR:
            map[x][y].tegn = '~';
            break;
        case SECT_DESERT:
            map[x][y].tegn = '+';
            break;
        default:
            map[x][y].tegn = 'O';
            sprintf(buf, "Map_exits: Bad sector type (%d) in room %d.",
                pRoom->sector_type, pRoom->vnum);
            bug(buf, 0);
            break;
    }
    map[x][y].vnum = pRoom->vnum;
    map[x][y].depth = depth;
    map[x][y].info = pRoom->room_flags;
    map[x][y].can_see = room_is_dark( pRoom );

    /* Limit recursion */
    if ( depth > MAXDEPTH ) return;

    /* This room is done, deal with it's exits */
    for( door = 0; door < 4; door++ )
    {
        /* Skip if there is no exit in this direction */
        if ( ( pExit = pRoom->exit[door] ) == NULL ) continue;

        /* Get the coords for the next exit and room in this direction */
        get_exit_dir( door, &exitx, &exity, x, y );
        get_exit_dir( door, &roomx, &roomy, exitx, exity );

        /* Skip if coords fall outside map */
        if ( BOUNDARY( exitx, exity ) || BOUNDARY( roomx, roomy )) continue;

        /* Skip if there is no room beyond this exit */
        if ( pExit->to_room == NULL ) continue;

        /* Ensure there are no clashes with previously defined rooms */
        if ( ( map[roomx][roomy].vnum != 0 ) &&
            ( map[roomx][roomy].vnum != pExit->to_room->vnum ))
        {
            /* Use the new room if the depth is higher */
            if ( map[roomx][roomy].depth <= depth ) continue;

            /* It is so clear the old room */
            clear_room( roomx, roomy );
        }

        /* No exits at MAXDEPTH */
        if ( depth == MAXDEPTH ) continue;

        /* No need for exits that are already mapped */
        if ( map[exitx][exity].depth > 0 ) continue;

        /* Fill in exit */
        map[exitx][exity].depth = depth;
        map[exitx][exity].vnum = pExit->to_room->vnum;
        map[exitx][exity].info = pExit->exit_info;
        map[exitx][exity].tegn = map_chars[door];

        /* More to do? If so we recurse */
        if ( ( depth < MAXDEPTH ) &&
            ( ( map[roomx][roomy].vnum == pExit->to_room->vnum ) ||
            ( map[roomx][roomy].vnum == 0 ) ) )
        {
            /* Depth increases by one each time */
            map_exits( ch, pExit->to_room, roomx, roomy, depth + 1 );
        }
    }
}

/* Reformat room descriptions to exclude undesirable characters */
void reformat_desc( char *desc )
{
    /* Index variables to keep track of array/pointer elements */
    int i, j;
    char buf[MAX_STRING_LENGTH], *p;

    i = 0;
    j = 0;
    buf[0] = '\0';

    if ( !desc  ) return;

    /* Replace all "\n" and "\r" with spaces */
    for( i = 0; i <= strlen( desc ); i++ )
    {
        if ( ( desc[i] == '\n' ) || ( desc[i] == '\r' ) ) desc[i] = ' ';
    }

    /* Remove multiple spaces */
    for( p = desc; *p != '\0'; p++ )
    {
        buf[j] = *p;
        j++;

        /* Two or more consecutive spaces? */
        if ( ( *p == ' ' ) && ( *( p + 1 ) == ' ' ) )
        {
            do
            {
                p++;
            }
            while( *(p + 1) == ' ' );
        }
    }

    buf[j] = '\0';

    /* Copy to desc */
    sprintf( desc, buf );
}

int get_line( char *desc, int max_len )
{
    int i, j = 0, length;

    length = strlen(desc);

    /* Return if it's short enough for one line */
    if ( length <= max_len ) return 0;

    /* Calculate end point in string without color */
    for( i = 0; i <= length; i++ )
    {
        /* Here you need to skip your color sequences */
        j++;

        if ( j > max_len ) break;
    }

    /* End point is now in i, find the nearest space */
    for( j = i; j > 0; j-- )
    {
        if ( desc[j] == ' ' ) break;
    }

    /* There could be a problem if there are no spaces on the line */
    return j + 1;
}

/* Display the map to the player */
void show_map( CHAR_DATA *ch, char *text )
{
    char buf[MAX_STRING_LENGTH * 2];
    int x, y, pos;
    char *p;
    bool alldesc = FALSE;                                   /* Has desc been fully displayed? */

    if ( !text ) alldesc = TRUE;

    pos = 0;
    p = text;
    buf[0] = '\0';

    /* Top of map frame */
    if (ch->level > 6)
        sprintf( buf, "#0*===#y(#RImm#0#y)#0===*#n %s #0[#7%d#0]#n\n\r",ch->in_room->name,ch->in_room->vnum);
    else
        sprintf( buf, "#0*====#y(#R=#y)#0====*  %s#n\n\r",ch->in_room->name);

    /* Write out the main map area with text */
    for( y = 0; y <= MAPY; y++ )
    {
        strcat( buf, "#0*#n" );

        for( x = 0; x <= MAPX; x++ )
        {
            switch(map[x][y].tegn)
            {
                case '-':
                case '|':
                    sprintf(buf + strlen(buf), "#o%c#n", map[x][y].tegn);
                    break;
                case 'X':
                    sprintf(buf + strlen(buf), "#R%c#n", map[x][y].tegn);
                    break;
                case '*':
                    sprintf(buf + strlen(buf), "#g%c#n", map[x][y].tegn);
                    break;
                case '@':
                    sprintf(buf + strlen(buf), "#o%c#n", map[x][y].tegn);
                    break;
                case '=':
                    sprintf(buf + strlen(buf), "#L%c#n", map[x][y].tegn);
                    break;
                case '~':
                    sprintf(buf + strlen(buf), "#C%c#n", map[x][y].tegn);
                    break;
                case '+':
                    sprintf(buf + strlen(buf), "#y%c#n", map[x][y].tegn);
                    break;
                default:
                    sprintf( buf + strlen( buf ), "%c", map[x][y].tegn );
            }
        }
        strcat( buf, "#0*#n " );

        if (y == 0 && IS_SET(ch->act, PLR_AUTOEXIT))        // the autoexits
        {
            sprintf(buf + strlen( buf ), "%s", get_exits(ch));
            continue;
        }

        /* Add the text, if necessary */
        if ( !alldesc )
        {
            pos = get_line( p, 63 );
            if ( pos > 0 )
            {
                strncat( buf, p, pos );
                p += pos;
            }
            else
            {
                strcat( buf, p );
                alldesc = TRUE;
            }
        }
        strcat( buf, "\n\r" );
    }

    /* Finish off map area */
    strcat( buf, "#0*====#y(#R=#y)#0====*#n " );
    if ( !alldesc )
    {
        pos = get_line( p, 63 );
        if ( pos > 0 )
        {
            strncat( buf, p, pos );
            strcat(buf, "\n\r");
            p += pos;
        }
        else
        {
            strcat( buf, p );
            alldesc = TRUE;
        }
    }

    /* Deal with any leftover text */
    if ( !alldesc )
    {
        do
        {
            /* Note the number - no map to detract from width */
            pos = get_line( p, 78 );
            if ( pos > 0 )
            {
                strncat( buf, p, pos );
                strcat(buf, "\n\r");
                p += pos;
            }
            else
            {
                strcat( buf, p );
                alldesc = TRUE;
            }
        }
        while( !alldesc );
    }
    strcat(buf, "\n\r");

    send_to_char(buf, ch);
}

/* Clear, generate and display the map */
void draw_map( CHAR_DATA *ch, char *desc )
{
    int x, y;
    static char buf[MAX_STRING_LENGTH];

    sprintf( buf, desc );
    /* Remove undesirable characters */
    reformat_desc( buf );

    /* Clear map */
    for( y = 0; y <= MAPY; y++ )
    {
        for( x = 0; x <= MAPX; x++ )
        {
            clear_coord( x, y );
        }
    }

    /* Start with players pos at centre of map */
    x = MAPX / 2;
    y = MAPY / 2;

    map[x][y].vnum = ch->in_room->vnum;
    map[x][y].depth = 0;

    /* Generate the map */
    map_exits( ch, ch->in_room, x, y, 0 );

    /* Current position should be a "X" */
    map[x][y].tegn = 'X';

    /* Send the map */
    show_map( ch, buf );
}
