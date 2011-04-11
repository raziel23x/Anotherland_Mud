/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*********************************
 * Upgrade commands for Dystopia *
 *********************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* local functions */

void clearshit  args((CHAR_DATA *ch));

bool is_upgrade(CHAR_DATA *ch)
{
    if (IS_NPC(ch)) return FALSE;

    if (IS_CLASS(ch, CLASS_DROID))         return TRUE;
    if (IS_CLASS(ch, CLASS_SAMURAI))     return TRUE;
    if (IS_CLASS(ch, CLASS_TANARRI))        return TRUE;
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))  return TRUE;
    if (IS_CLASS(ch, CLASS_ANGEL))          return TRUE;
    if (IS_CLASS(ch, CLASS_LICH))     return TRUE;
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER))   return TRUE;
    if (IS_CLASS(ch, CLASS_RUNEMASTER))   return TRUE;
    return FALSE;
}

bool is_baseupgrade(CHAR_DATA *ch)
{
    if (IS_NPC(ch)) return FALSE;

    if (IS_CLASS(ch, CLASS_DROW))         return TRUE;
    if (IS_CLASS(ch, CLASS_NINJA))     return TRUE;
    if (IS_CLASS(ch, CLASS_DEMON))        return TRUE;
    if (IS_CLASS(ch, CLASS_VAMPIRE))  return TRUE;
    if (IS_CLASS(ch, CLASS_MONK))          return TRUE;
    if (IS_CLASS(ch, CLASS_MAGE))     return TRUE;
    if (IS_CLASS(ch, CLASS_WEREWOLF))   return TRUE;
    return FALSE;
}

