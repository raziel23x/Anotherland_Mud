/*************
Orginal Class by Chrono
Modified and finished by Joshua - Not True, Never Finished, but Fixxed by Loial
Chrono AKA Greg Vines
Use for only Another Land
any other use requires my permission.
**************/

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

void do_flameeq(CHAR_DATA *ch, char *argument)
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;
    if( !IS_CLASS(ch, CLASS_PHOENIX))
    {
        send_to_char("What?\n\r",ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Please specify which piece of Phoenix armor you wish to make: Gloves, Sleeves, Wrist, Belt, Collar, Wings, Mask, Helmet, Plate, Ring, Leggings, Claws, Boots.\n\r",ch);
        return;
    }
    if ( ch->practice < 150 )
    {
        send_to_char("It costs 150 points of primal to create Phoenix armor.\n\r",ch);
        return;
    }
    if (ch->pcdata->upgrade_level >= 1)
    {
        if (!str_cmp(arg,"claws"               )) vnum = 5024;
        else if (!str_cmp(arg,"ring"                )) vnum = 5021;
        else if (!str_cmp(arg,"collar"              )) vnum = 5016;
        else if (!str_cmp(arg,"wrist"               )) vnum = 5014;
        else if (!str_cmp(arg,"plate"               )) vnum = 5020;
        else if (!str_cmp(arg,"helmet"              )) vnum = 5019;
        else if (!str_cmp(arg,"leggings"            )) vnum = 5022;
        else if (!str_cmp(arg,"boots"               )) vnum = 5023;
        else if (!str_cmp(arg,"gloves"              )) vnum = 5012;
        else if (!str_cmp(arg,"sleeves"             )) vnum = 5013;
        else if (!str_cmp(arg,"wings"               )) vnum = 5017;
        else if (!str_cmp(arg,"belt"                )) vnum = 5015;
        else if (!str_cmp(arg,"mask"                )) vnum = 5018;
        else
        {
            do_flameeq(ch,"");
            return;
        }
    }
    else
    {
        if (!str_cmp(arg,"claws")) vnum = 5010;
        else if (!str_cmp(arg,"wrist")) vnum = 5001;
        else if (!str_cmp(arg,"collar")) vnum = 5003;
        else if (!str_cmp(arg,"ring")) vnum = 5008;
        else if (!str_cmp(arg,"plate")) vnum = 5007;
        else if (!str_cmp(arg,"helmet")) vnum = 5006;
        else if (!str_cmp(arg,"leggings")) vnum = 5009;
        else if (!str_cmp(arg,"boots")) vnum = 5011;
        else if (!str_cmp(arg,"gloves")) vnum = 2651;
        else if (!str_cmp(arg,"sleeves")) vnum = 5000;
        else if (!str_cmp(arg,"wings")) vnum = 5004;
        else if (!str_cmp(arg,"belt")) vnum = 5002;
        else if (!str_cmp(arg,"mask")) vnum = 5005;
        else
        {
            do_flameeq(ch,"");
            return;
        }
    }
    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform a Coder.\n\r",ch);
        return;
    }
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    ch->practice -= 150;
    act("In a blast of flames, $p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("In a blast of flames, $p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_firelearn( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
        return;
    if (!IS_CLASS(ch, CLASS_PHOENIX) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    argument = one_argument (argument, arg);
    if (!str_cmp(arg,"show"))
    {
        if(ch->pcdata->phoenix[0]>0)
            send_to_char( "1.Firedive.\n\r", ch );
        if(ch->pcdata->phoenix[0]>1)
            send_to_char( "2.Phoenixsight.\n\r", ch );
        if(ch->pcdata->phoenix[0]>2)
            send_to_char( "3.Firestrike.\n\r", ch );
        if(ch->pcdata->phoenix[0]>3)
            send_to_char( "4.Flare.\n\r", ch );
        if(ch->pcdata->phoenix[0]>4)
            send_to_char( "5.Tailflare.\n\r", ch );
        if(ch->pcdata->phoenix[0]>5)
            send_to_char( "6.Sunfire.\n\r", ch );
        if(ch->pcdata->phoenix[0]>6)
            send_to_char( "7.Phoenixclaws.\n\r", ch );
        if(ch->pcdata->phoenix[0]>7)
            send_to_char( "8.Phoenixform.\n\r", ch );
        if(ch->pcdata->phoenix[0]>8)
            send_to_char( "9.Healingtears.\n\r", ch );
        if(ch->pcdata->phoenix[0]>9)
            send_to_char( "10.Rebirth.\n\r", ch );
    }
    else
    {
        if(ch->exp<1000000000)
        {
            stc("You need one billion exp to improve your skills.\n\r",ch);
            return;
        }
        else if(ch->pcdata->phoenix[0]>=10)
        {
            stc("Phoenix skills have been maxxed.\n\r",ch);
            return;
        }
        else
        {
            stc("Phoenix powers improved.\n\r",ch);
            ch->pcdata->phoenix[0]+=1;
            ch->exp-=1000000000;
        }
        return;
    }
}

void do_rebirth( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    victim = ch->fighting;
    if ( IS_NPC(ch) )
        return;
    if (!IS_CLASS(ch, CLASS_PHOENIX) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<10)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if(ch->pcdata->phoenix[1]>0)
    {
        stc("You have not recovered from your last rebirth.\n\r",ch);
        return;
    }
    if(ch->hit>0)
    {
        stc("You are not hurt enougth to be reborn.\n\r",ch);
        return;
    }
    else
    {
        ch->hit+=ch->max_hit*.18;
        stc("You rise from your #7A#0s#7h#0e#7s#n and are reborn.\n\r", ch);
        act("$n the #RP#roeni#Rx#n rises from their #7A#0s#7h#0e#7s#n and is reborn.", ch, NULL, victim, TO_ROOM);
        ch->position = POS_STANDING;

    }
    ch->pcdata->phoenix[1]=5;

}

void do_healingtears( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
        return;

    argument = one_argument (argument, arg);
    if (!IS_CLASS(ch, CLASS_PHOENIX) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if(ch->pcdata->phoenix[0]<9)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "Use HealingTears on whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "You cannot find them.\n\r", ch );
        return;
    }
    if ( !IS_NPC(victim) && victim->fight_timer > 0 )
    {
        if ( ch->fighting != NULL )
        {
            send_to_char( "Deal with your own problems.\n\r",ch);
            return;
        }
        send_to_char( "You are too busy avoiding the constant spray of flesh.\n\r", ch);
        return;
    }

    if(victim->in_room!=ch->in_room)
    {
        stc("They are not here.\n\r",ch);
        return;
    }
    victim->hit+=ch->pcdata->phoenix[0]*1000;
    if(victim->hit>victim->max_hit)victim->hit=victim->max_hit;
    act("You cry on $N easing their pain.", ch, NULL, victim, TO_CHAR);
    act("$n crys on $N easing their pain.", ch, NULL, victim, TO_ROOM);

    WAIT_STATE(ch,(14-(ch->pcdata->phoenix[0])));
}

void do_firedive( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
        return;
    argument = one_argument (argument, arg);

    if (!IS_CLASS(ch, CLASS_PHOENIX) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<1)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "FireDive whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "You cannot find them.\n\r", ch );
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

    act("You dive into the sun's glare and hunt for $N.", ch, NULL, victim, TO_CHAR);
    act("$n dive's into the sun's glare hunting for someone.", ch, NULL, victim, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("$n appear's out of a sun's eclipse in front of $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n appears out of a sun's eclipse.", ch, NULL, victim, TO_VICT);
    do_look(ch,"scry");
    return;
}

void do_tailflare(  CHAR_DATA *ch, char *argument  )
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_PHOENIX) )
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<5)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if ( IS_CLASS(ch, CLASS_PHOENIX) )
    {

        if ( ch->rage >= 500 )
        {
            send_to_char("But your already tail flaring.\n\r",ch);
            return;
        }
        if ( ch->move < 500 )
        {
            send_to_char("But you don't have enough move to cause a tail flare.\n\r", ch);
            return;
        }

        send_to_char("Your tail sparks a flare.\n\r",ch);
        act("$n tail sparks a blinding flare.",ch,NULL,NULL,TO_ROOM);
        ch->rage += 100;
        ch->move -= 500;
        WAIT_STATE(ch,12);
        return;
    }
    else
        send_to_char("But your already tail flaring.\n\r",ch);
    return;
}

