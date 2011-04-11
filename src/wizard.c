/****************************************
 * Original Class designed by Chrono    *
 * for Another Land. Any use without    *
 * consent from Chrono aka Greg Vines   *
 * is strictly forbidden                *
 * Copyrighted (C) 2003, Greg Vines     *
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
#include "wizard.h"

void do_arts( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
    }
    if (arg1[0] == '\0' && arg2[0] == '\0')
    {
        stc("#P+#R|#P+A#pr#yt#os#P+#R|#P+#n\n\r",ch);
        stc("#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#n\n\r",ch);
        sprintf(buf,"Skills #R[#y%2d#R]#n\n\r",ch->pcdata->powers[WL_SKILLS]);
        stc(buf,ch);
        stc("#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#n\n\r",ch);
        sprintf(buf,"Spells #R[#y%2d#R]#n\n\r",ch->pcdata->stats[WL_SPELLS]);
        stc(buf,ch);
        stc("#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#n\n\r",ch);
        return;
    }
    if (arg2[0] == '\0')
    {
        if (!str_cmp(arg1,"skills"))
        {
            sprintf(buf,"#P00#G=============================#P00\n\r");
            stc(buf, ch);
            sprintf(buf," #C(           #ySkills#C            )#n\n\r");
            stc(buf, ch);
            sprintf(buf," #C(#G=============================#C)\n\r");
            stc(buf, ch);
            send_to_char("#RSkills - #CThe physical skills necessary for the Wizard's power.\n\r",ch);
            if (ch->pcdata->powers[WL_SKILLS] < 1)
                send_to_char("#CYou are not skilled in the Wizard Arts.\n\r",ch);
            if (ch->pcdata->powers[WL_SKILLS] > 0)
                send_to_char("#RWizardsight - #CThe ability to see into the nether realm#n.\n\r",ch);
            if (ch->pcdata->powers[WL_SKILLS] > 0)
                send_to_char("#REntropy     - #CThe ability to transfer yourself to others#n.\n\r",ch);
            if (ch->pcdata->powers[WL_SKILLS] > 1)
                send_to_char("#RMagicsight - #CThe power to see all.#n\n\r",ch);
            send_to_char("#RResurrect   - #CThe ability to resurrect the dead#n.\n\r",ch);
            if (ch->pcdata->powers[WL_SKILLS] > 2)
                send_to_char("#RShadowshift - #CThe ability to merge into the Shadow Realm.#n\n\r",ch);
            if (ch->pcdata->powers[WL_SKILLS] > 3)
                send_to_char("#REnhanced Mnemonics - #CGreatly improved Wizard spellcasting#n\n\r",ch);
            return;
        }
        else if (!str_cmp(arg1,"spells"))
        {
            sprintf(buf,"#P00#G=============================#P00\n\r");
            stc(buf, ch);
            sprintf(buf," #C(           #ySpells#C            )#n\n\r");
            stc(buf, ch);
            sprintf(buf," #C(#G=============================#C)\n\r");
            stc(buf, ch);
            if (ch->pcdata->stats[WL_SPELLS] < 1)
            {
                sprintf(buf," #C( #RFirebolt             #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #RFirebolt             #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 2)
            {
                sprintf(buf," #C( #0N#negativity           #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #0N#negativity           #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 3)
            {
                sprintf(buf," #C( #yT#ohunderclap          #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #yT#ohunderclap          #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 4)
            {
                sprintf(buf," #C( #LI#lc#ce#cl#la#Lnce             #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #LI#lc#ce#cl#la#Lnce             #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 5)
            {
                sprintf(buf," #C( #0Phase Shift          #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #0Phase Shift          #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 6)
            {
                sprintf(buf," #C( #GC#ghaos #GSphere         #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #GC#ghaos #GSphere         #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 7)
            {
                sprintf(buf," #C( #RHell#LBlades           #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #RHell#LBlades           #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 8)
            {
                sprintf(buf," #C( #0Vortex               #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #0Vortex               #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            if (ch->pcdata->stats[WL_SPELLS] < 9)
            {
                sprintf(buf," #C( #PW#prath #Po#pf #Pt#phe #RD#ramned  #P[ ]    #C)\n\r");
                stc(buf, ch);
            }
            else
            {
                sprintf(buf," #C( #PW#prath #Po#pf #Pt#phe #RD#ramned  #P[#RX#P]    #C)\n\r");
                stc(buf, ch);
            }
            sprintf(buf,"#P00#G=============================#P00\n\r");
            stc(buf, ch);
            return;
        }
        stc("#P+#R|#P+A#pr#yt#os#P+#R|#P+#n\n\r",ch);
        stc("#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#n\n\r",ch);
        sprintf(buf,"Skills #R[#y%2d#R]#n\n\r",ch->pcdata->powers[WL_SKILLS]);
        stc(buf,ch);
        stc("#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#n\n\r",ch);
        sprintf(buf,"Spells #R[#y%2d#R]#n\n\r",ch->pcdata->stats[WL_SPELLS]);
        stc(buf,ch);
        stc("#g*#G.#g*#G.#g*#G.#g*#G.#g*#G.#g*#n\n\r",ch);
        return;
    }
    else if (!str_cmp(arg2,"gain") && !str_cmp(arg1,"skills"))
    {
        int cost;
        int max = 4;

        cost = (ch->pcdata->powers[WL_SKILLS]+1) * 100;
        if ( ch->pcdata->powers[WL_SKILLS] >= max )
        {
            sprintf(buf,"#CYou have already gained all of the #PWizard#C skills#n\n\r");
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->practice)
        {
            sprintf(buf,"#CYou need #R%d #Cprimal to gain another skill#n\n\r", cost);
            send_to_char(buf,ch);
            return;
        }
        ch->pcdata->powers[WL_SKILLS]++;
        ch->practice -= cost;
        sprintf(buf,"#CYour wizard skills increase!#n\n\r");
        send_to_char(buf,ch);
    }

    else if (!str_cmp(arg2,"gain") && !str_cmp(arg1,"spells"))
    {
        int cost;
        int max = 9;

        cost = (ch->pcdata->stats[WL_SPELLS]+1) * 100;
        if ( ch->pcdata->stats[WL_SPELLS] >= max )
        {
            sprintf(buf,"#CYou know the all #PWizard#C spells!#n\n\r");
            send_to_char(buf,ch);
            return;
        }
        if ( cost > ch->practice)
        {
            sprintf(buf,"#CYou need #R%d #Cprimal to learn a new spell#n\n\r", cost);
            send_to_char(buf,ch);
            return;
        }
        ch->pcdata->stats[WL_SPELLS]++;
        ch->practice -= cost;
        sprintf(buf,"#CYou gain a new spell!#n\n\r");
        send_to_char(buf,ch);
    }
    else send_to_char("#CTo learn powers, type #carts <skills/spells> gain.#n\n\r",ch);
    return;
}

void do_wizardarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Please specify what kind of equipment you want to create.\n\r", ch );
        send_to_char("Staff, Rod, Ring, Pendant, Bracer, Plate,\n\r", ch);
        send_to_char("Helmet, Leggings, Boots, Gauntlets,\n\r", ch );
        send_to_char("Sleeves, Cloak, Pouch, Mask.\n\r", ch);
        return;
    }
    if (ch->pcdata->upgrade_level >=1)
    {
        if      (!str_cmp(arg,"rod"        )) vnum = 114988;
        else if (!str_cmp(arg,"staff"      )) vnum = 114989;
        else if (!str_cmp(arg,"pouch"       )) vnum = 114987;
        else if (!str_cmp(arg,"cloak"      )) vnum = 114990;
        else if (!str_cmp(arg,"leggings"   )) vnum = 114991;
        else if (!str_cmp(arg,"boots"      )) vnum = 114992;
        else if (!str_cmp(arg,"sleeves"    )) vnum = 114993;
        else if (!str_cmp(arg,"gloves"     )) vnum = 114994;
        else if (!str_cmp(arg,"mask"       )) vnum = 114995;
        else if (!str_cmp(arg,"helmet"     )) vnum = 114996;
        else if (!str_cmp(arg,"plate"      )) vnum = 114997;
        else if (!str_cmp(arg,"pendant"    )) vnum = 114998;
        else if (!str_cmp(arg,"bracer"     )) vnum = 114999;
        else if (!str_cmp(arg,"ring"       )) vnum = 115000;
        else
        {
            do_wizardarmor(ch,"");
            return;
        }
    }

    else
    {
        if      (!str_cmp(arg,"rod"                 )) vnum = 103113;
        else if (!str_cmp(arg,"ring"                )) vnum = 103100;
        else if (!str_cmp(arg,"staff"               )) vnum = 103112;
        else if (!str_cmp(arg,"pendant"             )) vnum = 103102;
        else if (!str_cmp(arg,"bracer"          )) vnum = 103101;
        else if (!str_cmp(arg,"plate"               )) vnum = 103103;
        else if (!str_cmp(arg,"helmet"              )) vnum = 103104;
        else if (!str_cmp(arg,"leggings"        )) vnum = 103110;
        else if (!str_cmp(arg,"boots"               )) vnum = 103109;
        else if (!str_cmp(arg,"gauntlets"       )) vnum = 103107;
        else if (!str_cmp(arg,"sleeves"     )) vnum = 103108;
        else if (!str_cmp(arg,"cloak"               )) vnum = 103111;
        else if (!str_cmp(arg,"pouch"               )) vnum = 103105;
        else if (!str_cmp(arg,"mask"                )) vnum = 103106;
        else
        {
            send_to_char("That is an invalid type.\n\r", ch );
            return;
        }
    }

    if ( ch->practice < 60)
    {
        send_to_char("It costs 60 points of primal to create a piece of wizard armour.\n\r",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Chrono.\n\r",ch);
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

void do_entropy (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    CHAR_DATA *victim;

    one_argument (argument, arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }
    if ((victim = get_char_world(ch, arg)) == NULL)
    {
        send_to_char("Teleport to whom?\n\r", ch );
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
    location = victim->in_room;
    if (ch->mana < 250)
    {
        send_to_char("You don't have the mana to cast teleport.\n\r", ch );
        return;
    }
    act("You utter a single powerword.", ch, NULL, NULL, TO_CHAR);
    act("$n utters a strange sounding word and disappers.", ch, NULL, NULL, TO_ROOM);
    ch->mana -= 250;
    char_from_room(ch);
    char_to_room(ch, location);
    do_look(ch, "auto");
    act("You materialize.", ch, NULL, NULL, TO_CHAR);
    act("$n suddenly appear from out of nowhere.", ch, NULL, NULL, TO_ROOM);
    return;
}

void do_resurrect(CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH];
    char buf [MAX_INPUT_LENGTH];
    char arg [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    OBJ_DATA *obj;
    one_argument(argument,arg);
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (arg[0] == '\0')
    {
        send_to_char("Resurrect what corpse?\n\r",ch);
        return;
    }
    if ( IS_CLASS(ch, CLASS_WIZARD) && ch->pcdata->powers[WL_SKILLS] < 1 )
    {
        stc("You need level 1 in Wizard Skills\n\r",ch);
        return;
    }
    if (ch->pcdata->followers > 5)
    {
        send_to_char("Nothing happens.\n\r",ch);
        return;
    }
    if ((obj = get_obj_carry(ch,arg)) == NULL)
    {
        send_to_char("You dont have that corpse.",ch);
        return;
    }
    if (obj->item_type != ITEM_CORPSE_NPC)
    {
        send_to_char("You can only Resurrect original corpses.\n\r",ch);
        return;
    }
    ch->pcdata->followers++;

    victim=create_mobile( get_mob_index( obj->value[2] ) );
    sprintf(buf,"Resurrection of %s",victim->short_descr);
    sprintf(buf2,"Resurrection of %s is here.\n\r",victim->short_descr);
    free_string(victim->short_descr);
    victim->short_descr = str_dup(buf);
    free_string(victim->name);
    SET_BIT(victim->act, ACT_NOEXP);
    if ( IS_SET(victim->act,ACT_QUEST) ) REMOVE_BIT(victim->act,ACT_QUEST);
    victim->name = str_dup(buf);
    free_string(victim->long_descr);
    victim->long_descr= str_dup(buf2);
    victim->spec_fun = NULL;
    sprintf(buf,"I invoke the rebirth of %s!",victim->name);
    do_say( ch, buf );
    sprintf(buf, "%s clambers back up to its feet.\n\r",obj->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
    send_to_char(buf,ch);
    char_to_room( victim, ch->in_room );

    WAIT_STATE(ch,10);
    extract_obj(obj);

    if(victim->level > ((ch->pcdata->upgrade_level + 1) * 500))
    {
        send_to_char("You are unable to control the resurrected creature.\n\r", ch);
        return;
    }

    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    sprintf(buf,"%s",victim->name);
    do_group(ch,buf);
    return;
}

void do_wizardsight( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( IS_CLASS(ch, CLASS_WIZARD) && ch->pcdata->powers[WL_SKILLS] < 1 )
    {
        stc("You need level 1 in Wizard Skills\n\r",ch);
        return;
    }
    if (IS_AFFECTED(ch,AFF_SHADOWSIGHT) )
    {
        send_to_char("You can no longer see between planes.\n\r",ch);
        REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
        return;
    }
    send_to_char("You can now see between planes.\n\r",ch);
    SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
    return;
}

void do_magicsight( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (IS_CLASS(ch, CLASS_WIZARD) && ch->pcdata->powers[WL_SKILLS] < 2)
    {
        stc("You can't do that yet.\n\r",ch);
        return;
    }
    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "#CYour senses return to normal.\n\r", ch );
    }
    else
    {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char( "#CYour #sm#sa#sg#si#sc#sa#sl #Csenses increase to incredible proportions.\n\r", ch );
    }

    return;
}

void do_shadowshift( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (IS_CLASS(ch, CLASS_WIZARD) && ch->pcdata->powers[WL_SKILLS] < 3)
    {
        send_to_char("You need Level 3 in Wizard Skills.\n\r",ch);
        return;
    }
    if ( arg[0] == '\0' )
    {
        if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
        {
            if (has_timer(ch)) return;
            send_to_char("You fade into the plane of shadows.\n\r",ch);
            act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
            SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
            do_look(ch,"auto");
            return;
        }
        REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
        send_to_char("You fade back into the real world.\n\r",ch);
        act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
        do_look(ch,"auto");
        return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
        send_to_char( "What do you wish to toss into the shadow plane?\n\r", ch );
        return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
        send_to_char( "You toss it to the ground and it vanishes.\n\r", ch );
    else
        send_to_char( "You toss it into a shadow and it vanishes.\n\r", ch );
    return;
}

void do_thunderclap( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam = 0;
    char buf[MAX_STRING_LENGTH];

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        stc("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->stats[WL_SPELLS] < 3)
    {
        stc("#CYou do not yet know this spell.\n\r",ch);
        return;
    }
    if (ch->mana < 5000)
    {
        stc("#CYou do not have the required #c5000#r mana.\n\r",ch);
        return;
    }

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

        if (vch->level >=4) continue;
        if (vch != ch)
            vch->position = POS_STUNNED;

        dam = number_range( vch->hit/56, vch->hit/38);

        if (ch->pcdata->powers[WL_SKILLS] > 3)

            if (vch != ch)
                vch->hit -= dam;

        if (vch != ch)
        {
            sprintf(buf, "#R%s closes his eyes and brings his hands together, releasing a massive shockwave! #R[#c%d#R]#n\n\r", ch->name, dam);
            stc(buf, vch);
        }

        if (vch != ch)
            stc("#RYou fall to the ground, stunned!\n\r",vch);

        if (vch != ch)
        {
            sprintf(buf, "#R%s falls to the ground, stunned!\n\r", vch->name);
            stc(buf, ch);
        }

        if (vch->hit < -10)
            vch->hit = -10;
    }

    sprintf(buf, "#RYou close your eyes and bring your hands together, releasing a massive shockwave! #R[#c%d#R]#n\n\r", dam);
    stc(buf, ch);
    ch->mana -= 5000;
    WAIT_STATE(ch, PULSE_VIOLENCE);
    return;
}

void do_hellblades( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int cost = 0;
    argument = one_argument( argument, arg1);

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        stc("#CUse #RHell#LBlades#C on who?\n\r",ch);
        return;
    }

    if ((victim = get_char_room(ch, arg1)) == NULL)
    {
        stc("#CThey aren't here.\n\r", ch);
        return;
    }

    if (victim == ch)
    {
        stc("#CYeah ok..... #sd#so#sr#sk#C....\n\r",ch);
        return;
    }

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        stc("#CHuh?\n\r",ch);
        return;

    }

    if (ch->pcdata->stats[WL_SPELLS] < 7)
    {
        stc("#RYou do not yet have mastery of this spell.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim)) return;

    if (ch->pcdata->powers[WL_SKILLS] < 4) cost = 20000;
    if (ch->pcdata->powers[WL_SKILLS] > 3) cost = 10000;

    if (ch->mana < 20000 && ch->pcdata->powers[WL_SKILLS] < 4)
    {
        stc("#CYou do not have the #c20000#C required mana.\r\n",ch);
        return;
    }
    else if (ch->mana < 10000 && ch->pcdata->powers[WL_SKILLS] > 3)
    {
        stc("#CYou do not have the #c10000#C required mana.\r\n",ch);
        return;
    }

    if (ch->pcdata->powers[WL_SKILLS] > 3 && number_range(1, 100) < 10)
    {
        if (IS_LEG_L(victim,LOST_LEG))
        {
            sprintf( buf, "#7%s#C has already lost their left leg.\n\r", victim->name);
            stc( buf, ch);
            return;
        }

        if (!IS_LEG_L(victim,LOST_LEG))
            SET_BIT(victim->loc_hp[4],LOST_LEG);
        else
        {
            sprintf( buf, "#7%s#C has already lost theit left leg.\n\r", victim->name);
            stc( buf, ch);
            return;
        }

        if (!IS_BLEEDING(victim,BLEEDING_LEG_L))
            SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
            REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
        act("#CYour hellblades spin around you, cutting off #7$N's#C left leg!",ch,NULL,victim,TO_CHAR);
        act("#7$n's#C hellblades spin around them, cutting off #7$N's#C left leg!",ch,NULL,victim,TO_NOTVICT);
        act("#7$n's#C hellblades spin around them, cutting off your left leg!",ch,NULL,victim,TO_VICT);
        make_part(victim,"leg");
        if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
        {
            if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
                take_item(victim,obj);
        }
        if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
            take_item(victim,obj);

        if (IS_LEG_R(victim,LOST_LEG))
        {
            sprintf( buf, "#7%s#C has already lost their right leg.\n\r", victim->name);
            stc( buf, ch);
            return;
        }

        if (!IS_LEG_R(victim,LOST_LEG))
            SET_BIT(victim->loc_hp[5],LOST_LEG);
        else
        {
            sprintf( buf, "#7%s#C has already lost their right leg.\n\r", victim->name);
            stc( buf, ch);
            return;
        }

        if (!IS_BLEEDING(victim,BLEEDING_LEG_R))
            SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
        if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
            REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
        act("#CYour hellblades spin around you, cutting off #7$N's#C right leg!",ch,NULL,victim,TO_CHAR);
        act("#7$n's#C hellblades spin around them, cutting off #7$N's#C right leg!",ch,NULL,victim,TO_NOTVICT);
        act("#7$n's#C hellblades spin around him, cutting off your right leg!",ch,NULL,victim,TO_VICT);
        make_part(victim,"leg");
        if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
        {
            if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
                take_item(victim,obj);
        }
        if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
            take_item(victim,obj);

    }

    /*                if (ch->pcdata->powers[WL_SKILLS] > 3 && number_range(1, 75) < 5 && !IS_NPC(victim))
                    {
                            behead( victim );
                            sprintf( buf, "#7%s#C is cut to pieces by #7%s's#C hellblades.\n\r", victim->name, ch->name);
                            do_info( ch, buf);
                            ch->pkill ++;
                            victim->pdeath -= 1;
                            ch->race ++;
                            return;
                    }*/
    WAIT_STATE(ch, 17);
    victim->hit *= .95;
    ch->mana -= cost;
    return;
}

