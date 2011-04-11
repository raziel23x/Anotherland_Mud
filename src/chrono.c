/* Stuff added in from my code */
/* 	Chrono		       */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "merc.h"

int     ccenter[CCENTER_MAX];
char *fgetf( char *s, int n, register FILE *iop )
{
    register int c;
    register char *cs;
    
    c = '\0';
    cs = s;
    while( --n > 0 && (c = getc(iop)) != EOF)
    {
        if ((*cs++ = c) == '\0')
            break;
    }

    *cs = '\0';
    return((c == EOF && cs == s) ? NULL : s);
}
void do_pipe( CHAR_DATA *ch, char *argument )
{
    char buf[MSL];
    FILE *fp;

#if defined (unix)
    fp = popen( argument, "r" );
#endif

#if defined ( WIN32 )
	fp = _popen(argument, "r" );
#endif

    fgetf( buf, MSL, fp );

    if ( ch && ch != NULL )
        send_to_char( buf, ch );


#if defined (unix)
    pclose( fp );
#endif
#if defined ( WIN32 )
	_pclose (fp);
#endif

    return;
}
void do_search( CHAR_DATA *ch, char *argument )
{
        char buf[MSL];
	char arg[MSL];
	
	argument = one_argument(argument,arg);
	if ( argument[0] == '\0' )
	{
		send_to_char( "Syntax: Search Players/Logs <String>.\n\r", ch );
		return;
	}
	if ( !str_prefix(arg,"Players") )
	        sprintf(buf,"fgrep -ir '%s' %s", argument, PLAYER_DIR);
	else
	        sprintf(buf,"fgrep -i \"%s\" %s*", argument, LOG_DIR);
        do_pipe(ch, buf);   
        return; 
}        

void load_sysdata( void )
{
  FILE *sysfp;
  char sys_file_name[MAX_STRING_LENGTH];
  sprintf( sys_file_name, "%s", SYSDAT_FILE );

  if ( ( sysfp = fopen( sys_file_name, "r" ) ) == NULL )
  {
	  bug( "Load Sys Table: fopen", 0 );
	  log( "failed open of system.dat in load_sysdata" );
  }
  else
  {
    sysdata.silent_reboot = fread_number( sysfp );
  }
  fclose( sysfp );
}

void save_sysdata( void )
{

  FILE * fp;
  char sys_file_name[MAX_STRING_LENGTH];
  fclose( fpReserve );
  sprintf( sys_file_name, "%s", SYSDAT_FILE );

  if ( ( fp = fopen( sys_file_name, "w" ) ) == NULL )
  {
    bug( "Save Sysdata: fopen", 0 );
    log( "failed open of system.dat in save_sysdata" );
  }
  else
  {
    fprintf( fp, "%d\n", ( sysdata.silent_reboot ? 1 : 0 ) );
    fflush( fp );
    fclose( fp ); 
  }
  fpReserve = fopen( NULL_FILE, "r" );
  return;

}
	
void do_miscbag( CHAR_DATA *ch, char *argument )
{
    do_oload(ch,"30053");
    do_oload(ch,"30054");
    do_oload(ch,"30055");
    do_oload(ch,"30056");
    do_oload(ch,"30057");
    do_oload(ch,"30058");
    do_oload(ch,"30059");
    do_oload(ch,"30060");
    do_oload(ch,"30061");
    do_oload(ch,"30062");
    do_oload(ch,"30065");
    stcf(ch,"%s, here's the Gem's.\n\r",ch->name);
    do_oload(ch,"30063");
    do_oload(ch,"30064");
    do_oload(ch,"30066");
    do_oload(ch,"30067");
    do_oload(ch,"30068");
    do_oload(ch,"30069");
    do_oload(ch,"30070");
    do_oload(ch,"30071");
    stcf(ch,"And here's the Hilt's.\n\r",ch->name);
    do_oload(ch,"1111");
    stcf(ch,"Plus a free bag to put them in.\n\r",ch->name);
    do_put(ch,"all.gem bag");
    do_put(ch,"all.hilt bag");

    return;
}

void do_iwhere( CHAR_DATA *ch, char *argument )
{
    /* Like WHERE, except is global, and shows area & room.
     * --Stephen
     */
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int count = 0;
    buf2[0] = '\0';

    send_to_char( "Name          Room        Area\n\r", ch );
    send_to_char( "----          ----        ----\n\r", ch );

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        CHAR_DATA *vch;
        if (d->connected != CON_PLAYING)
            continue;
        vch = d->character;

        if ( !IS_NPC( vch ) && can_see( ch, vch ))
        {

            count++;
            sprintf( buf, "%-12s [%5d] %-20s\n\r",
                vch->name,
                vch->in_room == NULL ? 0 : vch->in_room->vnum,
                vch->in_room->area->name  );
            strcat( buf2, buf );
        }
    }

    if ( count == 0 )
        strcat( buf2, "No Players found!\n\r" );
    else
    {
        sprintf( buf, "%d Player%s found.\n\r", count,
            ( count > 1 ) ? "s" : "" );
        strcat( buf2, buf );
    }

    strcat( buf2, "\n\r" );
    send_to_char( buf2, ch );
    return;
}

