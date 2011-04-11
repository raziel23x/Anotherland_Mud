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
/***************************************************************************
 *                                 _/                            _/        *
 *      _/_/_/  _/_/      _/_/_/  _/    _/_/    _/    _/    _/_/_/         *
 *     _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/          *
 *    _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/           *
 *   _/    _/    _/    _/_/_/  _/    _/_/      _/_/_/    _/_/_/            *
 ***************************************************************************
 * Mindcloud Copyright 2001-2003 by Jeff Boschee (Zarius),                 *
 * Additional credits are in the help file CODECREDITS                     *
 * All Rights Reserved.                                                    *
 ***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "merc.h"
//#include "interp.h"
#include "olc.h"                                            // flag_string()

/* this is the minimum level needed by the immortal
 * such that any changes are saved. Lower level
 * immortals can still use the command, but changes
 * are not saved in the pfile.
 */
#define JOPE_SAVE_LEVEL        10

const struct jope_type jope_table [] =
{

    /* name,          function,      level */

    { "act",          jope_act,        10 },
    { "action",       jope_action,     12 },
    { "done",         jope_done,        7 },
    { "drop",         jope_drop,       10 },
    { "equipment",    jope_equipment,   7 },
    { "exp",          jope_exp,         7 },
    { "extra",        jope_extra,      10 },
    { "fix",          jope_fix,         7 },
    { "get",          jope_get,        10 },
    { "hit",          jope_hit,        10 },
    { "inventory",    jope_inventory,   7 },
    { "level",        jope_level,      12 },
    { "look",         jope_look,        7 },
    { "jflags",       jope_jflags,     10 },
    { "mana",         jope_mana,       10 },
    { "mdeath",       jope_mdeath,     10 },
    { "mkill",        jope_mkill,      10 },
    { "move",         jope_move,       10 },
    { "newbits",      jope_newbits,    10 },
    { "pdeath",       jope_pdeath,     10 },
    { "pkill",        jope_pkill,      10 },
    { "primal",       jope_primal,     10 },
    { "qps",        jope_qps,        10 },
    { "show",         jope_show,        7 },
    { "remove",       jope_remove,     10 },
    { "spells",       jope_spells,     10 },
    { "stances",      jope_stances,    10 },
    { "trust",        jope_trust,      12 },
    { "weapons",      jope_weapons,    10 },
    { "wear",         jope_wear,       10 },
    { "?",            jope_list,        7 },

    /*
     * End of table.
     */
    { "", 0, 0 }
};

const struct flag_type jflags_table [] =
{
    { "bully",       JFLAG_BULLY,       TRUE },
    { "noset",       JFLAG_NOSET,       TRUE },
    //  { "policy",      JFLAG_POLICY,      TRUE },
    { "setavatar",   JFLAG_SETAVATAR,   TRUE },
    { "setdecap",    JFLAG_SETDECAP,    TRUE },
    { "setlogin",    JFLAG_SETLOGOUT,   TRUE },
    { "setlogout",   JFLAG_SETLOGOUT,   TRUE },
    { "settie",      JFLAG_SETTIE,      TRUE },
    { "ss1",         JFLAG_SS1,         TRUE },
    { "ss2",         JFLAG_SS2,         TRUE },
    { "ss3",         JFLAG_SS3,         TRUE },
    { "ss4",       JFLAG_SS4,         TRUE },
    { "ss5",      JFLAG_SS5,         TRUE },
    { "forging",     JFLAG_KNOWS_FORGING, TRUE },

    /*
     * End of table.
     */
    { "", 0 }
};

const struct flag_type extra_table [] =
{
    { "afk",         EXTRA_AFK,        TRUE },
    { "blindfolded", BLINDFOLDED,      TRUE },
    { "born",        EXTRA_BORN,       TRUE },
    { "call_all",    EXTRA_CALL_ALL,   TRUE },
    { "done",        EXTRA_DONE,       TRUE },
    { "exp",         EXTRA_EXP,        TRUE },
    { "gagged",      GAGGED,           TRUE },
    { "labour",      EXTRA_LABOUR,     TRUE },
    { "newpass",     EXTRA_NEWPASS,    TRUE },
    { "oswitch",     EXTRA_OSWITCH,    TRUE },
    { "pregnant",    EXTRA_PREGNANT,   TRUE },
    { "prompt",      EXTRA_PROMPT,     TRUE },
    { "tied-up",     TIED_UP,          TRUE },

    /*
     * End of table.
     */
    { "", 0 }
};