void do_wrath( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int dam;
    argument = one_argument( argument, arg1 );

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0')
    {
        send_to_char( "#CRelease the Wrath of the Damned on who?\n\r", ch );
        return;
    }

    if ((victim = get_char_room(ch, arg1)) == NULL)
    {
        stc("#CThey aren't here.\n\r", ch);
        return;
    }

    if (ch->pcdata->stats[WL_SPELLS] < 9)
    {
        stc("#CYou do not yet have mastery of this spell.\n\r",ch);
        return;
    }

    if ((victim = get_char_world(ch, arg1)) == NULL)
    {
        sprintf(buf, "#7%s#C is not here.\n\r",arg1);
        stc( buf, ch);
        return;
    }

    if (victim->level < LEVEL_AVATAR)
    {
        stc("#CNot on mortals.\n\r",ch);
        return;
    }

    if(IS_IMMORTAL(victim))
    {
        send_to_char("Not on immortals.\n\r", ch);
        stcf(victim,"%s tried to wrath you!\n\r",ch->name);
        return;
    }

    if (ch->mana < 5000)
    {
        stc("#CYou do not have the 5000 required mana.\n\r",ch);
        return;
    }

    if ((victim = get_char_room(ch, arg1)) == NULL)
    {
        send_to_char("They are not here.\n\r", ch);
        return;
    }

    if (IS_BODY(victim, BROKEN_SPINE))
    {
        sprintf(buf, "#7%s#C already has a broken spine.\n\r",victim->name);
        stc( buf, ch);
        return;
    }

    act("#CYou close your eyes and chant dark words, calling forth the damned to release their wrath upon #7$N#C.", ch, NULL, victim, TO_CHAR);
    act("#7$n#C chants dark words, calling forth the damned to release their wrath upon #7$N#C.", ch, NULL, victim, TO_NOTVICT);
    act("#7$n#C chants dark words, calling forth the damned to release their wrath upon you.#n", ch, NULL, victim, TO_VICT);
    SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
    act("#7$N#C screams in agony as $S spine is snapped with a gruesome #C*#yPOP#C*#n.", ch, NULL, victim, TO_NOTVICT);
    act("#7$N#C screams in agony as $S spine is snapped with a gruesome #C*#yPOP#C*#n", ch, NULL, victim, TO_CHAR);
    stc("#CYou scream in agony as your spine is snapped with a gruesome #C*#yPOP#C*#n", victim);
    dam = (victim->hit) / 45;
    if (ch->pcdata->powers[WL_SKILLS] > 3) dam *= 2;
    hurt_person(ch, victim, dam);
    ch->mana -= 5000;

    if(!IS_NPC(victim))
    {
        ch->fight_timer += 5;
    }
    WAIT_STATE(ch, PULSE_VIOLENCE);

    return;
}