void do_insert( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA  *obj;
    OBJ_DATA  *obj2;
    //	char buf2[MAX_STRING_LENGTH];
    //    	AFFECT_DATA *pAf;
    int to_hit;
    int to_dam;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if ( ( arg == '\0' ) || ( arg2 == '\0' ) )
    {
        send_to_char( "Syntax:  forge (item) (weapon)\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "You do not have that item.\n\r", ch );
        return;
    }

    if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL )
    {
        send_to_char( "You do not have that item.\n\r", ch );
        return;
    }

    if ( obj->item_type != ITEM_SOCKET )
    {
        send_to_char( "That isn't a socket.\n\r", ch );
        return;
    }

    if ( obj2->item_type != ITEM_WEAPON )
    {
        send_to_char( "That is not a weapon!\n\r", ch );
        return;
    }

    to_hit = obj->value[0];
    to_dam = obj->value[1];

    extract_obj( obj );

    if ( to_hit != 0 )
    {
        AFFECT_DATA paf;

        paf.type           = 0;
        paf.duration       = -1;
        paf.location       = APPLY_HITROLL;
        paf.modifier       = to_hit;
        paf.bitvector      = 0;
        affect_to_obj(obj2, &paf);
    }
    if ( to_dam != 0 )
    {
        AFFECT_DATA paf;

        paf.type           = 0;
        paf.duration       = -1;
        paf.location       = APPLY_DAMROLL;
        paf.modifier       = to_dam;
        paf.bitvector      = 0;
        affect_to_obj(obj2, &paf);
    }

    sprintf(buf,"#G[#7*#G]#n %s",obj2->short_descr);
    free_string(obj2->short_descr);
    obj2->short_descr = str_dup(buf);

    send_to_char( "You have inserted the socket into your weapon, it glows and humms for an instant and now seems much more powerful.\n\r", ch);
    return;
}

void do_worldtest(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    char arg[MSL];
    argument=one_argument(argument,arg);

    if(arg[0]== '\0')
    {
        stc("TEST on or off?\n\r",ch);
        return;
    }
    if (!str_cmp(arg,"on"))
    {
        worldtest = FALSE;
        sprintf(buf,"%s has turned TEST mode on!\n\r",ch->name);
        do_info(ch,buf);
    }

    else if (!str_cmp(arg,"off"))
    {
        worldtest = TRUE;
        sprintf(buf,"%s has turned TEST mode off!\n\r",ch->name);
        do_info(ch,buf);
    }
    else
    {
        stc("TEST on or off?\n\r",ch);
    }
    return;
}

void do_worldpk(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    char arg[MSL];
    argument=one_argument(argument,arg);

    if(arg[0]== '\0')
    {
        stc("Pk on or off?\n\r",ch);
        return;
    }
    if (!str_cmp(arg,"on"))
    {
        worldpk = FALSE;
        sprintf(buf,"%s has turned pk on!\n\r",ch->name);
        do_info(ch,buf);
    }

    else if (!str_cmp(arg,"off"))
    {
        worldpk = TRUE;
        sprintf(buf,"%s has turned pk off!\n\r",ch->name);
        do_info(ch,buf);
    }
    else
    {
        stc("Pk on or off?\n\r",ch);
    }
    return;
}

void do_dismember(CHAR_DATA *ch, char *argument)
{
    char arg[MSL];
    CHAR_DATA *victim;
    argument=one_argument(argument,arg);

    if(arg[0]== '\0')
    {
        stc("Dismember whom?\n\r",ch);
        return;
    }
    if((victim = get_char_room(ch,arg))==NULL)
    {
        stc("They are not here.\n\r",ch);
        return;
    }
    if(victim->trust > ch->trust)
    {
        stc("I don't THINK so!\n\r",ch);
        return;
    }

    SET_BIT(victim->loc_hp[2],LOST_ARM);
    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
    SET_BIT(victim->loc_hp[0],LOST_EYE_L);
    SET_BIT(victim->loc_hp[0],LOST_EYE_R);
    SET_BIT(victim->loc_hp[0],LOST_EYE_L);
    SET_BIT(victim->loc_hp[0],LOST_EAR_L);
    SET_BIT(victim->loc_hp[0],LOST_EAR_R);
    SET_BIT(victim->loc_hp[0],LOST_EAR_L);
    SET_BIT(victim->loc_hp[0],LOST_NOSE);
    SET_BIT(victim->loc_hp[0],BROKEN_NOSE);
    SET_BIT(victim->loc_hp[0],BROKEN_JAW);
    SET_BIT(victim->loc_hp[2],LOST_ARM);
    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
    SET_BIT(victim->loc_hp[3],LOST_ARM);
    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
    SET_BIT(victim->loc_hp[2],BROKEN_ARM);
    SET_BIT(victim->loc_hp[3],BROKEN_ARM);
    SET_BIT(victim->loc_hp[2],LOST_HAND);
    SET_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
    SET_BIT(victim->loc_hp[3],LOST_HAND);
    SET_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
    SET_BIT(victim->loc_hp[2],LOST_THUMB);
    SET_BIT(victim->loc_hp[2],LOST_FINGER_I);
    SET_BIT(victim->loc_hp[2],LOST_FINGER_M);
    SET_BIT(victim->loc_hp[2],LOST_FINGER_R);
    SET_BIT(victim->loc_hp[2],LOST_FINGER_L);
    SET_BIT(victim->loc_hp[2],BROKEN_THUMB);
    SET_BIT(victim->loc_hp[2],BROKEN_FINGER_I);
    SET_BIT(victim->loc_hp[2],BROKEN_FINGER_M);
    SET_BIT(victim->loc_hp[2],BROKEN_FINGER_R);
    SET_BIT(victim->loc_hp[2],BROKEN_FINGER_L);
    SET_BIT(victim->loc_hp[3],LOST_THUMB);
    SET_BIT(victim->loc_hp[3],LOST_FINGER_I);
    SET_BIT(victim->loc_hp[3],LOST_FINGER_M);
    SET_BIT(victim->loc_hp[3],LOST_FINGER_R);
    SET_BIT(victim->loc_hp[3],LOST_FINGER_L);
    SET_BIT(victim->loc_hp[3],BROKEN_THUMB);
    SET_BIT(victim->loc_hp[3],BROKEN_FINGER_I);
    SET_BIT(victim->loc_hp[3],BROKEN_FINGER_M);
    SET_BIT(victim->loc_hp[3],BROKEN_FINGER_R);
    SET_BIT(victim->loc_hp[3],BROKEN_FINGER_L);
    SET_BIT(victim->loc_hp[4],LOST_LEG);
    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
    SET_BIT(victim->loc_hp[5],LOST_LEG);
    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
    SET_BIT(victim->loc_hp[4],BROKEN_LEG);
    SET_BIT(victim->loc_hp[5],BROKEN_LEG);
    SET_BIT(victim->loc_hp[4],LOST_FOOT);
    SET_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
    SET_BIT(victim->loc_hp[5],LOST_FOOT);
    SET_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);
    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);
    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);
    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);
    SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);
    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_16);
    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    SET_BIT(victim->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    SET_BIT(victim->loc_hp[LOC_LEG_L],BROKEN_LEG);
    SET_BIT(victim->loc_hp[LOC_LEG_R],BROKEN_LEG);
    SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_SPINE);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_NECK);
    make_part(victim,"arm");
    make_part(victim,"arm");
    make_part(victim,"leg");
    make_part(victim,"leg");
    make_part(victim,"blood");
    make_part(victim,"blood");

    act("You dismembered $N!",ch,NULL,victim,TO_CHAR);
    send_to_char("You have been #RDISMEMBERED#n\n\r", victim );
    victim->in_room->blood += 200;

}

