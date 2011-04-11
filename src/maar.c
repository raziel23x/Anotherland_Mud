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
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/**********************************
 * New Additions by Maar (Tempest *
 *********************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

char * global_chat = "Welcome";

void do_setglobalchat(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch)) return;
    if (ch->level < 10)
    {
        send_to_char("HuH?\n\r", ch);
        return;
    }
    else if (arg1[0] == '\0')
    {
        send_to_char("Usage: setglobal New Global String (Wrap it in double quotes)\n\r", ch);
        return;
    }
    else
    {
        global_chat = str_dup(arg1);
        sprintf(buf, "Global String changed to %s", str_dup(arg1));
        send_to_char(buf, ch);
        return;
    }
}

void do_globalchat(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch)) return;
    if (ch->level < 12)
    {
        send_to_char("HuH?\n\r", ch);
        return;
    }
    /* Will add in a second param to do info or chat
      else if (arg1[0] == '\0')
      {

      }
    */
    else
    {
        sprintf(buf, global_chat);
        do_info(ch, buf);
        send_to_char("Done!\n\r", ch);
        return;
    }
}
