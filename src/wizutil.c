/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvements copyright (C) 1992, 1993 by Michael         *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefiting.  We hope that you share your changes too.  What goes       *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#if !defined(WIN32)
#include <unistd.h>
#endif
#include <string.h>
#include "merc.h"

/*
===========================================================================
This snippet was written by Erwin S. Andreasen, erwin@pip.dknet.dk. You may
use this code freely, as long as you retain my name in all of the files. You
also have to mail me telling that you are using it. I am giving this,
hopefully useful, piece of source code to you for free, and all I require
from you is some feedback.

Please mail me if you find any bugs or have any new ideas or just comments.

All my snippets are publically available at:

http://pip.dknet.dk/~pip1773/

If you do not have WWW access, try ftp'ing to pip.dknet.dk and examine
the /pub/pip1773 directory.

===========================================================================
*/

extern ROOM_INDEX_DATA *       room_index_hash         [MAX_KEY_HASH];
typedef enum {exit_from, exit_to, exit_both}
exit_status;
const sh_int opposite_dir [6] = { DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP };

/*
 * get the 'short' name of an area (e.g. MIDGAARD, MIRROR etc.
 * assumes that the filename saved in the AREA_DATA struct is something like midgaard.are
 */
char * area_name (AREA_DATA *pArea)
{
    static char buffer[64];
    char  *period;

    assert (pArea != NULL);

    strncpy (buffer, pArea->filename, 64);
    period = strchr (buffer, '.');
    if (period)
        *period = '\0';
    return buffer;
}

void room_pair (ROOM_INDEX_DATA* left, ROOM_INDEX_DATA* right, exit_status ex, char *buffer)
{
    char *sExit;

    switch (ex)
    {
        default:
            sExit = "??"; break;                            /* invalid usage */
        case exit_from:
            sExit = "< "; break;
        case exit_to:
            sExit = " >"; break;
        case exit_both:
            sExit = "<>"; break;
    }
    sprintf (buffer, "%5d %-26.26s %s%5d %-26.26s(%-8.8s)\n\r",
        left->vnum, left->name, sExit, right->vnum, right->name, area_name(right->area));
    return;
}

/* for every exit in 'room' which leads to or from pArea but NOT both, print it */
void checkexits (ROOM_INDEX_DATA *room, AREA_DATA *pArea, char* buffer)
{
    char buf[MAX_STRING_LENGTH];
    int i;
    EXIT_DATA *exit;
    ROOM_INDEX_DATA *to_room;

    strcpy(buffer, "");

    for (i = 0; i < 6; i++)
    {
        exit = room->exit[i];
        if (!exit) continue;
        else to_room = exit->to_room;
        if (to_room)
        {
            if ( (room->area == pArea) && (to_room->area != pArea) )
            {
                if ( to_room->exit[opposite_dir[i]] && to_room->exit[opposite_dir[i]]->to_room == room )
                    room_pair (room,to_room,exit_both,buf); /* <> */
                else
                    room_pair (room,to_room,exit_to,buf);   /* > */
                strcat (buffer, buf);
            }
            else
            {
                if ( (room->area != pArea) && (exit->to_room->area == pArea) )
                {                                           /* an exit from another area to our area */
                    if  (!(to_room->exit[opposite_dir[i]] && to_room->exit[opposite_dir[i]]->to_room == room ))
                    {                                       /* two-way exits are handled in the other if */
                        room_pair (to_room,room,exit_from,buf);
                        strcat (buffer, buf);
                    }
                }                                           /* if room->area */
            }                                               /* for */
        }
    }
    return;
}

/* for now, no arguments, just list the current area */

void do_exlist (CHAR_DATA *ch, char * argument)
{
    AREA_DATA* pArea;
    ROOM_INDEX_DATA* room;
    int i;
    char buffer[MAX_STRING_LENGTH];

    pArea = ch->in_room->area;
    for (i = 0; i < MAX_KEY_HASH; i++)
    {
        for (room = room_index_hash[i]; room != NULL; room = room->next)
        {
            checkexits (room, pArea, buffer);
            send_to_char (buffer, ch);
        }
    }
    return;
}

void set_switchname( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
        bug( "Set_switchname: NPC.", 0 );
        return;
    }
    strcpy( buf, title );
    free_string( ch->pcdata->switchname );
    ch->pcdata->switchname = str_dup( buf );
    return;
}