const struct flag_type act_table [] =
{

    { "ambidex",     PLR_AMBI, TRUE      },
    { "ansi",        PLR_ANSI ,TRUE     },
    { "brief1",      PLR_BRIEF,TRUE     },
    { "brief2",      PLR_BRIEF2 ,TRUE   },
    { "brief3",      PLR_BRIEF3,TRUE     },
    { "brief4",      PLR_BRIEF4,TRUE     },
    { "brief5",      PLR_BRIEF5,TRUE     },
    { "deny",        PLR_DENY ,TRUE      },
    { "freeze",      PLR_FREEZE ,TRUE    },
    { "holylight",   PLR_HOLYLIGHT,TRUE  },
    { "lefthand",    PLR_LEFTHAND,TRUE   },
    { "righthand",   PLR_RIGHTHAND,TRUE  },
    { "silence",     PLR_SILENCE,TRUE    },
    { "wizinvis",    PLR_WIZINVIS ,TRUE  },

    /*
     * End of table.
     */
    { "", 0 }
};
const struct flag_type newbits_table [] =
{

    { "darkness",      NEW_DARKNESS,TRUE  },
    { "fourth",        FOURTH_HAND,TRUE   },
    { "mastery",       NEW_MASTERY,TRUE   },
    { "third",         THIRD_HAND ,TRUE   },

    /*
     * End of table.
     */
    { "", 0 }
};

void jope_newbits(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int iCount, cmd = 0;
    bool found = FALSE;

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Set which newbit ?\n\r\n\r", ch);
        for (;;)
        {
            iCount = 0;
            sprintf(buf, "  ");
            while (newbits_table[cmd].name[0] != '\0' && iCount < 5)
            {
                sprintf(buf2, "%-12s ", newbits_table[cmd].name);
                strcat(buf, buf2);
                cmd++;
                iCount++;
            }
            strcat(buf, "\n\r");
            send_to_char(buf, ch);
            if (newbits_table[cmd].name[0] == '\0') break;
        }
        return;
    }
    for (cmd = 0; newbits_table[cmd].name[0] != '\0'; cmd++)
    {
        if (arg[0] == newbits_table[cmd].name[0] && !str_prefix(arg, newbits_table[cmd].name))
        {
            found = TRUE;
            break;
        }
    }
    if (!found)
    {
        send_to_char("No such newbit.\n\r", ch);
        return;
    }
    if (IS_SET(victim->newbits, newbits_table[cmd].bit))
        REMOVE_BIT(victim->newbits, newbits_table[cmd].bit);
    else SET_BIT(victim->newbits, newbits_table[cmd].bit);
    send_to_char("Ok.\n\r", ch);
    return;
}

void jope_act(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int iCount, cmd = 0;
    bool found = FALSE;

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Set which act bit ?\n\r\n\r", ch);
        for (;;)
        {
            iCount = 0;
            sprintf(buf, "  ");
            while (act_table[cmd].name[0] != '\0' && iCount < 5)
            {
                sprintf(buf2, "%-12s ", act_table[cmd].name);
                strcat(buf, buf2);
                cmd++;
                iCount++;
            }
            strcat(buf, "\n\r");
            send_to_char(buf, ch);
            if (act_table[cmd].name[0] == '\0') break;
        }
        return;
    }
    for (cmd = 0; act_table[cmd].name[0] != '\0'; cmd++)
    {
        if (arg[0] == act_table[cmd].name[0] && !str_prefix(arg, act_table[cmd].name))
        {
            found = TRUE;
            break;
        }
    }
    if (!found)
    {
        send_to_char("No such act bit.\n\r", ch);
        return;
    }
    if (IS_SET(victim->act, act_table[cmd].bit))
        REMOVE_BIT(victim->act, act_table[cmd].bit);
    else SET_BIT(victim->act, act_table[cmd].bit);
    send_to_char("Ok.\n\r", ch);
    return;
}

