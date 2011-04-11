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
#include "wraith.h"

void do_spiritsoar( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
        return;

    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_WRAITH) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "Spiritsoar where?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "You cannot find them.\n\r", ch );
        return;
    }

    if( ch->move < 600)
    {
        stc( "You don't have enough movement points to spiritsoar yourself.\n\r",ch);
        return;
    }

    if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "You can't find it's room.\n\r",ch);
        return;
    }

    if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "That room is sealed against magic.\n\r",ch);
        return;
    }

    if (ch == victim)
    {
        send_to_char("Whoa! Can't do that!\n\r",ch);
        return;
    }
    if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
    {
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }
    if ( room_is_private(victim->in_room ) )
    {
        send_to_char( "That room is private right now.\n\r", ch );
        return;
    }
    if (victim->in_room == ch->in_room)
    {
        send_to_char("But you're already there!\n\r",ch);
        return;
    }

    act("You spiritsoar to $N.", ch, NULL, victim, TO_CHAR);
    act("$n disappears from the room.", ch, NULL, victim, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    ch->move -= 500;
    act("$n's body starts to take form.", ch, NULL, victim, TO_VICT);
    act("$n materializes in front of $N.", ch, NULL, victim, TO_NOTVICT);
    do_look(ch,"scry");
    return;
}

void do_wraitheq( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (ch->pcdata->powers[WRAITH_DEATH] < 0)
    {
        send_to_char("You must have level 1 in death before you can do that.\n\r",ch);
        return;
    }

    if(!IS_IMMORTAL(ch))
    {
        if (!IS_CLASS(ch, CLASS_WRAITH))
        {
            send_to_char("What?\n\r",ch);
            return;
        }
    }

    if (arg[0] == '\0')
    {
        send_to_char("Please specify which piece of eq you wish to make: Ring Amulet Plate Hood Greaves Boots Gauntlets Sleeves Cloak Belt Bracer Mask Scythe.\n\r",ch);
        return;
    }

    if ( ch->practice < 90 )
    {
        send_to_char("You dont have the 90 primal needed.\n\r",ch);
        return;
    }

    if (ch->pcdata->upgrade_level >= 1)
    {
        if (!str_cmp(arg,"ring")) vnum = 110071;
        else if (!str_cmp(arg,"amulet")) vnum = 110072;
        else if (!str_cmp(arg,"plate")) vnum = 110073;
        else if (!str_cmp(arg,"hood")) vnum = 110082;
        else if (!str_cmp(arg,"greaves")) vnum = 110076;
        else if (!str_cmp(arg,"boots")) vnum = 110077;
        else if (!str_cmp(arg,"gauntlets")) vnum = 110078;
        else if (!str_cmp(arg,"sleeves")) vnum = 110079;
        else if (!str_cmp(arg,"cloak")) vnum = 110080;
        else if (!str_cmp(arg,"belt")) vnum = 110081;
        else if (!str_cmp(arg,"bracer")) vnum = 110073;
        else if (!str_cmp(arg,"mask")) vnum = 110075;
        /*else if (!str_cmp(arg,"spirit")) vnum = 33333;
        else if (!str_cmp(arg,"power")) vnum = 33332;
        else if (!str_cmp(arg,"medal")) vnum = 33334;
        else if (!str_cmp(arg,"tattoo")) vnum = 33335;*/
        else if (!str_cmp(arg,"scythe")) vnum = 110070;
        else
        {
            do_wraitheq(ch,"");
            return;
        }
    }

    if (ch->pcdata->upgrade_level == 0)
    {
        if (!str_cmp(arg,"ring")) vnum = 33320;
        else if (!str_cmp(arg,"amulet")) vnum = 33321;
        else if (!str_cmp(arg,"plate")) vnum = 33322;
        else if (!str_cmp(arg,"hood")) vnum = 33323;
        else if (!str_cmp(arg,"greaves")) vnum = 33324;
        else if (!str_cmp(arg,"boots")) vnum = 33330;
        else if (!str_cmp(arg,"gauntlets")) vnum = 33325;
        else if (!str_cmp(arg,"sleeves")) vnum = 33329;
        else if (!str_cmp(arg,"cloak")) vnum = 33331;
        else if (!str_cmp(arg,"belt")) vnum = 33327;
        else if (!str_cmp(arg,"bracer")) vnum = 33328;
        else if (!str_cmp(arg,"mask")) vnum = 33326;
        /*else if (!str_cmp(arg,"spirit")) vnum = 33333;
        else if (!str_cmp(arg,"power")) vnum = 33332;
        else if (!str_cmp(arg,"medal")) vnum = 33334;
        else if (!str_cmp(arg,"tattoo")) vnum = 33335;*/
        else if (!str_cmp(arg,"scythe")) vnum = 33332;
        else
        {
            do_wraitheq(ch,"");
            return;
        }
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform a God .\n\r",ch);
        return;
    }

    if( !IS_IMMORTAL(ch) )
    {
        ch->practice -= 60;
    }
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("You glow with wicked power and produce $p.",ch,obj,NULL,TO_CHAR);
    act("$n's begins to glow wickedly and makes $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_ability( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        sprintf(buf,"Abilities: Death (%d), Power (%d).\n\r",
            ch->pcdata->powers[WRAITH_DEATH], ch->pcdata->powers[WRAITH_POWER]);
        send_to_char(buf,ch);
        return;
    }
    if (arg2[0] == '\0')
    {
        sprintf(buf,"Abilities: Death (%d), Power (%d).\n\r",
            ch->pcdata->powers[WRAITH_DEATH], ch->pcdata->powers[WRAITH_POWER]);
        send_to_char(buf,ch);
        return;
    }
    if (!str_cmp(arg2,"gain"))
    {
        int improve;
        int cost;
        int max;

        if (!str_cmp(arg1,"death"   )) {improve = WRAITH_DEATH;   max=6;}
        else if (!str_cmp(arg1,"power"   )) {improve = WRAITH_POWER;   max=6;}
        else
        {
            send_to_char("Abilities: #0Death#n, #RPower#n.\n\r",ch);
            return;
        }
        cost = (ch->pcdata->powers[improve]+1) * 100;
        arg1[0] = UPPER(arg1[0]);
        if ( ch->pcdata->powers[improve] >= max )
        {
            sprintf(buf,"You have already gained all the powers of the %s ability.\n\r", arg1);
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->practice )
        {
            sprintf(buf,"It costs you %d primal to improve your %s ability.\n\r", cost, arg1);
            send_to_char(buf,ch);
            return;
        }
        ch->pcdata->powers[improve] += 1;
        ch->practice -= cost;
        sprintf(buf,"You improve your ability in the %s ability.\n\r", arg1);
        send_to_char(buf,ch);
    }
    else send_to_char("To improve a ability, type: ability <type> gain.\n\r",ch);
    return;
}