void set_pc_name( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
        bug( "Set_pc_name: NPC.", 0 );
        return;
    }
    strcpy( buf, title );
    free_string( ch->name);
    ch->name = str_dup( buf );
    return;
}

void do_classself(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    //char buf[MAX_STRING_LENGTH];
    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch))return;

    if (!IS_NPC(ch) && ch->class != 0)
    {
        send_to_char("You already have a class.\n\r",ch);
        return;
    }
    if (ch->level < 3)
    {
        send_to_char("You must be avatar to selfclass.\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0' )
    {
        send_to_char("Classes: Type selfclass <class> to get classed.\n\r\n\r",ch);
        send_to_char("DEMON             WEREWOLF         #C DROW#n           NINJA\n\r",ch);
        send_to_char("VAMPIRE           MONK              MAGE           KAHN\n\r",ch);
        send_to_char("TANARRI           SHAPESHIFTER     #C SPIDERDROID #n   SAMURAI\n\r",ch);
        send_to_char("UNDEADKNIGHT      ANGEL             LICH           ELF\n\r",ch);
        send_to_char("DRAGON            PHOENIX         #R*#nWRAITH\n\r",ch);
        send_to_char("#R*#n class is being coded.  #C*#n class being remodeled.\n\r",ch);

        return;
    }
    do_clearstats2(ch,"");
    if (!str_cmp(arg1,"demon"))
    {
        ch->class = CLASS_DEMON;
        set_learnable_disciplines(ch);
        send_to_char( "You have chosen the #RDemonic#n path, may god have mercy on yer soul.\n\r", ch);
    }
    /*  if ( !str_cmp( arg1, "doomtrooper"      ) )
      { OBJ_INDEX_DATA *pObjIndex;
        OBJ_DATA *obj;
        int vnum = 0;
        ch->class = CLASS_MILITARY;
        ch->pcdata->Bootcamp[5]=28;
        send_to_char("You have now been DRAFTED SOLDIER!\n\r", ch);
        for(vnum=110975;vnum<110988;vnum++){
        pObjIndex = get_obj_index( vnum );
        obj = create_object(pObjIndex, 50);
        obj->questowner = str_dup(ch->pcdata->switchname);
        obj_to_char(obj, ch);}
        for(vnum=110975;vnum<110988;vnum++){
        pObjIndex = get_obj_index( vnum );
        obj = create_object(pObjIndex, 50);
        obj->questowner = str_dup(ch->pcdata->switchname);
        obj_to_char(obj, ch);}

        pObjIndex = get_obj_index( 110989 );
        obj = create_object(pObjIndex, 50);
        obj->questowner = str_dup(ch->pcdata->switchname);
        obj_to_char(obj, ch);
       send_to_char("Primary and Secondary set of EQ aquired Do NOT LOSE IT.\n\r", ch);
      }  */

    else if (!str_cmp(arg1,"werewolf"))
    {
        ch->class = CLASS_WEREWOLF;
        set_learnable_disciplines(ch);
        send_to_char( "You have chosen the path of the #0Garou#n, may gaia guide you.\n\r", ch);
    }

    /*  Removed until fixed - beorn
    else if (!str_cmp(arg1,"drow"))
    {
    ch->class = CLASS_DROW;
    send_to_char( "Choose your profession, and #PLloth#n will guide you.\n\r", ch);
    } */

    else if (!str_cmp(arg1,"rponly"))
    {
        if(ch->level<4)ch->home=100906;
        do_recall(ch,"");

        ch->hit=10;
        ch->max_hit=10;
        ch->mana=10;
        ch->max_mana=10;
        ch->move=10;
        ch->max_move=10;

        ch->class = CLASS_RPONLY;
        send_to_char( "#LHail #CAlmighty #RRole #PPlayer\n\r", ch);
    }

    else if (!str_cmp(arg1,"ninja"))
    {
        ch->class = CLASS_NINJA;
        send_to_char( "You have chosen a life in the #0shadows#n, assassinate at will.\n\r", ch);
    }

    else if (!str_cmp(arg1,"wizard"))
    {
        ch->class = CLASS_WIZARD;
        send_to_char( "You take up the arts of a #sW#si#sz#sa#sr#sd#n, go and wreck pain on all mortals.\n\r", ch);
    }

    else if (!str_cmp(arg1,"vampire"))
    {
        ch->class = CLASS_VAMPIRE;
        ch->beast = 30;
        set_learnable_disciplines(ch);
        send_to_char( "Fear the #ySun#n nosferatu, God's curse lives in you.\n\r", ch);
    }

    else if (!str_cmp(arg1,"monk"))
    {
        ch->class = CLASS_MONK;
        ch->level = 3;
        send_to_char( "You faith in God will guide you, destroy #7EVIL#n.\n\r", ch);
    }

    else if (!str_cmp(arg1,"mage") || !str_cmp(arg1,"battlemage"))
    {
        if (ch->max_mana >= 5000 && ch->spl[RED_MAGIC]>99 && ch->spl[BLUE_MAGIC]>99
            && ch->spl[YELLOW_MAGIC]>99 && ch->spl[GREEN_MAGIC]>99 && ch->spl[PURPLE_MAGIC]>99)
        {
            ch->class = CLASS_MAGE;
            ch->level = 3;
            send_to_char( "You start down the path of power, the #Rarcane#n is your weapon.\n\r", ch);
        }
        else
        {
            send_to_char("You need 5K mana and 100 in all your spellcolors.\n\r",ch);
            return;
        }
    }
    else if (!str_cmp(arg1,"shapeshifter"))
    {
        ch->class = CLASS_SHAPESHIFTER;
        ch->level = 3;
        send_to_char( "You step out of the shadows and become a #oShapeshifter#n.\n\r",ch);
    }
    /* -also removed until they can be rebuilt from ground up -beorn
    else if (!str_cmp(arg1,"spiderdroid"))
    {
    ch->class = CLASS_DROID;
    ch->level = 3;
    send_to_char( "#y[#RYou decide to become a Spider Droid.#y]#n\n\r",ch);
    }  */

    else if (!str_cmp(arg1,"tanarri"))
    {
        ch->class = CLASS_TANARRI;
        ch->level = 3;
        send_to_char( "You choose to be a Tanar'ri and fight in the Blood Wars.\n\r",ch);
    }

    else if (!str_cmp(arg1,"angel"))
    {
        ch->class = CLASS_ANGEL;
        ch->level = 3;
        ch->alignment = 1000;
        send_to_char( "#y[#RYou become an Angel and avenge God.#y]#n\n\r",ch);
    }

    else if (!str_cmp(arg1,"samurai"))
    {
        ch->class = CLASS_SAMURAI;
        ch->level = 3;
        send_to_char( "#yYou are a #RSamu#yrai#n\n\r",ch);
    }

    else if (!str_cmp(arg1,"undeadknight"))
    {
        ch->class = CLASS_UNDEAD_KNIGHT;
        ch->level = 3;
        send_to_char( "#y[#RYou decide to become an Undead Knight.#y]#n\n\r",ch);
    }

    else if (!str_cmp(arg1,"lich"))
    {
        if (ch->max_mana >= 5000 && ch->spl[RED_MAGIC]>99 && ch->spl[BLUE_MAGIC]>99
            && ch->spl[YELLOW_MAGIC]>99 && ch->spl[GREEN_MAGIC]>99 && ch->spl[PURPLE_MAGIC]>99)
        {
            ch->class = CLASS_LICH;
            ch->level = 3;
            send_to_char( "You decide to be an undead evil sorcer also known as a #7Lich#n.\n\r",ch);
        }
        else
        {
            send_to_char("You need 5K mana and 100 in all your spellcolors.\n\r",ch);
            return;
        }
    }

    else if (!str_cmp(arg1, "elf"))
    {
        ch->class = CLASS_ELF;
        ch->level = 3;
        send_to_char("You begin to grow into a beautiful creature known as #GE#glves#n.\n\r",ch);
        //send_to_char("Temporary closed due to unbalancement.\n\r",ch);
    }

    else if (!str_cmp(arg1, "kahn"))
    {
        ch->class = CLASS_KAHN;
        ch->level = 3;
        send_to_char("You emerge from out of the jungle to aid Gaia.  Destroy the Wyrm!\n\r",ch);
        //send_to_char("Temporary closed due to unbalancement.\n\r",ch);
    }

    else if (!str_cmp(arg1, "dragon"))
    {
        ch->class = CLASS_DRAGON;
        ch->level = 3;
        send_to_char("You come soaring down from the sky, you are now a massive #CD#cr#Ca#cg#Co#cn#n\n\r",ch);
    }

    else if(!str_cmp(arg1, "phoenix"))
    {
        ch->class = CLASS_PHOENIX;
        ch->level = 3;
        send_to_char("You have died, and were reborn as a #RF#rl#Ra#Rm#ri#Rn#rg #yP#Rh#yo#Re#yn#Ri#yx#n\n\r",ch);

    }

    else if(!str_cmp(arg1, "wraith"))
    {
        /*	ch->class = CLASS_WRAITH; */
        //	ch->level = 3;
        /*send_to_char("#7You bring the blade to your throat and slice away, the #Rblood#7 pours,\n\r",ch);
        send_to_char("#7down your chest and you start to feel faint and fall to the ground.\n\r",ch);
        send_to_char("#7the loss of blood makes you feel faint, you close your eyes...\n\r",ch);
        sprintf(buf, "%s has commited suicide to become a wraith.\n\r",ch->name);
        do_info(ch, buf);*/
        /*	send_to_char("          \n\r",);
            send_to_char("          \n\r",);
        */
        send_to_char("To become a Wraith, use CLASSME. #RWARNING:#n Wraith is under development. All unauthorized users will be banned.\n\r",ch);
    }

    else do_classself(ch,"");
    return;
}

//WRAITH CLASSING
void do_classme(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    if (ch->class == 0)
    {
        if(IS_SET(ch->affected_by2, PLR_CLASSME))
        {
            REMOVE_BIT(ch->affected_by2, PLR_CLASSME);
            sprintf(buf, "%s has revoked their oath against the gods.\n\r",ch->name);
            do_info(ch, buf);
            return;
        }

        else if(!IS_SET(ch->affected_by2, PLR_CLASSME))
        {
            SET_BIT(ch->affected_by2, PLR_CLASSME);
            sprintf(buf, "%s has sworn an oath against the gods.\n\r",ch->name);
            do_info(ch, buf);
            return;
        }
    }

    else send_to_char("Huh?\n\r",ch);
    return;
}

void do_reimb(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA       *vch;
    char            arg[MAX_INPUT_LENGTH];
    char            arg2[MAX_INPUT_LENGTH];
    int v;

    argument = one_argument(argument, arg);
    if (arg[0] == '\0')
    {
        send_to_char("Reimburse <target> <xp|qp> <amount>\n\r", ch);
        return;
    }
    if ((vch = get_char_world(ch, arg)) == NULL)
    {
        send_to_char("They aren't logged on.\n\r", ch);
        return;
    }
    if (IS_NPC(vch))
    {
        send_to_char("That is a mob.\n\r", ch);
        return;
    }
    argument = one_argument(argument, arg);
    argument = one_argument(argument, arg2);
    v = atoi(arg2);
    if (arg[0] == '\0' || arg2[0] == '\0'
        || (!is_number(arg2) && v >= 0))
    {
        do_reimb(ch, "");
        return;
    }
    if (!str_cmp(arg, "xp"))
    {
        vch->exp += v;
        vch->pcdata->score[SCORE_TOTAL_XP] += v;
    }
    else if (!str_cmp(arg, "qp"))
    {
        vch->pcdata->quest += v;
    }
    else
    {
        send_to_char("Please specify XP or QP.\n\r", ch);
        return;
    }
    if (vch->mkill < 5)
    {
        vch->mkill = 5;
        do_autosave(ch, "");
    }
    sprintf(arg2, "%s reimbursed %d %s.\n\r", vch->name, v, arg);
    send_to_char(arg2, ch);
    sprintf(arg2, "%s has reimbursed you %d %s.\n\r", ch->name, v, arg);
    send_to_char(arg2, vch);
}

void do_affects( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;

    if (IS_NPC(ch)) return;

    if (IS_ITEMAFF(ch, ITEMA_ICESHIELD))
        send_to_char("#CIceshield#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))
        send_to_char("#LLightningshield#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD))
        send_to_char("#GAcidshield#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_FIRESHIELD))
        send_to_char("#RFireshield#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))
        send_to_char("#pChaosshield#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_RETORT))
        send_to_char("#RRetort#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_BLOCK))
        send_to_char("#CBlock#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_STANCEA))
        send_to_char("#LUnicorn Stance Enabled#n.\n\r",ch);
    if (IS_ITEMAFF(ch, ITEMA_RESISTANCE))
        send_to_char("#ySuperior Protection#n.\n\r",ch);

    if (IS_AFFECTED(ch, AFF_SANCTUARY))
        send_to_char("#7Sanctuary#n.\n\r",ch);
    if ( IS_AFFECTED(ch, AFF_PROTECT))
        send_to_char("#LProtection from evil#n.\n\r",ch);
    if ( IS_AFFECTED(ch, AFF_PROTECT_GOOD))
        send_to_char("#LProtection from good#n.\n\r",ch);
    if ( IS_AFFECTED(ch, AFF_FLYING))
        send_to_char("#cFly#n\n\r",ch);
    if ( IS_AFFECTED(ch, AFF_FREEZING))
        send_to_char("#lFreezing#n\n\r",ch);
    send_to_char("\n\r",ch);
    if ( ch->affected == NULL )
    {
        send_to_char( "You have nothing affecting you at this time.\n\r", ch);
        return;
    }
    if ( ch->affected != NULL )
    {

        send_to_char( "You are affected by:\n\r", ch );
        for ( paf = ch->affected; paf != NULL; paf = paf->next )
        {
            sprintf( buf, "Spell: '%s'", skill_table[paf->type].name );
            send_to_char( buf, ch );
            if ( ch->level >= 0 )
            {
                sprintf( buf, " modifies %s by %d for %d hours with bits %s.\n\r",
                    affect_loc_name( paf->location ),
                    paf->modifier,
                    paf->duration,
                    affect_bit_name( paf->bitvector ) );
                send_to_char( buf, ch );
            }

        }

    }

    return;

}

char * plr2_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & PLR2_BLANK       ) strcat( buf, " blank"           );
    if ( arg & PLR2_AUTOGEM     ) strcat( buf, " autogem"         );
    if ( arg & PLR2_AUTOSLAB    ) strcat( buf, " autoslab"        );
    if ( arg & PLR2_AUTOHILT    ) strcat( buf, " autohilt"        );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * plr_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & PLR_IS_NPC       ) strcat( buf, " npc"           );
    if ( arg & PLR_AUTOEXIT     ) strcat( buf, " autoexit"      );
    if ( arg & PLR_AUTOLOOT     ) strcat( buf, " autoloot"      );
    if ( arg & PLR_AUTOSAC      ) strcat( buf, " autosac"       );
    if ( arg & PLR_BLANK        ) strcat( buf, " blank"         );
    if ( arg & PLR_BRIEF        ) strcat( buf, " brief"         );
    if ( arg & PLR_COMBINE      ) strcat( buf, " combine"       );
    if ( arg & PLR_PROMPT       ) strcat( buf, " prompt"        );
    if ( arg & PLR_TELNET_GA    ) strcat( buf, " telnet_ga"     );
    if ( arg & PLR_HOLYLIGHT    ) strcat( buf, " holylight"     );
    if ( arg & PLR_WIZINVIS     ) strcat( buf, " wizinvis"      );
    if ( arg & PLR_ANSI         ) strcat( buf, " ansi"          );
    if ( arg & PLR_SILENCE      ) strcat( buf, " silenced"      );
    if ( arg & PLR_NO_TELL      ) strcat( buf, " no_tell"       );
    if ( arg & PLR_LOG          ) strcat( buf, " log"           );
    if ( arg & PLR_FREEZE       ) strcat( buf, " freeze"        );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * extra_plr_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & EXTRA_TRUSTED            ) strcat( buf, " q_trusted" );
    if ( arg & EXTRA_NEWPASS            ) strcat( buf, " newpass" );
    if ( arg & EXTRA_OSWITCH            ) strcat( buf, " oswitch" );
    if ( arg & EXTRA_SWITCH             ) strcat( buf, " switch" );
    if ( arg & EXTRA_FAKE_CON           ) strcat( buf, " fake_con" );
    if ( arg & TIED_UP                  ) strcat( buf, " tied_up" );
    if ( arg & GAGGED                   ) strcat( buf, " gagged" );
    if ( arg & BLINDFOLDED              ) strcat( buf, " blindfolded" );
    if ( arg & EXTRA_DONE               ) strcat( buf, " non_virgin" );
    if ( arg & EXTRA_EXP                ) strcat( buf, " got_exp" );
    if ( arg & EXTRA_PREGNANT           ) strcat( buf, " pregnant" );
    if ( arg & EXTRA_LABOUR             ) strcat( buf, " labour" );
    if ( arg & EXTRA_BORN               ) strcat( buf, " born" );
    if ( arg & EXTRA_PROMPT             ) strcat( buf, " prompt" );
    if ( arg & EXTRA_MARRIED            ) strcat( buf, " married" );
    if ( arg & EXTRA_CALL_ALL           ) strcat( buf, " call_all" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char * get_position_name( int arg )
{
    switch( arg )
    {
        case 0: return "dead";
        case 1: return "mortal";
        case 2: return "incap";
        case 3: return "stunned";
        case 4: return "sleeping";
        case 5: return "meditating";
        case 6: return "sitting";
        case 7: return "resting";
        case 8: return "fighting";
        case 9: return "standing";
    }
    bug( "Get_position_name: unknown type %d.", arg );
    return "(unknown)";
}

/*
 * Itemaffect bit names :)
 */
char * itemaffect_bit_name( int arg )
{
    static char buf[512];

    buf[0] = '\0';

    if ( arg & ITEMA_SHOCKSHIELD        ) strcat( buf, " Shockshield"   );
    if ( arg & ITEMA_FIRESHIELD         ) strcat( buf, " Fireshield"    );
    if ( arg & ITEMA_ICESHIELD          ) strcat( buf, " Iceshield"     );
    if ( arg & ITEMA_ACIDSHIELD         ) strcat( buf, " Acidshield"    );
    if ( arg & ITEMA_CHAOSSHIELD        ) strcat( buf, " Chaoshield"    );
    if ( arg & ITEMA_ARTIFACT           ) strcat( buf, " Artifact"      );
    if ( arg & ITEMA_REGENERATE         ) strcat( buf, " Regeneration"  );
    if ( arg & ITEMA_SPEED              ) strcat( buf, " Speed"         );
    if ( arg & ITEMA_VORPAL             ) strcat( buf, " Vorpal"        );
    if ( arg & ITEMA_PEACE              ) strcat( buf, " Peace"         );
    if ( arg & ITEMA_RIGHT_SILVER       ) strcat( buf, " Right Silver"  );
    if ( arg & ITEMA_LEFT_SILVER        ) strcat( buf, " Left Silver"   );
    if ( arg & ITEMA_REFLECT            ) strcat( buf, " Darkshield"    );
    if ( arg & ITEMA_RESISTANCE         ) strcat( buf, " Resistance"    );
    if ( arg & ITEMA_VISION             ) strcat( buf, " Vision"        );
    if ( arg & ITEMA_STALKER            ) strcat( buf, " Stalker"       );
    if ( arg & ITEMA_VANISH             ) strcat( buf, " Vanish"        );
    if ( arg & ITEMA_RAGER              ) strcat( buf, " Rager"         );
    if ( arg & ITEMA_RETORT             ) strcat( buf, " Retort"        );
    if ( arg & ITEMA_BLOCK              ) strcat( buf, " Block"         );
    if ( arg & ITEMA_STANCEA            ) strcat( buf, " Unicorn"       );
    return ( buf[0] != '\0' ) ? (buf+1) : "none";
}

/* 
 * code by Tijer
 */
void do_pstat ( CHAR_DATA *ch, char *argument )
{
    char        arg[MAX_INPUT_LENGTH];
    char        buf[MAX_STRING_LENGTH];
    CHAR_DATA   *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char("Pstat whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char("They aren't here.\n\r", ch );
        return;
    }

    if (IS_NPC(victim))
    {
        send_to_char( "Only players please.\n\r",ch);
        WAIT_STATE(ch,24);
        return;
    }
    sprintf( buf, "#GName :#n %s.\n\r",
        IS_NPC( victim )        ? victim->short_descr : victim->name );
    send_to_char( buf, ch );
    sprintf( buf, "#GSex :#n %s. #GRoom :#n %d. #GAlign :#n %d. #GPrimal :#n %d. #GQuest :#n %d.\n\r",
        victim->sex == SEX_MALE         ? "Male"        :
    victim->sex == SEX_FEMALE       ? "Female"      : "None",
        victim->in_room == NULL         ? 0             : victim->in_room->vnum,
        victim->alignment,
        victim->practice,
        IS_NPC( victim )                ? 0             : victim->pcdata->quest
        );
    send_to_char( buf, ch );

    sprintf( buf, "#GLevel :#n %d. #GTrust :#n %d. #GSecurity:#n %d. #GGold :#n %d. #GExp :#n %d.\n\r",
        victim->level,
        victim->trust,
        victim->pcdata->security,
        victim->gold,
        victim->exp );
    send_to_char( buf, ch );

    sprintf( buf, "#GHit :#n %d. #GDam :#n %d. #GAC :#n %d. #GPosition :#n %s\n\r",
        char_hitroll( victim ),
        char_damroll( victim ),
        char_ac( victim ),
        capitalize( get_position_name( victim->position ) ));
    send_to_char( buf, ch );

    sprintf( buf, "#GHP #n%d#C/#n%d. #GMana #n%d#C/#n%d. #GMove #n%d#C/#n%d.\n\r",
        victim->hit, victim->max_hit,
        victim->mana, victim->max_mana,
        victim->move, victim->max_move );
    send_to_char( buf, ch );

    sprintf( buf, "#GStr: #n%d.  #GInt:#n %d.  #GWis:#n %d.  #GDex:#n %d.  #GCon:#n %d.\n\r",
        get_curr_str(victim),
        get_curr_int(victim),
        get_curr_wis(victim),
        get_curr_dex(victim),
        get_curr_con(victim) );
    send_to_char( buf, ch );

    sprintf( buf, "#GFighting :#n %s. (%d)\n\r",
        victim->fighting ? victim->fighting->name  : "(None)",
        victim->fighting ? victim->fighting->level : 0 );
    send_to_char( buf, ch );

    sprintf( buf, "#GPkill :#n %d. #GPdeath :#n %d. #GMkill :#n %d. #GMdeath :#n %d. #GStores:#n %d. #GPkpoints:#n %d\n\r",
        IS_NPC( victim ) ? 0 : victim->pkill,
        IS_NPC( victim ) ? 0 : victim->pdeath,
        IS_NPC( victim ) ? 0 : victim->mkill,
        IS_NPC( victim ) ? 0 : victim->mdeath,
        IS_NPC( victim ) ? 0 : victim->pcdata->store,
        IS_NPC( victim ) ? 0 : victim->pcdata->pkpoints );
    send_to_char( buf, ch );

    sprintf( buf, "#GGeneration:#n %d. #GMightScore:#n %d.\n\r",victim->generation, getMight(victim));
    send_to_char( buf, ch );

    sprintf( buf, "#GTotExp  :#n %12d. #GTotMobLev  :#n %10d. #GTotQuestPoints :#n %10d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_TOTAL_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_QUEST] );
    send_to_char( buf, ch );

    sprintf( buf, "#GHighExp :#n %12d. #GHighMobLev :#n %10d. #GTotQuests     :#n %10d.\n\r",
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_XP],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_HIGH_LEVEL],
        IS_NPC( victim ) ? 0 : victim->pcdata->score[SCORE_NUM_QUEST] );
    send_to_char( buf, ch );

    if ( !IS_NPC( victim ) )
    {
        sprintf( buf, "#GUnarmed :#n %4d. #G[#n%d#G]", victim->wpn[0],victim->wpn_lvl[0] );
        send_to_char( buf, ch );
        sprintf( buf, " #GSlice   :#n %4d. #G[#n%d#G]", victim->wpn[1],victim->wpn_lvl[1] );
        send_to_char( buf, ch );
        sprintf( buf, " #GStab    :#n %4d. #G[#n%d#G]", victim->wpn[2],victim->wpn_lvl[2] );
        send_to_char( buf, ch );
        sprintf( buf, " #GSlash   :#n %4d. #G[#n%d#G]", victim->wpn[3],victim->wpn_lvl[3] );
        send_to_char( buf, ch );
        sprintf( buf, " #GWhip    :#n %4d. #G[#n%d#G]\n\r", victim->wpn[4],victim->wpn_lvl[4] );
        send_to_char( buf, ch );
        sprintf( buf, "#GClaw    :#n %4d. #G[#n%d#G]", victim->wpn[5],victim->wpn_lvl[5] );
        send_to_char( buf, ch );
        sprintf( buf, "#G Blast   :#n %4d. #G[#n%d#G]", victim->wpn[6],victim->wpn_lvl[6] );
        send_to_char( buf, ch );
        sprintf( buf, "#G Pound   :#n %4d. #G[#n%d#G]", victim->wpn[7],victim->wpn_lvl[7] );
        send_to_char( buf, ch );
        sprintf( buf, "#G Crush   :#n %4d. #G[#n%d#G]", victim->wpn[8],victim->wpn_lvl[8] );
        send_to_char( buf, ch );
        sprintf( buf, "#G Grep    :#n %4d. #G[#n%d#G]\n\r", victim->wpn[9],victim->wpn_lvl[9] );
        send_to_char( buf, ch );
        sprintf( buf, "#GBite    :#n %4d. #G[#n%d#G]", victim->wpn[10],victim->wpn_lvl[10] );
        send_to_char( buf, ch );
        sprintf( buf, "#G Pierce  :#n %4d. #G[#n%d#G]", victim->wpn[11],victim->wpn_lvl[11] );
        send_to_char( buf, ch );
        sprintf( buf, "#G Suck    :#n %4d. #G[#n%d#G]\n\r",victim->wpn[12],victim->wpn_lvl[12] );
        send_to_char( buf, ch );

        sprintf( buf, "#P%-8s :#n %3d. #R%-8s : #n%3d. #L%-8s : #n%3d. #G%-8s : #n%3d. #y%-8s : #n%3d.\n\r",
            "Purple",   victim->spl[PURPLE_MAGIC],
            "Red",      victim->spl[RED_MAGIC],
            "Blue",     victim->spl[BLUE_MAGIC],
            "Green",    victim->spl[GREEN_MAGIC],
            "Yellow",   victim->spl[YELLOW_MAGIC] );
        send_to_char( buf, ch );
        sprintf( buf, "#G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %3d.\n\r",
            "Viper",    victim->stance[STANCE_VIPER],
            "Crane",    victim->stance[STANCE_CRANE],
            "Crab",     victim->stance[STANCE_CRAB],
            "Mongoose", victim->stance[STANCE_MONGOOSE],
            "Bull",     victim->stance[STANCE_BULL] );
        send_to_char( buf, ch );

        sprintf( buf, "#G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %-3d. #G%-8s :#n %3d.\n\r",
            "Mantis",   victim->stance[STANCE_MANTIS],
            "Dragon",   victim->stance[STANCE_DRAGON],
            "Tiger",    victim->stance[STANCE_TIGER],
            "Monkey",   victim->stance[STANCE_MONKEY],
            "Swallow",  victim->stance[STANCE_SWALLOW] );

        send_to_char( buf, ch );
        sprintf( buf, "#G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %3d. #G%-8s :#n %3d.\n\r",
            "ss1",   victim->stance[STANCE_SS1],
            "ss2",   victim->stance[STANCE_SS2],
            "ss3",    victim->stance[STANCE_SS3],
            "ss4",   victim->stance[STANCE_SS4],
            "ss5",  victim->stance[STANCE_SS5] );
        send_to_char( buf, ch );

        sprintf( buf, "#GAct         :#n %s\n\r", plr_bit_name(victim->act ));
        send_to_char( buf, ch );
        sprintf( buf, "#GAct2        :#n %s\n\r", plr2_bit_name(victim->act2 ));
        send_to_char( buf, ch );
        sprintf( buf, "#GExtra       :#n %s\n\r",
            victim->extra <= 0 ? "(None)" : extra_plr_bit_name( victim->extra ) );
        send_to_char( buf, ch );
        sprintf( buf, "#GItemAff     :#n %s\n\r",
            victim->itemaffect <= 0 ? "(None)" : itemaffect_bit_name(victim->itemaffect ) );
        send_to_char( buf, ch );

        sprintf( buf, "#GAffected by :#n %s.\n\r",
            affect_bit_name( victim->affected_by ) );
        send_to_char( buf, ch );
        send_to_char("#n\n\r",ch);

        return;
    }
}