void do_spinebreak(CHAR_DATA *ch, char *argument)
{
    char arg[MSL];
    CHAR_DATA *victim;
    argument=one_argument(argument,arg);

    if(arg[0]== '\0')
    {
        stc("Break Whos spine?\n\r",ch);
        return;
    }
    if((victim = get_char_room(ch,arg))==NULL)
    {
        stc("They are not here.\n\r",ch);
        return;
    }
    if(victim->trust > ch->trust)
    {
        stc("I don't THINK so!\n\r",ch);
        return;
    }

    SET_BIT(victim->loc_hp[LOC_BODY],BROKEN_SPINE);

    act("You break $N's spine!",ch,NULL,victim,TO_CHAR);
    send_to_char("Your spine spontaneously snaps and fills you with #RPAIN#n\n\r", victim );
}

void do_wiznet( CHAR_DATA *ch, char *argument )
{

    char arg [MAX_INPUT_LENGTH];
    int a;

    if( IS_NPC( ch ) )
        return;

    smash_tilde( argument );
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || arg[0] == '?' )
    {
        char buf [ MAX_STRING_LENGTH ];

        send_to_char( "#BWiznet Channel Details:#n\n\r\n\r", ch );
        for ( a = 0; wiznet_table[a].min_level != 0; a++ )
        {
            if ( wiznet_table[a].min_level > get_trust( ch ) )

                buf[0] = '\0';
            if( IS_SET( PCF(ch,wiznet), wiznet_table[a].channel))
                send_to_char( wiznet_table[a].on_name,ch);
            else
                send_to_char( wiznet_table[a].off_name,ch);
        }

        send_to_char( "\n\r#wWiznet <name> toggles the monitor channels.#n\n\r", ch );
        return;
    }
    else
    {
        char buf [ MSL ];
        int  bit = -1;
        bool fSet;

        if( arg[0] == '+' ) fSet = TRUE;
        else if( arg[0] == '-' ) fSet = FALSE;
        else
        {
            send_to_char( "Wiznet: config -option or +option?\n\r", ch );
            return;
        }

        if( !str_cmp( arg+1, "ALL" ) )
            bit = WIZ_LOGS | WIZ_LINKS | WIZ_COMMANDS | WIZ_TICKS
                | WIZ_MISC | WIZ_CREATE | WIZ_DEBUG | WIZ_DEATH;
        else
            for ( a = 0; wiznet_table[a].min_level != 0; a++ )
        {
            if ( !str_prefix( arg+1, wiznet_table[a].name ) )
            {
                bit = wiznet_table[a].channel;
                break;
            }
        }

        if( bit < 1 )
        {
            do_wiznet( ch, "" );
            return;
        }

        if( fSet )
        {
            SET_BIT( PCF(ch,wiznet), bit );
            sprintf( buf, "#Y%s is now ON.\n\r", arg+1 );
            buf[0] = UPPER( buf[0] );
            send_to_char( buf, ch );
        }
        else
        {
            REMOVE_BIT( PCF(ch,wiznet), bit );
            sprintf( buf, "#Y%s is now OFF.\n\r", arg+1 );
            buf[0] = UPPER( buf[0] );
            send_to_char( buf, ch );
        }
    }

    return;
}

const struct position_type position_table[] =
{
    {   "dead",                 "dead"  },
    {   "mortally wounded",     "mort"  },
    {   "incapacitated",        "incap" },
    {   "stunned",              "stun"  },
    {   "sleeping",             "sleep" },
    {   "meditating",           "medit" },
    {   "sitting",              "sit"   },
    {   "resting",              "rest"  },
    {   "fighting",             "fight" },
    {   "standing",             "stand" },
    {   "",                   ""    }
};