void do_deathslash (CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *ich;
    CHAR_DATA *ich_next;
    CHAR_DATA *victim = NULL;

    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (ch->pcdata->powers[WRAITH_DEATH] < 3 )
    {
        send_to_char("You don't have that power yet.\n\r",ch);
        return;
    }
    if (ch->move < 1000)
    {
        send_to_char("Not enough movement.\n\r",ch);
        return;
    }
    if (IS_CLASS(ch, CLASS_WRAITH)) send_to_char("You take your weapon and slash everyone in the room.\n\r",ch);
    ich_next = ch->in_room->people;

    ich = ich_next;
    while (ich_next != NULL)
    {
        ich_next = ich->next_in_room;
        if (ich != ch && !IS_IMMORTAL(ich))
        {
            //     if (ich != ch)
            {
                if (is_safe(ch,ich)) break;
                one_hit(ch,ich,gsn_deathslash,1);
                one_hit(ch,ich,gsn_deathslash,1);
                one_hit(ch,ich,gsn_deathslash,1);
                int dam = 0;
                if ( dam <= 0 )
                    dam = 1;
                set_fighting(ch,victim);

                dam = ((ch->pcdata->upgrade_level * (ch->pdeath+1))*100);

                sprintf(buf,"Your spit of acid hits $N incredibly hard! [%d]",dam);
                act(buf,ch,NULL,victim,TO_CHAR);
                sprintf(buf,"$n's spit of acid hits you incredibly hard! [%d]",dam);
                act(buf,ch,NULL,victim,TO_VICT);
                sprintf(buf,"$n's spit of acid hits $N incredibly hard! [%d]",dam);
                act(buf,ch,NULL,victim,TO_NOTVICT);

                send_to_char("\n\r",ch);
                hurt_person(ch,victim,dam);
            }
            ich = ich_next;
        }
        ch->move -= 800;
        WAIT_STATE(ch, 12);
        return;
    }
}