void jope_jflags(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int iCount, cmd = 0;
    bool found = FALSE;

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Set which jflag ?\n\r\n\r", ch);
        for (;;)
        {
            iCount = 0;
            sprintf(buf, "  ");
            while (jflags_table[cmd].name[0] != '\0' && iCount < 5)
            {
                sprintf(buf2, "%-12s ", jflags_table[cmd].name);
                strcat(buf, buf2);
                cmd++;
                iCount++;
            }
            strcat(buf, "\n\r");
            send_to_char(buf, ch);
            if (jflags_table[cmd].name[0] == '\0') break;
        }
        return;
    }
    for (cmd = 0; jflags_table[cmd].name[0] != '\0'; cmd++)
    {
        if (arg[0] == jflags_table[cmd].name[0] && !str_prefix(arg, jflags_table[cmd].name))
        {
            found = TRUE;
            break;
        }
    }
    if (!found)
    {
        send_to_char("No such jflag.\n\r", ch);
        return;
    }
    if (IS_SET(victim->pcdata->jflags, jflags_table[cmd].bit))
        REMOVE_BIT(victim->pcdata->jflags, jflags_table[cmd].bit);
    else SET_BIT(victim->pcdata->jflags, jflags_table[cmd].bit);
    send_to_char("Ok.\n\r", ch);
    return;
}

void jope_extra(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int iCount, cmd = 0;
    bool found = FALSE;

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Set which extra bit ?\n\r\n\r", ch);
        for (;;)
        {
            iCount = 0;
            sprintf(buf, "  ");
            while (extra_table[cmd].name[0] != '\0' && iCount < 5)
            {
                sprintf(buf2, "%-12s ", extra_table[cmd].name);
                strcat(buf, buf2);
                cmd++;
                iCount++;
            }
            strcat(buf, "\n\r");
            send_to_char(buf, ch);
            if (extra_table[cmd].name[0] == '\0') break;
        }
        return;
    }
    for (cmd = 0; extra_table[cmd].name[0] != '\0'; cmd++)
    {
        if (arg[0] == extra_table[cmd].name[0] && !str_prefix(arg, extra_table[cmd].name))
        {
            found = TRUE;
            break;
        }
    }
    if (!found)
    {
        send_to_char("No such extra bit.\n\r", ch);
        return;
    }
    if (IS_SET(victim->extra, extra_table[cmd].bit))
        REMOVE_BIT(victim->extra, extra_table[cmd].bit);
    else SET_BIT(victim->extra, extra_table[cmd].bit);
    send_to_char("Ok.\n\r", ch);
    return;
}

