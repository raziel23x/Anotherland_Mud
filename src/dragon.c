/****************************************
 * Original Class designed by Hutoshi	*
 * and fixxed and upgraded by Loial,    *
 * only for use by Anotherland.  Any	*
 * duplication or use of this class	*
 * without consent from Hutoshi aka	*
 * Anthony Davis is strictly prohibited.*
 * Copyrighted (C) 2003, Anthony Davis	*
 ****************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_dragonarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;
    if(!IS_IMMORTAL(ch))
    {
        if( !IS_CLASS(ch, CLASS_DRAGON) )
        {
            send_to_char("What?\n\r",ch);
            return;
        }
    }
    if (arg[0] == '\0')
    {
        send_to_char("Please specify which piece of dragon armor you wish to make: Claws Ring Collar Platemail Helmet Leggings Boots Gauntlets Sleeves Cape Belt Bracer Visor\n\r",ch);
        return;
    }
    if ( ch->practice < 150 )
    {
        send_to_char("It costs 150 points of primal to create dragon equipment.\n\r",ch);
        return;
    }
    if (ch->pcdata->upgrade_level >= 1)
    {
        if      (!str_cmp(arg,"claws"               )) vnum = 110050;
        else if (!str_cmp(arg,"ring"                )) vnum = 110051;
        else if (!str_cmp(arg,"collar"              )) vnum = 110052;
        else if (!str_cmp(arg,"bracer"              )) vnum = 110053;
        else if (!str_cmp(arg,"plate"               )) vnum = 110054;
        else if (!str_cmp(arg,"helmet"              )) vnum = 110055;
        else if (!str_cmp(arg,"leggings"            )) vnum = 110056;
        else if (!str_cmp(arg,"boots"               )) vnum = 110057;
        else if (!str_cmp(arg,"gauntlets"           )) vnum = 110058;
        else if (!str_cmp(arg,"sleeves"             )) vnum = 110059;
        else if (!str_cmp(arg,"cape"                )) vnum = 110060;
        else if (!str_cmp(arg,"belt"                )) vnum = 110061;
        else if (!str_cmp(arg,"visor"               )) vnum = 110062;
        else
        {
            send_to_char("That is an invalid type.\n\r", ch );
            return;
        }
    }

    else
    {
        if (!str_cmp(arg,"visor")) vnum = 40962;
        else if (!str_cmp(arg,"belt")) vnum = 40960;
        else if (!str_cmp(arg,"cape")) vnum = 40959;
        else if (!str_cmp(arg,"ring")) vnum = 40951;
        else if (!str_cmp(arg,"collar")) vnum = 40952;
        else if (!str_cmp(arg,"platemail")) vnum = 40953;
        else if (!str_cmp(arg,"helmet")) vnum = 40954;
        else if (!str_cmp(arg,"leggings")) vnum = 40955;
        else if (!str_cmp(arg,"boots")) vnum = 40956;
        else if (!str_cmp(arg,"gauntlets")) vnum = 40957;
        else if (!str_cmp(arg,"sleeves")) vnum = 40958;
        else if (!str_cmp(arg,"bracer")) vnum = 40961;
        else if (!str_cmp(arg,"claws")) vnum = 40963;
        else
        {
            do_dragonarmor(ch,"");
            return;
        }
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform a God.\n\r",ch);
        return;
    }
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    ch->practice -= 150;
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_dgain(CHAR_DATA *ch, char *argument)
{
    char arg[MIL];
    argument = one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Which path you want study ^^spirit^^ / ^^wisdom^^ / ^^power^^ / ^^age^^\n\r",ch);
        return;
    }

    if (!str_cmp(arg, "spirit"))
    {
        if (ch->pcdata->classfour[DRAGON_SPIRIT] > 9)
        {
            send_to_char("You already have all 10 levels.\n\r",ch);
            return;
        }

        if (ch->exp < (ch->pcdata->classfour[DRAGON_SPIRIT]+1)*1000000)
        {
            send_to_char("You don't have enough exp.\n\r",ch);
            return;
        }

        ch->pcdata->classfour[DRAGON_SPIRIT]++;
        ch->exp -= ch->pcdata->classfour[DRAGON_SPIRIT]*1000000;
        send_to_char("Ok.\n\r",ch);
    }

    else if (!str_cmp(arg, "age"))
    {
        if (ch->pcdata->classfour[DRAGON_AGE] > 15)
        {
            send_to_char("You already have all 15 levels.\n\r",ch);
            return;
        }

        if (ch->exp < (ch->pcdata->classfour[DRAGON_AGE]+1)*1000000)
        {
            send_to_char("You don't have enough exp.\n\r",ch);
            return;
        }

        ch->pcdata->classfour[DRAGON_AGE]++;
        ch->exp -= ch->pcdata->classfour[DRAGON_AGE]*1000000;
        send_to_char("Ok.\n\r",ch);
    }

    else if (!str_cmp(arg, "wisdom"))
    {
        if (ch->pcdata->classfour[DRAGON_WISDOM] > 7)
        {
            send_to_char("You already have all 8 levels.\n\r",ch);
            return;
        }

        if (ch->exp < (ch->pcdata->classfour[DRAGON_WISDOM]+1)*1000000)
        {
            send_to_char("You don't have enough exp.\n\r",ch);
            return;
        }

        ch->pcdata->classfour[DRAGON_WISDOM]++;
        ch->exp -= ch->pcdata->classfour[DRAGON_WISDOM]*1000000;
        send_to_char("Ok.\n\r",ch);
    }

    else if (!str_cmp(arg, "power"))
    {
        if (ch->pcdata->classfour[DRAGON_POWER] > 14)
        {
            send_to_char("You already have all 15 levels.\n\r",ch);
            return;
        }

        if (ch->exp < (ch->pcdata->classfour[DRAGON_POWER]+1)*1000000)
        {
            send_to_char("You don't have enough exp.\n\r",ch);
            return;
        }

        ch->pcdata->classfour[DRAGON_POWER]++;
        ch->exp -= ch->pcdata->classfour[DRAGON_POWER]*1000000;
        send_to_char("Ok.\n\r",ch);
    }

    else
    {
        send_to_char("Sorry, you cannot study this.\n\r",ch);
        return;
    }

    return;
}

void do_dfly( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    argument = one_argument(argument, arg);

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (ch->pcdata->classfour[DRAGON_POWER] < 3)
    {
        send_to_char("You must obtain level 3 in power to use fly.\n\r",ch);
        return;
    }
    if (ch->move < 250)
    {
        stc("You do not have enough vitality to do that!\n\r",ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char( "Fly to whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "You can't find it's room.\n\r",ch);
        return;
    }
    if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "You cannot sense any paths leading from this room.\n\r",ch);
        return;
    }

    if( victim == ch )
    {
        stc ( "Not to yourself.\n\r",ch);
        return;
    }
    if  (!IS_NPC(victim) && !IS_IMMUNE(victim,IMM_SUMMON) )
    {
        send_to_char("There summon is off!\n\r",ch);
        return;
    }
    if (ch == victim)
    {
        send_to_char("But you're already at yourself!\n\r",ch);
        return;
    }
    if (victim->in_room->vnum == ch->in_room->vnum)
    {
        send_to_char("But you're already there!\n\r",ch);
        return;
    }

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    if (IS_NPC(victim))
    {
        sprintf(buf,"You concentrate on flying to %s!\n\r",victim->short_descr);
        send_to_char(buf, ch);
    }
    if (!IS_NPC(victim))
    {
        sprintf(buf,"You concentrate on flying to %s!\n\r",victim->name);
        send_to_char(buf, ch);
    }
    act("$n appears from the sky\n\r",ch,NULL,NULL,TO_ROOM);
    do_look(ch, "auto");
    ch->move -= 250;
    return;
}

void do_dclaws(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON) || ch->pcdata->classfour[DRAGON_POWER] < 2)
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if( IS_SET(ch->pcdata->classfour[DRAGON_POWER], DRAG_CLAWS))
    {
        send_to_char("Claws grow back into your fingertips.\n\r", ch );
        REMOVE_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_CLAWS);
        REMOVE_BIT(ch->newbits, NEW_SCLAWS);
        REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
        REMOVE_BIT(ch->newbits, NEW_REND);
        REMOVE_BIT( ch->garou1, WOLF_RAZORCLAWS );
        return;
    }

    send_to_char("Claws grow out of your fingertips.\n\r", ch );
    SET_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_CLAWS);
    SET_BIT(ch->newbits, NEW_SCLAWS);
    SET_BIT(ch->newbits, NEW_MONKFLAME);
    SET_BIT(ch->newbits, NEW_REND);
    SET_BIT( ch->garou1, WOLF_RAZORCLAWS );

    return;
}

void do_dwings(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON) || ch->pcdata->classfour[DRAGON_POWER] < 1)
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if( IS_SET(ch->pcdata->classfour[DRAGON_POWER], DRAG_WINGS))
    {
        send_to_char("Your massive wings fold back into your back.\n\r", ch );
        REMOVE_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_WINGS);
        return;
    }

    send_to_char("Your massive wings unfold from out of your back.\n\r", ch );
    SET_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_WINGS);

    return;
}

void do_dtail(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON) || ch->pcdata->classfour[DRAGON_POWER] < 5)
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if( IS_SET(ch->pcdata->classfour[DRAGON_POWER], DRAG_TAIL))
    {
        send_to_char("Your massive tail turns back into a small tail behind you.\n\r", ch );
        REMOVE_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_TAIL);
        return;
    }

    send_to_char("Your small tail extends into a massive tail that cracks like a whip.\n\r", ch );
    SET_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_TAIL);

    return;
}

void do_dsight( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_POWER] < 2)
    {
        send_to_char("You need at least 2 in Dragon Power first.\n\r",ch);
        return;
    }

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        REMOVE_BIT(ch->newbits, NEW_PERCEPTION);
        send_to_char( "You're eyes turn back to normal.\n\r", ch );
    }

    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        SET_BIT(ch->newbits, NEW_PERCEPTION);
        send_to_char( "You are gifted  with the sight of the ancients.\n\r", ch );
        ch->move -= 250;
    }
    return;
}

void do_dfangs(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_POWER] < 2)
    {
        send_to_char("You need at least 2 in Dragon Power first.\n\r",ch);
        return;
    }

    if (IS_VAMPAFF(ch, VAM_FANGS))
    {
        send_to_char("Your fangs slide back into your gums.\n\r",ch);
        act("$n retracts $s fangs back into $s gums.",ch, NULL, NULL, TO_ROOM);
        REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    else
    {
        send_to_char("Sharp fangs slide out of your gums.\n\r",ch);
        act("Sharp fangs slide out of $n's gums.",ch, NULL, NULL, TO_ROOM);
        SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
}

void do_dform( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_POWER] < 8)
    {
        send_to_char("You need to have your dragon power level at atleast 8 to enter dragonform.\n\r",ch);
        return;
    }

    if ( !IS_SET(ch->polyaff, POLY_ZULOFORM) )
    {
        if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        send_to_char("#7You feel a great pain, and you clutch your head and scream!\n\r",ch);
        act("#7$n clutches their head and screams in pain!", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7You fall to the ground!\n\r",ch);
        act("#7$n falls to the ground, and start transforming into something...", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Your hands turn #Ggreen#7 and grows, and razor sharp claws retract!\n\r",ch);
        act("#7$n's hands turn #Ggreen#7 and grows, and razor sharp claws retract!", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Your head turns #Ggreen#7 and grows, and fangs sharp as razor retracts!\n\r",ch);
        act("#7$n's head turns #Ggreen#7 and starts to grow, and razor fangs retracts from $s mouth!", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7A big, #Ggreen#7, mighty tail grows from your back!\n\r",ch);
        act("#7A big, #Ggreen#7, mighty tail extends from $n's back!", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Large, #Ggreen#7, scaly wings burst from your back!\n\r",ch);
        act("#7A pair of large, #Ggreen#7, scaly wings burst from $n's back!", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Your feet turn #Ggreen#7, and start to grow!\n\r",ch);
        act("#7$n's feet turn #Ggreen#7, and start to grow!", ch, NULL, NULL, TO_ROOM);
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
        SET_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_TAIL);
        SET_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_WINGS);
        SET_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_CLAWS);
        ch->damroll += 3000;
        ch->hitroll += 3000;
        ch->armor   -= 3500;

        /* if (ch->pcdata->classfour[DRAGON_AGE] > 1)
        {
          if (ch->pcdata->classfour[DRAGON_AGE] == 2)
          sprintf(buf,"A young green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 3)
          sprintf(buf,"A young green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 4)
          sprintf(buf,"A juvenile green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 5)
          sprintf(buf,"A juvenile green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 6)
          sprintf(buf,"A large green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 7)
          sprintf(buf,"A large green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 8)
          sprintf(buf,"A huge green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 9)
          sprintf(buf,"A huge green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 10)
          sprintf(buf,"A gigantic green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 11)
          sprintf(buf,"A gigantic green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 12)
          sprintf(buf,"A colossal green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 13)
          sprintf(buf,"A colossal green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 14)
          sprintf(buf,"A colossal green dragon");
          else if (ch->pcdata->classfour[DRAGON_AGE] == 15)
          sprintf(buf,"A colossal green dragon");
        }
        else
         sprintf(buf,"A young green dragon %s", ch->name);
         free_string( ch->morph );
         ch->morph = str_dup(buf);
         return;*/
        sprintf(buf, "#7The #yAncient #Ggreen#7 Dragon #y%s#n", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        return;
    }
    else if ( IS_SET(ch->polyaff, POLY_ZULOFORM) )
    {
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
        REMOVE_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_TAIL);
        REMOVE_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_WINGS);
        REMOVE_BIT(ch->pcdata->classfour[DRAGON_POWER], DRAG_CLAWS);
        ch->damroll -= 3000;
        ch->hitroll -= 3000;
        ch->armor   += 3500;
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        send_to_char("#7You feel a great pain, clutch your head and scream!\n\r",ch);
        act("#7$n clutches their head and screams in pain!", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7You fall to the ground!\n\r",ch);
        act("#7$n falls to the ground, and start transforming into something...", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Your claws return back into normal hands and feet.\n\r",ch);
        act("#7$n's claws return back into normal hands and feet.",ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Your teeth reform their original form as your head shrinks.\n\r",ch);
        act("#7$n's teeth reform to their original form as $s's head shrinks.", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7The big, #Ggreen#7, mighty tail grows into your back!\n\r",ch);
        act("#7The big, #Ggreen#7, mighty tail grows into $n's back!", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Your large scaly wings slide into your back.\n\r",ch);
        act("#7$n's large scaly wings slide into $s back.", ch, NULL, NULL, TO_ROOM);
        send_to_char("#7Your feet shrink, and your claws retract.\n\r",ch);
        act("#7$n's large feet shrink, and $s claws retract.", ch, NULL, NULL, TO_ROOM);
        return;
    }
    else
        send_to_char("#7 Dragonform, the ability to harness the true power of the ancient dragons.\n\r", ch);
    return;
}

void do_dragondive(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int     dam;

    argument = one_argument(argument, arg);

    if (IS_NPC(ch))

        if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_POWER] < 6)
    {
        send_to_char("You must learn your powers to level 6 first.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Dive upon who?\n\r",ch);
        return;
    }

    if ((victim = get_char_room( ch, arg )) == NULL)
    {
        send_to_char("They are not here.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_CLASS(victim, CLASS_DRAGON))
    {
        send_to_char("You cant dive on another Dragon!",ch);
        act("#G$n#n just tried to dive upon you.",ch,NULL,victim, TO_VICT);
        return;
    }

    act("You soar from the clouds, diving on $N.\n\r",ch,NULL,victim, TO_CHAR);
    act("$n suddenly soars from the clouds, diving towards you.\n\r",ch,NULL,victim, TO_VICT);
    act("$n suddenly soars down from the clouds onto $N.\n\r",ch,NULL,victim, TO_NOTVICT);

    dam = ((victim->hit) * .1);

    if (ch->pcdata->classfour[DRAGON_POWER] == 7)  dam *= 1.2;
    if (ch->pcdata->classfour[DRAGON_POWER] == 8)  dam *= 1.4;
    if (ch->pcdata->classfour[DRAGON_POWER] == 9)  dam *= 1.6;
    if (ch->pcdata->classfour[DRAGON_POWER] == 10) dam *= 1.8;
    if (ch->pcdata->classfour[DRAGON_POWER] == 11) dam *= 2.0;
    if (ch->pcdata->classfour[DRAGON_POWER] == 12) dam *= 2.2;
    if (ch->pcdata->classfour[DRAGON_POWER] == 13) dam *= 2.4;
    if (ch->pcdata->classfour[DRAGON_POWER] == 14) dam *= 2.8;
    if (ch->pcdata->classfour[DRAGON_POWER] == 15) dam *= 3.0;

    if (ch->generation == 5) dam *= 1.05;
    if (ch->generation == 4) dam *= 1.10;
    if (ch->generation == 3) dam *= 1.15;
    if (ch->generation == 2) dam *= 1.20;
    if (ch->generation == 1) dam *= 1.25;
    if (ch->generation == 0) dam *= 1.30;

    sprintf( buf, "You soar down from the clouds, diving onto $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's soars down from the clouds, diving onto you#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's soars down from the clouds onto $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    ch->fighting = victim;
    victim->fighting = ch;
    autodrop(ch);
    autodrop(victim);
    ch->position = POS_FIGHTING;
    victim->position = POS_FIGHTING;
    update_pos(ch);
    update_pos(victim);
    WAIT_STATE(ch,2);
    return;
}

void do_icebreath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 2)
    {
        send_to_char("You must learn your dragon wisdom to level 2.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    if (IS_CLASS(victim, CLASS_DEMON))     dam *= 1.5;
    if (IS_CLASS(victim, CLASS_TANARRI))   dam *= 1.5;
    if (IS_CLASS(victim, CLASS_SAMURAI))   dam *= 1.5;
    if (IS_CLASS(victim, CLASS_LICH))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_PHOENIX))    dam *= 3.0;     /*Phoenix dont like ice */

    sprintf( buf, "You call upon your icebreath to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's icebreath freezes you over#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's icebreath strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,12);
    return;
}

void do_firebreath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 1)
    {
        send_to_char("You must learn your dragon wisdom to level 1.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (IS_CLASS(victim, CLASS_ELF)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_WEREWOLF)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_VAMPIRE))  dam *= 2.5;       /* Vampires dont like fire */
    if (IS_CLASS(victim, CLASS_DROID))    dam *= 1.5;
    if (IS_CLASS(victim, CLASS_LICH)) dam *= 1.5;

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    sprintf( buf, "You call upon your firebreath to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's firebreath burns you to the core#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's firebreath strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,12);
    return;
}

void do_hatredbreath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 3)
    {
        send_to_char("You must learn your dragon wisdom to level 3.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (IS_CLASS(victim, CLASS_ANGEL))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_NINJA))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_MAGE))   dam *= 1.5;

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    sprintf( buf, "You call upon your hatredbreath to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's hatredbreath strikes you hard#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's hatredbreath strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,12);
    return;
}

void do_acidbreath(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 4)
    {
        send_to_char("You must learn your dragon wisdom to level 4.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (IS_CLASS(victim, CLASS_MONK)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_SAMURAI))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_SHAPESHIFTER))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_KAHN)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_TANARRI))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_DROW)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_WEREWOLF)) dam *= 1.5;

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    sprintf( buf, "You call upon your acidbreath to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's acidbreath strikes you and you begin to melt#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's acidbreath strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,12);
    return;
}

void do_fireblast(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 5)
    {
        send_to_char("You must learn your dragon wisdom to level 5.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (IS_CLASS(victim, CLASS_LICH))      dam *= 1.5;
    if (IS_CLASS(victim, CLASS_ELF))      dam *= 1.5;
    if (IS_CLASS(victim, CLASS_DROID))      dam *= .5;
    if (IS_CLASS(victim, CLASS_DROW))      dam *= 1.5;
    if (IS_CLASS(victim, CLASS_VAMPIRE))    dam *= 3.0;     /* vampires dont like fire */

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    sprintf( buf, "You call upon your fireblast to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's fireblast burns you to the core#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's fireblast strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,14);
    return;
}

void do_iceblast(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 6)
    {
        send_to_char("You must learn your dragon wisdom to level 6.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (IS_CLASS(victim, CLASS_DEMON))     dam *= 1.5;
    if (IS_CLASS(victim, CLASS_TANARRI))   dam *= 1.5;
    if (IS_CLASS(victim, CLASS_SAMURAI))   dam *= 1.5;
    if (IS_CLASS(victim, CLASS_LICH))  dam *= .5;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_PHOENIX))      dam *= 3.0;   /*Phoenix dont like fire */

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    sprintf( buf, "You call upon your iceblast to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's iceblast freezes you over#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's iceblast strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,14);
    return;
}

void do_hatredblast(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 7)
    {
        send_to_char("You must learn your dragon wisdom to level 7.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (IS_CLASS(victim, CLASS_ANGEL))  dam *= 2.0;
    if (IS_CLASS(victim, CLASS_NINJA))  dam *= 2.0;
    if (IS_CLASS(victim, CLASS_MAGE))   dam *= 2.0;

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    sprintf( buf, "You call upon your hatredblast to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's hatredblast strikes you hard#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's hatredblast strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,14);
    return;
}

void do_acidblast(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 8)
    {
        send_to_char("You must learn your dragon wisdom to level 8.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dam = ((ch->hit) * .05);

    if (IS_CLASS(victim, CLASS_MONK)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_SAMURAI))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_SHAPESHIFTER))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_KAHN)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_TANARRI))  dam *= 1.5;
    if (IS_CLASS(victim, CLASS_DROW)) dam *= 1.5;
    if (IS_CLASS(victim, CLASS_WEREWOLF)) dam *= 1.5;

    if (ch->generation == 5) dam *=1.05;
    if (ch->generation == 4) dam *=1.07;
    if (ch->generation == 3) dam *=1.09;
    if (ch->generation == 2) dam *=1.10;
    if (ch->generation == 1) dam *=1.12;
    if (ch->generation == 0) dam *=1.20;

    sprintf( buf, "You call upon your acidblast to strike $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's acidblast strikes you and you begin to melt#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's acidblast strikes out at $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    WAIT_STATE(ch,12);
    return;
}

void do_dgrasp(CHAR_DATA *ch,char *argument)
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_POWER] < 9)
    {
        send_to_char("You must learn your dragon power to level 9.\n\r",ch);
        return;
    }

    if (!IS_SET(ch->newbits, NEW_JAWLOCK))
    {
        send_to_char("You roar and latch your claws onto your prey preventing them from escaping.\n\r",ch);
        SET_BIT(ch->newbits, NEW_JAWLOCK);
        return;
    }

    else if (IS_SET(ch->newbits, NEW_JAWLOCK))
    {
        send_to_char("You release your grip with a soft growl.\n\r",ch);
        REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
        return;
    }

    return;
}

void do_dstealth(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 6)
    {
        send_to_char("You must learn your dragon wisdom to level 6.\n\r",ch);
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
        send_to_char("You slowly apear from the heavy fog.\n\r",ch);
        act("$n slowly apears from the heavy fog.\n\r", ch, NULL, NULL, TO_ROOM );
        return;
    }

    send_to_char("You slowly disapear into a heavy fog.\n\r",ch);
    act("$n slowly disapears into a heavy fog.\n\r", ch, NULL, NULL, TO_ROOM );
    SET_BIT(ch->act,PLR_SUPERINVIS);
    return;
}

void do_dshield(CHAR_DATA *ch, char *argument)
{

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_SPIRIT] < 4)
    {
        send_to_char("You must learn your dragon spirit to level 4.\n\r",ch);
        return;
    }

    if (!IS_IMMUNE(ch,IMM_SHIELDED) )
    {
        send_to_char("You shield your mind and aura from those around you.\n\r",ch);
        SET_BIT(ch->immune, IMM_SHIELDED);
        return;
    }
    send_to_char("You stop shielding your mind and aura.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);

    return;
}

void do_dscales(CHAR_DATA *ch,char *argument)
{

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_POWER] < 3)
    {
        send_to_char("You must learn your dragon power to level 3.\n\r",ch);
        return;
    }

    if (!IS_SET(ch->newbits, NEW_SKIN))
    {
        send_to_char("Your scales harden.\n\r",ch);
        ch->armor -= 2000 ;
        SET_BIT(ch->newbits, NEW_SKIN);
        return;
    }
    else if (IS_SET(ch->newbits, NEW_SKIN))
    {
        send_to_char("Your scales soften.\n\r",ch);
        ch->armor += 2000;
        REMOVE_BIT(ch->newbits,NEW_SKIN);
        return;
    }
    return;
}

