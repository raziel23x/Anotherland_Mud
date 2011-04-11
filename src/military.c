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

void do_muutspecial(CHAR_DATA *ch, char *argument)
{
    ch->pcdata->Bootcamp[0]=0;
    ch->pcdata->Bootcamp[1]=0;
    ch->pcdata->Bootcamp[2]=0;
    ch->pcdata->Bootcamp[3]=0;
    ch->pcdata->Bootcamp[4]=0;
    ch->pcdata->Bootcamp[5]=50;
    ch->pcdata->store+=10;
}

void do_bootcamp(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int choice;
    char arg[MIL];
    argument = one_argument(argument, arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_MILITARY))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Which path you want study Weapons Speed Stealth Sniper Endurance or View\n\r",ch);
        return;
    }
    if(!str_cmp(arg, "weapons"))
        choice=0;
    else if(!str_cmp(arg, "speed"))
        choice=1;
    else if(!str_cmp(arg, "stealth"))
        choice=2;
    else if(!str_cmp(arg, "sniper"))
        choice=3;
    else if(!str_cmp(arg, "endurance"))
        choice=4;

    else if(!str_cmp(arg, "view"))
    {
        sprintf(buf,"You are level %d training in Weapons.\n\r", ch->pcdata->Bootcamp[0]);
        send_to_char(buf, ch);
        sprintf(buf,"You are level %d training in Speed.\n\r", ch->pcdata->Bootcamp[1]);
        send_to_char(buf, ch);
        sprintf(buf,"You are level %d training in Stealth.\n\r", ch->pcdata->Bootcamp[2]);
        send_to_char(buf, ch);
        sprintf(buf,"You are level %d training in Sniper.\n\r", ch->pcdata->Bootcamp[3]);
        send_to_char(buf, ch);
        sprintf(buf,"You are level %d training in Endurance.\n\r", ch->pcdata->Bootcamp[4]);
        send_to_char(buf, ch);

        if(ch->pcdata->Bootcamp[5]>0)
        {
            sprintf(buf,"You can train %d more levels.\n\r", ch->pcdata->Bootcamp[5]);
            send_to_char(buf, ch);
        }
        return;
    }

    else
    {
        send_to_char("What is your malfunction soldier? That is not trainable.\n\r",ch);
        return;
    }

    if(ch->pcdata->Bootcamp[choice] >= 10)
    {
        send_to_char("You have either maxxed out this ability or are no longer able to train.\n\r",ch);
        return;
    }
    if(ch->pcdata->store < ( ( 50 - ch->pcdata->Bootcamp[5] ) ) )
    {
        send_to_char("You dont have enough stores to train this any further at the moment.\n\r",ch);
        sprintf(buf,"You need [%d] more stores.\n\r", (ch->pcdata->Bootcamp[choice] - ch->pcdata->store));
        send_to_char(buf, ch);
        return;
    }
    //ch->pcdata->store-=((28 - ch->pcdata->Bootcamp[5])); // <---- ORIGINAL TRAIN COSTS
    ch->pcdata->store -= ((ch->pcdata->Bootcamp[choice]));
    ch->pcdata->Bootcamp[choice] += 1;
    ch->pcdata->Bootcamp[5] -= 1;
    send_to_char("Training Completed Soldier.\n\r", ch);
    return;
}

void do_march( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    CHAR_DATA *victim;

    one_argument (argument, arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_MILITARY))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
        send_to_char("March to whom?\n\r", ch );
        return;
    }
    if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "You can't find it's room.\n\r",ch);
        return;
    }
    if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
    {
        send_to_char("I don't think they want you to do that.\n\r",ch);
        return;
    }
    if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
    {
        stc( "Your room is not connected to the astral plane.\n\r",ch);
        return;
    }
    if (IS_SET(ch->in_room->room_flags, ROOM_ARENA) || IS_SET(victim->in_room->room_flags, ROOM_ARENA) )
    {
        stc( "You are in the arena you dumb ass.\n\r",ch);
        return;
    }
    location = victim->in_room;
    if (ch->move < 500)
    {
        send_to_char("You don't have the move to march that far.\n\r", ch );
        return;
    }
    act("You march into the wall.", ch, NULL, NULL, TO_CHAR);
    act("$n marches into the wall.", ch, NULL, NULL, TO_ROOM);
    ch->move -= 500;
    char_from_room(ch);
    char_to_room(ch, location);
    do_look(ch, "auto");
    act("You march onto $N.", ch, NULL, victim, TO_CHAR);
    act("$n marches through the wall.", ch, NULL, NULL, TO_ROOM);
    return;
}

void do_nightvision( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_MILITARY))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if
        (ch->pcdata->Bootcamp[0]+ch->pcdata->Bootcamp[2]+ch->pcdata->Bootcamp[3]+ch->pcdata->Bootcamp[4]+ch->pcdata->Bootcamp[1]<2)
    {
        send_to_char("You need more practice in your class before being able to use this Soldier.\n\r",ch);
        return;
    }
    if (IS_SET(ch->act, PLR_HOLYLIGHT))
    {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char("You take your Night Vision Goggles off.\n\r", ch);
    }
    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "You put on your Night Vision Goggles.\n\r", ch );
    }
    return;
}

