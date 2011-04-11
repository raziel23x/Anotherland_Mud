/**************************************
 * Class modified by Hutoshi strictly *
 * for Anotherland.                   *
 **************************************/

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
#include "class.h"

#define PUNC_OTHER(c,v,d)               hurt_person(c, v, d);

void do_kahnmaster( CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh??\n\n",ch);
        return;
    }

    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        sprintf(buf,"Forms [%d], Ranks [%d], Combat [%d], Tiger [%d].\n\r",
            ch->pcdata->classthree[KAHN_FORMS],
            ch->pcdata->classthree[KAHN_RANKS],
            ch->pcdata->classthree[KAHN_COMBAT],
            ch->pcdata->classthree[KAHN_TIGER]);
        send_to_char(buf,ch);
        return;
    }
    if (arg2[0] == '\0')
    {
        send_to_char("To improve a mastery, type: kahnmaster <catagory> learn.\n\r",ch);
        return;
    }
    if (!str_cmp(arg2, "learn"))
    {
        int learn;
        int cost;
        int max;
        int maxlevel = 5;

        if (!str_cmp(arg1,"forms"      ))
        {
            learn = KAHN_FORMS;
            max=maxlevel;
        }
        else if (!str_cmp(arg1,"ranks"     ))
        {
            learn = KAHN_RANKS;
            max=maxlevel;
        }
        else if (!str_cmp(arg1,"combat"    ))
        {
            learn = KAHN_COMBAT;
            max=maxlevel;
        }
        else if (!str_cmp(arg1,"tiger" ))
        {
            learn = KAHN_TIGER;
            max=maxlevel;
        }

        else
        {
            send_to_char("You can learn: Forms, Ranks, Combat, and Tiger.\n\r",ch);
            return;
        }

        cost = (ch->pcdata->classthree[learn]+1) * 2000;
        arg1[0] = UPPER(arg1[0]);
        if ( ch->pcdata->classthree[learn] >= max)
        {
            sprintf(buf,"You have already gained all the powers of the %s mastery.\n\r", arg1);
            send_to_char(buf,ch);
            return;
        }
        if (ch->pcdata->classthree[learn] >=max)
        {
            sprintf(buf,"You have already maxed %s.\n\r",arg1);
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->pcdata->stats[KAHN_CURRENT] )
        {
            sprintf(buf,"It costs you %d kahn points to improve your %s study.\n\r", cost, arg1);
            send_to_char(buf,ch);
            return;
        }
        ch->pcdata->classthree[learn] += 1;
        ch->pcdata->stats[KAHN_CURRENT] -= cost;
        sprintf(buf,"You improve your ability in the %s study.\n\r", arg1);
        send_to_char(buf,ch);
    }
    else send_to_char("To improve a mastery, type: master <catagory> learn.\n\r",ch);
    return;
}

void do_clawslash( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf [MAX_STRING_LENGTH];
    int dam=0;
    victim=ch->fighting;
    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char( "Huh?\n\r", ch );
        return;
    }

    if (ch->pcdata->classthree[KAHN_COMBAT] < 4)
    {
        send_to_char("You need level 4 combat to clawslash!\n\r",ch);
        return;
    }

    if (arg[0] == '\0')                                     /* && ch->fighting == NULL) */
    {
        send_to_char("Who do you wish to clawslash?\n\r",ch);
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "You cannot kill yourself!\n\r", ch );
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ( victim = get_char_room( ch, arg ) ) == NULL )
        {
            send_to_char( "They aren't here.\n\r", ch );
            return;
        }
    }

    if (IS_ITEMAFF(ch, ITEMA_PEACE)) do_majesty(ch,"");

    if (is_safe(ch,victim))
        return;

    WAIT_STATE( ch, PULSE_VIOLENCE );

    dam = (char_damroll(ch))*3;
    if (dam == 0) dam = 1;
    if ( !IS_AWAKE(victim) )
        dam *= 1.75;

    if ( dam <= 0 )
        dam = 1;
    if ( dam >= 20000 )
        dam = 20000;

    sprintf( buf, "Your slashing claw rips $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's slashing claw rips you up#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's slashing claw rips $N up!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);
    ch->fighting = victim;
    victim->fighting = ch;
    ch->position = POS_FIGHTING;
    victim->position = POS_FIGHTING;
    update_pos(ch);
    update_pos(victim);
    return;
}

