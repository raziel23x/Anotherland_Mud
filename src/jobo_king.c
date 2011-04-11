/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

/***********************************
 * Dystopian Kingdom Code, by Jobo *
 ***********************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void imm_kset     args (( CHAR_DATA *ch, char *argument ));
char *caCanKill(CHAR_DATA *ch, int nNum );

void do_krecall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *location;

    if (IS_NPC(ch)) return;

    if(ch->pcdata->kingdom == 0)
    {
        send_to_char("You do not belong to a kingdom\n\r", ch);
        return;
    }

    act( "$n's body flickers with green energy.", ch, NULL, NULL, TO_ROOM );
    act( "Your body flickers with green energy.", ch, NULL, NULL, TO_CHAR );

    if ( ch->position == POS_FIGHTING)
    {
        send_to_char( "You can't recall while fighting.\n\r",ch);
        return;
    }

    if ( !IS_NPC(ch) && ch->fight_timer > 0 )
    {
        send_to_char( "You cannot recall with a fight timer.\n\r",ch);
        return;
    }

    if ( ( location = get_room_index( kingdom_table[ch->pcdata->kingdom].recall ) ) == NULL )
    {
        send_to_char( "You are completely lost.\n\r", ch );
        return;
    }
    /*
        if ( IS_AFFECTED(ch, AFF_DARKNESS))
        {
        REMOVE_BIT(ch->affected_by, AFF_DARKNESS);
        return;
        }
    */
    if ( ch->in_room == location )
        return;

    if ( (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
        ||   IS_AFFECTED(ch, AFF_CURSE)) && (ch->level < 6) )
    {
        send_to_char( "You are unable to recall.\n\r", ch );
        return;
    }

    if (ch->fight_timer > 0)
    {
        send_to_char("Not with a fighttimer.\n\r",ch);
        return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_CHANT))
    {
        send_to_char( "You are unable to chant, its a no chant room.\n\r", ch );
        return;
    }

    if ( IS_SET(ch->flag2, AFF_TOTALBLIND) )
    {
        stc("You are unable to recall.\n\r",ch);
        return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
        if ( number_bits( 1 ) == 0 )
        {
            WAIT_STATE( ch, 4 );
            sprintf( buf, "You failed!\n\r" );
            send_to_char( buf, ch );
            return;
        }
        sprintf( buf, "You recall from combat!\n\r");
        send_to_char( buf, ch );
        stop_fighting( ch, TRUE );
    }

    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
}

void save_kingdoms()
{
    int i;
    FILE *fp;

    if ((fp = fopen("../txt/kingdoms.txt","w")) == NULL)
    {
        log_string("Error writing to kingdoms.txt");
        return;
    }
    for (i = 1; i <= MAX_KINGDOM; i++)
    {
        fprintf(fp, "%s~\n", kingdom_table[i].name);
        fprintf(fp, "%s~\n", kingdom_table[i].whoname);
        fprintf(fp, "%s~\n", kingdom_table[i].leader);
        fprintf(fp, "%s~\n", kingdom_table[i].general);
        fprintf(fp, "%d\n", kingdom_table[i].recall);
        fprintf(fp, "%d %d %d\n",
            kingdom_table[i].kills, kingdom_table[i].deaths, kingdom_table[i].qps);
        fprintf(fp, "%d %d %d %d\n",
            kingdom_table[i].req_hit, kingdom_table[i].req_move,
            kingdom_table[i].req_mana, kingdom_table[i].req_qps);
    }
    fclose(fp);
}