const  struct wiznet_type wiznet_table[] =
{
    {
        "Logs",       WIZ_LOGS,   9, "#o", "LOGS",
        "[+LOGS     ] You see logged strings.\n\r",
        "[-logs     ] You do not see loged strings.\n\r"
    },

    {
        "Links", WIZ_LINKS, 8, "#y", "LINKS",
        "[+LINKS    ] Shows details of players connecting to the mud.\n\r",
        "[-links    ] Not showing details of players connecting.\n\r"
    },

    {
        "Ticks", WIZ_TICKS, 8, "#0", "TICKS",
        "[+TICKS    ] Informs you of ALL area updates.\n\r",
        "[-ticks    ] You are not informed of area updates.\n\r"
    },
    {
        "Debug", WIZ_DEBUG, 8, "#G", "BUGS",
        "[+DEBUG    ] Notifies you of any errors.\n\r",
        "[-debug    ] You are not told of errors.\n\r"
    },

    {
        "Commands", WIZ_COMMANDS, 10, "#L", "CMDS",
        "[+COMMANDS ] You are notified of use of logged immortal commands.\n\r",
        "[-commands ] You are not told of the use of logged immortal commands.\n\r"
    },

    {
        "Create", WIZ_CREATE, 8, "#7", "CREATE",
        "[+CREATE   ] You are notified of use of logged mortal commands.\n\r",
        "[-create   ] You are not told of the use of logged mortal commands.\n\r"
    },

    {
        "Misc", WIZ_MISC, 8, "#C", "MISC",
        "[+MISC     ] Miscellaneous info is on.\n\r",
        "[-misc     ] Miscellaneous info is off.\n\r"
    },

    {
        "Death", WIZ_DEATH, 8, "#R", "DEATH",
        "[+DEATH    ] You now see deaths as they occur.\n\r",
        "[-death    ] You do not see death messages.\n\r"
    },

    { NULL, 0, 0, NULL, NULL }
};

void wiznet( CHAR_DATA *ch, int chan, int level, const char *string )
{

    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int a;
    bool found;

    found = FALSE;
    for ( a = 0; wiznet_table[a].min_level != 0; a++ )
        if( wiznet_table[a].channel == chan )
    {
        found = TRUE;
        break;
    }

    if( !found )
    {
        bug( "Wiznet: Bad channel %d", chan );
        return;
    }

    sprintf( buf, "#L[#7WIZNET#L] [%s%7s#L]#n %s#n\n\r",
        wiznet_table[a].col,
        wiznet_table[a].id,
        string );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected == CON_PLAYING
            && IS_SET( PCF( CH(d),wiznet ), chan )
            && d->character != ch
            && get_trust( CH(d) ) >= level
            && ( ch ? can_see( CH( d ), ch ) : TRUE ) )
            send_to_char( buf, d->character );
    }
    return;
}

void do_ccenter(CHAR_DATA * ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0')
    {
        sprintf(buf, "  #0[#G***#0]   #YAL Control Center   #0[#G***#0]#n\n\r\n\r");
        send_to_char(buf, ch);
        sprintf(buf, "  #R(#0lexp#R) #YMinimum Experience        #G%7d#n\n\r", ccenter[CCENTER_MIN_EXP]);
        send_to_char(buf, ch);
        sprintf(buf, "  #R(#0uexp#R) #YMaximum Experience        #G%7d#n\n\r", ccenter[CCENTER_MAX_EXP]);
        send_to_char(buf, ch);
        sprintf(buf, "  #R(#0elvl#R) #YExperience Level              #G%3d#n\n\r", ccenter[CCENTER_EXP_LEVEL]);
        send_to_char(buf, ch);
        sprintf(buf, "  #R(#0qlvl#R) #YQuestpoint Level              #G%3d#n\n\r", ccenter[CCENTER_QPS_LEVEL]);
        send_to_char(buf, ch);
        send_to_char("\n\r\n\r'ccenter reset' to restore default values.\n\r", ch);
        return;
    }
    if (!str_cmp(arg1, "lexp"))
    {
        ccenter[CCENTER_MIN_EXP] = atoi(arg2);
        send_to_char("Ok. Value Changed.\n\r", ch);
        save_ccenter();
        do_ccenter(ch, "");
        return;
    }
    else if (!str_cmp(arg1, "uexp"))
    {
        ccenter[CCENTER_MAX_EXP] = atoi(arg2);
        send_to_char("Ok. Value Changed.\n\r", ch);
        save_ccenter();
        do_ccenter(ch, "");
        return;
    }
    else if (!str_cmp(arg1, "elvl"))
    {
        ccenter[CCENTER_EXP_LEVEL] = atoi(arg2);
        send_to_char("Ok. Value Changed.\n\r", ch);
        save_ccenter();
        do_ccenter(ch, "");
        return;
    }
    else if (!str_cmp(arg1, "qlvl"))
    {
        ccenter[CCENTER_QPS_LEVEL] = atoi(arg2);
        send_to_char("Ok. Value Changed.\n\r", ch);
        save_ccenter();
        do_ccenter(ch, "");
        return;
    }
    else if (!str_cmp(arg1, "reset"))
    {
        ccenter[CCENTER_MIN_EXP] = 4000;
        ccenter[CCENTER_MAX_EXP] = 3000000;
        ccenter[CCENTER_EXP_LEVEL] = 100;
        ccenter[CCENTER_QPS_LEVEL] = 100;
        send_to_char("Ok. Values Reset.\n\r", ch);
        save_ccenter();
        do_ccenter(ch, "");
        return;
    }
    else
    {
        send_to_char("Nope.\n\r", ch);
        return;
    }
}

void load_ccenter()
{
    FILE *fp;
    int i;

    if ((fp = fopen("../txt/ccenter.txt", "r")) == NULL)
    {
        log_string("Error: ccenter.txt not found!");
        exit(1);
    }
    for (i = 0; i < CCENTER_MAX; i++)
    {
        ccenter[i] = fread_number(fp);
    }
    fclose(fp);
}

void save_ccenter()
{
    FILE *fp;
    int i;

    if ((fp = fopen("../txt/ccenter.txt", "w")) == NULL)
    {
        bug("could not write to ccenter.txt!", 0);
        return;
    }
    for (i = 0; i < CCENTER_MAX; i++)
    {
        fprintf(fp, "%d\n", ccenter[i]);
    }
    fclose(fp);
}