void do_kahnform( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ((ch->pcdata->classthree[KAHN_FORMS] < 3))
    {
        send_to_char("You need to be at 3 in the forms mastery to use that power.\n\r",ch);
        return;
    }

    if ( !IS_SET(ch->polyaff, POLY_ZULOFORM) )
    {
        if (ch->mounted == IS_RIDING) do_dismount(ch,"");
        stc("Your body grows and distorts into a ravenging weretiger.\n\r",ch);
        act("$n's body grows and distorts into a ravenging weretiger.",ch,NULL,NULL, TO_ROOM);
        SET_BIT(ch->polyaff, POLY_ZULOFORM);
        SET_BIT(ch->affected_by, AFF_POLYMORPH);
        ch->damroll += 2000;
        ch->hitroll += 2000;
        ch->armor   -= 2000;
        sprintf(buf, "#GThe ravenging weretiger #y%s#n", ch->name);
        free_string( ch->morph );
        ch->morph = str_dup( buf );
        return;
    }
    else if ( IS_SET(ch->polyaff, POLY_ZULOFORM) )
    {
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
        ch->damroll -= 2000;
        ch->hitroll -= 2000;
        ch->armor   += 2000;
        free_string( ch->morph );
        ch->morph = str_dup( "" );
        stc("Your body shrinks and distorts back to its normal form.\n\r",ch);
        act("The ravenging weretiger's body shrinks and distorts into the form of $n.",ch,NULL,NULL,TO_ROOM);
        return;
    }
    else
        send_to_char("#L Kahnform, the ability to harness the true power of the beast.\n\r", ch);
    return;
}

void do_gaiasight( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classthree[KAHN_COMBAT] < 1)
    {
        send_to_char("You need at least 1 in Combat first.\n\r",ch);
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
        send_to_char( "You are gifted  with the sight of gaia.\n\r", ch );
        ch->move -= 750;
    }
    return;
}

void do_kstalk( CHAR_DATA *ch, char *argument )
{

    char arg[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument(argument,arg);

    if (IS_NPC(ch))
        return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classthree[KAHN_COMBAT] < 3)
    {
        send_to_char("You need at least 3 in the Combat mastery first!\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Kstalk who?\n\r",ch);
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "No way! You are fighting.\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "Nobody by that name.\n\r", ch );
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

    if (ch == victim)
    {
        send_to_char("Not on yourself!\n\r",ch);
        return;
    }

    if(!IS_NPC(victim) && !IS_IMMUNE( victim, IMM_SUMMON ) )
    {
        send_to_char( "You cannot find a starting point to hunt them.\n\r", ch );
        return;
    }

    //    stc("#PYou sniff the air and leap into the bushes to hunt.\n\r",ch);

    act("#P$n sniffs the air and then leaps into the bushes!",ch,NULL,NULL,TO_ROOM);
    act("#PYou sniff the air and run after $N in a frenzy.",ch,NULL,victim,TO_CHAR);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("#P$n suddenly leaps out of the bushes in front of you!",ch,NULL,victim,TO_VICT);
    act("#P$n suddenly leaps out of the bushes in front of $N.",ch,NULL,victim,TO_ROOM);

    WAIT_STATE( ch, 4 );
    do_look(ch,"");
    return;

}

void do_warmth(CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch))
        return;
    else if (!IS_CLASS(ch, CLASS_KAHN) )
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }
    else if (ch->pcdata->classthree[KAHN_FORMS] < 1)
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    else if (ch->move < 500)
    {
        send_to_char("You don't have enough move.\n\r", ch );
        return;
    }
    else if (ch->position == POS_FIGHTING)
    {
        send_to_char("You cannot do this whilst fighting.\n\r", ch);
        return;
    }
    ch->move = ch->move - 500;
    if (ch->fight_timer == 0)
    {
        ch->hit = ch->hit + ch->max_hit / 4;
    }
    else if (ch->fight_timer > 0)
    {
        ch->hit = ch->hit + 500;
    }
    if (ch->hit > ch->max_hit)
        ch->hit = ch->max_hit;
    send_to_char("Gaia's warmth encompasses you.\n\r",ch);
    act("$n is surrounded in the warmth of Gaia.",ch,NULL,NULL,TO_ROOM);
    WAIT_STATE(ch, 12);
    return;
}