void load_kingdoms()
{
    int i;
    FILE *fp;

    kingdom_table[0].name = "";
    kingdom_table[0].whoname = "";
    kingdom_table[0].leader = "";
    kingdom_table[0].general = "";
    kingdom_table[0].recall = 0;
    kingdom_table[0].kills = 0;
    kingdom_table[0].deaths = 0;
    kingdom_table[0].qps = 0;
    kingdom_table[0].req_hit = 0;
    kingdom_table[0].req_move = 0;
    kingdom_table[0].req_mana = 0;
    kingdom_table[0].req_qps = 0;

    if ((fp = fopen("../txt/kingdoms.txt", "r")) == NULL)
    {
        log_string("Fatal Error: kingdoms.txt not found!");
        exit(1);
    }
    for (i = 1; i <= MAX_KINGDOM; i++)
    {
        kingdom_table[i].name = fread_string(fp);
        kingdom_table[i].whoname = fread_string(fp);
        kingdom_table[i].leader = fread_string(fp);
        kingdom_table[i].general = fread_string(fp);
        kingdom_table[i].recall = fread_number(fp);
        kingdom_table[i].kills = fread_number(fp);
        kingdom_table[i].deaths = fread_number(fp);
        kingdom_table[i].qps = fread_number(fp);
        kingdom_table[i].req_hit = fread_number(fp);
        kingdom_table[i].req_move = fread_number(fp);
        kingdom_table[i].req_mana = fread_number(fp);
        kingdom_table[i].req_qps = fread_number(fp);
    }
    fclose(fp);
}

void do_kingdoms(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char pkratio[20];                                       // perhaps some colors :)
    int i;

    if (IS_NPC(ch)) return;

    /*  sprintf(buf, "Name	       Pkills	Pdeaths   Ratio LPk : UPk  Req:hp    move    mana     qps      Treasury\n\r");*/
    sprintf(buf, "Name            Kills  Deaths   Ratio  LPK : UPK  Req:(===HP=== ==MOVE== ==MANA== ===QP===)    Treasury\n\r");
    for (i = 1; i <= MAX_KINGDOM; i++)
    {
        /*
         * calculation ratio
         */
        if (kingdom_table[i].kills > 0)
        {
            if (100 * kingdom_table[i].kills/(kingdom_table[i].kills + kingdom_table[i].deaths) < 100)
                sprintf(pkratio, "0.%-2d",
                    (100 * kingdom_table[i].kills/(kingdom_table[i].kills + kingdom_table[i].deaths)));
            else
                sprintf(pkratio, "1.00");
        }
        else sprintf(pkratio, "0.00");

        /*
         * buffering everything
         */
        /*    sprintf(buf2, "%s%-14s  %4d       %4d    %s %3d : %3d	%6d  %6d  %6d  %6d  %8d#n\n\r",
            caCanKill(ch,i), kingdom_table[i].name, kingdom_table[i].kills, kingdom_table[i].deaths, pkratio,
                kingdom_table[i].kills-kingdom_table[i].deaths-10, kingdom_table[i].kills-kingdom_table[i].deaths+10,
              kingdom_table[i].req_hit, kingdom_table[i].req_move, kingdom_table[i].req_mana, kingdom_table[i].req_qps,
        kingdom_table[i].qps);*/
        sprintf(buf2, "%-14s   %4d    %4d    %s  %s%3d : %3d#n       %8d %8d %8d %8d   %10d#n\n\r",
            kingdom_table[i].name, kingdom_table[i].kills, kingdom_table[i].deaths, pkratio, caCanKill(ch,i),
            kingdom_table[i].kills-kingdom_table[i].deaths-10, kingdom_table[i].kills-kingdom_table[i].deaths+10,
            kingdom_table[i].req_hit, kingdom_table[i].req_move, kingdom_table[i].req_mana, kingdom_table[i].req_qps, kingdom_table[i].qps);

        strcat(buf, buf2);
    }
    send_to_char(buf, ch);
    return;
}

char *caCanKill(CHAR_DATA *ch, int nNum )
{
    int nBaseCh = kingdom_table[ch->pcdata->kingdom].kills-kingdom_table[ch->pcdata->kingdom].deaths;
    int nBaseVict = kingdom_table[nNum].kills-kingdom_table[nNum].deaths;
    int nRange = 20;
    if ( IS_NPC(ch) || ch->pcdata->kingdom == 0 ) return "#0";
    if ( ch->pcdata->kingdom == nNum ) return "#y";
    if (nBaseCh > nBaseVict )
        if ( nBaseCh > ( nBaseVict + nRange ) ) return "#0";
    if (nBaseCh < nBaseVict )
        if ( (nBaseCh + nRange) < nBaseVict ) return "#0";
    return "#R";
}