void do_stimpack( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    int dam;
    int dammod=number_range(40,80);
    int dammod2=number_range(40,80);
    int dammod3=number_range(40,80);
    int dm=ch->pcdata->Bootcamp[4];
    argument = one_argument( argument, arg );
    if (!IS_CLASS(ch, CLASS_MILITARY))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
        if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ((victim = ch->fighting) == NULL)
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    dammod2+=ch->pcdata->Bootcamp[2];
    dammod3+=ch->pcdata->Bootcamp[1];
    if(dm>=2)dammod=2;
    if(dm>=5)dammod=3;
    if(dm>=8)dammod=4;
    if(dm>=10)dammod=5;
    if(dm<=3)dammod=1;
    dam=dammod*dammod2*dammod3;
    sprintf( buf, "You use a stimpack and go berserk on $N#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$n uses a stimpack and goes berserk on you#r[#l**#R%d#l**#r]#n",dam );
    act( buf, ch, NULL, victim, TO_VICT );
    act( "$n's soars down from the clouds onto $N!", ch, NULL, victim, TO_NOTVICT );

    hurt_person(ch, victim, dam);

    //    act("You use a #rstimpack#n and go berserk on $N.",ch,NULL,victim,TO_CHAR);
    //  act("$n uses a #rstimpack#n and goes berserk on you.",ch,NULL,victim,TO_VICT);
    WAIT_STATE(ch, 18);

}

void do_medkit( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int vnum = 110999;

    if(!IS_CLASS(ch,CLASS_MILITARY))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Joshua.\n\r",ch);
        return;
    }
    if(ch->pcdata->Bootcamp[5]>0)
    {
        send_to_char("You must train all your levels first.\n\r",ch);
        return;
    }
    if(ch->pcdata->Bootcamp[5]<(-1-ch->pcdata->Bootcamp[4]))
    {
        send_to_char("Your Endurance is too low to carry more medkits.\n\r",ch);
        return;
    }

    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("You pull a $p out of your backpack.",ch,obj,NULL,TO_CHAR);
    act("$n gets a $p from a #nmilitary #0backpack#n.",ch,obj,NULL,TO_ROOM);
    ch->pcdata->Bootcamp[5]-=1;
    WAIT_STATE(ch, 4);
    return;

}

void do_steroids (CHAR_DATA *ch, char *argument)
{
    /*char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];*/
    //CHAR_DATA *victim;
    AFFECT_DATA af;
    int sn;
    if(!IS_CLASS(ch,CLASS_MILITARY))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    send_to_char("You pop a couple pills and watch your strength grow.\n\r",ch);
    sn = skill_lookup("green sorcery");
    if (is_affected(ch, sn))
        return;

    af.type      = sn;
    af.duration  = (ch->pcdata->Bootcamp[4])*100;
    af.location  = APPLY_HITROLL;
    af.modifier  = (ch->pcdata->Bootcamp[4])*125;
    af.bitvector = 0;
    affect_to_char(ch, &af);
    af.type      = sn;
    af.duration  = (ch->pcdata->Bootcamp[4])*100;
    af.location  = APPLY_DAMROLL;
    af.modifier  = (ch->pcdata->Bootcamp[4])*125;
    af.bitvector = 0;
    affect_to_char(ch, &af);

}

void do_militaryeq (CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_MILITARY))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char ("Please specify which piece of military gear you want:Weapons,Ring,Bracer,Dogtags,Vest,Helmet,Leggings,Boots,Gloves,Sunglasses,Sleeves,Jacket,Belt.\n\r",ch);
        return;
    }
    if (ch->pcdata->upgrade_level >= 1)
    {
        if      (!str_cmp(arg,"weapons"             )) vnum = 110282;
        else if (!str_cmp(arg,"ring"                )) vnum = 110990;
        else if (!str_cmp(arg,"dogtags"             )) vnum = 110992;
        else if (!str_cmp(arg,"bracer"              )) vnum = 110991;
        else if (!str_cmp(arg,"vest"                )) vnum = 110993;
        else if (!str_cmp(arg,"helmet"              )) vnum = 110994;
        else if (!str_cmp(arg,"leggings"            )) vnum = 110995;
        else if (!str_cmp(arg,"boots"               )) vnum = 110996;
        else if (!str_cmp(arg,"gloves"              )) vnum = 110997;
        else if (!str_cmp(arg,"sleeves"             )) vnum = 110278;
        else if (!str_cmp(arg,"jacket"              )) vnum = 110279;
        else if (!str_cmp(arg,"belt"                )) vnum = 110280;
        else if (!str_cmp(arg,"sunglasses"          )) vnum = 110998;
        else
        {
            do_militaryeq(ch,"");
            return;
        }
    }

    else
    {
        if      (!str_cmp(arg,"ring"      ))   vnum = 110976;
        else if (!str_cmp(arg,"dogtags"   ))   vnum = 110978;
        else if (!str_cmp(arg,"vest"      ))   vnum = 110979;
        else if (!str_cmp(arg,"helmet"    ))   vnum = 110980;
        else if (!str_cmp(arg,"leggings"  ))   vnum = 110981;
        else if (!str_cmp(arg,"boots"     ))   vnum = 110982;
        else if (!str_cmp(arg,"gloves"    ))   vnum = 110983;
        else if (!str_cmp(arg,"sleeves"   ))   vnum = 110985;
        else if (!str_cmp(arg,"jacket"    ))   vnum = 110986;
        else if (!str_cmp(arg,"belt"      ))   vnum = 110987;
        else if (!str_cmp(arg,"bracer"    ))   vnum = 110977;
        else if (!str_cmp(arg,"sunglasses"))   vnum = 110984;
        else if (!str_cmp(arg,"weapons   "))   vnum = 110975;
        else
        {
            do_militaryeq(ch,"");
            return;
        }
    }
    if (ch->practice < 60)
    {
        send_to_char("It costs 60 points of primal to create a piece of miltary gear.\n\r",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Jobo.\n\r",ch);
        return;
    }
    ch->practice -= 60;
    obj = create_object(pObjIndex, 50);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj_to_char(obj, ch);
    act("$p appears in your hands in a blast of flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of flames.",ch,obj,NULL,TO_ROOM);
    return;

}