const char *eqname1[15] =
{
    "Splendid", "Ancient", "Dusty", "Scratched",
    "Flawed", "Burnt", "Heavy", "Gilded", "Spooky", "Flaming", "Plain",
    "Ornate", "Inscrutable", "Obscene", "Wrinkled"
};
const char *eqname2[12] =
{
    "Diamond", "Emerald", "Topaz", "Kryptonite", "Jade",
    "White Gold", "Onyx", "Tin", "Glass", "Marble", "Ruby", "Granite"
};
const char *eqname3[7] =
{
    "pebble", "bauble", "stone", "charm", "fetish",
    "bone", "trinket"
};
const char *eqname4[6] =
{
    "Leather", "Studded Leather", "Bronze",
    "Chain", "Silver", "Mithral"
};

void random_eq(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    char buffer[MSL];
    char buffer2[MSL];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    bool test=FALSE;
    char *name = "[random]";
    char *name2 = "[random]";
    int hpran = number_range(300,500);
    if(number_range(1,2)==2)
    {
        hpran*=number_range(2,5);
        test=TRUE;
    }
    if(number_range(1,6)==4&&test==TRUE)hpran*=number_range(2,5);
    else test=FALSE;
    if(number_range(1,15)==16&&test==TRUE)hpran*=2;
    test=FALSE;
    int mvran = number_range(100,500);
    if(number_range(1,2)==2)
    {
        mvran*=number_range(2,5);
        test=TRUE;
    }
    if(number_range(1,6)==4&&test==TRUE)mvran*=number_range(2,5);
    else test=FALSE;
    if(number_range(1,15)==16&&test==TRUE)mvran*=2;
    test=FALSE;
    int manran = number_range(100,500);
    if(number_range(1,2)==2)
    {
        manran*=number_range(2,5);
        test=TRUE;
    }
    if(number_range(1,6)==4&&test==TRUE)manran*=number_range(2,5);
    else test=FALSE;
    if(number_range(1,15)==16&&test==TRUE)manran*=2;
    test=FALSE;

    int loc = number_range(1,12);
    int ran1 = number_range(0,14);
    int ran2 = number_range(0,11);
    //   int ran3 = number_range(0,6);
    int ran4 = number_range(0,5);
    int value =0;
    int level;
    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
        send_to_char( "Error...missing object, please inform Chrono.\n\r", ch );
        return;
    }
    level = number_range(3,14);
    obj = create_object( pObjIndex, level );
    obj->item_type = ITEM_ARMOR;
    oset_affect(ch,obj,hpran,APPLY_HIT,FALSE);
    oset_affect(ch,obj,manran,APPLY_MANA,FALSE);
    oset_affect(ch,obj,mvran,APPLY_MOVE,FALSE);
    obj->value[0] = number_range(20,100);
    obj->value[3] = number_range(1,31);
    obj->level = level;
    if      ( loc==1) value = ITEM_WEAR_FINGER;
    else if ( loc==2) value = ITEM_WEAR_NECK;
    else if ( loc==3) value = ITEM_WEAR_BODY;
    else if ( loc==4) value = ITEM_WEAR_HEAD;
    else if ( loc==5) value = ITEM_WEAR_LEGS;
    else if ( loc==6) value = ITEM_WEAR_FEET;
    else if ( loc==7) value = ITEM_WEAR_HANDS;
    else if ( loc==8) value = ITEM_WEAR_ARMS;
    else if ( loc==9) value = ITEM_WEAR_ABOUT;
    else if ( loc==10) value = ITEM_WEAR_WAIST;
    else if ( loc==11) value = ITEM_WEAR_WRIST;
    else if ( loc==12) value = ITEM_WEAR_FACE;
    value += 1;
    obj->wear_flags = value;
    obj->weight = number_range(1,4);
    obj->cost = number_range(10,1000);
    if      ( loc==1) sprintf(buffer2, "%s %s Ring encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==2) sprintf(buffer2, "%s %s Necklace encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==3) sprintf(buffer2, "%s %s Armor encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==4) sprintf(buffer2, "%s %s Helmet encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==5) sprintf(buffer2, "%s %s Leggings encrusted with %s",eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==6) sprintf(buffer2, "%s %s Boots encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==7) sprintf(buffer2, "%s %s Gloves encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==8) sprintf(buffer2, "%s %s Sleeves encrusted with %s",eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==9) sprintf(buffer2, "%s %s Cloak encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==10) sprintf(buffer2, "%s %s Belt encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==11) sprintf(buffer2, "%s %s Bracer encrusted with %s",eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==12) sprintf(buffer2, "%s %s Mask encrusted with %s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    name2 = buffer2;
    if      ( loc==1) sprintf(buffer, "#R%s #G%s #0Ring #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==2) sprintf(buffer, "#R%s #G%s #0Necklace #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==3) sprintf(buffer, "#R%s #G%s #0Armor #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==4) sprintf(buffer, "#R%s #G%s #0Helmet #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==5) sprintf(buffer, "#R%s #G%s #0Leggings #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==6) sprintf(buffer, "#R%s #G%s #0Boots #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==7) sprintf(buffer, "#R%s #G%s #0Gloves #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==8) sprintf(buffer, "#R%s #G%s #0Sleeves #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==9) sprintf(buffer, "#R%s #G%s #0Cloak #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==10) sprintf(buffer, "#R%s #G%s #0Belt #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==11) sprintf(buffer, "#R%s #G%s #0Bracer #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    else if ( loc==12) sprintf(buffer, "#R%s #G%s #0Mask #yencrusted with #C%s", eqname1[ran1],eqname4[ran4],eqname2[ran2]);
    name = buffer;
    sprintf( buf, name );
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf( buf, name2 );
    free_string( obj->name );
    obj->name = str_dup( buf );
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = NULL;
    obj->points = 0;
    obj_to_char(obj,ch);
    return;
}

char * CHAR2               args( ( long iNum ) );

void do_oscatter( CHAR_DATA *ch, char *argument )
{
    char arg [ MIL ];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    bool found;

    argument = one_argument( argument, arg );

    if( arg[0] == '\0' )
    {
        send_to_char("Syntax: scatter <item>\n\r", ch );
        send_to_char("   Will loop until you have no more in inventory.\n\r", ch );
        return;
    }

    found = FALSE;

    for( obj = get_obj_carry(ch,arg); obj; obj = get_obj_carry(ch,arg) )
    {
        if( !obj )
        {
            send_to_char("There are no more objects in your inventory.\n\r",ch);
            return;
        }
        for ( victim = char_list; victim; victim = victim->next )
        {
            if( number_range(1,20000) == 1 )
            {
                found = TRUE;
                obj_from_char(obj);
                obj_to_char( obj, victim);
            }
        }
    }
    if( found )
        send_to_char( "Done\n\r",ch);
    return;
}

void do_cforce( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
        send_to_char( "Cloak Force whom to do what?\n\r", ch );
        send_to_char( "Valid syntax: cforce <players/gods/ <command>\n\r", ch );
        return;
    }

    one_argument(argument,arg2);

    if (!str_cmp(arg2,"delete") || !str_cmp(arg2,"commit") || !str_cmp(arg2,"quit confirm"))
    {
        send_to_char("That will NOT be done.\n\r",ch);
        return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 2)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;
            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
            {
                interpret( vch, argument );
            }
        }
    }
    else if (!str_cmp(arg,"players"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 4)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
                &&   vch->level < LEVEL_HERO)
            {
                interpret( vch, argument );
            }
        }
    }
    else if (!str_cmp(arg,"gods"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (get_trust(ch) < MAX_LEVEL - 3)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;

            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
                &&   vch->level >= LEVEL_HERO)
            {
                interpret( vch, argument );
            }
        }
    }
    else
    {
        CHAR_DATA *victim;
        if ( ( victim = get_char_world( ch, arg ) ) == NULL )
        {
            send_to_char( "They aren't here.\n\r", ch );
            return;
        }

        if ( victim == ch )
        {
            send_to_char( "Aye aye, right away!\n\r", ch );
            return;
        }

        if ( get_trust( victim ) >= get_trust( ch ) )
        {
            send_to_char( "Do it yourself!\n\r", ch );
            return;
        }

        if ( !IS_NPC(victim) && get_trust(ch) < MAX_LEVEL -4)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }

        interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_donate( CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *pit;
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *original;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int amount;

    argument = one_argument(argument, arg);

    if (arg[0] == '\0' )
    {
        send_to_char("Donate what?\n\r",ch);
        return;
    }

    original = ch->in_room;
    if (ch->position == POS_FIGHTING)
    {
        send_to_char(" You're #yfighting!#n\n\r",ch);
        return;
    }
    if ( (obj = get_obj_carry (ch, arg)) == NULL)
    {
        send_to_char("You do not have that!\n\r",ch);
        return;
    }
    else
    {
        if (!can_drop_obj(ch, obj) && ch->level < 7)
        {
            send_to_char("Its stuck to you.\n\r",ch);
            return;
        }
        if (IS_SET(obj->extra_flags, ITEM_DONATED))
        {
            send_to_char("This object has already been donated try another item.\n\r",ch);
            return;
        }
        if ( (obj->item_type == ITEM_CORPSE_NPC) ||
            (obj->item_type == ITEM_CORPSE_PC) )
        {
            send_to_char("You cannot donate corpses.\n\r",ch);
            return;
        }
        if (IS_SET(obj->quest, QUEST_ANLA) ||
            IS_SET(obj->quest, QUEST_RELIC) ||
            IS_SET(obj->quest, QUEST_ARTIFACT) )
        {
            send_to_char("You cannot donate quest items.\n\r",ch);
            return;
        }
        if (IS_SET(obj->extra_flags, ITEM_ENGRAVED) || ( obj->item_type == ITEM_PIECE ) )
        {
            send_to_char("You cannot donate that item!!\n\r",ch);
            return;
        }
        if (obj->timer > 0)
        {
            send_to_char("You cannot donate that.\n\r",ch);
            return;
        }
        if (ch->in_room != get_room_index(ROOM_VNUM_ALTAR))
            act("$n donates $p.",ch,obj,NULL,TO_ROOM);
        act("You donate $p.",ch,obj,NULL,TO_CHAR);
        if ((!IS_OBJ_STAT(obj ,ITEM_ANTI_EVIL) && IS_EVIL(ch)) ||
            (!IS_OBJ_STAT(obj ,ITEM_ANTI_GOOD) && IS_GOOD(ch)) ||
            IS_NEUTRAL(ch))
            if (obj->cost > 0 && obj->level > 0)
        {
            amount = number_range(1,100);
            sprintf( buf, "You receive #C%d#n #0qps#n for your donation.",amount);
            send_to_char(buf,ch);
            ch->pcdata->quest += amount;
        }

        char_from_room(ch);
        char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
        //        obj->questowner = NULL;
        SET_BIT(obj->extra_flags, ITEM_DONATED);
        pit = get_obj_list(ch, "pit", ch->in_room->contents);
        obj_from_char(obj);
        obj_to_obj(obj, pit);
        char_from_room(ch);
        char_to_room(ch,original);
        return;
    }
}