void do_kinduct(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (ch->pcdata->kingdom == 0)
    {
        send_to_char("What kingdoms did you say you where from ?\n\r", ch);
        return;
    }
    if (str_cmp(kingdom_table[ch->pcdata->kingdom].leader, ch->name) &&
        str_cmp(kingdom_table[ch->pcdata->kingdom].general, ch->name))
    {
        send_to_char("Talk to your kingdom leaders, they are the only persons allowed to induct new members.\n\r", ch);
        return;
    }
    if ((victim = get_char_room(ch, arg)) == NULL)
    {
        send_to_char("They are not here.\n\r", ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("That's silly.\n\r", ch);
        return;
    }
    if (victim->pcdata->kingdom != 0)
    {
        send_to_char("They are already a member of another kingdom.\n\r", ch);
        return;
    }
    if (victim->max_hit < kingdom_table[ch->pcdata->kingdom].req_hit ||
        victim->max_move < kingdom_table[ch->pcdata->kingdom].req_move ||
        victim->pcdata->quest < kingdom_table[ch->pcdata->kingdom].req_qps ||
        victim->max_mana < kingdom_table[ch->pcdata->kingdom].req_mana)
    {
        send_to_char("They don't have the reqs to get inducted.\n\r", ch);
        return;
    }
    if (!IS_SET(victim->pcdata->jflags, JFLAG_WANT_KINGDOM))
    {
        send_to_char("They don't want to be inducted.\n\r", ch);
        return;
    }
    victim->pcdata->kingdom = ch->pcdata->kingdom;
    victim->pcdata->quest -= kingdom_table[ch->pcdata->kingdom].req_qps;
    kingdom_table[ch->pcdata->kingdom].qps += kingdom_table[ch->pcdata->kingdom].req_qps;
    send_to_char("You induct them into your kingdom.\n\r", ch);
    send_to_char("You have been inducted.\n\r", victim);
    save_kingdoms();
    return;
}

void do_wantkingdom(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;

    if (IS_SET(ch->pcdata->jflags, JFLAG_WANT_KINGDOM))
    {
        REMOVE_BIT(ch->pcdata->jflags, JFLAG_WANT_KINGDOM);
        send_to_char("You can no longer be inducted.\n\r", ch);
        return;
    }
    SET_BIT(ch->pcdata->jflags, JFLAG_WANT_KINGDOM);
    send_to_char("You can now be inducted.\n\r", ch);
    return;
}

void do_treasury(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf2[MAX_INPUT_LENGTH];
    int i;

    if (IS_NPC(ch) || ch->level < 1)
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if ( ch->pcdata->kingdom == 0 )
    {
        stc("You are not in a kingdom.\n\r",ch);
        return;
    }
    if (arg1[0] == '\0')
    {
        send_to_char("Syntax : treasury <amount>\n\r", ch);
        return;
    }
    if ((i = atoi(arg1)) > ch->pcdata->quest)
    {
        send_to_char("You don't have enough QPs to put into a treasury.\n\r", ch);
        return;
    }
    if ((i = atoi(arg1)) < 100)
    {
        send_to_char("Min value is 100 QPs.\n\r", ch);
        return;
    }
    kingdom_table[ch->pcdata->kingdom].qps += i;
    ch->pcdata->quest -= i;
    send_to_char("Ok, you have given QP's to the treasury.\n\r", ch);
    sprintf(buf2, "%s puts %d qps into %s's treasury now totaling %d!",ch->name, i, kingdom_table[ch->pcdata->kingdom].whoname, kingdom_table[ch->pcdata->kingdom].qps);
    do_info(NULL,buf2);
    save_kingdoms();
    return;
}

void do_kbuy(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char buf2[MAX_INPUT_LENGTH];
    int i;
    int a;

    if (IS_NPC(ch) || ch->level < 1)
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    a = atoi(arg2);
    i = atoi(arg1);
    if (arg1[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char("Syntax : kbuy <amount> <kingdom number>\n\r", ch);
        return;
    }
    if ( a > MAX_KINGDOM )
    {
        send_to_char("That kingdom doesn't exist.\n\r", ch);
        return;
    }
    if ( i > kingdom_table[a].qps )
    {
        send_to_char( "Kingdom does not have enough qps.\n\r",ch);
        return;
    }
    if ( a < 1 || a > MAX_KINGDOM )
    {
        send_to_char("Please pick a valid kingdom.\n\r", ch);
        return;
    }
    kingdom_table[a].qps -= i;
    sprintf(buf,"Ok, you have taken %dqps from %s's Treasury.\n\r", i, kingdom_table[a].whoname);
    sprintf(buf2, "%s takes %d qps from %s's treasury now totaling %d qps!",ch->name, i, kingdom_table[a].whoname, kingdom_table[a].qps);
    send_to_char(buf,ch);
    do_info(NULL,buf2);
    save_kingdoms();
    return;
}

void do_kset(CHAR_DATA *ch, char *argument)
{
    char keyword[MAX_INPUT_LENGTH];
    char value[MAX_INPUT_LENGTH];

    if (IS_NPC(ch)) return;
    if (ch->level > 6)
    {
        imm_kset(ch, argument);
        return;
    }
    argument = one_argument(argument, keyword);
    one_argument(argument, value);
    if (ch->pcdata->kingdom == 0)
    {
        send_to_char("But you are not a member of a kingdom.\n\r", ch);
        return;
    }
    if (str_cmp(kingdom_table[ch->pcdata->kingdom].leader, ch->name))
    {
        send_to_char("Only the leader can change the kingdom settings.\n\r", ch);
        return;
    }
    if (keyword[0] == '\0' || value[0] == '\0')
    {
        send_to_char("What do you want to change ?\n\r\n\r", ch);
        send_to_char("req_mana, req_hit, req_move, req_qps or general.\n\r", ch);
        return;
    }
    if (!str_cmp(keyword, "req_mana"))
    {
        kingdom_table[ch->pcdata->kingdom].req_mana = atoi(value);
    }
    else if (!str_cmp(keyword, "req_qps"))
    {
        kingdom_table[ch->pcdata->kingdom].req_qps = atoi(value);
    }
    else if (!str_cmp(keyword, "req_hit"))
    {
        kingdom_table[ch->pcdata->kingdom].req_hit = atoi(value);
    }
    else if (!str_cmp(keyword, "req_move"))
    {
        kingdom_table[ch->pcdata->kingdom].req_move = atoi(value);
    }
    else if (!str_cmp(keyword, "general"))
    {
        free_string(kingdom_table[ch->pcdata->kingdom].general);
        kingdom_table[ch->pcdata->kingdom].general = str_dup(value);
    }
    else
    {
        do_kset(ch, "");
        return;
    }
    send_to_char("Done.\n\r", ch);
    save_kingdoms();
    return;
}

void imm_kset(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int i;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if (arg1[0] == '\0' || arg2[0] == '\0' || argument[0] == '\0')
    {
        send_to_char("Syntax       : kset <kingdom> <field> <value>\n\r", ch);
        send_to_char("Valid fields : leader, name, whoname, recall, kills, deaths, load\n\r", ch);
        return;
    }
    if (!is_number(arg1))
    {
        send_to_char("Please pick a number as the kingdom.\n\r", ch);
        return;
    }
    i = atoi(arg1);
    if (i < 1 || i > MAX_KINGDOM)
    {
        send_to_char("Please pick a real kingdom.\n\r", ch);
        return;
    }
    if (!str_cmp(arg2, "leader"))
    {
        free_string(kingdom_table[i].leader);
        argument[0] = UPPER(argument[0]);
        kingdom_table[i].leader = str_dup(argument);
    }
    else if (!str_cmp(arg2, "name"))
    {
        free_string(kingdom_table[i].name);
        argument[0] = UPPER(argument[0]);
        kingdom_table[i].name = str_dup(argument);
    }
    else if (!str_cmp(arg2, "load"))
        load_kingdoms();
    else if (!str_cmp(arg2, "whoname"))
    {
        free_string(kingdom_table[i].whoname);
        argument[0] = UPPER(argument[0]);
        kingdom_table[i].whoname = str_dup(argument);
    }
    else if (!str_cmp(arg2, "recall"))
    {
        kingdom_table[i].recall = atoi(argument);
    }
    else if (!str_cmp(arg2, "kills"))
    {
        kingdom_table[i].kills = atoi(argument);
    }
    else if (!str_cmp(arg2, "deaths"))
    {
        kingdom_table[i].deaths = atoi(argument);
    }
    else if (!str_cmp(arg2, "treasury"))
    {
        kingdom_table[i].qps = atoi(argument);
    }
    else
    {
        imm_kset(ch, "");
        return;
    }
    send_to_char("Done.\n\r", ch);
    save_kingdoms();
    return;
}

void do_koutcast(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (ch->pcdata->kingdom == 0)
    {
        send_to_char("What kingdoms did you say you where from ?\n\r", ch);
        return;
    }
    if (str_cmp(kingdom_table[ch->pcdata->kingdom].leader, ch->name) &&
        str_cmp(kingdom_table[ch->pcdata->kingdom].general, ch->name))
    {
        send_to_char("You are not allowed to outcast members.\n\r", ch);
        return;
    }
    if ((victim = get_char_room(ch, arg)) == NULL)
    {
        send_to_char("Outcast whom?\n\r", ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("That's a monster, not a player.\n\r", ch);
        return;
    }
    if (ch == victim)
    {
        send_to_char("You cannot outcast yourself.\n\r", ch);
        return;
    }
    if (victim->pcdata->kingdom != ch->pcdata->kingdom)
    {
        send_to_char("They are not a member of your kingdom.\n\r", ch);
        return;
    }
    if (!str_cmp(victim->name, kingdom_table[ch->pcdata->kingdom].leader))
    {
        send_to_char("That is not a good plan.\n\r", ch);
        return;
    }
    victim->pcdata->kingdom = 0;
    send_to_char("Done.\n\r", ch);
    send_to_char("You have been outcasted from your kingdom.\n\r", victim);
    victim->home = 3001;
    return;
}

void do_kstats(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int i;

    if (IS_NPC(ch)) return;
    if ((i = ch->pcdata->kingdom) == 0)
    {
        send_to_char("You are not a member of any kingdom.\n\r", ch);
        return;
    }
    sprintf(buf, " [***]	The Kingdom stats of %s  [***]\n\r\n\r", kingdom_table[i].whoname);
    sprintf(buf2, " Current Leader    : %s\n\r", kingdom_table[i].leader);
    strcat(buf, buf2);
    sprintf(buf2, " Current General   : %s\n\r", kingdom_table[i].general);
    strcat(buf, buf2);
    sprintf(buf2, " Treasury	    : %d qps\n\r", kingdom_table[i].qps);
    strcat(buf, buf2);
    send_to_char(buf, ch);
    return;
}

void do_ktreasury(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int i;

    if (IS_NPC(ch)) return;

    sprintf(buf,    "Name 	Treasury\n\r");
    for (i = 1; i <= MAX_KINGDOM; i++)
    {
        /*
         * buffering everything
         */
        sprintf(buf2, "%-14s	%5d\n\r",
            kingdom_table[i].name, kingdom_table[i].qps);
        strcat(buf, buf2);
    }
    send_to_char(buf, ch);
    return;
}

void do_kingset(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int i;

    if (IS_NPC(ch) || ch->level < 7)
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);
    if (arg1[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char("Syntax : kingset <player> <kingdom number>\n\r", ch);
        return;
    }
    if ((victim = get_char_world(ch, arg1)) == NULL)
    {
        send_to_char("They are not here.\n\r", ch);
        return;
    }
    if ((i = atoi(arg2)) > MAX_KINGDOM)
    {
        send_to_char("Please pick a valid kingdom.\n\r", ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("Please pick a player.\n\r", ch);
        return;
    }
    victim->pcdata->kingdom = i;
    send_to_char("Ok.\n\r", ch);
    send_to_char("Your kingdom has been changed.\n\r", victim);
    return;
}