void jope_action(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    interpret(victim, argument);
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_inventory(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    do_inventory(victim, "");
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_wear(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    do_wear(victim, argument);
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_drop(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    do_drop(victim, argument);
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_equipment(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    do_equipment(victim, argument);
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_get(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    do_get(victim, argument);
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_look(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    do_look(victim, argument);
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_remove(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    ch->desc->connected = CON_PLAYING;
    victim->desc = ch->desc;
    do_remove(victim, argument);
    victim->desc = NULL;
    ch->desc->connected = CON_PFILE;
    return;
}

void jope_qps(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the qps.\n\r", ch);
        return;
    }
    victim->pcdata->quest = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_primal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the primal.\n\r", ch);
        return;
    }
    victim->practice = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_level(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the level.\n\r", ch);
        return;
    }
    victim->level = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_trust(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the trust.\n\r", ch);
        return;
    }
    victim->trust = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_hit(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the hitpoints.\n\r", ch);
        return;
    }
    victim->max_hit = atoi(arg);
    victim->hit     = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_mana(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the mana.\n\r", ch);
        return;
    }
    victim->max_mana = atoi(arg);
    victim->mana     = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_move(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the move.\n\r", ch);
        return;
    }
    victim->max_move = atoi(arg);
    victim->move     = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_mdeath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the mdeaths.\n\r", ch);
        return;
    }
    victim->mdeath  = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_mkill(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the mkills.\n\r", ch);
        return;
    }
    victim->mkill  = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_pdeath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the pdeaths.\n\r", ch);
        return;
    }
    victim->pdeath  = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_pkill(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the pkills.\n\r", ch);
        return;
    }
    victim->pkill  = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_stances(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int value;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (arg1[0] == '\0')
    {
        send_to_char("What stance do you wish to change ?\n\r", ch);
        return;
    }
    if (!is_number(arg2))
    {
        send_to_char("What do you wish to set the stance to (a number) ?\n\r", ch);
        return;
    }
    value = atoi(arg2);
    if (!str_prefix(arg1, "viper"))
        victim->stance[STANCE_VIPER] = value;
    else if (!str_prefix(arg1, "bull"))
        victim->stance[STANCE_BULL] = value;
    else if (!str_prefix(arg1, "crane"))
        victim->stance[STANCE_CRANE] = value;
    else if (!str_prefix(arg1, "crab"))
        victim->stance[STANCE_CRAB] = value;
    else if (!str_prefix(arg1, "mongoose"))
        victim->stance[STANCE_MONGOOSE] = value;
    else if (!str_prefix(arg1, "mantis"))
        victim->stance[STANCE_MANTIS] = value;
    else if (!str_prefix(arg1, "dragon"))
        victim->stance[STANCE_DRAGON] = value;
    else if (!str_prefix(arg1, "tiger"))
        victim->stance[STANCE_TIGER] = value;
    else if (!str_prefix(arg1, "monkey"))
        victim->stance[STANCE_MONKEY] = value;
    else if (!str_prefix(arg1, "swallow"))
        victim->stance[STANCE_SWALLOW] = value;
    else
    {
        send_to_char("No such stance.\n\r", ch);
        return;
    }
    send_to_char("Ok.\n\r", ch);
    return;
}

void jope_weapons(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int value;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (arg1[0] == '\0')
    {
        send_to_char("What weapon do you wish to change ?\n\r", ch);
        return;
    }
    if (!is_number(arg2))
    {
        send_to_char("What do you wish to set the weapon to (a number) ?\n\r", ch);
        return;
    }
    value = atoi(arg2);
    if (!str_prefix(arg1, "unarmed"))
        victim->wpn[0] = value;
    else if (!str_prefix(arg1, "slice"))
        victim->wpn[1] = value;
    else if (!str_prefix(arg1, "stab"))
        victim->wpn[2] = value;
    else if (!str_prefix(arg1, "slash"))
        victim->wpn[3] = value;
    else if (!str_prefix(arg1, "whip"))
        victim->wpn[4] = value;
    else if (!str_prefix(arg1, "claw"))
        victim->wpn[5] = value;
    else if (!str_prefix(arg1, "blast"))
        victim->wpn[6] = value;
    else if (!str_prefix(arg1, "pound"))
        victim->wpn[7] = value;
    else if (!str_prefix(arg1, "crush"))
        victim->wpn[8] = value;
    else if (!str_prefix(arg1, "grep"))
        victim->wpn[9] = value;
    else if (!str_prefix(arg1, "bite"))
        victim->wpn[10] = value;
    else if (!str_prefix(arg1, "pierce"))
        victim->wpn[11] = value;
    else if (!str_prefix(arg1, "suck"))
        victim->wpn[12] = value;
    else
    {
        send_to_char("No such weapontype\n\r", ch);
        return;
    }
    send_to_char("Ok.\n\r", ch);
    return;
}

void jope_spells(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int value;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (arg1[0] == '\0')
    {
        send_to_char("What spellcolor do you wish to change ?\n\r", ch);
        return;
    }
    if (!is_number(arg2))
    {
        send_to_char("What do you wish to set the spell to (a number) ?\n\r", ch);
        return;
    }
    value = atoi(arg2);
    if (!str_prefix(arg1, "purple"))
        victim->spl[PURPLE_MAGIC] = value;
    else if (!str_prefix(arg1, "red"))
        victim->spl[RED_MAGIC] = value;
    else if (!str_prefix(arg1, "blue"))
        victim->spl[BLUE_MAGIC] = value;
    else if (!str_prefix(arg1, "green"))
        victim->spl[GREEN_MAGIC] = value;
    else if (!str_prefix(arg1, "yellow"))
        victim->spl[YELLOW_MAGIC] = value;
    else
    {
        send_to_char("No such spellcolor\n\r", ch);
        return;
    }
    send_to_char("Ok.\n\r", ch);
    return;
}

void jope_exp(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Please use a number for the exp.\n\r", ch);
        return;
    }
    victim->exp = atoi(arg);
    send_to_char("Done.\n\r", ch);
    return;
}

void jope_list(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int cmd = 0, iCount;

    for (;;)
    {
        iCount = 0;
        sprintf(buf, "  ");
        while (jope_table[cmd].name[0] != '\0' && iCount < 5)
        {
            if (jope_table[cmd].level > ch->level)
            {
                cmd++;
                continue;
            }
            sprintf(buf2, "%-12s ", jope_table[cmd].name);
            strcat(buf, buf2);
            cmd++;
            iCount++;
        }
        strcat(buf, "\n\r");
        send_to_char(buf, ch);
        if (jope_table[cmd].name[0] == '\0') break;
    }
}

void jope_show(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char bufbanner[MAX_STRING_LENGTH];
    char line[MAX_STRING_LENGTH];

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    sprintf(line, "\n\r");
    sprintf(bufbanner, "<----------------=============={ J.O.P.E }==============---------------->\n\r");
    send_to_char(bufbanner, ch);
    send_to_char(line, ch);

    sprintf(buf, "  Exp : %d   Qps : %d   Primal : %d   Level : %d   Trust : %d\n\r",
        victim->exp, victim->pcdata->quest, victim->practice, victim->level, victim->trust);
    send_to_char(buf, ch);
    sprintf(buf, "  Hit : %d/%d   Move : %d/%d   Mana : %d/%d\n\r",
        victim->hit, victim->max_hit, victim->move, victim->max_move, victim->mana, victim->max_mana);
    send_to_char(buf, ch);
    sprintf(buf, "  Pkill : %d   Pdeath : %d   Mkill : %d   Mdeath : %d\n\r",
        victim->pkill, victim->pdeath, victim->mkill, victim->mdeath);
    send_to_char(buf, ch);
    send_to_char(line, ch);
    sprintf(buf, "  Purple : %3d  Red    : %3d  Blue   : %3d  Green  : %3d  Yellow : %3d\n\r",
        victim->spl[PURPLE_MAGIC], victim->spl[RED_MAGIC], victim->spl[BLUE_MAGIC],
        victim->spl[GREEN_MAGIC], victim->spl[YELLOW_MAGIC]);
    send_to_char( buf, ch );
    send_to_char(line, ch);
    sprintf(buf, "  Viper  : %3d  Crane  : %3d  Crab   : %3d  Mong   : %3d  Bull   : %3d\n\r",
        victim->stance[STANCE_VIPER], victim->stance[STANCE_CRANE], victim->stance[STANCE_CRAB],
        victim->stance[STANCE_MONGOOSE], victim->stance[STANCE_BULL]);
    send_to_char( buf, ch );
    sprintf(buf, "  Mantis : %3d  Dragon : %3d  Tiger  : %3d  Monkey : %3d  Swall  : %3d\n\r",
        victim->stance[STANCE_MANTIS], victim->stance[STANCE_DRAGON], victim->stance[STANCE_TIGER],
        victim->stance[STANCE_MONKEY], victim->stance[STANCE_SWALLOW]);
    send_to_char( buf, ch );
    send_to_char(line, ch);
    sprintf(buf, "  Unarm : %4d  Slice : %4d  Stab  : %4d  Slash : %4d  Whip  : %4d\n\r",
        victim->wpn[0], victim->wpn[1], victim->wpn[2], victim->wpn[3], victim->wpn[4]);
    send_to_char( buf, ch );
    sprintf(buf, "  Claw  : %4d  Blast : %4d  Pound : %4d  Crush : %4d  Grep  : %4d\n\r",
        victim->wpn[5], victim->wpn[6], victim->wpn[7], victim->wpn[8], victim->wpn[9]);
    send_to_char( buf, ch );
    sprintf(buf, "  Bite  : %4d  Pierc : %4d  Suck  : %4d\n\r",
        victim->wpn[10], victim->wpn[11], victim->wpn[12]);
    send_to_char( buf, ch );
    send_to_char(line, ch);

    sprintf(buf, "  Act     : %s\n\r", flag_string(act_table, victim->act));
    send_to_char(buf, ch);
    sprintf(buf, "  Newbits : %s\n\r", flag_string(newbits_table, victim->newbits));
    send_to_char(buf, ch);
    sprintf(buf, "  Jflags  : %s\n\r", flag_string(jflags_table, victim->pcdata->jflags));
    send_to_char(buf, ch);
    sprintf(buf, "  Extra   : %s\n\r", flag_string(extra_table, victim->extra));
    send_to_char(buf, ch);

    send_to_char(line, ch);
    send_to_char(bufbanner, ch);
    return;
}

void jope_fix(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing anyone.\n\r", ch);
        return;
    }
    free_string(victim->morph);
    victim->morph = str_dup("");
    victim->in_room->vnum = ROOM_VNUM_ALTAR;
    victim->loc_hp[0] = 0;
    REMOVE_BIT(victim->affected_by,AFF_POLYMORPH);
    victim->pcdata->chobj = NULL;
    victim->position = POS_STANDING;
    send_to_char("Pfile fixed.\n\r", ch);
}

void jope_done(CHAR_DATA *ch, char *argument)
{
    jope_free_char(ch);
    if (!ch->desc) return;
    ch->desc->connected = CON_PLAYING;
    send_to_char("Ok, pfile saved.\n\r", ch);
    return;
}

void jope_interp(CHAR_DATA *ch, char *argument)
{
    char command[MAX_INPUT_LENGTH];
    bool found = FALSE;
    int cmd;

    while (isspace(*argument)) argument++;
    if (argument[0] == '\0') return;

    argument = one_argument(argument, command);

    /*
     * Let's locate the command
     */
    for (cmd = 0; jope_table[cmd].name[0] != '\0'; cmd++)
    {
        if (command[0] == jope_table[cmd].name[0]
            && !str_prefix(command, jope_table[cmd].name)
            && jope_table[cmd].level <= ch->level)
        {
            found = TRUE;
            break;
        }
    }

    /*
     * Let's get a prompt.
     */
    if (ch->desc != NULL) write_to_buffer(ch->desc,"\n\r", 2);

    if (!found)
    {
        send_to_char("No such J.O.P.E command.\n\r", ch);
        return;
    }

    (*jope_table[cmd].do_fun) (ch, argument);

    return;
}

/*
 * The only non-jope command.
 */
void do_pfile(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (!ch->desc) return;
    if (strlen(arg) < 3)
    {
        send_to_char("Which pfile do you wish to edit?\n\r", ch);
        return;
    }
    arg[0] = UPPER(arg[0]);
    if (!jope_load_char(ch, arg)) return;
    ch->desc->connected = CON_PFILE;
    send_to_char("Pfile opened, your in edit mode.\n\r", ch);
    if (ch->pcdata->pfile->level > ch->level)
    {
        send_to_char("You cannot edit pfiles of a higher level than your own.\n\r", ch);
        jope_free_char(ch);
        ch->desc->connected = CON_PLAYING;
    }
    return;
}

/*
 * The loading and freeing functions follows
 */
bool jope_load_char(CHAR_DATA *ch, char *arg)
{
    DESCRIPTOR_DATA *dummy;

    if (ch->pcdata->pfile != NULL)
    {
        send_to_char("Your already editing a pfile.\n\r", ch);
        return FALSE;
    }

    if (descriptor_free == NULL )
    {
        dummy = alloc_perm(sizeof(*dummy));
    }
    else
    {
        dummy = descriptor_free;
        descriptor_free = descriptor_free->next;
    }
    if (load_char_obj(dummy, arg)) ch->pcdata->pfile = dummy->character;
    else send_to_char("No such pfile.\n\r", ch);
    dummy->next     = descriptor_free;
    descriptor_free = dummy;
    if (ch->pcdata->pfile == NULL) return FALSE;
    return TRUE;
}

void jope_free_char(CHAR_DATA *ch)
{
    CHAR_DATA *victim;

    if ((victim = ch->pcdata->pfile) == NULL)
    {
        send_to_char("You are not editing a char.\n\r", ch);
        return;
    }
    if (ch->level >= JOPE_SAVE_LEVEL) save_char_obj(victim);
    free_char(victim);
    ch->pcdata->pfile = NULL;
    return;
}