void do_clawhold(CHAR_DATA *ch,char *argument)
{

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char( "Huh?\n\r", ch );
        return;
    }

    if (ch->pcdata->classthree[KAHN_FORMS] < 4)
    {
        send_to_char("You havn't learned how to use your claws in such a manner.\n\r",ch);
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

void do_blitz(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    char buf [MAX_STRING_LENGTH];

    if(IS_NPC(ch)) return;

    if(!IS_CLASS( ch, CLASS_KAHN))
    {
        stc("Huh?\n\r",ch);
        return;
    }

    if(ch->pcdata->classthree[KAHN_RANKS] < 5)
    {
        stc("You need level 5 Ranking to blitz.\n\r",ch);
        return;
    }

    act("You roar in outrage and attack all in the room.",ch,NULL,NULL,TO_CHAR);
    act("$n roars and savagly attacks all in the room.",ch,NULL,NULL,TO_ROOM);

    dam = char_damroll(ch) * .5;
    if (dam <= 0) dam = 1;

    for(vch = char_list; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next;
        if(ch->in_room->vnum != vch->in_room->vnum) continue;
        if(ch == vch) continue;
        if(!IS_NPC(vch)) continue;
        if(is_safe(ch,vch)) continue;
        sprintf( buf, "Your slashing claw rips $N up [%i]", dam );
        act( buf, ch, NULL, vch, TO_CHAR );
        sprintf( buf, "$n's slashing claw rips you up [%i]", dam );
        act( buf, ch, NULL, vch, TO_VICT );
        act("$n #sCLAWS#n $N.",ch,NULL,vch,TO_ROOM);
        hurt_person(ch, vch, dam);
        ch->fighting = vch;
        vch->fighting = ch;
        ch->position = POS_FIGHTING;
        vch->position = POS_FIGHTING;
        update_pos(ch);
        update_pos(vch);

    }
    WAIT_STATE(ch,10);
    return;
}

void do_kicetouch( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = NULL;
    int   dam;
    char buf [MAX_STRING_LENGTH];
    char arg [MAX_INPUT_LENGTH];

    if ( ( !IS_NPC(ch) ) && ( !IS_CLASS(ch, CLASS_KAHN) ) )
        return;

    if (arg[0] == '\0')
    {
        send_to_char("Icetouch whom?\n\r",ch);
        return;
    }
    /*  if ( ch->fighting != NULL )
         victim = ch->fighting;
         else {
                 stc("But you are not in a fight.\n\r",ch);
                 return;
              } */
    if (ch->pcdata->classthree[KAHN_COMBAT] < 5)
    {
        send_to_char("You need level 5 combat to icetouch!\n\r",ch);
        return;
    }

    if (ch->pcdata->classthree[KAHN_TICK] > 0)
    {
        send_to_char("You are too exhausted from the last time you used this.\n\r",ch);
        return;
    }

    if ( !IS_NPC(ch) )
    {

        if ( ch->mana < 300 )
        {
            stc("You don't have enough magical energy for that.\n\r",ch);
            return;
        }

        ch->mana -= 300;
    }
    dam = number_range(5000, 10000);
    dam += number_range(1000, 1100);
    dam += number_range(1000, 1100);
    dam += number_range(1000, 1100);
    dam += number_range(750, 1100);

    act("\nYou say, '#yGaia guide my freezing touch!#n'\n\r",ch,NULL,NULL,TO_CHAR);
    act("\n$n says, '#yGaia guide my freezing touch!#n'\n\r",ch,NULL,NULL,TO_ROOM);
    act("Your claws glow blue as you slash towards $N's body.\n\r",ch,NULL,victim,TO_CHAR);
    act("$n swings their claws down towards you.\n\r",ch,NULL,victim,TO_VICT);
    act("$n swings their claws down towards $N.\n\r",ch,NULL,victim,TO_ROOM);
    act("A loud howl is heard, and a mysterious fog fills the land.\n\r",ch,NULL,NULL,TO_ROOM);
    act("Your claws are surround by an icy aura.\n\r",ch,NULL,NULL,TO_CHAR);
    sprintf(buf,"You scream loudly as the icy haze slices into your body [#R%d#n].\n\r",dam);
    stc(buf,victim);
    sprintf(buf,"%s screams loudly as the icy haze slices into theire body [#R%d#n].\n\r",victim->name,dam);
    stc(buf,ch);
    act("$n screams loudly as the icy haze slices into $s's body.\n\r",ch,NULL,victim, TO_NOTVICT);

    ch->pcdata->classthree[KAHN_TICK] = 1;
    victim->position = POS_STUNNED;
    hurt_person(ch, victim, dam);
    WAIT_STATE(ch, 14);
    return;
}

void do_darkroar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    char arg[MAX_STRING_LENGTH];
    int sn;

    argument = one_argument(argument,arg);

    if (IS_NPC(ch))
        return;

    if (!IS_CLASS( ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->classthree[KAHN_RANKS] < 2)
    {
        send_to_char("You need level 2 in Ranks first!\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Roar at who?\n\r",ch);
        return;
    }

    if ((victim = get_char_room( ch, arg )) == NULL)
    {
        send_to_char("They are not here.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (is_affected(victim, gsn_equilibrium))
    {
        send_to_char("You cannot roar at them anymore!\n\r",ch);
        return;
    }

    if ( ( sn = skill_lookup( "equilibrium" ) ) < 0 )
        return;

    af.type      = sn;
    af.duration  = ch->spl[PURPLE_MAGIC];
    af.location  = APPLY_HITROLL;
    af.modifier  = -250;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char("You roar loudly at your victim!\n\r",ch);
    act("$n roars loudly at you shattering your eardrums!",ch,NULL,victim,TO_VICT);

    WAIT_STATE(ch,8);
}

void do_savageshred( CHAR_DATA *ch, char *argument )
{
    char buf[MSL];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int limb;
    int dam;

    if (IS_NPC(ch)) return;
    if( !IS_CLASS(ch, CLASS_KAHN) )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (ch->pcdata->classthree[KAHN_FORMS] < 5)
    {
        send_to_char("Your forms are not mastered enough.\n\r",ch);
        return;
    }
    if ( ch->fighting == NULL )
    {
        send_to_char( "That wont work unless you are fighting!\n\r", ch );
        return;
    }
    victim = ch->fighting;
    dam = number_range(4000,6000);
    sprintf( buf, "You leap upon $N and savagly shred them to tiny ribbons.#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n's leaps upon you and #Rshreds#n you to ribbons.#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's leaps on $N and shreds them into tiny ribbons.!", ch, NULL, victim, TO_NOTVICT );

    victim->hit -= dam;
    limb = number_range(1,4);
    if (number_percent() <= 25)
    {
        if (limb == 1)
        {
            if (!IS_ARM_R(victim,LOST_ARM))
                SET_BIT(victim->loc_hp[3],LOST_ARM);
            if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
                SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
            if (IS_BLEEDING(victim,BLEEDING_HAND_R))
                REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
            if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
                take_item(victim,obj);
        }
        else if (limb > 1)
        {
            if (!IS_ARM_L(victim,LOST_ARM))
                SET_BIT(victim->loc_hp[2],LOST_ARM);
            if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
                SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
            if (IS_BLEEDING(victim,BLEEDING_HAND_L))
                REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
            if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
                take_item(victim,obj);
            if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
                take_item(victim,obj);
        }
    }
    WAIT_STATE(ch,16);
    return;
}

void do_kahns( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS( ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    send_to_char("The following Kahn are stalking about.\n\r",ch);
    send_to_char("#G[    #gName    #G] [ #gHits #G] [ #gMana #G] [ #gMove #G] [#gExp#G]\n\r",ch);
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
        if ( IS_NPC(gch) ) continue;
        if ( !IS_CLASS(gch, CLASS_KAHN))
            continue;
        if ( !str_cmp(ch->clan,gch->clan) )
        {
            sprintf( buf,
                "#8[#3%-12s#8] [#3%-6d#8] [#3%-6d#8] [#3%-6d#8] [#3%7d#8]\n\r",
                capitalize( gch->name ),
                gch->hit,gch->mana,gch->move,
                gch->exp);
            send_to_char( buf, ch );
        }
    }
    return;
}

void do_stealth(CHAR_DATA *ch, char *argument)
{

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        stc("Huh?",ch);
        return;
    }

    if (ch->pcdata->classthree[KAHN_TIGER] < 5)
    {
        stc("You are not a powerfull enough tiger to sneak up on your victims.\n\r",ch);
        return;
    }

    if ( IS_SET(ch->act, PLR_SUPERINVIS) )
    {
        REMOVE_BIT(ch->act, PLR_SUPERINVIS);
        send_to_char("You slowly apear from the bushes.\n\r",ch);
        act("$n slowly apears from the bushes.\n\r",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if (ch->fight_timer >0)
    {
        send_to_char("Not until your fight timer expires.\n\r", ch );
        return;
    }

    stc("You slowly fade into the undergrowth as you start your hunt for prey.\n\r",ch);
    act("$n slowly disapears into the undergrowth.\n\r",ch,NULL,NULL, TO_ROOM);
    SET_BIT(ch->act, PLR_SUPERINVIS);
    return;
}

void do_ambush(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int     dam;

    argument = one_argument(argument, arg);

    if (IS_NPC(ch))

        if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh?",ch);
        return;
    }

    if ( !IS_SET(ch->act, PLR_SUPERINVIS) )
    {
        send_to_char("You cant ambush someone from clear view.",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Ambush who?\n\r",ch);
        return;
    }

    if ((victim = get_char_room( ch, arg )) == NULL)
    {
        send_to_char("They are not here.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_CLASS(victim, CLASS_KAHN))
    {
        send_to_char("You cant ambush another Kahn.... your victim smelt you a mile away.",ch);
        act("#G$n just tryed to ambush you",ch,NULL,victim, TO_VICT);
        return;
    }

    act("You leap out of the bushes and pounce on $N.\n\r",ch,NULL,victim, TO_CHAR);
    act("$n suddenly leaps out of the bushes at you.\n\r",ch,NULL,victim, TO_VICT);
    act("$n suddenly bursts from the bushes, landing right on $N.\n\r",ch,NULL,victim, TO_NOTVICT);
    REMOVE_BIT(ch->act, PLR_SUPERINVIS);

    dam *= 1.5;

    one_hit(ch, victim, gsn_clawslash, 0);
    one_hit(ch, victim, gsn_clawslash, 0);
    one_hit(ch, victim, gsn_clawslash, 0);

    set_fighting(ch,victim);
    WAIT_STATE(ch, 20);
    return;
}

void do_kahnfangs(CHAR_DATA *ch, char *argument)
{
    if (IS_CLASS(ch, CLASS_KAHN))
    {
        if (ch->rage >0)
        {
            send_to_char("You cannot retract your fangs while the beast is within you.\n\r", ch);
            return;
        }
    }
    if (IS_CLASS(ch, CLASS_KAHN))
    {
        if (ch->pcdata->classthree[KAHN_FORMS] < 2)
        {
            send_to_char("You must learn your forms to level 2.\n\r",ch);
            return;
        }
    }

    else if (!IS_CLASS(ch, CLASS_KAHN))
    {
        stc("Huh?\n\r",ch);
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

void do_gaiabless (CHAR_DATA *ch, char *argument)
{
    char buf1[MAX_STRING_LENGTH];
    AFFECT_DATA af;
    int i=0, dam=0, count, sn=0;
    int red_magic = ch->spl[RED_MAGIC];
    int blue_magic = ch->spl[BLUE_MAGIC];
    int green_magic = ch->spl[GREEN_MAGIC];
    int purple_magic = ch->spl[PURPLE_MAGIC];
    int yellow_magic = ch->spl[YELLOW_MAGIC];
    int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY))
    {
        magic_power +=100;
        red_magic +=20;
        yellow_magic +=20;
        purple_magic +=20;
        green_magic +=20;
        blue_magic +=20;
    }

    if (ch->mana <2500)
    {
        send_to_char("You need 2500 mana.\n\r",ch);
        return;
    }
    if (ch->position == POS_DEAD)
        return;
    count = 0;
    for (i = 0; i<5 ; i++)
    {
        if (i==0) {sn = skill_lookup("purple sorcery");dam=purple_magic;}
        if (i==1) {sn = skill_lookup("yellow sorcery");dam=yellow_magic;}
        if (i==2) {sn = skill_lookup("green sorcery");dam=green_magic;}
        if (i==3) {sn = skill_lookup("red sorcery");dam=red_magic;}
        if (i==4) {sn = skill_lookup("blue sorcery");dam=blue_magic;}
        if (is_affected(ch, sn))
            continue;

        count++;

        af.type      = sn;
        af.duration  = dam/4;
        af.location  = APPLY_HIT;
        af.modifier  = dam*4;
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
        af.modifier  = dam*4;
        af.bitvector = 0;
        affect_to_char(ch, &af);
        af.type      = sn;
        af.duration  = dam/4;
        af.location  = APPLY_MANA;
        af.modifier  = dam*4;
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
    }
    ch->mana -= count * 500;
    WAIT_STATE(ch,6);
    sprintf(buf1, "You are blessed with the power of gaia.");
    act(buf1, ch, NULL, ch, TO_CHAR);
    return;

}

void do_kahnclaws(CHAR_DATA *ch, char *argument)
{

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN) || ch->pcdata->classthree[KAHN_TIGER] < 2)
    {
        send_to_char("Huh?\n\r", ch );
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

void do_kahnawe    ( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    if (ch->pcdata->classthree[KAHN_TIGER] < 3)
    {
        send_to_char("You must master tiger to level 3 first.\n\r", ch);
        return;
    }
    if (IS_EXTRA(ch, EXTRA_AWE))
    {
        REMOVE_BIT(ch->extra, EXTRA_AWE);
        send_to_char("You are no longer one of the big cats.\n\r", ch);
        act("$n is no longer one of the big cats.", ch, NULL, NULL, TO_ROOM);
        return;
    }
    SET_BIT(ch->extra, EXTRA_AWE);
    send_to_char("You are now a 'Big Cat'.\n\r", ch);
    act("$n is now a 'Big Cat'", ch, NULL, NULL, TO_ROOM);
    return;
}

void do_kahneq( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
        send_to_char("Claw, Ring, Collar, Bracer, Plate,\n\r", ch);
        send_to_char("Helmet, Leggings, Boots, Gauntlets,\n\r", ch );
        send_to_char("Sleeves, Cloak, Belt, Mask.\n\r", ch);
        return;
    }
    else if (ch->pcdata->upgrade_level >= 1)
    {
        if      (!str_cmp(arg,"claw"                )) vnum = 110000;
        else if (!str_cmp(arg,"ring"                )) vnum = 110001;
        else if (!str_cmp(arg,"collar"              )) vnum = 110002;
        else if (!str_cmp(arg,"bracer"              )) vnum = 110003;
        else if (!str_cmp(arg,"plate"               )) vnum = 110004;
        else if (!str_cmp(arg,"helmet"              )) vnum = 110005;
        else if (!str_cmp(arg,"leggings"            )) vnum = 110006;
        else if (!str_cmp(arg,"boots"               )) vnum = 110007;
        else if (!str_cmp(arg,"gauntlets"           )) vnum = 110008;
        else if (!str_cmp(arg,"sleeves"             )) vnum = 110009;
        else if (!str_cmp(arg,"cloak"               )) vnum = 110010;
        else if (!str_cmp(arg,"belt"                )) vnum = 110011;
        else if (!str_cmp(arg,"mask"                )) vnum = 110012;
        else
        {
            send_to_char("That is an invalid type.\n\r", ch );
            return;
        }
    }
    else
    {
        if      (!str_cmp(arg,"claw"                )) vnum = 33292;
        else if (!str_cmp(arg,"ring"                )) vnum = 33280;
        else if (!str_cmp(arg,"collar"              )) vnum = 33282;
        else if (!str_cmp(arg,"bracer"              )) vnum = 33281;
        else if (!str_cmp(arg,"plate"               )) vnum = 33283;
        else if (!str_cmp(arg,"helmet"              )) vnum = 33284;
        else if (!str_cmp(arg,"leggings"    )) vnum = 33290;
        else if (!str_cmp(arg,"boots"               )) vnum = 33289;
        else if (!str_cmp(arg,"gauntlets"   )) vnum = 33287;
        else if (!str_cmp(arg,"sleeves"             )) vnum = 33288;
        else if (!str_cmp(arg,"cloak"               )) vnum = 33291;
        else if (!str_cmp(arg,"belt"                )) vnum = 33285;
        else if (!str_cmp(arg,"mask"                )) vnum = 33286;
        else
        {
            send_to_char("That is an invalid type.\n\r", ch );
            return;
        }
    }

    if ( ch->practice < 60)
    {
        send_to_char("it costs 60 points of primal to create a piece of kahn armour.\n\r",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Joshua.\n\r",ch);
        return;
    }

    ch->practice -= 60;
    obj = create_object(pObjIndex, 50);
    // obj->points=0;
    obj->questowner = str_dup(ch->pcdata->switchname);
    /*joshval=0;
    for(test=0;joshval<(fakeprimal-(5000*joshval));fakeprimal-=(joshval*5000))
    {joshval+=1;};
    oset_affect(ch,obj,joshval,APPLY_DAMROLL,TRUE);
    ch->pcdata->quest += obj->points;
    obj->points = 0;*/

    obj_to_char(obj, ch);
    act("$p appears in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands.",ch,obj,NULL,TO_ROOM);
    /*if(joshval==0)
    return;
    else if(ch->practice < joshval*750)
    ch->practice -= joshval*750;
    else
    ch->practice=0;*/
    return;
}