void do_dertslash (CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *ich;
    CHAR_DATA *ich_next;
    //CHAR_DATA *victim;
    int dam = 0;

    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (ch->pcdata->powers[WRAITH_DEATH] < 3 )
    {
        send_to_char("You don't have that power yet.\n\r",ch);
        return;
    }
    if (ch->move < 1000)
    {
        send_to_char("Not enough movement.\n\r",ch);
        return;
    }
    if (IS_CLASS(ch, CLASS_WRAITH)) send_to_char("You take your weapon and slash everyone in the room.\n\r",ch);
    ich_next = ch->in_room->people;
    ich = ich_next;
    while (ich_next != NULL && ich_next != ch)
    {
        send_to_char("OKWHILE\n\r",ch);
        ich_next = ich->next_in_room;
        //if (ich != ch && !IS_IMMORTAL(ich))
        if (IS_NPC(ich))
        {
            send_to_char("OKNPC\n\r",ch);
            //if (is_safe(ch,ich)) break;
            one_hit(ch,ich,gsn_deathslash,1);
            one_hit(ch,ich,gsn_deathslash,1);
            one_hit(ch,ich,gsn_deathslash,1);
        }
        if (!IS_NPC(ich))
        {
            send_to_char("OK!NPC\n\r",ch);
            if (ich == ch) ich_next = ich->next_in_room;
            if (IS_IMMORTAL(ich)) ich_next = ich->next_in_room;
            if (!IS_NPC(ich))
            {
                if (ich != ch)
                {
                    if (is_safe(ch,ich)) break;

                    //int dam = 0;
                    if ( dam <= 0 ) dam = 1;
                    set_fighting(ch,ich);

                    dam = ((ch->pcdata->upgrade_level * (ch->pdeath+1)*100));
                    if (number_range(1,2) == 1)
                    {
                        dam = dam * 2;
                        if (number_range(1,2) == 1)
                        {
                            dam = dam * 2;
                            if (number_range(1,3) == 1)
                            {
                                dam = dam * 2;
                                if (number_range(1,3) == 1)
                                {
                                    dam = dam * 2;
                                }
                            }
                        }
                    }
                }
                if ( dam <= 0 ) dam = 1;

                sprintf(buf,"Your deathslash digs in to $N's soul! [%d]",dam);
                act(buf,ch,NULL,ich,TO_CHAR);
                sprintf(buf,"You scream in pain as $n's deathslash cuts in to your soul! [%d]",dam);
                act(buf,ch,NULL,ich,TO_VICT);
                sprintf(buf,"$N's screams in pain as $n cuts down to their soul! [%d]",dam);
                act(buf,ch,NULL,ich,TO_NOTVICT);

                send_to_char("OKEND\n\r",ch);
                hurt_person(ch,ich,dam);
                ich_next = ich->next_in_room;
            }
        }
        ich = ich->next_in_room;
        ch->move -= 800;
        WAIT_STATE(ch, 12);
        return;
    }
}

void do_deathaura(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WRAITH)) return;
    if (ch->pcdata->powers[WRAITH_DEATH] < 5)
    {
        send_to_char("You must have level 5 in death to use this.\n\r",ch);
        return;
    }
    if (IS_SET(ch->pcdata->powers[AURAS], DEATH_AURA))
    {
        REMOVE_BIT(ch->pcdata->powers[AURAS], DEATH_AURA);
        send_to_char("Your aura of death fades away.\n\r",ch);
        return;
    }
    else
    {
        SET_BIT(ch->pcdata->powers[AURAS], DEATH_AURA);
        send_to_char("An aura of death surrounds you.\n\r",ch);
        return;
    }
}