void do_dshape(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    argument=one_argument(argument,arg1);
    argument=one_argument(argument,arg2);
    strcpy(arg3,argument);

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classfour[DRAGON_WISDOM] < 6)
    {
        send_to_char("You must learn your dragon power to level 3.\n\r",ch);
        return;
    }

    if ( ( obj = get_obj_carry( ch,arg1 ) ) == NULL )
    {
        send_to_char("You dont have that item.\n\r",ch);
        return;
    }

    if (!(!str_cmp(arg2,"short") || !str_cmp(arg2,"name")))
    {
        send_to_char("syntax : reshape <item> <short/name> <newname>.\n\r",ch);
        return;
    }

    if (strlen(arg3) > 60 || strlen(arg3) < 3)
    {
        send_to_char("The name should be between 3 and 60 characters.\n\r",ch);
        return;
    }
    if (has_bad_chars(ch, arg3))
    {
        send_to_char("Illegal chars.\n\r",ch);
        return;
    }
    if (IS_SET(obj->quest, QUEST_ARTIFACT) || IS_SET(obj->quest, QUEST_PRIZE))
    {
        send_to_char("Not on artifacts and prizes.\n\r",ch);
        return;
    }

    if (!str_cmp(arg2,"name"))
    {
        free_string(obj->name);
        obj->name = str_dup(arg3);
        obj->questmaker = str_dup(ch->name);
    }
    if (!str_cmp(arg2,"short"))
    {
        free_string(obj->short_descr);
        obj->short_descr=str_dup(arg3);
        obj->questmaker = str_dup(ch->name);
    }

    send_to_char("Ok.\n\r",ch);
    return;
}