void do_upgrade(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int mana_needed = 80000;
    int hit_needed  = 80000;
    int move_needed = 80000;
    int qp_needed = 0;
    int pkscore_needed = 0;
    int i;

    if (IS_NPC(ch)) return;
    /*  if (ch->pcdata->upgrade_level >= 9)
      {
        send_to_char("You have reached the maximum level.\n\r",ch);
        return;
       }    */
    /*why cap it?  seems stupid. */
    qp_needed = (ch->pcdata->upgrade_level+1) * 15000;
    hit_needed = hit_needed + ch->pcdata->upgrade_level * 10000;
    mana_needed = mana_needed + ch->pcdata->upgrade_level * 10000;
    move_needed = move_needed + ch->pcdata->upgrade_level * 10000;
    pkscore_needed = ch->pcdata->upgrade_level * 4;
    stcf(ch,"You need %d quest points.\n\r",qp_needed);
    stcf(ch,"You need %d Hp, %d Mana, %d Move.\n\r",hit_needed,mana_needed,move_needed);
    stcf(ch,"You need %d status.\n\r",pkscore_needed);
    stcf(ch,"Your missing requirements:");
    if (ch->max_hit < hit_needed)
    {
        sprintf(buf,"You need %d more hitpoints.\n\r", hit_needed - ch->max_hit);
        send_to_char(buf,ch);
        return;
    }
    if (ch->max_mana < mana_needed)
    {
        sprintf(buf,"You need %d more manapoints.\n\r", mana_needed - ch->max_mana);
        send_to_char(buf,ch);
        return;
    }
    if (ch->max_move < move_needed)
    {
        sprintf(buf,"You need %d more movepoints.\n\r", move_needed - ch->max_move);
        send_to_char(buf,ch);
        return;
    }
    if (ch->pcdata->quest < qp_needed)
    {
        sprintf(buf,"You need %d more qps.\n\r", qp_needed - ch->pcdata->quest);
        send_to_char(buf,ch);
        return;
    }
    if ((ch->race) < pkscore_needed)
    {
        sprintf(buf,"You need %d status.\n\r",pkscore_needed );
        send_to_char(buf,ch);
        return;
    }
    for (i = 19; i <= 23; i++)
    {
        if (ch->stance[i] == -1)
        {
            send_to_char("You need all your superstances to upgrade.\n\r", ch);
            return;
        }
    }
    ch->pcdata->upgrade_level++;
    ch->pcdata->quest -= qp_needed;
    //  ch->max_hit  = 5000;
    ch->hit      = 5000;
    //  ch->max_move = 5000;
    ch->move     = 5000;
    //  ch->max_mana = 5000;
    ch->mana     = 5000;
    ch->exp      = 100000000;
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) sprintf(buf,"The torments and horrors of undead life has made %s even more powerful",ch->name);
    else if (IS_CLASS(ch, CLASS_LICH)) sprintf(buf,"Through studies of ancient and forbidden texts %s has grown more powerful",ch->name);
    else if (IS_CLASS(ch, CLASS_SAMURAI)) sprintf(buf,"Through hard work and countless battles, %s's skills with weapons have grown beyond that of any mortal",ch->name);
    else if (IS_CLASS(ch, CLASS_DROID)) sprintf(buf,"Becoming truly one with Lloth, %s choses to let the queen take more control",ch->name);
    else if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) sprintf(buf,"The Malaugrym grins in truimph as %s uncovers the ancient powers of Malaug",ch->name);
    else if (IS_CLASS(ch, CLASS_ANGEL)) sprintf(buf,"The sky shakes with thunder as %s is proclaimed the messanger of God",ch->name);
    else if (IS_CLASS(ch, CLASS_WRAITH)) sprintf(buf,"The apparition of %s becomes even more solid as he gains his power in death",ch->name);
    else if (IS_CLASS(ch, CLASS_TANARRI)) sprintf(buf,"Through hardwon battles in the famous Blood Wars, %s has gained skills beyond this world",ch->name);
    else if (IS_CLASS(ch, CLASS_WEREWOLF)) sprintf(buf,"Wolves howl in moot as the mighty deeds of %s gain them recognition and rank",ch->name);
    else if (IS_CLASS(ch, CLASS_PHOENIX)) sprintf(buf,"%s dies and is reborn as a more powerful Phoenix",ch->name);
    else if (IS_CLASS(ch, CLASS_DEMON)) sprintf(buf,"Bellowing in triumph, %s ascends over the broken souls of fallen mortals to a higher Plane of the Abyss",ch->name);
    else if (IS_CLASS(ch, CLASS_DRAGON)) sprintf(buf,"Soaring through the skies, %s has become an even more ancient Dragon",ch->name);
    else if (IS_CLASS(ch, CLASS_NINJA)) sprintf(buf,"Through stealthy arts and secret assassinations, %s has learned more of Ninjitsu",ch->name);
    else sprintf(buf,"%s has upgraded\n\r", ch->name);
    do_info(ch,buf);
    return;
}

