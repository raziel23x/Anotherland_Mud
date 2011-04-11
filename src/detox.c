#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* Needs Major Revision!!! -Gol*/
void do_detox( CHAR_DATA *ch, char *argument )
{
    char arg[100];
    argument = one_argument(argument, arg);
    if ( str_cmp(arg,"confirm") )
        return stcf(ch,"Detox will totally wipe your character.\n\rAll of your stances, spells, and weapons will be halved.\n\rYou will lose one upgrade level.\n\rYour stats will be reset.\n\rYou will be set as a mortal, and anything you have trained will be lost.\n\rIf you're sure you want to detox, go to the detox chamber, and type detox confirm\n\r");
    if ( IS_NPC(ch) )
        return;
    if (IS_CLASS(ch, CLASS_RPONLY))return;

    if (ch->class == 0)
    {
        send_to_char("You are already classless.\n\r", ch);
        return;
    }
    else if (ch->level < 3)
    {
        send_to_char("Train avatar before detox.\n\r", ch);
        return;
    }
    else if (ch->in_room->vnum != 3206)
    {
        send_to_char("You are not in the #cd#Cetox #gt#Gank#n.\n\r", ch);
        return;
    }
    /*else if (ch->pcdata->quest < 1000)
    {
        send_to_char("You cannot afford the #71000 #0qp#n required to detox.\n\r", ch);
        return;
    }*/
    else if ( IS_AFFECTED(ch, AFF_CHARM)   )
    {
        send_to_char( "Not whilst charmed!" ,ch);
        return;
    }
    /*else if (ch->spl[2] < 99)
    {
        send_to_char("You must have 100 or more in #bb#Blue #nmagic to do this.\n\r", ch);
        return;
    }*/
    else
    {
        do_remove(ch, "all");
        /* Vampires */
        if (IS_CLASS(ch, CLASS_VAMPIRE))
        {
            do_mortalvamp(ch,"");
            if (IS_POLYAFF(ch, POLY_ZULOFORM))
            {
                if (IS_EXTRA(ch, EXTRA_DRAGON))
                {
                    REMOVE_BIT(ch->extra,EXTRA_DRAGON);
                    ch->damroll = ch->damroll - 25;
                    ch->hitroll = ch->hitroll - 25;
                    free_string( ch->morph );
                    ch->morph = str_dup( "A big black monster" );
                }
                REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
                REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
                ch->damroll = ch->damroll - 50;
                ch->hitroll = ch->hitroll - 50;
            }
            if (IS_EXTRA(ch, EXTRA_POTENCY))
            {
                ch->generation =ch->generation +1;
                REMOVE_BIT(ch->extra, EXTRA_POTENCY);
            }
            REMOVE_BIT(ch->newbits, NEW_TIDE);
            if (IS_EXTRA(ch, EXTRA_BAAL))
            {
                ch->power[DISC_VAMP_POTE] -= 1;
                ch->power[DISC_VAMP_CELE] -= 1;
                ch->power[DISC_VAMP_FORT] -=1;
                REMOVE_BIT(ch->extra, EXTRA_BAAL);
            }
            if (IS_EXTRA(ch, EXTRA_FLASH))
            {
                ch->power[DISC_VAMP_CELE] -= 1;
                REMOVE_BIT(ch->extra, EXTRA_FLASH);
            }
            if (IS_VAMPAFF(ch,VAM_FANGS) )
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FANGS);
            if (IS_VAMPAFF(ch,VAM_DISGUISED) )
            {
                free_string( ch->morph );
                ch->morph = str_dup( ch->name );
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
            }
            if (IS_VAMPAFF(ch,VAM_CLAWS) )
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
            if (IS_VAMPAFF(ch,VAM_DISGUISED) )
            {
                free_string( ch->morph );
                ch->morph = str_dup( ch->name );
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_DISGUISED);
            }
            if (IS_SET(ch->newbits, NEW_MONKFLAME))
                REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
            if (IS_CLASS(ch, CLASS_WEREWOLF) && IS_VAMPAFF(ch,VAM_CLAWS) )
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CLAWS);
            if (IS_VAMPAFF(ch,VAM_NIGHTSIGHT) )
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
            if (IS_SET(ch->special, SPC_INCONNU))
                REMOVE_BIT(ch->special, SPC_INCONNU );
            if (IS_SET(ch->pcdata->stats[UNI_AFF], VAM_CHANGED))
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
            if (IS_POLYAFF(ch, POLY_MIST))
            {
                REMOVE_BIT(ch->polyaff, POLY_MIST);
                REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
            }
            if (IS_AFFECTED(ch, AFF_POLYMORPH))
            {
                if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_BAT))
                {
                    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_FLYING);
                    REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_SONIC);
                    REMOVE_BIT(ch->polyaff, POLY_BAT);
                }
                else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))
                {
                    REMOVE_BIT(ch->polyaff, POLY_WOLF);
                    ch->max_hit = ch->max_hit - 500;
                    ch->hit = ch->hit - 500;
                    if (ch->hit < 1)
                        ch->hit = 1;
                }
                else if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_MIST))
                {
                    REMOVE_BIT(ch->polyaff, POLY_MIST);
                    REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
                }
                REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
                REMOVE_BIT(ch->pcdata->stats[UNI_AFF], VAM_CHANGED);
                clear_stats(ch);
                free_string( ch->morph );
                ch->morph = str_dup( "" );
            }
        }
        /* Mage */
        else if (IS_CLASS(ch, CLASS_MAGE))
        {
            if (IS_CLASS(ch, CLASS_MAGE) && IS_AFFECTED(ch, AFF_POLYMORPH))
                do_unpolymorph(ch,"");
        }
        /* Drow */
        else if (IS_CLASS(ch, CLASS_DROW))
        {

            if (IS_SET(ch->special, SPC_DROW_WAR))
                REMOVE_BIT(ch->special, SPC_DROW_WAR );
            if (IS_SET(ch->special, SPC_DROW_MAG))
                REMOVE_BIT(ch->special, SPC_DROW_MAG );
            if (IS_SET(ch->special, SPC_DROW_CLE))
                REMOVE_BIT(ch->special, SPC_DROW_CLE );
            if (IS_SET(ch->affected_by, AFF_POLYMORPH))
                REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
            if (IS_SET(ch->newbits, NEW_DARKNESS))
            {
                REMOVE_BIT(ch->newbits, NEW_DARKNESS);
                if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
                    REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
            }
            if (IS_SET(ch->newbits, NEW_DROWHATE))
                REMOVE_BIT(ch->newbits, NEW_DROWHATE);
            ch->pcdata->powers[DROW_POWER]     = 0;
            ch->pcdata->stats[DROW_TOTAL]      = 0;
            ch->pcdata->stats[DROW_MAGIC]      = 0;
            ch->pcdata->powers[1]              = 0;
        }
        /* Monk */
        else if (IS_CLASS(ch, CLASS_MONK))
        {
            if (IS_SET(ch->newbits, NEW_MONKFLAME))
                REMOVE_BIT(ch->newbits, NEW_MONKFLAME);
            if (IS_SET(ch->newbits, NEW_MONKADAM))
                REMOVE_BIT(ch->newbits, NEW_MONKADAM);
        }
        /* Werewolf */
        else if (IS_CLASS(ch, CLASS_WEREWOLF))
        {
            if (IS_SET(ch->special, SPC_WOLFMAN))
                do_unwerewolf(ch,"");
            if (IS_SET(ch->newbits, NEW_SCLAWS))
                REMOVE_BIT(ch->newbits, NEW_SCLAWS);
            if (IS_SET(ch->newbits, NEW_SLAM))
                REMOVE_BIT(ch->newbits,NEW_SLAM);
            if (IS_SET(ch->newbits, NEW_QUILLS))
                REMOVE_BIT(ch->newbits,NEW_QUILLS);
            if (IS_SET(ch->newbits, NEW_PERCEPTION))
                REMOVE_BIT(ch->newbits,NEW_PERCEPTION);
            if (IS_SET(ch->newbits, NEW_JAWLOCK))
                REMOVE_BIT(ch->newbits,NEW_JAWLOCK);
            if (IS_SET(ch->newbits, NEW_REND))
                REMOVE_BIT(ch->newbits,NEW_REND);
        }
        /* Demons */
        else if (IS_CLASS(ch, CLASS_DEMON))
        {
            if (IS_DEMAFF(ch,DEM_HORNS))
                REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
            if (IS_DEMAFF(ch,DEM_HOOVES))
                REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
            if (IS_DEMAFF(ch,DEM_WINGS))
            {
                if (IS_DEMAFF(ch,DEM_UNFOLDED) )
                    REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
                REMOVE_BIT(ch->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
            }
            if ( IS_EXTRA(ch, EXTRA_OSWITCH) )
                do_humanform(ch,"");
            ch->pcdata->powers[DPOWER_FLAGS]   = 0;
            ch->pcdata->stats[DEMON_TOTAL]     = 0;
            ch->pcdata->stats[DEMON_CURRENT]   = 0;
            if (IS_SET(ch->newbits, THIRD_HAND))
                REMOVE_BIT(ch->newbits, THIRD_HAND);
            if (IS_SET(ch->newbits, FOURTH_HAND))
                REMOVE_BIT(ch->newbits, FOURTH_HAND);
        }
        /* Ninja */
        else if (IS_CLASS(ch, CLASS_NINJA))
        {
            ch->pcdata->powers[NPOWER_SORA] = 0;
            ch->pcdata->powers[NPOWER_CHIKYU] = 0;
            ch->pcdata->powers[NPOWER_NINGENNO] = 0;
        }
        /*weapons*/
        ch->wpn[0] /= 2;
        ch->wpn[1] /= 2;
        ch->wpn[2] /= 2;
        ch->wpn[3] /= 2;
        ch->wpn[4] /= 2;
        ch->wpn[5] /= 2;
        ch->wpn[6] /= 2;
        ch->wpn[7] /= 2;
        ch->wpn[8] /= 2;
        ch->wpn[8] /= 2;
        ch->wpn[9] /= 2;
        ch->wpn[10] /= 2;
        ch->wpn[11] /= 2;
        ch->wpn[12] /= 2;

        /*stances*/
        ch->stance[STANCE_VIPER] /= 2;
        ch->stance[STANCE_CRANE] /= 2;
        ch->stance[STANCE_CRAB] /= 2;
        ch->stance[STANCE_MONGOOSE] /= 2;
        ch->stance[STANCE_BULL] /= 2;
        ch->stance[STANCE_MANTIS] /= 2;
        ch->stance[STANCE_DRAGON] /= 2;
        ch->stance[STANCE_TIGER] /= 2;
        ch->stance[STANCE_MONKEY] /= 2;
        ch->stance[STANCE_SWALLOW] /= 2;
        ch->stance[STANCE_SS1] /= 2;
        ch->stance[STANCE_SS2] /= 2;
        ch->stance[STANCE_SS3] /= 2;
        ch->stance[STANCE_SS4] /= 2;
        ch->stance[STANCE_SS5] /= 2;

        /*magic*/
        ch->spl[RED_MAGIC]    /= 2;
        ch->spl[PURPLE_MAGIC] /= 2;
        ch->spl[GREEN_MAGIC]  /= 2;
        ch->spl[BLUE_MAGIC]   /= 2;
        ch->spl[YELLOW_MAGIC] /= 2;

        /*immunities immune*/
        if (IS_IMMUNE(ch,IMM_DRAIN) )
            REMOVE_BIT(ch->immune, IMM_DRAIN);
        if (IS_IMMUNE(ch,IMM_VOODOO) )
            REMOVE_BIT(ch->immune, IMM_VOODOO);
        if (IS_IMMUNE(ch,IMM_SLASH) )
            REMOVE_BIT(ch->immune, IMM_SLASH);
        if (IS_IMMUNE(ch,IMM_STAB) )
            REMOVE_BIT(ch->immune, IMM_STAB);
        if (IS_IMMUNE(ch,IMM_ANIMAL) )
            REMOVE_BIT(ch->immune, IMM_ANIMAL);
        if (IS_IMMUNE(ch,IMM_SMASH) )
            REMOVE_BIT(ch->immune, IMM_SMASH);
        if (IS_IMMUNE(ch,IMM_MISC) )
            REMOVE_BIT(ch->immune, IMM_MISC);
        if (IS_IMMUNE(ch,IMM_CHARM) )
            REMOVE_BIT(ch->immune, IMM_CHARM);
        if (IS_IMMUNE(ch,IMM_HEAT) )
            REMOVE_BIT(ch->immune, IMM_HEAT);
        if (IS_IMMUNE(ch,IMM_COLD) )
            REMOVE_BIT(ch->immune, IMM_COLD);
        if (IS_IMMUNE(ch,IMM_LIGHTNING) )
            REMOVE_BIT(ch->immune, IMM_LIGHTNING);
        if (IS_IMMUNE(ch,IMM_ACID) )
            REMOVE_BIT(ch->immune, IMM_ACID);
        if (IS_IMMUNE(ch,IMM_HURL) )
            REMOVE_BIT(ch->immune, IMM_HURL);
        if (IS_IMMUNE(ch,IMM_BACKSTAB) )
            REMOVE_BIT(ch->immune, IMM_BACKSTAB);
        if (IS_IMMUNE(ch,IMM_KICK) )
            REMOVE_BIT(ch->immune, IMM_KICK);
        if (IS_IMMUNE(ch,IMM_DISARM) )
            REMOVE_BIT(ch->immune, IMM_DISARM);
        if (IS_IMMUNE(ch,IMM_STEAL) )
            REMOVE_BIT(ch->immune, IMM_STEAL);
        if (IS_IMMUNE(ch,IMM_SLEEP) )
            REMOVE_BIT(ch->immune, IMM_SLEEP);

        /* General */
        if (IS_SET(ch->affected_by, AFF_ETHEREAL)    )
            REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
        if (IS_SET(ch->act, PLR_HOLYLIGHT) )
            REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        if (IS_SET(ch->act, PLR_WIZINVIS) )
            REMOVE_BIT(ch->act, PLR_WIZINVIS);
        if (IS_AFFECTED(ch,AFF_SHADOWPLANE) )
            REMOVE_BIT(ch->affected_by,AFF_SHADOWPLANE);
        if (IS_IMMUNE(ch,IMM_SHIELDED) )
            REMOVE_BIT(ch->immune, IMM_SHIELDED);
        ch->power[DISC_VAMP_CELE] = -1;
        ch->power[DISC_VAMP_FORT] = -1;
        ch->power[DISC_VAMP_OBTE] = -1;
        ch->power[DISC_VAMP_PRES] = -1;
        ch->power[DISC_VAMP_QUIE] = -1;
        ch->power[DISC_VAMP_THAU] = -1;
        ch->power[DISC_VAMP_AUSP] = -1;
        ch->power[DISC_VAMP_DOMI] = -1;
        ch->power[DISC_VAMP_OBFU] = -1;
        ch->power[DISC_VAMP_POTE] = -1;
        ch->power[DISC_VAMP_PROT] = -1;
        ch->power[DISC_VAMP_SERP] = -1;
        ch->power[DISC_VAMP_VICI] = -1;
        ch->power[DISC_VAMP_DAIM] = -1;
        ch->power[DISC_VAMP_ANIM] = -1;

        ch->power[DISC_WERE_BEAR] = -1;
        ch->power[DISC_WERE_LYNX] = -1;
        ch->power[DISC_WERE_BOAR] = -1;
        ch->power[DISC_WERE_OWL] = -1;
        ch->power[DISC_WERE_SPID] = -1;
        ch->power[DISC_WERE_WOLF] = -1;
        ch->power[DISC_WERE_HAWK] = -1;
        ch->power[DISC_WERE_MANT] = -1;
        ch->power[DISC_WERE_RAPT] = -1;
        ch->power[DISC_WERE_LUNA] = -1;
        ch->power[DISC_WERE_PAIN] = -1;
        ch->power[DISC_WERE_CONG] = -1;

        ch->power[DISC_DAEM_HELL] = -1;
        ch->power[DISC_DAEM_ATTA] = -1;
        ch->power[DISC_DAEM_TEMP] = -1;
        ch->power[DISC_DAEM_MORP] = -1;
        ch->power[DISC_DAEM_CORR] = -1;
        ch->power[DISC_DAEM_GELU] = -1;
        ch->power[DISC_DAEM_DISC] = -1;
        ch->power[DISC_DAEM_NETH] = -1;
        ch->power[DISC_DAEM_IMMU] = -1;

        ch->pcdata->stats[UNI_AFF]     = 0;
        ch->pcdata->stats[UNI_CURRENT] = -1;

        do_remove(ch, "all");
        ch->max_move = 1000;
        ch->max_mana = 1000;
        ch->max_hit = 1000;
        ch->mkill = 0;
        ch->mdeath = 0;
        ch->practice = 0;
        ch->move = ch->max_move;
        ch->mana = ch->max_mana;
        ch->hit = ch->max_hit;
        ch->pcdata->rank = 0;
        ch->rage = 0;
        ch->generation = 3;
        ch->pcdata->upgrade_level--;
        if (ch->pcdata->upgrade_level < 0) ch->pcdata->upgrade_level = 0;
        if (IS_SET(ch->newbits, NEW_MASTERY) ) REMOVE_BIT(ch->newbits, NEW_MASTERY);
        free_string(ch->lord);
        ch->lord     = str_dup( "" );
        free_string(ch->clan);
        ch->clan     = str_dup( "" );
        ch->generation     = 6;
        ch->hitroll = 0;
        ch->damroll = 0;
        ch->armor = 100;
        ch->beast = 15;
        ch->class = 0;
        ch->trust = 0;
        ch->level = 2;
        ch->pcdata->quest -= 1000;

        //	ch->pcdata->bloodpower[0] = 0;
        //	ch->pcdata->bloodpower[1] = 0;

        /*armor-weapons-items*/
        interpret(ch, "remove all");
        send_to_char("You are fully detoxified and now a puny #rMortal#n", ch);
    }
}