void do_challenge(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg);
    one_argument(argument, arg2);

    if (IS_NPC(ch)) return;
    if (arg[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char("Syntax : Challenge <person> spar\n\r", ch);
        return;
    }
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("You can't challenge monsters.\n\r", ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Ehm, no.\n\r", ch);
        return;
    }
    if (victim->level != 3)
    {
        send_to_char("You can only challenge avatars.\n\r", ch);
        return;
    }
    if (victim == ch->challenger)
    {
        send_to_char("You have already challenged them.\n\r", ch);
        return;
    }
    if (!str_cmp(arg2, "death")) ch->deathmatch = TRUE;
    else if (!str_cmp(arg2, "spar")) ch->deathmatch = FALSE;
    else
    {
        do_challenge(ch, "");
        return;
    }

    if (ch->deathmatch)
    {
        /*    bool canDecap, canKillYou;

            if (!str_cmp(ch->pcdata->last_decap[0], victim->name)) canDecap = FALSE;
            else canDecap = fair_fight(ch, victim);
            if (!str_cmp(victim->pcdata->last_decap[0], ch->name)) canKillYou = FALSE;
            else canKillYou = fair_fight(victim, ch);

            if (!canKillYou || !canDecap)
            {*/
        send_to_char("You cannot challenge someone to a deathmatch. Learn to hunt pussy.\n\r", ch);
        return;
    }
    //}

    ch->challenger = victim;
    send_to_char("You challenge them.\n\r", ch);
    //  if (ch->deathmatch) sprintf(buf, "You have been challenged to a #Gdeathmatch#n by %s. Type agree %s to start the fight.\n\r", ch->name, ch->name);

    sprintf(buf, "You have been challenged to a #Gspar#n by %s. Type agree %s to start the fight.\n\r", ch->name, ch->name);
    send_to_char(buf, victim);
    WAIT_STATE(ch, 8);
    return;
}