/*
void do_upgrade(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  int mana_needed = 35000;
  int hit_needed = 50000;
  int move_needed = 35000;
  int qp_needed = 15000;
  int pkscore_needed = 1000;
  int penalty = 3000;

  if (IS_NPC(ch)) return;
  if ((ch->in_room == NULL || ch->in_room->vnum != 3054))
  {
    send_to_char("You can only upgrade at the Temple Altar of Midgaard.\n\r",ch);
    return;
  }
  do_clearstats(ch,"");
  if (is_upgrade(ch)) {upgrade2(ch); return;}
  if (is_baseupgrade(ch)) {upgrade2(ch); return;}
  if (get_ratio(ch) < pkscore_needed)
  {
    pkscore_needed = (pkscore_needed - UMAX(get_ratio(ch),0)) / 100;
    mana_needed += penalty * pkscore_needed;
    move_needed += penalty * pkscore_needed;
    hit_needed  += penalty * pkscore_needed;
  }
  if (ch->max_hit < hit_needed)
  {
    sprintf(buf,"You need %d more hitpoints.\n\r", hit_needed - ch->max_hit);
    send_to_char(buf,ch);
    return;
  }
  if (ch->max_mana < mana_needed)
  {
    sprintf(buf,"You need %d more manapoints.\n\r", mana_needed - ch->max_mana);
    send_to_char(buf,ch);
    return;
  }
  if (ch->max_move < move_needed)
  {
    sprintf(buf,"You need %d more movepoints.\n\r", move_needed - ch->max_move);
    send_to_char(buf,ch);
    return;
  }
  if (ch->pcdata->quest < qp_needed)
  {
    sprintf(buf,"You need %d more qps.\n\r", qp_needed - ch->pcdata->quest);
    send_to_char(buf,ch);
    return;
  }
  if (ch->generation != 1)
  {
    send_to_char("You need to be generation 1.\n\r",ch);
    return;
  }
  sprintf(buf,"%s has started his quest to be a knight!\n\r",ch->name);
  ch->pcdata->quest -= qp_needed;
  do_info(ch,buf);
  ch->pcdata->upgrade_level++;
  return;
}

*/
void do_autoclass(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    if(IS_NPC(ch)) return;
    if((ch->in_room == NULL || ch->in_room->vnum != 3054))
    {
        send_to_char("You can only upgrade at the Temple Altar of Midgaard.\n\r",ch);
        return;
    }

    if(ch->pcdata->quest < 5000)
    {
        send_to_char( "You don't have the required 5K qps.\n\r",ch);
        return;
    }

    if (ch->class == CLASS_TANARRI || ch->class == CLASS_DROID || ch->class == CLASS_SAMURAI || ch->class == CLASS_UNDEAD_KNIGHT || ch->class ==
        CLASS_ANGEL || ch->class == CLASS_SHAPESHIFTER || ch->class == CLASS_LICH )
    {
        send_to_char( "You already have the upgraded class.\n\r",ch);
        return;
    }

    if(ch->exp < 100000000)
    {
        send_to_char( "You don't have 100Million exp.\n\r",ch);
        return;
    }

    if (ch->class == CLASS_DEMON) {ch->class = CLASS_TANARRI; sprintf(buf,"%s is reborn as a Tanar'ri", ch->name);}
    else if (ch->class == CLASS_DROW) {ch->class = CLASS_DROID; sprintf(buf,"%s makes the ultimate sacrifice to Lloth", ch->name);}
    else if (ch->class == CLASS_NINJA) {ch->class = CLASS_SAMURAI; sprintf(buf,"%s have become a true master of weapons", ch->name);}
    else if (ch->class == CLASS_VAMPIRE) {ch->class = CLASS_UNDEAD_KNIGHT; sprintf(buf,"%s has chosen the darker path", ch->name);}
    else if (ch->class == CLASS_MONK) {ch->class = CLASS_ANGEL; sprintf(buf,"%s becomes one with God", ch->name);}
    else if (ch->class == CLASS_WEREWOLF) {ch->class = CLASS_SHAPESHIFTER; sprintf(buf,"%s reverts to the true form of the Malaugrym",ch->name);}
    else if (ch->class == CLASS_MAGE) {ch->class = CLASS_LICH; sprintf(buf,"%s have sacrificed life for power", ch->name);}
    else return;
    ch->exp -= 100000000;
    ch->pcdata->quest -= 5000;
    clearshit(ch);
    do_info(ch,buf);
    set_learnable_disciplines(ch);
    return;
}

