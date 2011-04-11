/****************************************
 * Original Class designed by Hutoshi	*
 * only for use by Anotherland.  Any	*
 * duplication or use of this class	*
 * without consent from Hutoshi aka	*
 * Anthony Davis is strictly prohibited.*
 * Copyrighted (C) 2003, Anthony Davis	*
 ****************************************/

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
#include "elf.h"

void do_elfblink( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
        return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_ELF) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "Elfblink to whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "You cannot find them.\n\r", ch );
        return;
    }

    if( ch->move < 500)
    {
        stc( "You don't have enough movement points to blind to them.\n\r",ch);
        return;
    }

    if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "You can't find it's room.\n\r",ch);
        return;
    }

    if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "Your body can't go there.\n\r",ch);
        return;
    }

    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
        return;
    }

    if ( room_is_private(victim->in_room ) )
    {
        send_to_char( "That room is private right now.\n\r", ch );
        return;
    }
    if ( !IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
    {
        send_to_char( "They are hiding from you.\n\r", ch );
        return;
    }
    if (victim->in_room == ch->in_room)
    {
        send_to_char("But you're already there!\n\r",ch);
        return;
    }

    act("You flicker out of existence, finding $N.", ch, NULL, victim, TO_CHAR);
    act("$n flickers out of existence, obviously sneaking up on someone.", ch, NULL, victim, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 500;
    act("$n appears out of nowhere behind $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n appears out of nowhere from behind.", ch, NULL, victim, TO_VICT);
    do_look(ch,"scry");
    return;
}

void do_elfbasics( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        send_to_char("<-------<<<<	   Elven Powers    >>>>------->\n\r",ch);

        send_to_char("\n\r", ch);

        if ( ch->pcdata->classtwo[ELF_GEN] < 1 )

            send_to_char("You have not learned any Elven powers yet.\n\r", ch);

        if ( ch->pcdata->classtwo[ELF_GEN] >= 1 )

            send_to_char("	  Elven Eyes  -  Truesight, Nightsight\n\r", ch);

        if ( ch->pcdata->classtwo[ELF_GEN] >= 2 )

            send_to_char("	  Elven Ears  -  Scry\n\r", ch);

        if ( ch->pcdata->classtwo[ELF_GEN] >= 3 )

            send_to_char("  Elven Intuition  -  Readaura\n\r", ch);

        if ( ch->pcdata->classtwo[ELF_GEN] >= 4 )

            send_to_char(" Elven Protection  -  Shield your aura\n\r",ch);

        if ( ch->pcdata->classtwo[ELF_GEN] >= 5 )

            send_to_char("	Elven Vanish  -  Vanish from sight\n\r", ch);

        if ( ch->pcdata->classtwo[ELF_GEN] >= 6 )

            send_to_char("   Elven Strength  -  Increase your strength\n\r", ch);

        if ( ch->pcdata->classtwo[ELF_GEN] >= 7 )

            send_to_char("   You have completed your elven training.\n\r", ch);

        send_to_char("\n\r<-------<<<<	Elven Powers	>>>>------->\n\r",ch);
        return;
    }

    if (!str_cmp(arg2,"improve"))
    {
        int improve;
        int cost;
        int max;

        if (!str_cmp(arg1,"general"   ))
        {
            improve = ELF_GEN;
            max=6;
        }
        else
        {
            send_to_char("<-------<<<<	   Elven Powers    >>>>------->\n\r",ch);
            send_to_char("	  Elven Eyes  -  Truesight, Nightsight \n\r",ch);
            send_to_char("	  Elven Ears  -  Scry		       \n\r",ch);
            send_to_char("   Elven Intuition  -  Readaura	       \n\r",ch);
            send_to_char("  Elven Protection  -  Shield your aura      \n\r",ch);
            send_to_char("	Elven Vanish  -  Vanish from sight     \n\r",ch);
            send_to_char("    Elven Strength  -  Increase your strength\n\r",ch);
            send_to_char("<-------<<<<	   Elven Powers    >>>>------->\n\r\n\r",ch);
            return;
        }

        cost = (ch->pcdata->classtwo[improve]) * 500;
        arg1[0] = UPPER(arg1[0]);
        if ( ch->pcdata->classtwo[improve] >= max )
        {
            sprintf(buf,"You have already gained all your Elfish powers.\n\r");
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->practice )
        {
            sprintf(buf,"It costs you %d primal learn more Elven powers.\n\r", cost);
            send_to_char(buf,ch);
            return;
        }

        ch->pcdata->classtwo[improve] += 1;
        ch->practice -= cost;
        sprintf(buf,"You learn another Elven power.\n\r");
        send_to_char(buf,ch);
    }
    else send_to_char("To learn an Elven power, type: Elfbasics general improve.\n\r",ch);
    return;
}