bool in_fortress(CHAR_DATA *ch)
{
    if (ch->in_room == NULL) return FALSE;

    if ((ch->in_room->vnum >= 151) && (ch->in_room->vnum <= 160))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void fortresskill(CHAR_DATA *ch, CHAR_DATA *victim)
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;

    if (IS_NPC(ch) || IS_NPC(victim))
    {
        bug("Fortress failure", 0);
        return;
    }

    /*
     * cleanup
     */
    ch->fight_timer = 0;
    victim->fight_timer = 0;
    free_string(victim->morph);
    victim->morph = str_dup("");
    do_restore(ch, ch->name);
    do_restore(ch, victim->name);
    REMOVE_BIT(victim->extra, TIED_UP);
    REMOVE_BIT(victim->extra, GAGGED);
    REMOVE_BIT(victim->extra, BLINDFOLDED);

    /*
     * Tally the score
     */
    ch->pcdata->awins++;
    victim->pcdata->alosses++;

    /*
     * Information
     */
    sprintf(buf, "%s was beaten in The Forbidden Fortress by %s.", victim->name, ch->name);
    do_info(ch, buf);

    /*
     * Out they go.
     */
    if ((location = get_room_index(ROOM_VNUM_ALTAR)) == NULL) return;
    char_from_room(ch);
    char_to_room(ch, location);
    char_from_room(victim);
    char_to_room(victim, location);
    do_call(ch, "all");
    do_call(victim, "all");
    do_clearstats(victim, "");
    victim->level = 3;

    /*
     * Open the Fortress
     */
    arena2 = TRUE;
    return;
}

void do_decline(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (arg[0] == '\0')
    {
        send_to_char("Decline whom?\n\r", ch);
        return;
    }
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("You can't decline a monster, since it can't challenge you.\n\r", ch);
        return;
    }
    if (victim->challenger != ch)
    {
        send_to_char("They aren't challenging you. (they may have cancelled the challenge)\n\r", ch);
        return;
    }
    victim->challenger = NULL;
    send_to_char("You decline their challenge.\n\r", ch);
    send_to_char("Your challenge has been declined.\n\r", victim);
    return;
}