void do_firebolt( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int dam;

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch) )
        return;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0')
    {
        send_to_char( "#CCast #Rf#ri#Rr#re#Rb#ro#Rl#rt#n on whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "#RThey aren't here.#n\n\r", ch );
        return;
    }

    if (victim->level < LEVEL_AVATAR)
    {
        stc("Not on mortals.\n\r",ch);
        return;
    }

    if ((victim == ch))
    {
        stc("#RYeah ok...\n\r",ch);
        return;
    }
    if (ch->pcdata->stats[WL_SPELLS] < 1)
    {
        stc("#RYou do not yet have mastery of this spell.\n\r",ch);
        return;
    }

    if (ch->mana < 1000)
    {
        stc("#RYou do not have the required 1000 mana.\n\r",ch);
        return;
    }

    dam = number_range(2000, 3500);
    if (ch->pcdata->powers[WL_SKILLS] > 3)
    {
        dam += number_range( 3000, 4000 );
    }
    if (IS_NPC(victim)) dam *= 1.5;

    if (number_percent() < 50)
    {
        act("#y$n#R points $s hands towards #y$N#R, releasing a powerful bolt of flame.", ch, NULL, victim, TO_NOTVICT);
        sprintf( buf, "#RYou point your hands toward $N, releasing a powerful blast of flame#n #y[#C%d#y]#n\n\r", dam);
        act( buf, ch,NULL,victim,TO_CHAR);
        sprintf( buf, "#y$n#R points his hands towards you, releasing a powerful bolt of flame.#n   #y[#C%d#y]#n\n\r", dam);
        act( buf, ch,NULL,victim,TO_VICT);        if (!IS_SET(victim->affected_by, AFF_FLAMING))
        SET_BIT(victim->affected_by, AFF_FLAMING);

        hurt_person(ch, victim, dam);
        if (victim->hit < -10) victim->hit = -10;
    }

    if (number_percent() < 10)
    {
        act("#y$n#R points $s hands towards #y$N#R, releasing a powerful bolt of flame.", ch, NULL, victim, TO_NOTVICT);
        sprintf( buf, "#RYou point your hands toward $N, releasing a powerful blast of flame#n #y[#C%d#y]#n\n\r", dam);
        act( buf, ch,NULL,victim,TO_CHAR);
        sprintf( buf, "#y$n#R points his hands towards you, releasing a powerful bolt of flame.#n   #y[#C%d#y]#n\n\r", dam);
        act( buf, ch,NULL,victim,TO_VICT);
        if (!IS_SET(victim->affected_by, AFF_FLAMING))
            SET_BIT(victim->affected_by, AFF_FLAMING);

        hurt_person(ch, victim, dam);
        if (victim->hit < -10) victim->hit = -10;
    }

    else
    {
        act("#y$n#R points $s hands towards #y$N#R, releasing a powerful bolt of flame.", ch, NULL, victim, TO_NOTVICT);
        sprintf( buf, "#RYou point your hands toward $N, releasing a powerful blase of flame#n #y[#C%d#y]#n\n\r", dam);
        act( buf, ch,NULL,victim,TO_CHAR);
        sprintf( buf, "#y$n#R points his hands towards you, releasing a powerful bolt of flame.#n   #y[#C%d#y]#n\n\r", dam);
        act( buf, ch,NULL,victim,TO_VICT);        if (!IS_SET(victim->affected_by, AFF_FLAMING))
        SET_BIT(victim->affected_by, AFF_FLAMING);
        ch->mana -= 1000;
        hurt_person(ch, victim, dam);
        if (victim->hit < -10) victim->hit = -10;
    }

    WAIT_STATE(ch, PULSE_VIOLENCE);
    if (ch->fighting == NULL) ch->fighting = victim;
    return;
}