void do_elfmagetrain( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_SET(ch->special, SPC_ELF_MAGE))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        send_to_char("<-------<<<<     Elf Stones    >>>>------->\n\r",ch);
        sprintf(buf,"	#GGreen #C(%d)	   #RRed #C(%d)     #LBlue #C(%d)\n\r",
            ch->pcdata->classtwo[ELF_GREENSTONE], ch->pcdata->classtwo[ELF_REDSTONE],
            ch->pcdata->classtwo[ELF_BLUESTONE]);
        send_to_char(buf,ch);
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"green"))
        {
            send_to_char("<-------<<<<     Green Elfstone    >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_GREENSTONE] < 1)
                send_to_char("You have no knowledge of the green elfstone.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_GREENSTONE] > 0)
                send_to_char("EarthBless - Recieve a bless from mother earth.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_GREENSTONE] > 1)
                send_to_char("NaturesTouch - Recieve a heal by natures touch.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_GREENSTONE] > 2)
                send_to_char("NaturesFury - Call nature to aid your attack.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Green Elfstone    >>>>------->\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"red"))
        {
            send_to_char("<-------<<<<     Red Elfstone    >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_REDSTONE] < 1)
                send_to_char("You have no knowledge of the red elfstone.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_REDSTONE] > 0)
                send_to_char("FireBless - Recieve a bless from fire.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_REDSTONE] > 1)
                send_to_char("FlameRing - Ring of fire around you.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_REDSTONE] > 2)
                send_to_char("FireCurse - Recieve a curse from fire.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Red Elfstone	 >>>>------->\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"blue"))
        {
            send_to_char("<-------<<<<     Blue Elfstone    >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_BLUESTONE] < 1)
                send_to_char("You have no knowledge of the blue elfstone.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_BLUESTONE] > 0)
                send_to_char("WaterBless - Recieve a bless from the water.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_BLUESTONE] > 1)
                send_to_char("WaterWall - Create barriers of water around you.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_BLUESTONE] > 2)
                send_to_char("IceTouch - Freeze your opponent.	(stun power)\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Blue Elfstone	  >>>>------->\n\r",ch);
            return;
        }
        send_to_char("<-------<<<<     Elf Stones    >>>>------->\n\r",ch);
        sprintf(buf,"	#GGreen #C(%d)	   #RRed #C(%d)     #LBlue #C(%d)\n\r",
            ch->pcdata->classtwo[ELF_GREENSTONE], ch->pcdata->classtwo[ELF_REDSTONE],
            ch->pcdata->classtwo[ELF_BLUESTONE]);
        send_to_char(buf,ch);
        return;
    }
    if (!str_cmp(arg2,"learn"))
    {
        int improve;
        int cost;
        int max;

        if (!str_cmp(arg1,"green"   )) {improve = ELF_GREENSTONE;   max=3;}
        else if (!str_cmp(arg1,"red"   )) {improve = ELF_REDSTONE;   max=3;}
        else if (!str_cmp(arg1,"blue"   )) {improve = ELF_BLUESTONE; max=3;}
        else
        {
            send_to_char("<-------<<<<     Elf Stones    >>>>------->\n\r",ch);
            send_to_char("	  #GGreen	    #RRed	     #LBlue#n\n\r",ch);
            return;
        }
        cost = (ch->pcdata->classtwo[improve]+1) * 500;
        arg1[0] = UPPER(arg1[0]);
        if ( ch->pcdata->classtwo[improve] >= max )
        {
            sprintf(buf,"You have already learned all the powers of the %s elfstone.\n\r", arg1);
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->practice )
        {
            sprintf(buf,"It costs you %d primal to learn your %s elfstone.\n\r", cost, arg1);
            send_to_char(buf,ch);
            return;
        }
        ch->pcdata->classtwo[improve] += 1;
        ch->practice -= cost;
        sprintf(buf,"You improve your knowledge in the %s elfstone.\n\r", arg1);
        send_to_char(buf,ch);
    }
    else send_to_char("To learn an elfstone power, type: elfmagetrain <elfstone color> learn.\n\r",ch);
    return;
}

void do_elfwartrain( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_SET(ch->special, SPC_ELF_WARRIOR))
    {
        send_to_char("You are not an elven warrior.\n\r",ch);
        return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        send_to_char("<-------<<<<     Elf Warriors    >>>>------->\n\r",ch);
        sprintf(buf,"	   #GFury #7(%d)  #GRage #7(%d)  #GTechnique #7(%d)#n\n\r",
            ch->pcdata->classtwo[ELF_WAR_FURY], ch->pcdata->classtwo[ELF_WAR_RAGE],
            ch->pcdata->classtwo[ELF_WAR_TECH]);
        send_to_char(buf,ch);
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"fury"))
        {
            send_to_char("<-------<<<<     Elven Fury    >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_FURY] < 1)
                send_to_char("You have no fury within you.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_FURY] > 0)
                send_to_char("Elven Fury level 1 - Increase attacks.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_FURY] > 1)
                send_to_char("Elven Fury level 2 - Increase attacks.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_FURY] > 2)
                send_to_char("Elven Fury level 3 - Increase attacks.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_FURY] > 3)
                send_to_char("Elven Fury level 4 - Increase attacks.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_FURY] > 4)
                send_to_char("Elven Fury level 5 - Increase attacks.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Elven Fury    >>>>------->\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"rage"))
        {
            send_to_char("<-------<<<<     Elven Rage    >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_RAGE] < 1)
                send_to_char("You have no rage within you.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 0)
                send_to_char("Elven Rage level 1 - Increase damage.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 1)
                send_to_char("Elven Rage level 2 - Increase damage.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 2)
                send_to_char("Elven Rage level 3 - Increase damage.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 3)
                send_to_char("Elven Rage level 4 - Increase damage.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 4)
                send_to_char("Elven Rage level 5 - Increase damage.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Elven Rage    >>>>------->\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"technique"))
        {
            send_to_char("<-------<<<<     Elven Technique	>>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_TECH] < 1)
                send_to_char("You have no technique within you.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_TECH] > 0)
                send_to_char("Elfweb - Web your enemies to prevent escape.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_TECH] > 1)
                send_to_char("Elfpoison - put poison on the end of your swords.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_TECH] > 2)
                send_to_char("Ancestorswill - Call ancestors to aid in battle.(auto)\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_TECH] > 3)
                send_to_char("Swordslash - Mid-round attack with your swords.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_WAR_TECH] > 4)
                send_to_char("Wishsong - Powerful attack by use of song.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Elven Technique    >>>>------->\n\r",ch);
            return;
        }
        send_to_char("<-------<<<<     Elf Warriors    >>>>------->\n\r",ch);
        sprintf(buf,"	   #GFury #7(%d)  #GRage #7(%d)  #GTechnique #7(%d)#n\n\r",
            ch->pcdata->classtwo[ELF_WAR_FURY], ch->pcdata->classtwo[ELF_WAR_RAGE],
            ch->pcdata->classtwo[ELF_WAR_TECH]);
        send_to_char(buf,ch);
        return;
    }

    if (!str_cmp(arg2,"improve"))
    {
        int improve;
        int cost;
        int max;

        if (!str_cmp(arg1,"fury"   )) {improve = ELF_WAR_FURY;   max=5;}
        else if (!str_cmp(arg1,"rage"   )) {improve = ELF_WAR_RAGE;   max=5;}
        else if (!str_cmp(arg1,"technique"   )) {improve = ELF_WAR_TECH; max=5;}
        else
        {
            send_to_char("<-------<<<<     Elf Warriors    >>>>------->\n\r",ch);
            send_to_char("	      Fury	   Rage        Technique#n\n\r",ch);
            return;
        }
        cost = (ch->pcdata->classtwo[improve]+1) * 500;
        arg1[0] = UPPER(arg1[0]);
        if ( ch->pcdata->classtwo[improve] >= max )
        {
            sprintf(buf,"You have already raised all your %s.\n\r", arg1);
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->practice )
        {
            sprintf(buf,"It costs you %d primal to raise your %s.\n\r", cost, arg1);
            send_to_char(buf,ch);
            return;
        }
        ch->pcdata->classtwo[improve] += 1;
        ch->practice -= cost;
        sprintf(buf,"You raise your %s even more.\n\r", arg1);
        send_to_char(buf,ch);
    }
    else send_to_char("To learn a warrior power, type: elfwartrain <warrior power> improve.\n\r",ch);
    return;
}

void do_elfarctrain( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_SET(ch->special, SPC_ELF_ARCHER))
    {
        send_to_char("You are not an elven archer.\n\r",ch);
        return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        send_to_char("<-------<<<<	    Elf Archers 	 >>>>------->\n\r",ch);
        sprintf(buf," #GFireArrow:#7%d #GIceArrow:#7%d #GLightningArrow:#7%d #GArcherPower:#7%d#n\n\r",
            ch->pcdata->classtwo[ELF_ARC_FIREARROW], ch->pcdata->classtwo[ELF_ARC_ICEARROW],
            ch->pcdata->classtwo[ELF_ARC_LIGHTARROW], ch->pcdata->classtwo[ELF_ARC_ARCPOWER]);
        send_to_char(buf,ch);
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"firearrow"))
        {
            send_to_char("<-------<<<<       Fire Arrow      >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_FIREARROW] < 1)
                send_to_char("You have no use of the fire arrows.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 0)
                send_to_char("Fire Arrow - You gain use of the fire arrow.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 1)
                send_to_char("Fire Arrow - Your fire arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 2)
                send_to_char("Fire Arrow - Your fire arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 3)
                send_to_char("Fire Arrow - Your fire arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 4)
                send_to_char("Fire Arrow - Your fire arrow damage increases.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Fire Arrow    >>>>------->\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"icearrow"))
        {
            send_to_char("<-------<<<<       Ice Arrow      >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ICEARROW] < 1)
                send_to_char("You have no use of the ice arrows.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 0)
                send_to_char("Ice Arrow - You gain use of the ice arrow.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 1)
                send_to_char("Ice Arrow - Your ice arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 2)
                send_to_char("Ice Arrow - Your ice arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 3)
                send_to_char("Ice Arrow - Your ice arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 4)
                send_to_char("Ice Arrow - Your ice arrow damage increases.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Ice Arrow    >>>>------->\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"lightningarrow"))
        {
            send_to_char("<-------<<<<       Lightning Arrow	    >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] < 1)
                send_to_char("You have no use of the lightning arrows.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 0)
                send_to_char("Lightning Arrow - You gain use of the lightning arrow.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 1)
                send_to_char("Lightning Arrow - Your lightning arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 2)
                send_to_char("Lightning Arrow - Your lightning arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 3)
                send_to_char("Lightning Arrow - Your lightning arrow damage increases.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 4)
                send_to_char("Lightning Arrow - Your lightning arrow damage increases.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Lightning Arrow    >>>>------->\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"archerpower"))
        {
            send_to_char("<-------<<<<       Archer Powers	  >>>>------->\n\r\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ARCPOWER] < 1)
                send_to_char("You have no knowledge of any archer powers.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ARCPOWER] > 0)
                send_to_char("Web Arrow - Web your opponents by using an arrow.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ARCPOWER] > 1)
                send_to_char("Elven Aim - Shadowsight.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ARCPOWER] > 2)
                send_to_char("ArchersFury - Midround attack with lots of arrows.\n\r",ch);
            if (ch->pcdata->classtwo[ELF_ARC_ARCPOWER] > 3)
                send_to_char("ArchersRage - Raging power.\n\r",ch);
            send_to_char("\n\r<-------<<<<	 Archer Powers	  >>>>------->\n\r",ch);
            return;
        }
        send_to_char("<-------<<<<	    Elf Archers 	 >>>>------->\n\r",ch);
        sprintf(buf," #GFireArrow:#7%d #GIceArrow:#7%d #GLightningArrow:#7%d #GArcherPower:#7%d#n\n\r",
            ch->pcdata->classtwo[ELF_ARC_FIREARROW], ch->pcdata->classtwo[ELF_ARC_ICEARROW],
            ch->pcdata->classtwo[ELF_ARC_LIGHTARROW], ch->pcdata->classtwo[ELF_ARC_ARCPOWER]);
        send_to_char(buf,ch);
        return;
    }

    if (!str_cmp(arg2,"improve"))
    {
        int improve;
        int cost;
        int max;

        if (!str_cmp(arg1,"firearrow"   )) {improve = ELF_ARC_FIREARROW;   max=5;}
        else if (!str_cmp(arg1,"icearrow"   )) {improve = ELF_ARC_ICEARROW;   max=5;}
        else if (!str_cmp(arg1,"lightningarrow"   )) {improve = ELF_ARC_LIGHTARROW;   max=5;}
        else if (!str_cmp(arg1,"archerpower"   )) {improve = ELF_ARC_ARCPOWER; max=4;}
        else
        {
            send_to_char("<-------<<<<	  Elf Archers	    >>>>------->\n\r",ch);
            send_to_char(" FireArrow  IceArrow  LightningArrow  ArcherPower#n\n\r",ch);
            return;
        }
        cost = (ch->pcdata->classtwo[improve]+1) * 500;
        arg1[0] = UPPER(arg1[0]);
        if ( ch->pcdata->classtwo[improve] >= max )
        {
            sprintf(buf,"You have already raised all your %s power.\n\r", arg1);
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->practice )
        {
            sprintf(buf,"It costs you %d primal to raise your %s power.\n\r", cost, arg1);
            send_to_char(buf,ch);
            return;
        }
        ch->pcdata->classtwo[improve] += 1;
        ch->practice -= cost;
        sprintf(buf,"You raise your %s even more.\n\r", arg1);
        send_to_char(buf,ch);
    }
    else send_to_char("To learn an archer power, type: elfarctrain <archer power> improve.\n\r",ch);
    return;
}

void do_elfprofession( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch))return;

    if (!IS_CLASS(ch, CLASS_ELF))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (IS_SET(ch->special, SPC_ELF_MAGE) || IS_SET(ch->special, SPC_ELF_WARRIOR)
        || IS_SET(ch->special, SPC_ELF_ARCHER))
    {
        send_to_char("You already have a profession.\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char("To choose a profession type: elfprofession <profession>.\n\r",ch);
        send_to_char("Valid professions are MAGE, WARRIOR, ARCHER.\n\r",ch);
        return;
    }

    if (!str_cmp(arg1,"mage"))
    {
        SET_BIT(ch->special, SPC_ELF_MAGE);
        send_to_char("You are now an elf mage.\n\r",ch);
    }

    else if (!str_cmp(arg1,"warrior"))
    {
        SET_BIT(ch->special, SPC_ELF_WARRIOR);
        send_to_char("You are now an elven warrior.\n\r",ch);
    }

    else if (!str_cmp(arg1,"archer"))
    {
        SET_BIT(ch->special, SPC_ELF_ARCHER);
        send_to_char("You are now an elven archer.\n\r",ch);
    }

    else do_elfprofession(ch,"");
    return;
}

void do_elveneyes( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) )
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if (IS_CLASS(ch, CLASS_ELF) && ch->pcdata->classtwo[ELF_GEN] < 1 )
    {
        send_to_char("You must learn your basic elven powers to level 1.\n\r", ch);
        return;
    }

    if (arg1[0] == '\0' )
    {
        send_to_char("You must choose either truesight or nightsight.\n\r",ch);
        return;
    }

    if (!str_cmp(arg1,"truesight"))
    {
        if (ch->move < 500)
        {
            send_to_char("You don't have 500 move to increase your awareness.\n\r", ch);
            return;
        }

        if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
        {
            REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
            send_to_char( "You're eyes return to normal human eyes.\n\r", ch );
        }
        else
        {
            SET_BIT(ch->act, PLR_HOLYLIGHT);
            send_to_char( "Your eyes shift and you begin to see with your elven eyes.\n\r", ch );
            ch->move -= 500;
        }
    }
    else if (!str_cmp(arg1,"nightsight"))
    {
        if (ch->move < 500)
        {
            send_to_char("You don't have 500 move to increase your awareness.\n\r", ch);
            return;
        }

        if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
        {
            send_to_char("The red glow in your eyes fades.\n\r",ch);
            act("The red glow in $n's eyes fades.", ch, NULL, NULL, TO_ROOM);
            REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
        }
        else
        {
            send_to_char("Your eyes start glowing red.\n\r",ch);
            act("$n's eyes start glowing red.", ch, NULL, NULL, TO_ROOM);
            SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
        }
    }
    else send_to_char("You must type: elveneyes <truesight/nightsight>.\n\r",ch);
    return;
}

void do_elvenears( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || ch->pcdata->classtwo[ELF_GEN] < 2)
    {
        send_to_char("You must learn your basic elven powers to level 2.\n\r", ch );
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char( "Scry on whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
        REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
        do_look(ch,"auto");
        SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
        SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
        do_look(ch,"auto");
        REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
        do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    if (!IS_NPC(victim)) ch->fight_timer += 5;
    return;
}

void do_elvenintuition( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || ch->pcdata->classtwo[ELF_GEN] < 3)
    {
        send_to_char( "You must learn your basic elven powers to level 3.\n\r", ch );
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "Read the aura on what?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
        {
            send_to_char( "Read the aura on what?\n\r", ch );
            return;
        }
        act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
        spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
        return;
    }

    if (!IS_NPC(victim) && (IS_IMMUNE(victim,IMM_SHIELDED) ||
        (IS_CLASS(victim, CLASS_DROW) &&
        IS_SET(victim->pcdata->powers[1],DPOWER_DROWSHIELD))) && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
        send_to_char("You are unable to read their aura.\n\r",ch);
        return;
    }

    act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you intently.",ch,NULL,victim,TO_VICT);
    if (IS_NPC(victim)) sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else
    {
        if  (victim->level == 13) sprintf(buf, "%s is a CREATOR.\n\r", victim->name);
        else if (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
        else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
        else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
        else if (victim->level == 9 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
        else if (victim->level == 8 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
        else if (victim->level == 7 ) sprintf(buf, "%s is an Intern.\n\r", victim->name);
        else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
        else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
    }
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
        sprintf(buf,"Str:%d, Int:%d, Wis:%d, Dex:%d, Con:%d.\n\r",get_curr_str(victim),get_curr_int(victim),get_curr_wis(victim),get_curr_dex(victim),get_curr_con(victim));
        send_to_char(buf,ch);
    }
    sprintf(buf,"Hp:%d/%d, Mana:%d/%d, Move:%d/%d.\n\r",victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
    send_to_char(buf,ch);
    if (!IS_NPC(victim)) sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
    else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
    send_to_char(buf,ch);
    if (!IS_NPC(victim))
    {
        if (IS_CLASS(victim, CLASS_VAMPIRE))
        {
            sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
            send_to_char(buf,ch);
        }
    }
    sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
    if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT))
        act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
    {
        send_to_char("Disciplines:",ch);
        if (IS_VAMPAFF(victim, VAM_PROTEAN)) send_to_char(" Protean",ch);
        if (IS_VAMPAFF(victim, VAM_CELERITY)) send_to_char(" Celerity",ch);
        if (IS_VAMPAFF(victim, VAM_FORTITUDE)) send_to_char(" Fortitude",ch);
        if (IS_VAMPAFF(victim, VAM_POTENCE)) send_to_char(" Potence",ch);
        if (IS_VAMPAFF(victim, VAM_OBFUSCATE)) send_to_char(" Obfuscate",ch);
        if (IS_VAMPAFF(victim, VAM_OBTENEBRATION)) send_to_char(" Obtenebration",ch);
        if (IS_VAMPAFF(victim, VAM_SERPENTIS)) send_to_char(" Serpentis",ch);
        if (IS_VAMPAFF(victim, VAM_AUSPEX)) send_to_char(" Auspex",ch);
        if (IS_VAMPAFF(victim, VAM_DOMINATE)) send_to_char(" Dominate",ch);
        if (IS_VAMPAFF(victim, VAM_PRESENCE)) send_to_char(" Presence",ch);
        if (IS_VAMPAFF(victim, VAM_VICISSITUDE)) send_to_char(" Vicissitude",ch);
        if (IS_VAMPAFF(victim, VAM_THAU)) send_to_char(" Thaumaturgy",ch);
        if (IS_VAMPAFF(victim, VAM_ANIMAL)) send_to_char(" Animalism",ch);
        if (IS_VAMPAFF(victim, VAM_QUIETUS)) send_to_char(" Quietus",ch);
        send_to_char(".\n\r",ch);
    }
    return;
}

void do_elvenvanish( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) )
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if (IS_CLASS(ch, CLASS_ELF) && ch->pcdata->classtwo[ELF_GEN] < 5 )
    {
        send_to_char("You must learn your basic elven powers to level 5.\n\r", ch);
        return;
    }

    if (ch->move < 5000)
    {
        send_to_char("You don't have 5000 move to activate your power.\n\r", ch);
        return;
    }

    if (ch->fight_timer >0)
    {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
    }

    if ( IS_SET(ch->act, PLR_SUPERINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SUPERINVIS);
        act( "$n appears from thin air.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "You appear from thin air.\n\r", ch );
    }
    else
    {
        act( "$n vanishes into thin air.", ch, NULL, NULL, TO_ROOM );
        send_to_char( "You vanish into thin air.\n\r", ch );
        ch->move -= 5000;
        SET_BIT(ch->act,PLR_SUPERINVIS);
    }
    return;
}

void do_elvenstrength( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (IS_CLASS(ch, CLASS_ELF) && ch->pcdata->classtwo[ELF_GEN] < 6 )
    {
        send_to_char("You must learn your basic elven powers to level 6.\n\r",ch);
        return;
    }

    if( IS_SET(ch->pcdata->classtwo[ELF_GEN], ELVEN_STRENGTH))
    {
        REMOVE_BIT(ch->pcdata->classtwo[ELF_GEN], ELVEN_STRENGTH);
        ch->damroll -= 100;
        ch->hitroll -= 100;
        ch->armor   += 300;
        send_to_char("You feel weaker.\n\r",ch);
        return;
    }
    else if( !IS_SET(ch->pcdata->classtwo[ELF_GEN], ELVEN_STRENGTH))
    {
        SET_BIT(ch->pcdata->classtwo[ELF_GEN], ELVEN_STRENGTH);
        ch->damroll += 100;
        ch->hitroll += 100;
        ch->armor   -= 300;
        send_to_char("You feel stronger.\n\r",ch);
        return;
    }
}

void do_elfarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_ELF))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
        send_to_char("Sword, Ring, Collar, Wristbands, Plate,\n\r", ch);
        send_to_char("Helmet, Leggings, Boots, Gauntlets,\n\r", ch );
        send_to_char("Paddings, Cape, Girdle, Visor.\n\r", ch);
        return;
    }
    if (ch->pcdata->upgrade_level >= 1)
    {
        if      (!str_cmp(arg,"sword"                )) vnum = 110100;
        else if (!str_cmp(arg,"ring"                 )) vnum = 110101;
        else if (!str_cmp(arg,"collar"               )) vnum = 110102;
        else if (!str_cmp(arg,"wristbands"           )) vnum = 110103;
        else if (!str_cmp(arg,"plate"                )) vnum = 110104;
        else if (!str_cmp(arg,"helmet"               )) vnum = 110105;
        else if (!str_cmp(arg,"leggings"             )) vnum = 110106;
        else if (!str_cmp(arg,"boots"                )) vnum = 110107;
        else if (!str_cmp(arg,"gauntlets"            )) vnum = 110108;
        else if (!str_cmp(arg,"paddings"             )) vnum = 110109;
        else if (!str_cmp(arg,"cape"                 )) vnum = 110110;
        else if (!str_cmp(arg,"girdle"               )) vnum = 110111;
        else if (!str_cmp(arg,"visor"                )) vnum = 110112;
        else
        {
            do_elfarmor(ch,"");
            return;
        }
    }

    else
    {
        if      (!str_cmp(arg,"sword"       )) vnum = 33300;
        else if (!str_cmp(arg,"ring"        )) vnum = 33301;
        else if (!str_cmp(arg,"collar"      )) vnum = 33302;
        else if (!str_cmp(arg,"wristbands"          )) vnum = 33303;
        else if (!str_cmp(arg,"plate"       )) vnum = 33304;
        else if (!str_cmp(arg,"helmet"      )) vnum = 33305;
        else if (!str_cmp(arg,"leggings"        )) vnum = 33306;
        else if (!str_cmp(arg,"boots"       )) vnum = 33307;
        else if (!str_cmp(arg,"gauntlets"           )) vnum = 33308;
        else if (!str_cmp(arg,"paddings"        )) vnum = 33309;
        else if (!str_cmp(arg,"cape"        )) vnum = 33310;
        else if (!str_cmp(arg,"girdle"      )) vnum = 33311;
        else if (!str_cmp(arg,"visor"       )) vnum = 33312;
        else
        {
            do_elfarmor(ch,"" );
            return;
        }
    }
    if ( ch->practice < 60)
    {
        send_to_char("It costs 60 points of primal to create a piece of elf armour.\n\r",ch);
        return;
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Hutoshi.\n\r",ch);
        return;
    }
    ch->practice -= 60;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

/*
 * Green Elfstone Powers
 */

void do_earthbless( CHAR_DATA * ch, char * argument)
{
    OBJ_DATA * obj;
    int sn;
    int dam;
    AFFECT_DATA af;

    if(IS_NPC(ch))
    {
        return;
    }

    if(!IS_CLASS(ch, CLASS_ELF) || !IS_SET(ch->special, SPC_ELF_MAGE))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if(ch->pcdata->classtwo[ELF_GREENSTONE] < 1)
    {
        send_to_char("You must first learn your green elfstone powers to level 1.\n\r", ch);
        return;
    }

    for(obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
        if(obj->item_type == ITEM_GREENSTONE)
        {
            break;
        }
    }

    if(obj == NULL)
    {
        send_to_char("You are not carrying a #GGreen#n Elfstone.\n\r", ch);
        return;
    }

    if(ch->mana < 1000)
    {
        send_to_char("You don't have enough mana.\n\r", ch);
        return;
    }

    sn = skill_lookup("green sorcery");
    dam = ch->spl[GREEN_MAGIC]*2;

    if(is_affected(ch, sn))
    {
        return;
    }

    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HIT;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_AC;
    af.modifier  = -dam/2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MOVE;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MANA;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HITROLL;
    af.modifier  = dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_DAMROLL;
    af.modifier  = dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);

    send_to_char("You raise up a #Ggreen#n elfstone and it begins to glow.\n\r",ch);
    send_to_char("You are blessed by the #Ggreen#n elfstone!\n\r", ch);
    act("$n raises up a #Ggreen#n elfstone and it begins to glow",ch,NULL,NULL,TO_ROOM);
    act("$n is blessed by the #Ggreen#n elfstone!",ch,NULL,NULL,TO_ROOM);

    WAIT_STATE(ch, 2);
    ch->mana -= 1000;
    return;
}

void do_naturestouch(CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    char arg[MAX_STRING_LENGTH];

    argument=one_argument(argument,arg);

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classtwo[ELF_GREENSTONE] < 2)
    {
        send_to_char("You must first learn your green elfstone powers to level 2.\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, "greenstone" ) ) == NULL )
    {
        send_to_char("You are not carrying a #GGreen #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_GREENSTONE )
    {
        send_to_char( "That's not a #GGreen #nElfstone.\n\r", ch );
        return;
    }

    send_to_char("You raise a #GGreen#n elfstone into the air.\n\r",ch);
    send_to_char("You feel nature's touch and begin to regain your strength.\n\r",ch);
    ch->hit = UMIN(ch->hit + (ch->spl[GREEN_MAGIC] * 2.5), ch->max_hit);
    return;
}

void do_naturesfury(CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    char buf [MAX_INPUT_LENGTH];
    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int spell;
    AFFECT_DATA af;
    argument=one_argument(argument,arg);

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classtwo[ELF_GREENSTONE] < 3)
    {
        send_to_char("You must first learn your green elfstone powers to level 3.\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, "greenstone" ) ) == NULL )
    {
        send_to_char("You are not carrying a #GGreen #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_GREENSTONE )
    {
        send_to_char( "That's not a #GGreen #nElfstone.\n\r", ch );
        return;
    }

    if (arg[0]=='\0')
    {
        send_to_char("Summon what?\n\rYour choices are:\n\r",ch);
        send_to_char("BUNNY  SNAKE  FOX  TIMBERWOLF  BOAR\n\r",ch);
        send_to_char("BROWNBEAR  BLACKBEAR  WHITEWOLF CLONE WHITETIGER\n\r",ch);
        return;
    }

    spell = ch->spl[GREEN_MAGIC];

    if (ch->pcdata->followers > 1)
    {
        send_to_char("Nothing happens.\n\r",ch);
        return;
    }

    ch->pcdata->followers++;
    victim = create_mobile( get_mob_index( MOB_VNUM_DOG ) );

    if (!str_cmp(arg,"rabbit")&& spell > 25)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("bunny");
        victim->short_descr=str_dup("a little bunny");
        victim->long_descr=str_dup("a little bunny stands here.");
        victim->level = 40;
        victim->hit = 300;
        victim->max_hit = 300;
        victim->hitroll = 40;
        victim->damroll = 40;
    }
    else if (!str_cmp(arg,"snake")&& spell > 50)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("snake");
        victim->short_descr=str_dup("a slithering snake");
        victim->long_descr=str_dup("a slithering snake stands here.");
        victim->level = 50;
        victim->hit = 400;
        victim->max_hit = 400;
        victim->hitroll = 50;
        victim->damroll = 50;
    }
    else if (!str_cmp(arg,"fox") && spell > 75)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("sly fox");
        victim->short_descr=str_dup("a Sly Fox");
        victim->long_descr=str_dup("a Sly Fox stands here.");
        victim->level = 60;
        victim->hit = 500;
        victim->max_hit = 500;
        victim->hitroll = 60;
        victim->damroll = 60;
    }
    else if (!str_cmp(arg,"timberwolf") && spell > 100)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("timber wolf");
        victim->short_descr=str_dup("a Huge Timber Wolf");
        victim->long_descr=str_dup("a Huge Timber Wolf stands here.");
        victim->level = 70;
        victim->hit = 2000;
        victim->max_hit = 2000;
        victim->hitroll = 70;
        victim->damroll = 70;
    }
    else if (!str_cmp(arg,"boar")&& spell > 125)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("boar");
        victim->short_descr=str_dup("a Huge Boar");
        victim->long_descr=str_dup("a Huge Boar stands here.");
        victim->level = 100;
        victim->hit = 3000;
        victim->max_hit = 3000;
        victim->hitroll = 100;
        victim->damroll = 100;
    }
    else if (!str_cmp(arg,"brownbear") && spell > 150)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("brown bear");
        victim->short_descr=str_dup("a Huge Brown Bear");
        victim->long_descr=str_dup("a Brown Bear stands here.");
        victim->level = 150;
        victim->hit = 4000;
        victim->max_hit = 4000;
        victim->hitroll = 150;
        victim->damroll = 150;
    }
    else if (!str_cmp(arg,"blackbear") && spell > 175)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("black bear");
        victim->short_descr=str_dup("a Huge Black Bear");
        victim->long_descr=str_dup("a Huge Bear stands here.");
        victim->level = 200;
        victim->hit = 5000;
        victim->max_hit = 5000;
        victim->hitroll = 200;
        victim->damroll = 200;
    }
    else if (!str_cmp(arg,"whitewolf")&& spell > 200)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("white tiger");
        victim->short_descr=str_dup("a Huge White Wolf");
        victim->long_descr=str_dup("a Huge Wolf stands here.");
        victim->level = 250;
        victim->hit = 6000;
        victim->max_hit = 6000;
        victim->hitroll = 250;
        victim->damroll = 250;
    }
    else if (!str_cmp(arg,"clone")&& spell > 225)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup(ch->name);
        victim->short_descr=str_dup(ch->name);
        victim->long_descr=str_dup(ch->name);
        victim->level = 500;
        victim->hit = 10000;
        victim->max_hit = 10000;
        victim->hitroll =500;
        victim->damroll =500;
    }
    else if (!str_cmp(arg,"whitetiger")&& spell > 250)
    {
        free_string(victim->short_descr);
        free_string(victim->name);
        free_string(victim->long_descr);
        victim->name=str_dup("white tiger");
        victim->short_descr=str_dup("a Huge White Tiger");
        victim->long_descr=str_dup("a Huge White Tiger stands here.");
        victim->level = 2500;
        victim->hit = 80000;
        victim->max_hit = 30000;
        victim->hitroll = 4000;
        victim->damroll = 4000;
    }
    else
    {
        send_to_char("Your incorrect call attracts a weak partner\n\r",ch);
    }

    sprintf(buf,"%s runs out of nowhere.\n\r",victim->short_descr);
    send_to_char(buf,ch);

    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf,"%s\n\r",victim->long_descr);
    free_string(victim->long_descr);
    victim->long_descr=str_dup(buf);
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);

    return;
}

/*
 * Red Elfstone Powers
 */

void do_firebless( CHAR_DATA * ch, char * argument)
{
    OBJ_DATA * obj;
    int sn;
    int dam;
    AFFECT_DATA af;

    if(IS_NPC(ch))
    {
        return;
    }

    if(!IS_CLASS(ch, CLASS_ELF) || !IS_SET(ch->special, SPC_ELF_MAGE))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if(ch->pcdata->classtwo[ELF_REDSTONE] < 1)
    {
        send_to_char("You must first learn your red elfstone powers to level 1.\n\r", ch);
        return;
    }

    for(obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
        if(obj->item_type == ITEM_REDSTONE)
        {
            break;
        }
    }

    if(obj == NULL)
    {
        send_to_char("You are not carrying a #RRed#n Elfstone.\n\r", ch);
        return;
    }

    if(ch->mana < 1000)
    {
        send_to_char("You don't have enough mana.\n\r", ch);
        return;
    }

    sn = skill_lookup("red sorcery");
    dam = ch->spl[RED_MAGIC]*2;

    if(is_affected(ch, sn))
    {
        return;
    }

    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HIT;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_AC;
    af.modifier  = -dam/2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MOVE;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MANA;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HITROLL;
    af.modifier  = dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_DAMROLL;
    af.modifier  = dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);

    send_to_char("You raise up a #Rred#n elfstone and it begins to glow.\n\r",ch);
    send_to_char("You are blessed by the #Rred#n elfstone!\n\r", ch);
    act("$n raises up a #Rred#n elfstone and it begins to glow",ch,NULL,NULL,TO_ROOM);
    act("$n is blessed by the #Rred#n elfstone!",ch,NULL,NULL,TO_ROOM);

    WAIT_STATE(ch, 2);
    ch->mana -= 1000;
    return;
}

void do_flamering( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classtwo[ELF_REDSTONE] < 2)
    {
        send_to_char("You must first learn your red elfstone powers to level 2.\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, "redstone" ) ) == NULL )
    {
        send_to_char("You are not carrying a #RRed #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_REDSTONE )
    {
        send_to_char( "That's not a #RRed #nElfstone.\n\r", ch );
        return;
    }
    if (!IS_SET(ch->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING))
    {
        SET_BIT(ch->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING);
        send_to_char("A massive ring of fire explodes around you.\n\r",ch);
    }
    else if (IS_SET(ch->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING))
    {
        REMOVE_BIT(ch->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING);
        send_to_char("A massive ring of fire slowly fades from around you.\n\r",ch);
    }
    return;
}

void do_firecurse( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    int sn;
    int dam;
    AFFECT_DATA af;

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if (ch->pcdata->classtwo[ELF_REDSTONE] < 3)
    {
        send_to_char("You must first learn your red elfstone powers to level 3.\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, "redstone" ) ) == NULL )
    {
        send_to_char("You are not carrying a #RRed #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_REDSTONE )
    {
        send_to_char( "That's not a #RRed #nElfstone.\n\r", ch );
        return;
    }

    if( IS_SET(ch->affected_by2, AFF_FIRECURSE))
    {
        send_to_char("You already have a curse from the red elfstone!\n\r",ch);
        return;
    }

    if (ch->mana < 1000)
    {
        send_to_char("You don't have enough mana.\n\r", ch );
        return;
    }

    sn = skill_lookup("red sorcery");
    dam=ch->spl[RED_MAGIC]*2;
    if (is_affected(ch, sn))
        return;

    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HIT;
    af.modifier  = -dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_AC;
    af.modifier  = +dam/2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MOVE;
    af.modifier  = -dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MANA;
    af.modifier  = -dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HITROLL;
    af.modifier  = -dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_DAMROLL;
    af.modifier  = -dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);

    send_to_char("You raise up a #RRed#n elfstone and it begins to glow.\n\r",ch);
    send_to_char("You are cursed by the #RRed#n elfstone!\n\r", ch);
    act("$n raises up a #RRed#n elfstone and it begins to glow",ch,NULL,NULL,TO_ROOM);
    act("$n is cursed by the #RRed#n elfstone!",ch,NULL,NULL,TO_ROOM);
    if (!IS_SET(ch->affected_by2, AFF_FIRECURSE))
        SET_BIT( ch->affected_by2, AFF_FIRECURSE);
    WAIT_STATE( ch, 5 );
    ch->mana = ch->mana - 1000;
    return;
}

/*
 * Blue Elfstone Powers
 */

void do_waterbless( CHAR_DATA * ch, char * argument)
{
    OBJ_DATA * obj;
    int sn;
    int dam;
    AFFECT_DATA af;

    if(IS_NPC(ch))
    {
        return;
    }

    if(!IS_CLASS(ch, CLASS_ELF) || !IS_SET(ch->special, SPC_ELF_MAGE))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if(ch->pcdata->classtwo[ELF_BLUESTONE] < 1)
    {
        send_to_char("You must first learn your blue elfstone powers to level 1.\n\r", ch);
        return;
    }

    for(obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
        if(obj->item_type == ITEM_BLUESTONE)
        {
            break;
        }
    }

    if(obj == NULL)
    {
        send_to_char("You are not carrying a #LBlue#n Elfstone.\n\r", ch);
        return;
    }

    if(ch->mana < 1000)
    {
        send_to_char("You don't have enough mana.\n\r", ch);
        return;
    }

    sn = skill_lookup("blue sorcery");
    dam = ch->spl[BLUE_MAGIC]*2;

    if(is_affected(ch, sn))
    {
        return;
    }

    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HIT;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_AC;
    af.modifier  = -dam/2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MOVE;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_MANA;
    af.modifier  = dam*2;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_HITROLL;
    af.modifier  = dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = dam/4;
    af.location  = APPLY_DAMROLL;
    af.modifier  = dam/3;
    af.bitvector = 0;
    affect_to_char(ch, &af);

    send_to_char("You raise up a #Lblue#n elfstone and it begins to glow.\n\r",ch);
    send_to_char("You are blessed by the #Lblue#n elfstone!\n\r", ch);
    act("$n raises up a #Lblue#n elfstone and it begins to glow",ch,NULL,NULL,TO_ROOM);
    act("$n is blessed by the #Lblue#n elfstone!",ch,NULL,NULL,TO_ROOM);

    WAIT_STATE(ch, 2);
    ch->mana -= 1000;
    return;
}

void do_waterwall (CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj = NULL;
    OBJ_DATA *objc;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if (ch->pcdata->classtwo[ELF_BLUESTONE] < 2)
    {
        send_to_char("You must first learn your blue elfstone powers to level 2.\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, "bluestone" ) ) == NULL )
    {
        send_to_char("You are not carrying a #LBlue #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_BLUESTONE )
    {
        send_to_char( "That's not a #LBlue #nElfstone.\n\r", ch );
        return;
    }

    if (ch->in_room->sector_type == SECT_AIR)
    {
        send_to_char("This ability cannot be used in the air.\n\r", ch);
        return;
    }

    if (!TIME_UP(ch, TIMER_ENTOMB)
        && ch->level < 12)
    {
        send_to_char("This ability can only be used once every 12 hours.\n\r", ch);
        return;
    }

    if ((objc = get_obj_list(ch,"walln",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
        obj=create_object(get_obj_index(30043),0);
        sprintf(buf,"A wall of water is here, blocking your exit north.");
        free_string(obj->description);
        obj->description = str_dup(buf);
        obj_to_room(obj,ch->in_room);
        obj->timer=5;
        obj->item_type = ITEM_WALL;
    }
    if ((objc = get_obj_list(ch,"walls",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
        obj=create_object(get_obj_index(30044),0);
        sprintf(buf,"A wall of water is here, blocking your exit south.");
        free_string(obj->description);
        obj->description = str_dup(buf);
        obj_to_room(obj,ch->in_room);
        obj->timer=5;
        obj->item_type = ITEM_WALL;
    }
    if ((objc = get_obj_list(ch,"walle",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
        obj=create_object(get_obj_index(30045),0);
        sprintf(buf,"A wall of water is here, blocking your exit east.");
        free_string(obj->description);
        obj->description = str_dup(buf);
        obj_to_room(obj,ch->in_room);
        obj->timer=5;
        obj->item_type = ITEM_WALL;
    }
    if ((objc = get_obj_list(ch,"wallw",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
        obj=create_object(get_obj_index(30046),0);
        sprintf(buf,"A wall of water is here, blocking your exit west.");
        free_string(obj->description);
        obj->description = str_dup(buf);
        obj_to_room(obj,ch->in_room);
        obj->timer=5;
        obj->item_type = ITEM_WALL;
    }
    if ((objc = get_obj_list(ch,"wallu",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
        obj=create_object(get_obj_index(30046),0);
        sprintf(buf,"A wall of water is here, blocking your exit up.");
        free_string(obj->description);
        obj->description = str_dup(buf);
        obj_to_room(obj,ch->in_room);
        obj->timer=5;
        obj->item_type = ITEM_WALL;
    }
    if ((objc = get_obj_list(ch,"walld",ch->in_room->contents)) != NULL) objc=NULL;
    else
    {
        obj=create_object(get_obj_index(30046),0);
        sprintf(buf,"A wall of water is here, blocking your exit down.");
        free_string(obj->description);
        obj->description = str_dup(buf);
        obj_to_room(obj,ch->in_room);
        obj->timer=5;
        obj->item_type = ITEM_WALL;
    }

    ch->hit += ch->max_hit * 0.1;
    if (ch->hit > ch->max_hit)
        ch->hit = ch->max_hit;
    SET_TIMER(ch, TIMER_ENTOMB, 12);

    act("$n calls forth a barrier of water, blocking your path in every direction.", ch, NULL, NULL, TO_ROOM);
    send_to_char("You call forth barriers of water.\n\r", ch);
    send_to_char("You grin wickedly.\n\r",ch);
    do_say(ch,"I call upon the power of the blue elfstone. You shall not leave this place.");
    WAIT_STATE(ch, 10);
    return;
}

void do_icetouch( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *mount;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        stc("Huh?\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, "bluestone" ) ) == NULL )
    {
        send_to_char("You are not carrying a #LBlue #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_BLUESTONE )
    {
        send_to_char( "That's not a #LBlue #nElfstone.\n\r", ch );
        return;
    }

    if ( ch->pcdata->classtwo[ELF_BLUESTONE] < 3 )
    {
        stc("You must learn the blue elfstone powers to level 3 first.\n\r",ch);
        return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
    {
        stc("Your icetouch will have no effect here.\n\r",ch);
        return;
    }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
        vch_next    = vch->next;

        if ( vch->in_room == NULL )
            continue;
        if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
            continue;
        if ( ch == vch )
            continue;
        if ( vch->in_room == ch->in_room )
        {
            if ( ( mount = ch->mount ) != NULL )
            {
                if ( mount == vch )
                    continue;
            }
            if (can_see(ch,vch))
            {
                if ( number_range( 1, (5 - ch->pcdata->classtwo[ELF_BLUESTONE])) == 2 )
                {
                    act("$n raises the #Lblue#n elfstone and pieces of ice flies out of it!",ch,NULL,vch,TO_ROOM);
                    stc("You begin to freeze, stunned by the coldness of the ice.\n\r",vch);
                    vch->position = POS_STUNNED;
                    act("$n is completely frozen, stunned within the ice.",vch,NULL,vch,TO_NOTVICT);
                    return;
                }
                else
                {
                    act("$n raises a #Lblue#n elfstone and pieces of ice flies out of it!",ch,NULL,vch,TO_ROOM);
                    return;
                }
            }
            else continue;

            return;
        }
    }

    return;
}

void do_stonecreate( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;
    int add = 0;

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    argument = one_argument( argument, arg );

    if (arg[0] == '\0')
    {
        send_to_char("Syntax: stonecreate <green/red/blue>.\n\r",ch);
        return;
    }
    if (!str_cmp(arg,"green"    )) {vnum = 29500;  add = ITEM_GREENSTONE;  }
    else if (!str_cmp(arg,"red"  )) {vnum = 29502;  add = ITEM_REDSTONE;    }
    else if (!str_cmp(arg,"blue"     )) {vnum = 29501;  add = ITEM_BLUESTONE;   }
    else
    {
        send_to_char("Syntax: stonecreate <green/red/blue>.\n\r",ch);
    }

    if (ch->pcdata->quest < 2000)
    {
        send_to_char("It costs 2,000 qps to create an elfstone.\n\r",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Hutoshi.\n\r",ch);
        return;
    }

    ch->pcdata->quest -= 2000;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj->item_type = add;
    obj_to_char(obj, ch);
    act("You begin to chant and suddenly $p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$n begins to chant and suddenly $p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;

}

void do_elfstones( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    CHAR_DATA *victim;
    int dam = 0;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch, "bluestone" ) ) == NULL )
    {
        send_to_char("You are missing a #LBlue #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_BLUESTONE )
    {
        send_to_char( "That's not a #LBlue #nElfstone.\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, "greenstone" ) ) == NULL )
    {
        send_to_char("You are missing a #GGreen #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_GREENSTONE )
    {
        send_to_char( "That's not a #GGreen #nElfstone.\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, "redstone" ) ) == NULL )
    {
        send_to_char("You are missing a #RRed #nElfstone.\n\r",ch);
        return;
    }

    if ( obj->item_type != ITEM_REDSTONE )
    {
        send_to_char( "That's not a #RRed #nElfstone.\n\r", ch );
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ch->spl[RED_MAGIC];
    dam += ch->spl[BLUE_MAGIC];
    dam += ch->spl[YELLOW_MAGIC];
    dam += ch->spl[GREEN_MAGIC];
    dam += ch->spl[PURPLE_MAGIC];
    dam *= 0.85;
    hurt_person(ch,victim, dam);

    //  one_hit(ch,victim,gsn_elfstones,10);
    //  one_hit(ch,victim,gsn_elfstones,1);
    //  one_hit(ch,victim,gsn_elfstones,1);
    //  one_hit(ch,victim,gsn_elfstones,1);
    sprintf(buf1,"$n's elfstones flash angrily and lash out with power at $N [#R%d#n]",dam);
    sprintf(buf2,"Your elfstones coruscate with spectral fire and lash out at $N [#R%d#n]",dam);
    sprintf(buf3,"You scream in pain as $n's elfstones punish you! [#R%d#n]",dam);
    act(buf1,ch,NULL,victim,TO_NOTVICT);
    act(buf2,ch,NULL,victim,TO_CHAR);
    act(buf3,ch,NULL,victim,TO_VICT);

    WAIT_STATE(ch,12);
    return;
}

/* bless spells for elfstones */

void spell_earthbless( int sn, int level, CHAR_DATA *ch, void *vo)
{
    AFFECT_DATA af1;
    AFFECT_DATA af2;
    AFFECT_DATA af3;
    AFFECT_DATA af4;
    AFFECT_DATA af5;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    act("You raise the green elfstone into the air and it begins to hum.\n\rYou feel better.",ch,NULL,NULL,TO_CHAR);
    act("$n raises a green elfstone into the air and it begins to hum.\n\rHe begins to glow bright green.",ch,NULL,NULL,TO_NOTVICT);

    if( IS_SET(ch->affected_by2, AFF_EARTHBLESS))
    {
        send_to_char("You are already blessed by the green elfstone.\n\r",ch);
        return;
    }

    af1.type    = sn;
    af1.location    = APPLY_AC;
    af1.modifier    = -200;
    af1.duration    = 100;
    af1.bitvector   = 0;
    affect_to_char(ch, &af1);

    af2.type    = sn;
    af2.location    = APPLY_STR;
    af2.modifier    = +10;
    af2.duration    = 100;
    af2.bitvector   = 0;
    affect_to_char(ch, &af2);

    af3.type    = sn;
    af3.location    = APPLY_HITROLL;
    af3.modifier    = 200;
    af3.duration    = 100;
    af3.bitvector   = 0;
    affect_to_char(ch, &af3);

    af4.type    = sn;
    af4.location    = APPLY_DAMROLL;
    af4.modifier    = 200;
    af4.duration    = 100;
    af4.bitvector   = 0;
    affect_to_char(ch, &af4);

    af5.type    = sn;
    af5.location    = APPLY_DEX;
    af5.modifier    = +10;
    af5.duration    = 100;
    af5.bitvector   = 0;
    affect_to_char(ch, &af5);

    send_to_char("You are blessed by the green elfstone!\n\r", ch);
    act("$n is blessed by the green elfstone!",ch,NULL,NULL,TO_ROOM);
    if (!IS_SET(ch->affected_by2, AFF_EARTHBLESS))
        SET_BIT( ch->affected_by2, AFF_EARTHBLESS);
    return;
}

void spell_firebless( int sn, int level, CHAR_DATA *ch, void *vo)
{
    AFFECT_DATA af1;
    AFFECT_DATA af2;
    AFFECT_DATA af3;
    AFFECT_DATA af4;
    AFFECT_DATA af5;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    act("You raise the red elfstone into the air and it begins to hum.\n\rYou feel better.",ch,NULL,NULL,TO_CHAR);
    act("$n raises a red elfstone into the air and it begins to hum.\n\rHe begins to glow bright red.",ch,NULL,NULL,TO_NOTVICT);

    if( IS_SET(ch->affected_by2, AFF_FIREBLESS))
    {
        send_to_char("You are already blessed by the red elfstone.\n\r",ch);
        return;
    }

    af1.type    = sn;
    af1.location    = APPLY_AC;
    af1.modifier    = -200;
    af1.duration    = 100;
    af1.bitvector   = 0;
    affect_to_char(ch, &af1);

    af2.type    = sn;
    af2.location    = APPLY_STR;
    af2.modifier    = +10;
    af2.duration    = 100;
    af2.bitvector   = 0;
    affect_to_char(ch, &af2);

    af3.type    = sn;
    af3.location    = APPLY_HITROLL;
    af3.modifier    = 200;
    af3.duration    = 100;
    af3.bitvector   = 0;
    affect_to_char(ch, &af3);

    af4.type    = sn;
    af4.location    = APPLY_DAMROLL;
    af4.modifier    = 200;
    af4.duration    = 100;
    af4.bitvector   = 0;
    affect_to_char(ch, &af4);

    af5.type    = sn;
    af5.location    = APPLY_DEX;
    af5.modifier    = +10;
    af5.duration    = 100;
    af5.bitvector   = 0;
    affect_to_char(ch, &af5);

    send_to_char("You are blessed by the red elfstone!\n\r", ch);
    act("$n is blessed by the red elfstone!",ch,NULL,NULL,TO_ROOM);
    if (!IS_SET(ch->affected_by2, AFF_FIREBLESS))
        SET_BIT( ch->affected_by2, AFF_FIREBLESS);
    return;
}

/*
void spell_waterbless( int sn, int level, CHAR_DATA *ch, void *vo)
{
    AFFECT_DATA af1;
    AFFECT_DATA af2;
    AFFECT_DATA af3;
    AFFECT_DATA af4;
    AFFECT_DATA af5;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    act("You raise the blue elfstone into the air and it begins to hum.\n\rYou feel better.",ch,NULL,NULL,TO_CHAR);
    act("$n raises a blue elfstone into the air and it begins to hum.\n\rHe begins to glow bright blue.",ch,NULL,NULL,TO_NOTVICT);

    if( IS_SET(ch->affected_by2, AFF_WATERBLESS))
    {
        send_to_char("You are already blessed by the blue elfstone.\n\r",ch);
        return;
    }

    af1.type	= sn;
    af1.location	= APPLY_AC;
    af1.modifier	= -200;
    af1.duration	= 100;
    af1.bitvector	= 0;
    affect_to_char(ch, &af1);

    af2.type	= sn;
    af2.location	= APPLY_STR;
    af2.modifier	= +10;
    af2.duration	= 100;
    af2.bitvector	= 0;
    affect_to_char(ch, &af2);

    af3.type	= sn;
    af3.location	= APPLY_HITROLL;
    af3.modifier	= 200;
    af3.duration	= 100;
    af3.bitvector	= 0;
    affect_to_char(ch, &af3);

    af4.type	= sn;
    af4.location	= APPLY_DAMROLL;
    af4.modifier	= 200;
    af4.duration	= 100;
    af4.bitvector	= 0;
    affect_to_char(ch, &af4);

    af5.type	= sn;
    af5.location	= APPLY_DEX;
    af5.modifier	= +10;
    af5.duration	= 100;
    af5.bitvector	= 0;
    affect_to_char(ch, &af5);

    send_to_char("You are blessed by the blue elfstone!\n\r", ch);
    act("$n is blessed by the blue elfstone!",ch,NULL,NULL,TO_ROOM);
    if (!IS_SET(ch->affected_by2, AFF_WATERBLESS))
    SET_BIT( ch->affected_by2, AFF_WATERBLESS);
    return;
}
*/
void spell_firecurse( int sn, int level, CHAR_DATA *ch, void *vo)
{
    AFFECT_DATA af1;
    AFFECT_DATA af2;
    AFFECT_DATA af3;
    AFFECT_DATA af4;
    AFFECT_DATA af5;
    AFFECT_DATA af6;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_MAGE)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    act("You raise the red elfstone into the air and it begins to hum.\n\rYou feel weak.",ch,NULL,NULL,TO_CHAR);
    act("$n raises a red elfstone into the air and it begins to hum.\n\rHe begins to glow bright red.",ch,NULL,NULL,TO_NOTVICT);

    if( IS_SET(ch->affected_by2, AFF_FIRECURSE))
    {
        send_to_char("You are already cursed by the red elfstone.\n\r",ch);
        return;
    }

    af1.type    = sn;
    af1.location    = APPLY_AC;
    af1.modifier    = +500;
    af1.duration    = 100;
    af1.bitvector   = 0;
    affect_to_char(ch, &af1);

    af2.type    = sn;
    af2.location    = APPLY_HIT;
    af2.modifier    = -5000;
    af2.duration    = 100;
    af2.bitvector   = 0;
    affect_to_char(ch, &af2);

    af3.type    = sn;
    af3.location    = APPLY_MANA;
    af3.modifier    = -5000;
    af3.duration    = 100;
    af3.bitvector   = 0;
    affect_to_char(ch, &af3);

    af4.type    = sn;
    af4.location    = APPLY_MOVE;
    af4.modifier    = -5000;
    af4.duration    = 100;
    af4.bitvector   = 0;
    affect_to_char(ch, &af4);

    af5.type    = sn;
    af5.location    = APPLY_HITROLL;
    af5.modifier    = -400;
    af5.duration    = 100;
    af5.bitvector   = 0;
    affect_to_char(ch, &af5);

    af6.type    = sn;
    af6.location    = APPLY_DAMROLL;
    af6.modifier    = -400;
    af6.duration    = 100;
    af6.bitvector   = 0;
    affect_to_char(ch, &af6);

    send_to_char("You are cursed by the red elfstone!\n\r", ch);
    act("$n is cursed by the red elfstone!",ch,NULL,NULL,TO_ROOM);
    if (!IS_SET(ch->affected_by2, AFF_FIRECURSE))
        SET_BIT( ch->affected_by2, AFF_FIRECURSE);
    return;
}

//  Elf Warrior commands

void do_swordslash(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_WARRIOR)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classtwo[ELF_WAR_TECH] < 4)
    {
        send_to_char("You must learn your technique to level 4.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    one_hit(ch,victim,gsn_swordslash,1);
    one_hit(ch,victim,gsn_swordslash,1);

    if (ch->pcdata->classtwo[ELF_WAR_TECH] < 2)
    {
        if (!IS_AFFECTED(victim, AFF_POISON))
            SET_BIT(victim->affected_by, AFF_POISON);
    }
    WAIT_STATE(ch,14);
    return;
}

void do_wishsong(CHAR_DATA *ch, char *argument)
{
    int spell;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_WARRIOR)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    spell = ch->spl[RED_MAGIC];

    if (ch->pcdata->classtwo[ELF_WAR_TECH] < 5)
    {
        send_to_char("You must learn your technique to level 5.\n\r",ch);
        return;
    }

    if ((victim = get_char_room(ch, arg)) == NULL)
    {
        send_to_char("They are not here.\n\r", ch);
        return;
    }

    if (is_safe(ch,victim)) return;

    if (victim == ch)
    {
        send_to_char("That doesn't seem like a good idea.\n\r",ch);
        return;
    }

    send_to_char("You begin to chant the ancient wishsong.\n\r",ch);

    if (spell > 100)
    {
        one_hit(ch,victim,gsn_wishsong,1);
    }
    if (spell > 150)
    {
        one_hit(ch,victim,gsn_wishsong,1);
        one_hit(ch,victim,gsn_wishsong,1);
    }
    if (spell > 200)
    {
        one_hit(ch,victim,gsn_wishsong,1);
        one_hit(ch,victim,gsn_wishsong,1);
        one_hit(ch,victim,gsn_wishsong,1);
    }
    if (spell > 300)
    {
        one_hit(ch,victim,gsn_wishsong,1);
        one_hit(ch,victim,gsn_wishsong,1);
        one_hit(ch,victim,gsn_wishsong,1);
        one_hit(ch,victim,gsn_wishsong,1);
    }
    WAIT_STATE(ch,14);
    return;
}

// Elf Archers powers

void do_archersfury(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_ARCHER)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classtwo[ELF_ARC_ARCPOWER] < 3)
    {
        send_to_char("You must learn your technique to level 3.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }
    if (ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 0)
        one_hit(ch,victim,gsn_firearrow,1);
    if (ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 0)
        one_hit(ch,victim,gsn_icearrow,1);
    if (ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 0)
        one_hit(ch,victim,gsn_lightningarrow,1);

    WAIT_STATE(ch,14);
    return;
}

void do_archersrage(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ELF) || (!IS_SET(ch->special,SPC_ELF_ARCHER)))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classtwo[ELF_ARC_ARCPOWER] < 4)
    {
        send_to_char("You must learn your archer powers to level 4.\n\r",ch);
        return;
    }

    if (IS_SET(ch->pcdata->classtwo[ELF_ARC_ARCPOWER], ARCHERS_RAGE))
    {
        ch->hitroll -= 250;
        ch->damroll -= 250;
        send_to_char("You relax from your raging state.\n\r",ch);
        REMOVE_BIT(ch->pcdata->classtwo[ELF_ARC_ARCPOWER], ARCHERS_RAGE);
    }
    else
    {
        ch->damroll += 250;
        ch->hitroll += 250;
        send_to_char("You become full of rage.\n\r",ch);
        SET_BIT(ch->pcdata->classtwo[ELF_ARC_ARCPOWER], ARCHERS_RAGE);
    }
    return;
}
