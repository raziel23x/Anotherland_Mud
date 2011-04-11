/*
 * Communication storage system. Ye Jobo.
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "merc.h"

void init_channel_storage()
{
    int i, j;

    for (i = 1; i < STORE_MAX; i++)
    {
        for (j = 1; j <= MAX_STORAGE; j++)
        {
            old_messages[i].last[j] = str_dup("");
        }
    }
}

void update_channel_storage(char *buf, int chan)
{
    char xbuf[MAX_STRING_LENGTH];
    /*  char tbuf[20];*/
    int i;

    /*  if (chan != STORE_TELL)
      {
        sprintf(tbuf, "%-18.18s", buf);
        sprintf(xbuf, "%s %s", tbuf, line_indent(&buf[19], 15, 85));
      }
      else */
    sprintf(xbuf, buf);                                     

    if (chan >= STORE_MAX)
    {
        bug("update_channel_storage : Bad channel", 0);
        return;
    }

    for (i = MAX_STORAGE; i > 1; i--)
    {
        free_string(old_messages[chan].last[i]);
        old_messages[chan].last[i] = str_dup(old_messages[chan].last[i-1]);
    }
    free_string(old_messages[chan].last[1]);
    old_messages[chan].last[1] = str_dup(xbuf);
}

void do_lastmessage(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (!str_cmp(arg, "chat")) showchannel(ch, STORE_CHAT);
    else if (!str_cmp(arg, "newbie")) showchannel(ch, STORE_NEWBIE);
    else if (!str_cmp(arg, "flame")) showchannel(ch, STORE_FLAME);
    else if (!str_cmp(arg, "immt") && (ch->level > 8)) showchannel(ch, STORE_IMMT);
    else if (!str_cmp(arg, "roleplay")) showchannel(ch, STORE_ROLEPLAY);
    else if (!str_cmp(arg, "ooc")) showchannel(ch, STORE_OOC);
    //  else if (!str_cmp(arg, "tell") && ch->level > 6) showchannel(ch, STORE_TELL);
    else if (ch->level < 7) send_to_char("Show the last chat/newbie/ooc/flame/roleplay messages ?\n\r", ch);
    else send_to_char("Show the last chat/newbie/ooc/flame/roleplay/immt messages ?\n\r", ch);
    return;
}

void showchannel(CHAR_DATA *ch, int chan)
{
    char buf[MAX_STRING_LENGTH];
    int i;

    send_to_char(" #R=====#0[#7History#0]#R=====#n\n\r\n\r", ch);
    for (i = MAX_STORAGE; i > 0; i--)
    {
        if (strlen(old_messages[chan].last[i]) < 1) continue;
        sprintf(buf, "%s\n\r", old_messages[chan].last[i]);
        send_to_char(buf, ch);
    }
}
