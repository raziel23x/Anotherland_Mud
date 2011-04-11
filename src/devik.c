#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "merc.h"

int colorlen( const char *string )
{
    char buf[MAX_STRING_LENGTH];
    int i, count=0;

    if (!string) return -1;

    strcpy(buf,string);
    for ( i = 0; buf[i]; i++ )
        if ( buf[i] == '#' )
    {
        if ( buf[i+1] == '\0' )
        {
            count += 1;
            break;
        }
        else
        {
            count+=2;
            i++;
            continue;
        }
    }

    return --i;
}

char *strcut( char *string, int length )
{
    char buf[MAX_STRING_LENGTH];
    int i, count=0;

    if ( !string ) return 0;

    strcpy(buf,string);
    for ( i = 0; buf[i]; i++ )
    {
        if ( (i-count) == length ) break;
        if ( buf[i] == '#' )
        {
            if ( buf[i+1] == '\0' )
            {
                count += 1;
                break;
            }
            else
            {
                count += 2;
                continue;
            }
        }
    }
    if ( buf[i-1] == '#' )
        //	buf[i-1] = '\0';
        //    else buf[i] = '\0';
        i--;
    buf[i++] = '#';
    buf[i++] = 'n';
    buf[i] = '\0';
    strcpy(string,buf);
    return string;
}

char *strpad( char *string, int length )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int i, count=0;

    if ( !string ) return 0;

    strcpy(buf,string);
    for ( i = 0; buf[i]; i++ )
    {
        if ( buf[i] == '#' )
        {
            if ( buf[i+1] == '\0' )
            {
                count += 1;
                break;
            }
            else
            {
                count += 2;
                continue;
            }
        }
    }
    if ( buf[i-1] == '#' )
        buf[i-1] = '\0';
    else buf[i] = '\0';

    if ( (i-count) < length )
    {
        sprintf(buf2,"%*s",(length+count),buf);
        strcpy(string,buf2);
    } else strcpy(string,buf);

    return string;
}

char *strcutpad( char *string, int length )
{
    if ( colorlen(string) > length )
        return strcut(string,length);
    else if ( colorlen(string) < length )
        return strpad(string,length);
    else return string;
}

char *strip_color( char *string )
{
    char buf[MAX_STRING_LENGTH];
    char *c;
    int i;

    if ( !string ) return 0;

    for ( c = string, i = 0; *c; c++ )
    {
        if ( *c == '#' )
        {
            if ( *(c++) == '\0' )
                break;
            else if ( *c == '#' )
            {
                buf[i++] = '#';
                buf[i++] = '#';
            }
            continue;
        }
        buf[i++] = *c;
    }
    buf[i] = '\0';
    strcpy(string,buf);
    return string;
}

char *randcolor( char *string )
{
    char buf[MAX_STRING_LENGTH];
    char *c;
    int i;

    if ( !string ) return 0;

    strip_color(string);
    for ( c = string, i = 0; *c; c++ )
    {
        if ( *c == ' ' || *c == '\n' || *c == '\r' )
        {
            buf[i++] = *c;
            continue;
        }
        if ( number_range(1,4) == 1 || c == string )
        {
            buf[i++] = '#';
            buf[i++] = 's';
        }
        if ( *c == '#' )
        {
            if ( *(c+1) == '#' )
                { buf[i++] = *c; c++; }
                else buf[i++] = '#';
        }
        buf[i++] = *c;
    }
    buf[i] = '\0';
    strcpy(string,buf);
    return string;
}

void do_fryballs( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( !argument[0] )
        return stcf(ch,"Who should fry balls?\n\r");

    if ( !(victim = get_char_world(ch,argument)) )
        return stcf(ch,"They're not here!\n\r");

    if ( IS_NPC(victim) )
        return stcf(ch,"Not on NPC's.\n\r");

    if ( !IS_SET(victim->extra, EXTRA_FRYBALLS) )
    {
        SET_BIT(victim->extra, EXTRA_FRYBALLS);
        stcf(ch,"%s is now frying balls.\n\r",victim->name);
    }
    else
    {
        REMOVE_BIT(victim->extra, EXTRA_FRYBALLS);
        stcf(ch,"%s is no longer frying balls.\n\r",victim->name);
    }

}

void do_fixhead( CHAR_DATA *ch, char *argument )
{
    char arg1[MSL];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    victim = get_char_world(ch, arg1);

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: fixhead <player name>\n\r",ch);
        return;
    }
    if (victim == NULL)
    {
        send_to_char("They must be playing.\n\r", ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("Not on NPC's\n\r", ch);
        return;
    }

    if ( !IS_HEAD(victim,LOST_HEAD) )
        return stcf(ch,"They're not a head!\n\r");

    REMOVE_BIT(victim->loc_hp[0],LOST_HEAD);
    REMOVE_BIT(victim->affected_by,AFF_POLYMORPH);
    if ( victim->morph ) free_string(victim->morph);
    stcf(ch,"%s has been fixed.\n\r",victim->name);
    stcf(victim,"You have been fixed!\n\r");
}