void do_soundwave( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int limb;

    if (IS_NPC(ch)) return;
    if( !IS_CLASS(ch, CLASS_WRAITH) )
    {
        send_to_char("What?\n\r",ch);
        return;
    }
    if (ch->pcdata->powers[WRAITH_POWER] < 3)
    {
        send_to_char("You need level 3 in power to use this.\n\r",ch);
        return;
    }
    if (( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }
    if (number_range(1,2) == 1)
    {
        act("You send out a screeching sound.",ch,NULL,victim,TO_CHAR);
        act("You clutch your ears in pain as $n delivers a piercing soundwave.",ch,NULL,victim,TO_VICT);
        act("$n sends a screeching sound and $N falls to the ground holding $S ears.",ch,NULL,victim,TO_NOTVICT);
        one_hit(ch,victim,gsn_soundwave,1);
        WAIT_STATE(ch, 15);
        return;
    }
    else if (number_range(1,2) == 2)
    {
        act("You send out a screeching sound.",ch,NULL,victim,TO_CHAR);
        act("You clutch your ears in pain as $n delivers a piercing soundwave.",ch,NULL,victim,TO_VICT);
        act("$n sends a screeching sound and $N falls to the ground holding $S ears.",ch,NULL,victim,TO_NOTVICT);
        one_hit(ch,victim,gsn_soundwave,1);
        limb = number_range(1,9);
        if (number_range(1,100) < 99 && !IS_NPC(victim))
        {
            if (limb < 9)
            {
                if (!IS_ARM_R(victim,LOST_ARM))            SET_BIT(victim->loc_hp[3],LOST_ARM);
                if (!IS_BLEEDING(victim,BLEEDING_ARM_R))   SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
                if (IS_BLEEDING(victim,BLEEDING_HAND_R))   REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
                if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)     take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)    take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)    take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)  take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL) take_item(victim,obj);
            }
            else
            {
                if (!IS_ARM_L(victim,LOST_ARM))            SET_BIT(victim->loc_hp[2],LOST_ARM);
                if (!IS_BLEEDING(victim,BLEEDING_ARM_L))   SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
                if (IS_BLEEDING(victim,BLEEDING_HAND_L))   REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
                if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)     take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)     take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)    take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)  take_item(victim,obj);
                if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL) take_item(victim,obj);
            }
            WAIT_STATE(ch, 14);
            return;
        }
    }
}

void do_shadowform( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (ch->pcdata->powers[WRAITH_POWER] < 2)
    {
        send_to_char("You need level 2 in power to use this.\n\r",ch);
        return;
    }
    if (has_timer(ch)) return;
    if ((IS_AFFECTED(ch, AFF_ETHEREAL)) && IS_AFFECTED(ch, AFF_SHADOWPLANE))
    {
        REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
        REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
        act("You become solid again.",  ch, NULL, NULL, TO_CHAR);
        act("$n becomes solid again.",  ch, NULL, NULL, TO_ROOM);
        return;
    }
    SET_BIT(ch->affected_by, AFF_ETHEREAL);
    SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    act("You phase out of sight.", ch, NULL, NULL, TO_CHAR);
    act("$n's body flickers with raw energy then blinks out of sight.", ch, NULL, NULL, TO_ROOM);
    return;
}

void do_wraithform( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->powers[WRAITH_POWER] < 5)
    {
        send_to_char("You need level 5 in power to use this.\n\r",ch);
        return;
    }

    if (IS_POLYAFF(ch, POLY_ZULOFORM))
    {
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        act( "You trransform your body to regular form.", ch, NULL, NULL, TO_CHAR );
        act( "$n's body turns back into a regular wraith", ch, NULL, NULL, TO_ROOM);
        ch->damroll -= 350;
        ch->hitroll -= 350;
        ch->armor   += 600;
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_POLYMORPH))
    {
        send_to_char("You cant wraith form when changed.\n\r",ch);
        return;
    }
    if (ch->stance[0] != -1) do_stance(ch,"");
    if (ch->mounted == IS_RIDING) do_dismount(ch,"");
    act( "You transform into a wicked wraith.", ch, NULL, NULL, TO_CHAR);
    act( "$n's body grows with wicked power.", ch, NULL, NULL, TO_ROOM );
    ch->pcdata->mod_str = 15;
    ch->pcdata->mod_dex = 15;
    SET_BIT(ch->polyaff, POLY_ZULOFORM);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    sprintf(buf, "%s the wicked wraith", ch->name);
    free_string( ch->morph );
    ch->morph = str_dup( buf );
    ch->damroll += 350;
    ch->hitroll += 350;
    ch->armor   -= 600;
    return;
}

void do_wraithsight( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WRAITH))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (ch->pcdata->powers[WRAITH_POWER] < 1)
    {
        send_to_char("You need level 1 in power\n\r",ch);
        return;
    }
    if (IS_SET(ch->act, PLR_HOLYLIGHT))
    {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char("You lose your ability to see perfectly.\n\r", ch);
    }
    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "Your eyes glow with dark power, allowing you to see everything.\n\r", ch );
    }
    return;
}