void do_pshift( CHAR_DATA *ch, char *argument )
{

    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    int door;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        stc("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->stats[WL_SPELLS] < 5)
    {
        stc("#RYou do not yet have mastery of this spell.#n\n\r",ch);
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
            ch->position = POS_STANDING;
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if(ch->mana < 750)
    {
        send_to_char("#RYou d not have the 750 required mana.\n\r",ch);
        return;
    }

    door = number_door( );
    if ( ( pexit = ch->in_room->exit[door] ) == NULL
        ||   (to_room = pexit->to_room) == NULL)
    {
        act( "#y$n#R mutters some words and disappears!", ch, NULL, NULL, TO_ROOM );
        send_to_char( "#RYou mutter some dark words and vanish from combat.\n\r", ch );
        char_from_room( ch );
        char_to_room( ch, get_room_index(ROOM_VNUM_ALTAR));
        stop_fighting( ch, TRUE );
        return;
    };

    act( "#y$n#R mutters some words and disappears!", ch, NULL, NULL, TO_ROOM );
    send_to_char( "#RYou mutter some dark words and vanish from combat.\n\r", ch );
    char_from_room(ch);
    char_to_room( ch, to_room );
    do_look(ch,"auto");
    stop_fighting( ch, TRUE );
    ch->mana -= 750;
    return;
}

void do_vortex(CHAR_DATA *ch, char *argument)
{
    ROOM_INDEX_DATA *pRoom;
    ROOM_INDEX_DATA *pDir;
    OBJ_DATA *pObj;
    CHAR_DATA *pChar;
    int i;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (ch->pcdata->stats[WL_SPELLS] < 8)
    {
        stc("#RYou do not yet have mastery over this spell.\n\r",ch);
        return;
    }

    if (ch->mana < 7500)
    {
        stc("#RYou do not have the required 7500 mana.\n\r",ch);
        return;
    }

    pRoom = ch->in_room;

    for ( i = 0; i < 5; i++ )
    {
        char buf[MAX_STRING_LENGTH];
        if ( !pRoom->exit[i] || !pRoom->exit[i]->to_room )
            continue;
        pDir = pRoom->exit[i]->to_room;
        if ( IS_SET(pDir->room_flags,ROOM_SAFE) )
            continue;
        for ( pObj = pDir->contents; pObj; pObj = pDir->contents )
        {
            if (!pObj) continue;
            if (pObj->in_room == NULL) continue;
            obj_from_room(pObj);
            obj_to_room(pObj,pRoom);
            if ( pDir->people )
            {
                sprintf(buf,"#RA vortex sucks $p to the %s.",dir_name[rev_dir[i]]);
                //              act(buf,pDir->people,pObj,NULL,TO_CHAR);
                act(buf,pDir->people,pObj,NULL,TO_ROOM);
            }
            sprintf(buf,"#RA vortex sucks $p from the %s.",dir_name[i]);
            act(buf,ch,pObj,NULL,TO_CHAR);
            act(buf,ch,pObj,NULL,TO_ROOM);
        }
        for ( pChar = pDir->people; pChar; pChar = pDir->people )
        {
            if (!pChar) continue;
            if (pChar->in_room == NULL) continue;
            if (pChar == ch) continue;
            sprintf(buf,"#RA vortex sucks $n to the %s.",dir_name[rev_dir[i]]);
            act(buf,pChar,NULL,NULL,TO_ROOM);
            sprintf(buf,"#RA vortex sucks you to the %s.",dir_name[rev_dir[i]]);
            send_to_char(buf,pChar);
            char_from_room(pChar);
            char_to_room(pChar,pRoom);
            sprintf(buf,"#RA vortex sucks $N from the %s.",dir_name[i]);
            act(buf,ch,NULL,pChar,TO_CHAR);
            act(buf,ch,NULL,pChar,TO_ROOM);
        }
    }
    ch->mana -= 7500;
    WAIT_STATE(ch, 4);
    return;
}

void do_icelance( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int dam;

    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch) )
        return;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0')
    {
        send_to_char( "#RUse icelance on who?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char( "#RThey aren't here.#n\n\r", ch );
        return;
    }

    if ((victim == ch))
    {
        stc("#RYeah ok...\n\r",ch);
        return;
    }

    if (ch->pcdata->stats[WL_SPELLS] < 4)
    {
        stc("#RYou do not yet have mastery of this spell.\n\r",ch);
        return;
    }

    if (ch->mana < 5000)
    {
        stc("#RYou do not have the required 5000 mana.\n\r",ch);
        return;
    }

    dam = number_range(1000, 2000);
    if (ch->pcdata->powers[WL_SKILLS] > 3)
    {
        dam += number_range( 1000, 2000 );
    }
    if (IS_NPC(victim))
    {
        dam *= 1.5;
    }
    act("#y$n#R points $s hands towards #y$N#R and sends forth a wicked spear of ice.", ch, NULL, victim, TO_NOTVICT);
    sprintf( buf, "#RYou point your hands toward %s and sends forth a wicked spear of ice#n #y[#C%d#y]#n\n\r", victim->name, dam);
    stc( buf, ch);
    sprintf( buf, "#y%s#R points his hands towards you and sends forth a wicked spear.#n   #y[#C%d#y]#n\n\r", ch->name, dam);
    stc( buf, victim);

    ch->mana -= 5000;
    victim->hit -= dam;
    if (!IS_BLEEDING(victim, BLEEDING_THROAT))
        SET_BIT(victim->loc_hp[6], BLEEDING_THROAT);
    if (victim->hit < -10) victim->hit = -10;
    WAIT_STATE(ch, PULSE_VIOLENCE);
    if (ch->fighting == NULL) ch->fighting = victim;
    return;
}

void do_negativity( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int dam;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch) )
        return;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        stc("Huh?\n\r",ch);
        return;
    }

    if ( arg[0] == '\0')
    {
        send_to_char( "#RCast negativity on who?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "#RThey aren't here.#n\n\r", ch );
        return;
    }

    if ((victim == ch))
    {
        stc("#RYeah ok...\n\r",ch);
        return;
    }

    if (ch->pcdata->stats[WL_SPELLS] < 2)
    {
        stc("You do not yet have mastery of this spell.",ch);
        return;
    }

    dam = number_range(1750, 2500);
    if (ch->pcdata->powers[WL_SKILLS] > 3) dam *= 2;

    if (ch->mana < dam * 2)
    {
        stc("You do not have enough mana for this spell.",ch);
        return;
    }

    hurt_person(ch, victim, dam);
    ch->hit += dam;
    ch->mana -= dam;

    sprintf(buf, "#rYou place your hands on #R%s,#r sucking the life out of them. #B[#R%d#B]#n\n\r", victim->name, dam);
    stc(buf, ch);
    sprintf(buf, "#R%s#r places his hands on you, sucking out your life. #B[#R%d#B]#n\n\r", ch->name, dam);
    stc(buf, victim);
    WAIT_STATE(ch, 10);
    return;
}