void do_flare( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int flare;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_PHOENIX))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<4)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if ( arg[0] == '\0')
    {
        send_to_char( "#RSend a solar flare on whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }
    if (ch->mana < 5000)
    {
        send_to_char( "You do not have enough mana!\n\r", ch);
        return;
    }
    if (is_safe(ch, victim))
    {
        stc("You cannot attack them.\n\r",ch);
        return;
    }

    act("#r$n#0 begins soaring into the sky, summoning forth a #ySolar Flare#0 upon $N!#n", ch, NULL, victim, TO_NOTVICT);
    sprintf( buf, "#0You summon forth a #ysolar Flare#0 upon $N!#n\n\r");
    act(buf, ch, NULL, victim, TO_CHAR);
    sprintf( buf, "#r$n#0 begins soaring into the sky, summoning forth a #ySolar Flare#0 upon you!#n\n\r");

    flare = number_range(5,15);
    if ( IS_AFFECTED(victim, AFF_BLIND))
    {
        af.type      = gsn_blindness;
        af.location  = APPLY_HITROLL;
        af.modifier  = -4;
        af.duration  =  flare * 10;
        af.bitvector = AFF_BLIND;
        affect_to_char( victim, &af );
        WAIT_STATE(victim, number_range(5, 15));
        send_to_char( "#yYou are blinded!#n\n\r", victim );
    }
    else
    {
        stc("They are already blinded.\n\r",ch);
    }

    one_hit(ch,victim, gsn_wrathofgod,1);
    one_hit(ch,victim, gsn_wrathofgod,1);

    WAIT_STATE(ch, 8);
    ch->mana -= 5000;
    if (victim->hit < -10) victim->hit = -10;
    return;
}

void do_phoenixshift( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_PHOENIX))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<8)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if ( !IS_SET(ch->polyaff, POLY_ZULOFORM) )
    {
        if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        stc("Your body shrinks into a flaming phoenix.\n\r",ch);
        act("$n's body shrinks into a flaming phoenix.",ch,NULL,NULL, TO_ROOM);
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        ch->damroll += 3000;
        ch->hitroll += 3000;
        ch->armor   -= 1000;
        sprintf(buf, "#RF#rl#Ra#rm#Ri#rn#Rg #RP#yh#Ro#ye#Rn#yi#Rx #R%s#n", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        return;
    }
    else if ( IS_SET(ch->polyaff, POLY_ZULOFORM) )
    {
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        ch->damroll -= 3000;
        ch->hitroll -= 3000;
        ch->armor   += 1000;
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        stc("Your body grows back to its normal form.\n\r",ch);
        act("The #RF#rl#Ra#rm#Ri#rn#Rg #RP#yh#Ro#ye#Rn#yi#Rx#n #R$n reshapes back into a human.",ch,NULL,NULL,TO_ROOM);
        return;
    }
    return;
}