void clearshit(CHAR_DATA *ch)
{
    int sn;

    ch->pcdata->jflags                  = 0;
    ch->extra                           = 0;
    ch->exp                 = 0;
    ch->practice                        = 0;
    ch->special                         = 0;
    ch->newbits                         = 0;
    ch->max_hit                 = 5000;
    ch->max_mana                        = 5000;
    ch->max_move                        = 5000;
    ch->hit                             = 5000;
    ch->mana                            = 5000;
    ch->move                            = 5000;
    ch->generation              = 5;
    ch->pcdata->familiar                = NULL;
    ch->pcdata->chobj                   = NULL;
    ch->pcdata->memorised               = NULL;
    ch->pcdata->disc_research           = -1;
    ch->pcdata->relrank             = 0;
    ch->lord                            = str_dup( "" );
    ch->morph                           = str_dup( "" );
    ch->pload                           = str_dup( "" );
    ch->poweraction                     = str_dup( "" );
    ch->powertype                       = str_dup( "" );
    ch->hunting                         = str_dup( "" );
    ch->pcdata->followers               = 0;
    ch->spectype                        = 0;
    ch->specpower                       = 0;
    ch->mounted                         = 0;
    ch->home                            = 3001;
    ch->vampgen_a                       = 0;
    ch->paradox[0]                      = 0;                /* Total paradox */
    ch->paradox[1]                      = 0;                /* Current Paradox */
    ch->paradox[2]                      = 0;                /* Paradox Ticker */
    ch->damcap[0]                       = 1000;
    ch->damcap[1]                       = 0;
    ch->vampaff_a                       = 0;
    ch->itemaffect                      = 0;
    ch->polyaff                         = 0;
    ch->immune                          = 0;
    ch->form                            = 1048575;
    ch->beast                           = 15;
    for( sn=0 ; sn < 7 ; sn ++ )
        ch->loc_hp[sn]               = 0;
    for( sn=0 ; sn < 13 ; sn++ )
        ch->wpn[sn]                  = 0;
    for( sn=0 ; sn < 5 ; sn++ )
        ch->spl[sn]                  = 4;
    for( sn=0 ; sn < 8 ; sn++ )
        ch->cmbt[sn]                 = 0;
    for( sn=0 ; sn < 11 ; sn++ )
        ch->cmbt[sn]                 = 0;
    ch->pcdata->followers               = 0;
    ch->pcdata->perm_str                = 1;
    ch->pcdata->perm_int                = 1;
    ch->pcdata->perm_wis                = 1;
    ch->pcdata->perm_dex                = 1;
    ch->pcdata->perm_con                = 1;
    ch->pcdata->wolf                    = 0;
    ch->pcdata->rank                    = 0;
    ch->pcdata->language[0]             = 0;
    ch->pcdata->language[1]             = 0;
    ch->pcdata->stage[0]                = 0;
    ch->pcdata->stage[1]                = 0;
    ch->pcdata->stage[2]                = 0;
    ch->pcdata->wolfform[0]             = 1;
    ch->pcdata->wolfform[1]             = 1;
    ch->pcdata->score[0]                = 0;
    ch->pcdata->score[1]                = 0;
    ch->pcdata->score[2]                = 0;
    ch->pcdata->score[3]                = 0;
    ch->pcdata->score[4]                = 0;
    ch->pcdata->score[5]                = 0;
    for( sn=0 ; sn < 11 ; sn++ )
        ch->pcdata->disc_a[sn]       = 0;
    for( sn=0 ; sn < 10 ; sn++ )
        ch->pcdata->genes[sn]        = 0;
    for ( sn=0 ; sn < 20 ; sn++)
        ch->pcdata->powers[sn]        = 0;
    for ( sn=0 ; sn < 12 ; sn++)
        ch->pcdata->stats[sn]         = 0;
    for  ( sn=0; sn < 19; sn++)
        ch->stance[sn]            = 0;
    for  ( sn=19; sn < 24; sn++)
        ch->stance[sn]                = -1;
    for  ( sn=0; sn < 44; sn++)
        ch->power[sn]                 = -2;
    ch->pcdata->fake_skill              = 0;
    ch->pcdata->fake_stance             = 0;
    ch->pcdata->fake_hit                = 0;
    ch->pcdata->fake_dam                = 0;
    ch->pcdata->fake_ac                 = 0;
    ch->pcdata->fake_hp                 = 0;
    ch->pcdata->fake_mana               = 0;
    ch->pcdata->fake_move               = 0;
    ch->pcdata->obj_vnum                = 0;
    ch->pcdata->condition[COND_THIRST]  = 48;
    ch->pcdata->condition[COND_FULL]    = 48;
    ch->pcdata->stat_ability[STAT_STR]  = 0;
    ch->pcdata->stat_ability[STAT_END]  = 0;
    ch->pcdata->stat_ability[STAT_REF]  = 0;
    ch->pcdata->stat_ability[STAT_FLE]  = 0;
    ch->pcdata->stat_amount[STAT_STR]   = 0;
    ch->pcdata->stat_amount[STAT_END]   = 0;
    ch->pcdata->stat_amount[STAT_REF]   = 0;
    ch->pcdata->stat_amount[STAT_FLE]   = 0;
    ch->pcdata->stat_duration[STAT_STR] = 0;
    ch->pcdata->stat_duration[STAT_END] = 0;
    ch->pcdata->stat_duration[STAT_REF] = 0;
    ch->pcdata->stat_duration[STAT_FLE] = 0;
    ch->pcdata->exhaustion              = 0;
    if (IS_SET(ch->act, PLR_HOLYLIGHT)) REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
    return;
}