void do_chaos( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int chaos, dam;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_WIZARD))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }
    if (ch->pcdata->stats[WL_SPELLS] < 6)
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( arg[0] == '\0')
    {
        send_to_char( "#RCast chaos orb on whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }
    if (is_safe(ch, victim))
    {
        stc("You cannot attack them.\n\r",ch);
        return;
    }

    if (ch->mana < 5000)
    {
        stc("You do not ave the required 5000 mana.\n\r",ch);
        return;
    }

    chaos = number_range(1, 6);
    act("#r$n#0 begins chanting dark words, summoning forth a sphere of #rc#lh#pa#go#cs#0 to hurl at #r$N!#n", ch, NULL, victim, TO_NOTVICT);
    sprintf( buf, "#0You summon forth a perfect sphere of #rc#lh#pa#go#cs#0 and hurl it at #r$N!#n\n\r");
    act(buf, ch, NULL, victim, TO_CHAR);
    sprintf( buf, "#r$n#0 summons forth a perfect sphere of #rc#lh#pa#go#cs#0 and hurls it at you!#n\n\r");
    act(buf, ch, NULL, victim, TO_VICT);

    if (chaos == 1 || chaos == 6)
    {
        stc("The sphere explodes in a shower of #Ggreen bubbles#n.\n\r",victim);
        stc("Your sphere explodes in a shower of #Ggreen bubbles#n.\n\r",ch);
        if ( IS_AFFECTED(victim, AFF_POISON)) return;
        af.type      = gsn_poison;
        af.duration  = chaos * 10;
        af.location  = APPLY_STR;
        af.modifier  = 0 - number_range(1,3);
        af.bitvector = AFF_POISON;
        affect_join( victim, &af );
        WAIT_STATE(victim, number_range(5, 20));
        send_to_char( "#GYou are poisoned!#n\n\r", victim );
    }
    else if (chaos == 2 || chaos == 7 )
    {
        stc("The sphere explodes, forming a #yyellow haze#n.\n\r",victim);
        stc("Your sphere explodes, forming a #yyellow haze#n.\n\r",ch);
        if ( IS_AFFECTED(victim, AFF_BLIND))
        {
            af.type      = gsn_blindness;
            af.location  = APPLY_HITROLL;
            af.modifier  = -4;
            af.duration  =  chaos * 10;
            af.bitvector = AFF_BLIND;
            affect_to_char( victim, &af );
            WAIT_STATE(victim, number_range(5, 15));
            send_to_char( "#yYou are blinded!#n\n\r", victim );
        }
        else
        {
            stc("They are already blinded.\n\r",ch);
        }
    }
    else if (chaos == 3 || chaos == 8)
    {
        stc("The sphere explodes in a shower of #Rred flames#n.\n\r",victim);
        stc("Your sphere explodes in a shower of #Rred flames#n.\n\r",ch);
        WAIT_STATE(victim, number_range(5, 20));
        SET_BIT(victim->affected_by, AFF_FLAMING);
    }
    else if (chaos == 4 || chaos == 9)
    {
        dam = number_range(3750, 7500);
        sprintf(buf,"The sphere explodes in a scattering of #Lblue sparks#n. #R[#y%d#R]#n\n\r", dam);
        stc( buf, victim);
        sprintf(buf,"Your sphere explodes in a scattering of #Lblue sparks#n.  #R[#y%d#R]#n\n\r", dam);
        stc( buf, ch);
        WAIT_STATE(victim, number_range(5, 8));
        hurt_person(ch, victim, dam);
    }
    else if (chaos == 5 )
    {
        stc("The sphere dissipates before it hits you#n.\n\r",victim);
        stc("Your sphere dissipates shortly after casting#n.\n\r",ch);
    }

    one_hit(ch,victim, gsn_wrathofgod,1);
    one_hit(ch,victim, gsn_wrathofgod,1);

    WAIT_STATE(ch, 4);
    ch->mana -= 5000;
    if (victim->hit < -10) victim->hit = -10;
    return;
}