void do_phoenixsight( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_PHOENIX))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<2)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "You're eyes turn back to normal.\n\r", ch );
    }

    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "You are gifted  with the sight of the phoenix.\n\r", ch );
    }
    return;
}

void do_phoenixclaws(CHAR_DATA *ch, char *argument)
{

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_PHOENIX))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<7)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if (IS_VAMPAFF(ch, VAM_CLAWS))
    {
        send_to_char("Claws grow back into your fingertips.\n\r", ch );
        REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
        return;
    }

    send_to_char("Claws grow out of your fingertips.\n\r", ch );
    SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);

    return;
}

void do_sunfire(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    int dam;

    argument = one_argument(argument, arg1);

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_PHOENIX))
    {
        stc("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<6)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if (( victim = get_char_room(ch, arg1)) == NULL)
    {
        if (( victim = ch->fighting) == NULL)
        {
            send_to_char("Use Sun Fire upon whom?\n\r",ch);
            return;
        }
        else victim = ch->fighting;
    }
    if (is_safe(ch, victim))
    {
        send_to_char("They are safe from such a hostile action.\n\r",ch);
        return;
    }
    if (ch->mana < 10000)
    {
        send_to_char("You do not have the 10,000 mana required to use this.\n\r",ch);
        return;
    }
    WAIT_STATE(ch, 16);
    dam = number_range(4000, 9000);
    if (!IS_NPC(victim) && victim->alignment == 1000) dam *= 0.5;
    if (IS_NPC(victim)) dam += victim->hit/10;
    sprintf(buf, "#CYou call upon a sun fire to harm your opponent! #7[%d]\n\r",dam);
    stc(buf,ch);
    sprintf(buf, "#C%s calls upon a sun fire to harm you! #7[%d]\n\r",ch->name,dam);
    stc(buf,victim);
    act("#C$n calls upon a sun fire to harm $N!#n\n\r",ch,NULL,victim,TO_NOTVICT);
    ch->mana -= 10000;
    if (number_range(1,4) == 4)
    {
        ch->hit += dam/2;
        if(ch->hit>ch->max_hit)ch->hit=ch->max_hit;
        stc ("#LYour sun fire has healed you.#n\n\r",ch);
    }
    hurt_person(ch, victim, dam);
    one_hit(ch,victim, gsn_wrathofgod,2);
    one_hit(ch,victim, gsn_wrathofgod,2);
    one_hit(ch,victim, gsn_wrathofgod,2);
    one_hit(ch,victim, gsn_wrathofgod,2);
    if (ch->fighting == NULL) ch->fighting = victim;
    if (victim->hit < -10) victim->hit = -10;
    return;
}

void do_firestrike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_PHOENIX) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if(ch->pcdata->phoenix[0]<3)
    {
        stc("Phoenix powers not high enougth.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
    {
        send_to_char("You aren't fighting anyone though.\n\r",ch);
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char("They aren't stanced though.\n\r",ch);
        return;
    }
    WAIT_STATE(ch, 8);
    do_say(ch,"#RMy Flame Will Destroy You!#n");
    do_stance(victim, "");
    return;
}