/*
 * Downgrade by Devik
 * Make players maintain their pkscore to keep upgrade.
 */
bool downgrade( CHAR_DATA *ch )
{
    int pk = ch->race;
    sh_int pks[] = { 0, 0, 4, 8, 12, 16, 20, 24, 28, 32 };
    sh_int qps[] = { 0, 15,  30,  45,   60,   75,   90,  105,  120,  135 };
    char buf[MAX_STRING_LENGTH];

    if ( ch->pcdata->upgrade_level == 0 ) return FALSE;

    if ( ch->pcdata->upgrade_level > 9 ) return FALSE;

    if ( pks[ch->pcdata->upgrade_level] == 0 ) return FALSE;

    if ( pk < pks[ch->pcdata->upgrade_level] )
    {
        ch->pcdata->quest += qps[ch->pcdata->upgrade_level] * 1000;
        ch->pcdata->upgrade_level--;
        sprintf(buf,"#R%s#G has been downgraded!#n",ch->name);
        do_info(ch,buf);
        //return TRUE;

        if ((ch->pcdata->perm_str) > (ch->pcdata->upgrade_level * 25) + 25)
        {
            ch->pcdata->perm_str = (ch->pcdata->upgrade_level * 25) + 25;
            sprintf(buf,"#R%s#G has lost strength!#n",ch->name);
            do_info(ch,buf);
        }
        if ((ch->pcdata->perm_dex) > (ch->pcdata->upgrade_level * 25) + 25)
        {
            ch->pcdata->perm_dex = (ch->pcdata->upgrade_level * 25) + 25;
            sprintf(buf,"#R%s#G has lost dexterity!#n",ch->name);
            do_info(ch,buf);
        }
        if ((ch->pcdata->perm_con) > (ch->pcdata->upgrade_level * 25) + 25)
        {
            ch->pcdata->perm_con = (ch->pcdata->upgrade_level * 25) + 25;
            sprintf(buf,"#R%s#G has lost constitution!#n",ch->name);
            do_info(ch,buf);
        }
        if ((ch->pcdata->perm_wis) > (ch->pcdata->upgrade_level * 25) + 25)
        {
            ch->pcdata->perm_wis = (ch->pcdata->upgrade_level * 25) + 25;
            sprintf(buf,"#R%s#G has lost wisdom!#n",ch->name);
            do_info(ch,buf);
        }
        if ((ch->pcdata->perm_int) > (ch->pcdata->upgrade_level * 25) + 25)
        {
            ch->pcdata->perm_int = (ch->pcdata->upgrade_level * 25) + 25;
            sprintf(buf,"#R%s#G has lost intelligence!#n",ch->name);
            do_info(ch,buf);
        }
        return TRUE;
    }
    return FALSE;
}