void do_accept2(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    bool canDecap, canKillYou;
    ROOM_INDEX_DATA *location;

    one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (!arena2)
    {
        send_to_char("The Forbidden Fortress is currently being used by someone else.\n\r", ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Accept who's challenge?\n\r", ch);
        return;
    }
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("You cannot accept a challenge from a monster.\n\r", ch);
        return;
    }
    if (victim->challenger != ch)
    {
        send_to_char("You haven't been challenged by them.\n\r", ch);
        return;
    }
    if (victim->level != 3)
    {
        send_to_char("They have to be avatar.\n\r", ch);
        return;
    }
    if (((get_age(ch) - 17) < 5) || ((get_age(victim) - 17) < 5))
    {
        send_to_char("Either you or they are in newbie hours.\n\r", ch);
        return;
    }
    if (victim->fight_timer > 0)
    {
        send_to_char("They have a fighttimer currently, you'll have to wait.\n\r", ch);
        return;
    }
    if (ch->fight_timer > 0)
    {
        send_to_char("Not with a fight timer.\n\r", ch);
        return;
    }
    if (IS_SET(victim->extra,EXTRA_AFK))
    {
        send_to_char("They are AFK!\n\r", ch);
        return;
    }
    if (victim->position != POS_STANDING)
    {
        send_to_char("They are not standing, you'll have to wait.\n\r", ch);
        return;
    }
    if (ch->in_room != NULL)
    {
        if (IS_SET(ch->in_room->room_flags, ROOM_ARENA) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
        {
            send_to_char("This room is not connected to the astral sphere.\n\r",ch);
            return;
        }
    }
    if (victim->in_room != NULL)
    {
        if (IS_SET (victim->in_room->room_flags, ROOM_ARENA) || IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
        {
            send_to_char("Their room is not connected to the astral sphere.\n\r",ch);
            return;
        }
    }

    if (victim->deathmatch)
    {
        if (!str_cmp(ch->pcdata->last_decap[0], victim->name)) canDecap = FALSE;
        else canDecap = fair_fight(ch, victim);
        if (!str_cmp(victim->pcdata->last_decap[0], ch->name)) canKillYou = FALSE;
        else canKillYou = fair_fight(victim, ch);

        if (!canKillYou || !canDecap)
        {
            send_to_char("You cannot accept a deathmatch if you cannot kill eachother.\n\r", ch);
            return;
        }
    }

    if ((location = get_room_index(ROOM_VNUM_FORTRESS1)) == NULL)
    {
        bug("Fortress Missing.", 0);
        return;

    }
    char_from_room(ch);
    char_to_room(ch, location);

    if ((location = get_room_index(ROOM_VNUM_FORTRESS2)) == NULL)
    {
        bug("Fortress Missing.", 0);
        return;
    }
    char_from_room(victim);
    char_to_room(victim, location);

    do_restore(victim, victim->name);
    do_restore(ch, ch->name);

    if (!victim->deathmatch)
        sprintf(buf, "%s and %s enter #CThe Forbidden Fortress#n to test their skills", ch->name, victim->name);
    else
        sprintf(buf, "%s and %s enter #CThe Forbidden Fortress#n to duel for their lives", ch->name, victim->name);
    do_info(ch, buf);

    arena2 = FALSE;
    if (victim->deathmatch) arena2death = TRUE;
    else arena2death = FALSE;
    victim->challenger = NULL;
    return;
}

void do_checkpk(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *check1;
    CHAR_DATA *check2;
    char buf[MAX_STRING_LENGTH];

    for (d = descriptor_list;d != NULL; d = d->next)
    {
        if ((check1 = d->character) == NULL) continue;
        if (d->connected > CON_PLAYING) continue;

        if (ch->fight_timer > 0)
        {
            sprintf(buf," %s (#C%dFT)#n\n\r", check1->name,check1->fight_timer);
            send_to_char(buf, ch);
        }

        if ((check2 = d->character->fighting) == NULL) continue;

        if (IS_NPC(check2)) continue;
        sprintf(buf, "#y%s #0(#R%d#0/#C%d#0) #nis fighting #y%s #0(#R%d#0/#C%d#0) in %d#n\n\r", check1->name,check1->hit, check1->max_hit,check2->name,check2->hit,check2->max_hit,check1->in_room->vnum);
        send_to_char(buf, ch);
        sprintf(buf," %s (#C%dFT)#n\n\r", check1->name,check1->fight_timer);
        send_to_char(buf,ch);
    }
    for (d = descriptor_list; d; d = d->next)
    {
        if ((check1 = d->character) == NULL) continue;
        if (d->connected > CON_PLAYING) continue;

        if (check1->hit <10000)
        {
            sprintf(buf, "#y%s's #Chp #nis at #0(#R%d#0/#C%d#0) #0(#RFT#0:#C%d#0)#n\n\r", check1->name,check1->hit, check1->max_hit,check1->fight_timer);
            send_to_char(buf,ch);
        }
    }
    send_to_char("Done.\n\r", ch);
    return;
}

char * CHAR2 ( long iNum )
{
    static char csdf [ 256 ];
    sprintf( csdf, "%ld", iNum );
    return csdf;
}

void do_beep(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];
    CHAR_DATA *vict;

    argument = one_argument(argument, arg1);

    if((vict = get_char_world(ch, arg1)) == NULL)
    {
        stc("They're not here!\n\r", ch);
        return;
    }

    sprintf(log_buf, "\a%s\a", argument);
    stc(log_buf, vict);

    return;
}

void randomize_object(int vnum)
{
    OBJ_DATA *obj;
    ROOM_INDEX_DATA        *rm = NULL;
    int count = 0;
    int a;

    for (a = 500; a <= 90000; a++)
    {
        if (get_room_index(a) != NULL) count++;
    }

    for (a = 500; a <= 90000; a++)
    {
        if ((rm = get_room_index(a)) != NULL && number_range(1, count * 2) == 1)
            break;
        rm = NULL;
    }

    if (rm)
    {
        obj = create_object(get_obj_index(vnum),0);
        if (!obj) return;
        obj_to_room(obj, rm);
    }
    else randomize_object(vnum);
    return;
}

void do_randomload(CHAR_DATA *ch, char *argument)
{
    if (!get_obj_index(atoi(argument)))
    {
        stc("That object doesn't exist!\n\r", ch);
        return;
    }
    stc(get_obj_index((atoi(argument)))->short_descr, ch);
    if(ch->level > 6)
    {
        stc(" randomly loaded to a room on the mud.\n\r", ch);
    }

    randomize_object(atoi(argument));

    return;
}

void do_sfind( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    extern int top_mob_index;
    extern int top_room;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    MOB_INDEX_DATA *pMobIndex;
    ROOM_INDEX_DATA *pRoom;
    int vnum;
    int nMatch;
    int count=0;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "sfind what?\n\r", ch );
        return;
    }

    fAll        = FALSE;
    found       = FALSE;
    nMatch      = 0;
    count = atoi(argument);

    send_to_char("Objects: \n\r",ch);
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
        if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
        {
            nMatch++;
            if ( fAll || is_name( arg, pObjIndex->name ) )
            {
                found = TRUE;
                sprintf( buf, "[%5d] %s\n\r",
                    pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
                send_to_char( buf, ch );
                if (count > 10 )
                    break;
                else
                    count++;
            }
        }
    }
    send_to_char("\n\r",ch);

    send_to_char("Mobiles: \n\r",ch);
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
        if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
        {
            nMatch++;
            if ( fAll || is_name( arg, pMobIndex->player_name ) )
            {
                found = TRUE;
                sprintf( buf, "[%5d] %s\n\r",
                    pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
                send_to_char( buf, ch );
                if (count > 10 )
                    break;
                else
                    count++;
            }
        }
    }
    send_to_char("\n\r",ch);

    send_to_char("Rooms: \n\r",ch);
    for ( vnum = 0; nMatch < top_room; vnum++ )
    {
        if ( ( pRoom = get_room_index( vnum ) ) != NULL )
        {
            nMatch++;
            if ( fAll || is_name( arg, pRoom->name ) )
            {
                found = TRUE;
                sprintf( buf, "[%5d] %s\n\r",
                    pRoom->vnum, pRoom->name );
                send_to_char( buf, ch );
                if (count > 10 )
                    break;
                else
                    count++;
            }
        }
    }
    send_to_char("\n\r",ch);

    if ( !found )
        send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}
