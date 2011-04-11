/***********************************
 * Different needed function calls *
 ***********************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                                         /* close */
#include <time.h>
#include "merc.h"

/*
 * Is astr contained within bstr ?
 *
 */
void log_string2(const char *str)
{
    char *strtime;

    strtime = ctime(&current_time);
    strtime[strlen(strtime)-1] = '\0';
    fprintf(stderr, "%s :: %s\n", strtime, str);

    return;
}

char *line_indent(char *text, int wBegin, int wMax)
{
    static char buf[MAX_STRING_LENGTH];
    char *ptr;
    char *ptr2;
    int count = 0;
    bool stop = FALSE;
    int wEnd = 0;

    buf[0] = '\0';
    ptr = text;
    ptr2 = buf;

    while (!stop)
    {
        if (count == 0)
        {
            if (*ptr == '\0') wEnd = wMax - wBegin;
            else if (strlen(ptr) < (wMax - wBegin)) wEnd = wMax - wBegin;
            else
            {
                int x = 0;

                while (*(ptr + (wMax - wBegin - x)) != ' ') x++;
                wEnd = wMax - wBegin - (x - 1);
                if (wEnd < 1) wEnd = wMax - wBegin;
            }
        }
        if (count == 0 && *ptr == ' ') ptr++;
        else if (++count != wEnd)
        {
            if ((*ptr2++ = *ptr++) == '\0') stop = TRUE;
        }
        else if (*ptr == '\0')
        {
            stop = TRUE;
            *ptr2 = '\0';
        }
        else
        {
            int k;

            count = 0;
            *ptr2++ = '\n';
            *ptr2++ = '\r';
            for (k = 0; k < wBegin; k++)
                *ptr2++ = ' ';
        }
    }
    return buf;
}

void check_help_soundex(char *argument, CHAR_DATA *ch)
{
    HELP_DATA *pHelp;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char keyword[MAX_INPUT_LENGTH];
    char *str;
    bool found = FALSE;

    one_argument(argument, arg);

    if (arg[0] == '\0') return;

    sprintf(buf, "\n\r[Perhaps:");
    for (pHelp = first_help; pHelp; pHelp = pHelp->next)
    {
        if (pHelp->level > ch->level) continue;
        str = pHelp->keyword;
        str = one_argument(str, keyword);
        while (keyword[0] != '\0')
        {
            /*      if (SoundexMatch(GetSoundexKey(arg), GetSoundexKey(keyword)) > 75)
                  {
                    found = TRUE;
                    sprintf(tbuf, " %s", keyword);
                    strcat(buf, tbuf);
                  }*/
            str = one_argument(str, keyword);
        }
    }
    strcat(buf, "]\n\r");
    if (found) send_to_char(buf, ch);
}

bool is_contained( const char *astr, const char *bstr )
{
    int i, alen, blen, count;

    alen = strlen(astr);
    blen = strlen(bstr);
    if (alen > blen) return FALSE;
    for (i = 0; i <= (blen - alen); i++)
    {
        count = 0;
        while (count < alen && UPPER(astr[count]) == UPPER(bstr[i+count])) count++;
        if (count == alen) return TRUE;
    }
    return FALSE;
}

bool is_contained2( const char *astr, const char *bstr )
{
    int i, alen, blen, count;

    alen = strlen(astr);
    blen = strlen(bstr);
    if (alen > blen) return FALSE;
    for (i = 0; i <= (blen - alen); i++)
    {
        count = 0;
        while (count < alen && astr[count] == bstr[i+count]) count++;
        if (count == alen) return TRUE;
    }
    return FALSE;
}

int strlen2(const char *s)
{
    int i, b, count=0;

    if (s[0] == '\0') return 0;
    b = strlen(s);
    for (i = 0; i < b; i++)
    {
        if (s[i] == '#') count++;
    }
    return (b + 7 * count);
}

void win_prize( CHAR_DATA *ch )
{
    int i,vnum;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pIndex;

    if (IS_NPC(ch)) return;
    i = number_range(1,110);
    if (i < 50) vnum = OBJ_VNUM_PROTOPLASM;
    else if (i < 55) vnum = 33851;
    else if (i < 65) vnum = 33852;
    else if (i < 75) vnum = 33853;
    else if (i < 80) vnum = 33854;
    else if (i < 85) vnum = 33855;
    else if (i < 90) vnum = 33856;
    else if (i < 95) vnum = 113028;
    else if (i < 97) vnum = 113029;
    else vnum = 33857;
    if ((pIndex = get_obj_index(vnum)) == NULL)
    {
        bug("BAD PRIZE!!",0);
        return;
    }
    obj = create_object(pIndex, 50);
    if (vnum == OBJ_VNUM_PROTOPLASM)
    {
        obj->level = 1;
        free_string(obj->short_descr);
        free_string(obj->name);
        free_string(obj->description);
        obj->short_descr = str_dup("A prize token");
        obj->description = str_dup("A token lies on the floor");
        obj->name = str_dup("prize token");
        obj->value[0] = number_range(1000,5000);
        obj->item_type = ITEM_QUEST;
    }
    obj_to_char(obj,ch);
    return;
}

void do_clearstats2( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    if (IS_NPC(ch)) return;

    powerdown(ch);                                          /* remove class shit */

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        if ( obj->wear_loc != WEAR_NONE )
            {obj_from_char(obj); obj_to_char(obj,ch);}
    }

    while ( ch->affected )
        affect_remove( ch, ch->affected );

    if (IS_SET(ch->affected_by, AFF_POLYMORPH)) REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    if (IS_SET(ch->affected_by, AFF_ETHEREAL)) REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
    if (IS_EXTRA(ch,EXTRA_DRAGON)) REMOVE_BIT(ch->extra, EXTRA_DRAGON);
    if (IS_VAMPAFF(ch, VAM_CHANGED) && IS_POLYAFF(ch, POLY_WOLF))  REMOVE_BIT(ch->polyaff, POLY_WOLF);

    ch->affected_by      = 0;
    ch->armor            = 100;
    ch->hit              = UMAX( 1, ch->hit  );
    ch->mana             = UMAX( 1, ch->mana );
    ch->move             = UMAX( 1, ch->move );
    ch->hitroll          = 0;
    ch->damroll          = 0;
    ch->saving_throw     = 0;
    ch->pcdata->mod_str  = 0;
    ch->pcdata->mod_int  = 0;
    ch->pcdata->mod_wis  = 0;
    ch->pcdata->mod_dex  = 0;
    ch->pcdata->mod_con  = 0;
    ch->pcdata->followers = 0;
    if (IS_SET(ch->newbits, NEW_DFORM)) REMOVE_BIT(ch->newbits, NEW_DFORM);
    if (IS_POLYAFF(ch, POLY_ZULOFORM)) REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
    if (IS_SET(ch->newbits, NEW_CUBEFORM)) REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    save_char_obj( ch );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
    return;
}

/*
 *  If an item have a bad char in it's short/long/name desc, the mud
 *  will freeze when the char with that item tries to log on, so use
 *  this check on any renaming of items (glamour/reshape/etc).
 */
bool has_bad_chars(CHAR_DATA *ch, char *argument)
{
    int i;

    if ( argument[0] == '\0' ) return FALSE;
    for (i = 0; argument[i] != '\0' ; i++)
    {
        if (argument[i] == '~') return TRUE;
    }
    return FALSE;
}

void ragnarok_stop()
{
    DESCRIPTOR_DATA *d;

    ragnarok = FALSE;
    do_info(NULL,"#CPeace has been restored in the realms, the time of apocalypse is no more#n");
    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->character && d->connected == CON_PLAYING)
        {
            d->character->fight_timer = 0;
            d->character->pcdata->safe_counter = 5;
            do_call(d->character,"all");
            do_restore(d->character,"self");
        }
    }
    return;
}

/* agrr_test by blade of E, version 1.31. */
void aggr_test(CHAR_DATA * ch)
{
    char buf[60];
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *victim;

    if (!IS_NPC(ch) && ch->level < 7 && ch->in_room != NULL && !IS_SET(ch->in_room->room_flags,ROOM_SAFE))
    {
        for ( wch = ch->in_room->people; wch != NULL; wch = wch_next )
        {
            wch_next = wch->next_in_room;
            if ( !IS_NPC(wch) || !IS_SET(wch->act, ACT_AGGRESSIVE)
                || wch->fighting != NULL || IS_AFFECTED(wch, AFF_CHARM)
                || !IS_AWAKE(wch) || ( IS_SET(wch->act, ACT_WIMPY) && IS_AWAKE(ch))
                || !can_see( wch, ch ) || number_bits(2) == 0)
            {
                continue;
            }
            victim = wch;
            do_look(victim,"");
            if ( victim == NULL ) continue;
            sprintf(buf,"%s screams and attacks!\n\r", victim->name);
            send_to_char(buf, ch);
            multi_hit( victim, ch, TYPE_UNDEFINED );
        }
    }
    return;
}

void powerdown(CHAR_DATA *ch)
{
    if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_DEMON))
    {
        if (IS_POLYAFF(ch, POLY_ZULOFORM))                  /* demonform */
        {
            REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
            free_string( ch->morph );
            ch->morph = str_dup( "" );
        }
    }
    else if (IS_CLASS(ch, CLASS_DROW))
    {
        if (IS_SET(ch->newbits,NEW_DFORM))                  /* spiderform */
        {
            free_string(ch->morph);
            ch->morph = str_dup("");
            REMOVE_BIT(ch->newbits,NEW_DFORM);
            REMOVE_BIT(ch->newbits,THIRD_HAND);
            REMOVE_BIT(ch->newbits,FOURTH_HAND);
        }
    }
    else if (IS_CLASS(ch, CLASS_VAMPIRE))
    {
        if (IS_EXTRA(ch, EXTRA_DRAGON))                     /* dragonform */
        {
            REMOVE_BIT(ch->extra,EXTRA_DRAGON);
            free_string( ch->morph );
            ch->morph = str_dup( "" );
        }
        if (IS_POLYAFF(ch, POLY_ZULOFORM))                  /* zuloform */
        {
            REMOVE_BIT(ch->polyaff, POLY_ZULOFORM);
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
            free_string( ch->morph );
            ch->morph = str_dup( "" );
        }
    }
    else if (IS_CLASS(ch, CLASS_TANARRI))
    {
        if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)       /* fury */
            ch->pcdata->powers[TANARRI_FURY_ON] = 0;
    }
    else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
        if (IS_SET(ch->affected_by, AFF_POLYMORPH))         /* reset form */
        {
            ch->pcdata->powers[SHAPE_FORM]=0;
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
            free_string(ch->morph);
            ch->morph=str_dup("");
        }
    }
    else if (IS_CLASS(ch, CLASS_DROID))
    {
        if (IS_SET(ch->newbits, NEW_CUBEFORM))              /* avatar of Lloth */
        {
            REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
            free_string(ch->morph);
            ch->morph=str_dup("");
        }
    }
    else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
    {
        if (IS_SET(ch->pcdata->powers[AURAS], MIGHT_AURA))  /* aura of might */
            REMOVE_BIT(ch->pcdata->powers[AURAS], MIGHT_AURA);
    }
    else if (IS_CLASS(ch, CLASS_ANGEL))
    {
        if (IS_SET(ch->newbits, NEW_CUBEFORM))              /* godly favor */
        {
            REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
            REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
            free_string(ch->morph);
            ch->morph=str_dup("");
        }
    }
    return;
}

int get_ratio(CHAR_DATA *ch)
{
    int ratio;

    if (IS_NPC(ch)) return 0;
    if ((ch->pkill + ch->pdeath) == 0) ratio = 0;           // to avoid divide by zero.
    else if ( ch->pdeath >= ch->pkill ) ratio = 0;
    else
        ratio = (ch->pkill*100)-(ch->pdeath*100);
    return ratio;
}

CHAR_DATA * mmulticheck(CHAR_DATA *ch)
//bool mmulticheck(CHAR_DATA *ch)
{
    CHAR_DATA *gch;
    char buf[MAX_STRING_LENGTH];

    if (ch->level > 6) return FALSE;

    for (gch = char_list; gch; gch = gch->next)
    {
        if (IS_NPC(gch)) continue;
        if (gch == ch) continue;
        if (gch->level > 6) continue;
        if (strlen(gch->lasthost) > 2)
        {
            if (gch->desc)
            {
                if (!str_cmp(gch->desc->host, ch->desc->host))
                {
                    sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
                    log_string(buf);
                    //do_info(ch, buf);
                    return gch;
                }
            }
            else if (!str_cmp(gch->lasthost, ch->desc->host))
            {
                sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
                log_string(buf);
                return gch;
            }
        }
    }
    return FALSE;
}

/*
CHAR_DATA * mmmmulticheck(CHAR_DATA *ch)
{
  CHAR_DATA *gch;
  char buf[MAX_STRING_LENGTH];
  bool bDenied = FALSE;

  if (ch->level > 7) return FALSE;

  for (gch = char_list; gch; gch = gch->next)
  {
    if (IS_NPC(gch)) continue;
    if (gch == ch) continue;

    if (strlen(gch->lasthost) > 2)
    {
      if (gch->desc)
      {
          if (gch->desc->host == "dsl-123.163.240.220.lns02-waym-adl.dsl.comindico.com.au") //ditoke home
          {
//			  return;
          }
        if (!str_cmp(gch->desc->host, ch->desc->host))
        {
          sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
      if ( ch->level < 7 && gch->level < 7
        && str_cmp(ch->name,"Serena") 	&& str_cmp(gch->name,"Serena")
        && str_cmp(ch->name,"Pithound")	&& str_cmp(gch->name,"Pithound")
        && str_cmp(ch->name,"Useless")	&& str_cmp(gch->name,"Useless")
                && str_cmp(ch->name,"Chrono")    && str_cmp(gch->name,"Chrono")
                && str_cmp(ch->name,"Dertaherup")    && str_cmp(gch->name,"Dertaherup")
        && str_cmp(ch->name,"Beorn")	&& str_cmp(gch->name,"Beorn")
        && str_cmp(ch->name,"Zizina")   && str_cmp(gch->name,"Xalode") && !IS_IMMORTAL(ch))
      {
            send_to_char("You will be denied. Log on as someone else if you want to be reinstated.\n\r",ch);
            send_to_char("You will be denied. Log on as someone else if you want to be reinstated.\n\r",gch);
        bDenied = TRUE;
      }
          log_string(buf);
        wiznet( NULL, WIZ_MISC, 0, buf );
//          return TRUE;
      return gch;
        }
      }
      else if (!str_cmp(gch->lasthost, ch->desc->host))
      {
        sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
    if ( ch->level < 13 && gch->level < 13
                && str_cmp(ch->name,"Serena")   && str_cmp(gch->name,"Serena")
                && str_cmp(ch->name,"Pithound") && str_cmp(gch->name,"Pithound")
        && str_cmp(ch->name,"Angela")	&& str_cmp(gch->name,"Angela")
                && str_cmp(ch->name,"Chrono")    && str_cmp(gch->name,"Chrono")
        && str_cmp(ch->name,"Useless")	&& str_cmp(gch->name,"Useless")
        && str_cmp(ch->name,"Beorn")    && str_cmp(gch->name,"Beorn")
        && str_cmp(ch->name,"Xalode")   && str_cmp(gch->name,"Zizina") && !IS_IMMORTAL(ch))
    {
        send_to_char("You will be denied. Log on as someone else if you want to be reinstated.\n\r",ch);
        send_to_char("You will be denied. Log on as someone else if you want to be reinstated.\n\r",gch);
            bDenied = TRUE;
        }
        wiznet( NULL, WIZ_MISC, 0, buf );
        log_string(buf);
//        return TRUE;
    return gch;
      }
    }
  }
  return NULL;
}
*/
bool mmmulticheck(CHAR_DATA *ch)
{
    CHAR_DATA *gch;
    char buf[MAX_STRING_LENGTH];
    bool bDenied = FALSE;

    if (ch->level > 6) return FALSE;

    for (gch = char_list; gch; gch = gch->next)
    {
        if (IS_NPC(gch)) continue;
        if (gch == ch) continue;
        if (gch->level > 8) continue;
        if (strlen(gch->lasthost) > 2)
        {
            if (gch->desc)
            {
                if (!str_cmp(gch->desc->host, ch->desc->host) && !IS_IMMORTAL(ch))
                {
                    sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
                    wiznet( NULL, WIZ_LINKS, 0, buf );
                    log_string(buf);
                    //do_info(ch, buf);
                    bDenied = TRUE;
                }
                return (bDenied);
            }
            else if (!str_cmp(gch->lasthost, ch->desc->host) && !IS_IMMORTAL(ch))
            {
                sprintf(buf,"%s has connected from the same IP as %s", ch->name, gch->name);
                wiznet( NULL, WIZ_LINKS, 0, buf );
                log_string(buf);
                bDenied = TRUE;
            }
            return (bDenied);
        }
    }
    return FALSE;
}

bool reachedDecapLimit( CHAR_DATA *ch )
{
    AFFECT_DATA *paf;
    OBJ_DATA *obj;
    int limit = 0, objhps = 0, spellhps = 0;
    int hps, i;

    if (IS_NPC(ch)) return TRUE;
    if (ch->level > 6) return FALSE;

    if (!IS_CLASS(ch, CLASS_SAMURAI))
    {
        for (paf = ch->affected; paf != NULL; paf = paf->next)
        {
            if (paf->location == APPLY_HIT)
                spellhps += paf->modifier;
        }
    }
    for (i = 0; i < MAX_WEAR; i++)
    {
        if ((obj = get_eq_char(ch, i)) == NULL) continue;
        if (IS_CLASS(ch, CLASS_SAMURAI) && obj->pIndexData->vnum != 33177) continue;
        for (paf = obj->pIndexData->affected; paf; paf = paf->next)
        {
            if (paf->location == APPLY_HIT)
                objhps += paf->modifier;
        }
        for (paf = obj->affected; paf; paf = paf->next)
        {
            if (paf->location == APPLY_HIT)
                objhps += paf->modifier;
        }
    }

    hps = (ch->max_hit - (spellhps + objhps));

    if (hps > 10000) limit += 10;
    else limit += hps/1000;
    if ((hps -= 10000) > 10000) limit += 20;
    else if (hps > 0) limit += 2 * hps/1000;
    if ((hps -= 10000) > 10000) limit += 30;
    else if (hps > 0) limit += 3 * hps/1000;
    if ((hps -= 10000) > 10000) limit += 40;
    else if (hps > 0) limit += 4 * hps/1000;
    if ((hps -= 10000) > 10000) limit += 50;
    else if (hps > 0) limit += 5 * hps/1000;
    if ((hps -= 10000) > 10000) limit += 60;
    else if (hps > 0) limit += 6 * hps/1000;

    if (is_upgrade(ch)) limit *= (2 + ch->pcdata->upgrade_level);

    /*
     * For those with no skill
     */
    limit += ch->pdeath;

    if (limit > ch->pkill) return FALSE;
    else return TRUE;
}

void death_info(char *str)
{
    DESCRIPTOR_DATA *d;

    if (str[0] == '\0') return;

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING && d->character != NULL)
        {
            send_to_char("#C<- #RDeath #C->#n ", d->character);
            send_to_char(str, d->character);
            send_to_char("\n\r", d->character);
        }
    }
    return;
}

void avatar_info(char *str)
{
    DESCRIPTOR_DATA *d;

    if (str[0] == '\0') return;

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING && d->character != NULL)
        {
            send_to_char("#C<- #RAvatar #C->#n ", d->character);
            send_to_char(str, d->character);
            send_to_char("\n\r", d->character);
        }
    }
    return;
}

void leave_info(char *str)
{
    DESCRIPTOR_DATA *d;
    //if( IS_SET(ch->deaf, CHANNEL_CHAT)return;

    if (str[0] == '\0') return;

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING && d->character != NULL)
        {
            //if( IS_SET(d->character->deaf, CHANNEL_CHAT))return;

            send_to_char("#C<- #RLeaves #C->#n ", d->character);
            send_to_char(str, d->character);
            send_to_char("\n\r", d->character);
        }
    }
    return;
}

void enter_info(CHAR_DATA *ch, char *str)
{
    DESCRIPTOR_DATA *d;

    if (str[0] == '\0') return;
    //if( IS_SET(ch->deaf, CHANNEL_INFO)return;

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING && d->character != NULL)
        {

            //	if( IS_SET(d->character->deaf, CHANNEL_CHAT))return;
            if ( !fair_fight(ch, d->character) )
            {
                send_to_char("#C<- #REnters #C->#n ", d->character);
                send_to_char(str, d->character);
                send_to_char("\n\r", d->character);
            }
        }
    }
    return;
}

int getMight(CHAR_DATA *ch)
{
    AFFECT_DATA *paf;
    OBJ_DATA *obj;
    int spellhps = 0;
    int objhps = 0;
    int might, temp, i;

    if (!IS_CLASS(ch, CLASS_SAMURAI))
    {
        for (paf = ch->affected; paf != NULL; paf = paf->next)
        {
            if (paf->location == APPLY_HIT)
                spellhps += paf->modifier;
        }
    }

    for (i = 0; i < MAX_WEAR; i++)
    {
        if ((obj = get_eq_char(ch, i)) == NULL) continue;
        if (IS_CLASS(ch, CLASS_SAMURAI) && obj->pIndexData->vnum != 33177 && obj->pIndexData->vnum > 101099 && obj->pIndexData->vnum < 101000) continue;
        for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
        {
            if (paf->location == APPLY_HIT)
                objhps += paf->modifier;
        }
        for (paf = obj->affected; paf; paf = paf->next)
        {
            if (paf->location == APPLY_HIT)
                objhps += paf->modifier;
        }
    }

    might = (ch->max_hit - ((spellhps/2) + (objhps/2)))/1000;

    for (i = 0; i < 5; i++)  might += UMIN(2, ch->spl[i]/100);
    for (i = 0; i < 13; i++) might += UMIN(4, ch->wpn[i]/500);
    for (i = 1; i < 11; i++) might += UMIN(4, ch->stance[i]/500);
    if (IS_SET(ch->newbits, NEW_MASTERY)) might += 2;
    if (ch->stance[23] != -1) might += 250;
    else if (ch->stance[22] != -1) might += 200;
    else if (ch->stance[21] != -1) might += 150;
    else if (ch->stance[20] != -1) might += 100;
    else if (ch->stance[19] != -1) might += 50;
    else if (ch->generation >= 0) might += 30*(6-ch->generation);
    if (ch->pkill >= 1)
    {
        might += 100;
    }
    if (ch->pdeath*5 < ch->pkill*25)
    {
        might += ((ch->pkill*50)-(ch->pdeath*10));
    }
    /*  if ( ch->pcdata->bloodpower[0] > 0 )
        might += ch->pcdata->bloodpower[0];
      if ( ch->pcdata->bloodpower[1] > 0 )
        might += ch->pcdata->bloodpower[1];*/
    if (might >= 150)
    {
        if (is_upgrade(ch)) might *= 2;
        if (ch->pcdata->rune_count > 0) might += UMIN(150, ch->pcdata->rune_count * 10);
                                                            // 10 points for each pkpower.
        if ((temp = ch->race) >= 5) might += UMIN(100, temp / 5);
        if (ch->pcdata->upgrade_level > 0) might += ch->pcdata->upgrade_level * 50;
    }
    return might;
}

void forge_affect(OBJ_DATA *obj, int value)
{
    AFFECT_DATA paf;

    paf.type           = gsn_forge;
    paf.duration       = -1;
    paf.location       = APPLY_HITROLL;
    paf.modifier       = value;
    paf.bitvector      = 0;
    affect_to_obj(obj, &paf);

    paf.type           = gsn_forge;
    paf.duration       = -1;
    paf.location       = APPLY_DAMROLL;
    paf.modifier       = value;
    paf.bitvector      = 0;
    affect_to_obj(obj, &paf);
}

void forge_affect2(OBJ_DATA *obj, int value)
{
    AFFECT_DATA paf;

    paf.type           = gsn_forge;
    paf.duration       = -1;
    paf.location       = APPLY_HIT;
    paf.modifier       = value;
    paf.bitvector      = 0;
    affect_to_obj(obj, &paf);
}

void forge_affect3(OBJ_DATA *obj, int value)
{
    AFFECT_DATA paf;

    paf.type           = gsn_forge;
    paf.duration       = -1;
    paf.location       = APPLY_AC;
    paf.modifier       = -value;
    paf.bitvector      = 0;
    affect_to_obj(obj, &paf);
}

void dump_last_command()
{
    FILE *fp;
    char buf[MAX_STRING_LENGTH];

    fp = fopen("../src/crash.txt","a");
    fprintf (fp,"Last command typed : %s\n",last_command);
    fflush(fp);
    fclose(fp);

    /*
     * creates a note to the immortals
     */
    sprintf(buf, "It seems we have crashed, the last command typed was\n\r\n\r");
    strcat(buf, last_command);
    strcat(buf, "\n\r\n\rPlease remember that this doesn't mean that this caused the crash.\n\r\n\rRegards,\n\r\n\rThe Crash Code");
    make_note("Immortal", "Crash Code", "imm", "We Crashed", 7, buf);

}

void update_revision(CHAR_DATA *ch)
{
    int i;
    if (IS_NPC(ch)) return;
    if (ch->pcdata->revision == CURRENT_REVISION) return;

    /*
     * We don't end cases with break, since we want the player to be fully updated.
     */
    switch (ch->pcdata->revision)
    {
        case 1:
        {
            for (i = 0; i < 20; i++)
            {
                ch->pcdata->classone[i] = 0;
                ch->pcdata->classtwo[i] = 0;
                ch->pcdata->classthree[i] = 0;
                ch->pcdata->classfour[i] = 0;
            }
            ch->pcdata->revision++;
        }
        case 2:
        {
            if ( ch->pkill <= 0 ) {ch->pcdata->pkpoints = 0;}
            else
                {ch->pcdata->pkpoints = ch->pkill;}
                ch->pcdata->revision++;
        }
        case 3:
        {
            ch->pcdata->store = ch->pcdata->classone[0];
            ch->pcdata->classone[0] = 0;
            ch->pcdata->revision++;
        }
        case 4:
        {
            ch->pcdata->revision++;
        }
        case 5:
        {
            ch->pcdata->revision++;
        }
        case 6:
        {
            if ( ch->max_hit > 200000 ) ch->max_hit = 200000;
            if ( ch->max_mana > 200000 ) ch->max_mana = 200000;
            if ( ch->max_move > 200000 ) ch->max_move = 200000;
            do_restore(ch,"self");
            ch->pcdata->revision++;
        }
        case 7:
        {
            ch->pcdata->revision++;
        }
        case 8:
        {
            if ((ch->pkill - ch->pdeath) < 0)
            {
                ch->race = 0;
            }
            else
                ch->race = (ch->pkill - ch->pdeath);
            ch->pcdata->revision++;
        }
        case 9:
        {
            for (i = 0; i < 13; i++)
            {
                ch->wpn_lvl[i] = 0;
            }
            ch->pcdata->revision++;
        }
        case 10:
        {
            for (i = 0; i < 6; i++)
            {
                ch->pcdata->Bootcamp[i] = 0;
            }
            ch->pcdata->revision++;
        }
        case 11:
        {
            ch->pcdata->revision++;
        }
        case 12:
        {
            for (i = 0; i < 2; i++)
            {
                ch->pcdata->phoenix[i] = 0;
            }
            ch->pcdata->revision++;
        }
        case 13:
        {
            ch->act2 = 0;
            ch->pcdata->revision++;
        }

        default: break;
    }
    return;
}

void logout_message(CHAR_DATA *ch)
{
    static char * const he_she  [] = { "it", "he",  "she" };
    static char * const him_her [] = { "it", "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };

    DESCRIPTOR_DATA *d;
    char buf[400];                                          // that should be plenty.
    const char *dmess;
    const char *i;
    char *ptr2;
    char *ptr;
    int size;

    size = strlen2(ch->pcdata->logoutmessage);
    if (size > 380)
    {
        bug("Bad logoutmessage.",0);
        return;
    }

    ptr2  = "#C<- #RLeaves #C->#n ";
    ptr   = buf;
    dmess = ch->pcdata->logoutmessage;

    while ((*ptr = *ptr2) != '\0')
        ++ptr, ++ptr2;

    while (*dmess != '\0')
    {
        if ( *dmess != '$' )
        {
            *ptr++ = *dmess++;
            continue;
        }
        ++dmess;
        switch (*dmess)
        {
            default:  i = ""; break;
            case 'n': i = ch->name; break;
            case 'e': i = he_she  [URANGE(0, ch->sex, 1)]; break;
            case 'm': i = him_her [URANGE(0, ch->sex, 1)]; break;
            case 's': i = his_her [URANGE(0, ch->sex, 1)]; break;
        }
        ++dmess;
        /* copying the data into the pointer */
        while ((*ptr = *i) != '\0')
            ++ptr, ++i;
    }
    *ptr++ = '\n';
    *ptr++ = '\r';

    for (d = descriptor_list; d; d = d->next)
    {
        if (d->lookup_status != STATUS_DONE) continue;
        if (d->connected != CON_PLAYING ) continue;
        write_to_buffer( d, buf, ptr - buf );
    }
    return;
}

void tie_message(CHAR_DATA *ch, CHAR_DATA *victim)
{
    static char * const he_she  [] = { "it", "he",  "she" };
    static char * const him_her [] = { "it", "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };
    DESCRIPTOR_DATA *d;
    char buf[400];                                          // that should be plenty.
    const char *dmess;
    const char *i;
    char *ptr2;
    char *ptr;
    int size;

    size = strlen2(ch->pcdata->tiemessage);
    if (size > 380)
    {
        bug("Bad tiemessage.",0);
        return;
    }

    ptr2  = "#C<- #RTie #C->#n ";
    ptr   = buf;
    dmess = ch->pcdata->tiemessage;

    while ((*ptr = *ptr2) != '\0')
        ++ptr, ++ptr2;

    while (*dmess != '\0')
    {
        if ( *dmess != '$' )
        {
            *ptr++ = *dmess++;
            continue;
        }
        ++dmess;
        switch (*dmess)
        {
            default:  i = ""; break;
            case 'n': i = ch->name; break;
            case 'e': i = he_she  [URANGE(0, ch->sex, 1)]; break;
            case 'm': i = him_her [URANGE(0, ch->sex, 1)]; break;
            case 's': i = his_her [URANGE(0, ch->sex, 1)]; break;
            case 'N': i = victim->name; break;
            case 'S': i = his_her [URANGE(0, victim->sex, 1)]; break;
            case 'M': i = him_her [URANGE(0, victim->sex, 1)]; break;
            case 'E': i = he_she  [URANGE(0, victim->sex, 1)]; break;
        }
        ++dmess;
        /* copying the data into the pointer */
        while ((*ptr = *i) != '\0')
            ++ptr, ++i;
    }
    *ptr++ = '\n';
    *ptr++ = '\r';

    for (d = descriptor_list; d; d = d->next)
    {
        if (d->lookup_status != STATUS_DONE) continue;
        if (d->connected != CON_PLAYING ) continue;
        write_to_buffer( d, buf, ptr - buf );
    }
    return;
}

void login_message(CHAR_DATA *ch)
{
    static char * const he_she  [] = { "it", "he",  "she" };
    static char * const him_her [] = { "it", "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };

    DESCRIPTOR_DATA *d;
    char buf[400];                                          // that should be plenty.
    const char *dmess;
    const char *i;
    char *ptr2;
    char *ptr;
    int size;

    size = strlen2(ch->pcdata->loginmessage);
    if (size > 380)
    {
        bug("Bad loginmessage.",0);
        return;
    }

    ptr2  = "#C<- #REnters #C->#n ";
    ptr   = buf;
    dmess = ch->pcdata->loginmessage;

    while ((*ptr = *ptr2) != '\0')
        ++ptr, ++ptr2;

    while (*dmess != '\0')
    {
        if ( *dmess != '$' )
        {
            *ptr++ = *dmess++;
            continue;
        }
        ++dmess;
        switch (*dmess)
        {
            default:  i = ""; break;
            case 'n': i = ch->name; break;
            case 'e': i = he_she  [URANGE(0, ch->sex, 1)]; break;
            case 'm': i = him_her [URANGE(0, ch->sex, 1)]; break;
            case 's': i = his_her [URANGE(0, ch->sex, 1)]; break;
        }
        ++dmess;
        /* copying the data into the pointer */
        while ((*ptr = *i) != '\0')
            ++ptr, ++i;
    }
    *ptr++ = '\n';
    *ptr++ = '\r';

    for (d = descriptor_list; d; d = d->next)
    {
        if (d->lookup_status != STATUS_DONE) continue;
        if (d->connected != CON_PLAYING ) continue;
        if ( !fair_fight(ch, d->character) )
            write_to_buffer( d, buf, ptr - buf );
    }
    return;
}

void special_decap_message(CHAR_DATA *ch, CHAR_DATA *victim)
{
    static char * const he_she  [] = { "it", "he",  "she" };
    static char * const him_her [] = { "it", "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };
    DESCRIPTOR_DATA *d;
    char buf[400];                                          // that should be plenty.
    const char *dmess;
    const char *i;
    char *ptr2;
    char *ptr;
    int size;

    size = strlen2(ch->pcdata->decapmessage);
    if (size > 380)
    {
        bug("Bad decapmessage.",0);
        return;
    }

    ptr2  = "#C<- #RDeath #C->#n ";
    ptr   = buf;
    dmess = ch->pcdata->decapmessage;

    while ((*ptr = *ptr2) != '\0')
        ++ptr, ++ptr2;

    while (*dmess != '\0')
    {
        if ( *dmess != '$' )
        {
            *ptr++ = *dmess++;
            continue;
        }
        ++dmess;
        switch (*dmess)
        {
            default:  i = ""; break;
            case 'n': i = ch->name; break;
            case 'e': i = he_she  [URANGE(0, ch->sex, 1)]; break;
            case 'm': i = him_her [URANGE(0, ch->sex, 1)]; break;
            case 's': i = his_her [URANGE(0, ch->sex, 1)]; break;
            case 'N': i = victim->name; break;
            case 'S': i = his_her [URANGE(0, victim->sex, 1)]; break;
            case 'M': i = him_her [URANGE(0, victim->sex, 1)]; break;
            case 'E': i = he_she  [URANGE(0, victim->sex, 1)]; break;
        }
        ++dmess;
        /* copying the data into the pointer */
        while ((*ptr = *i) != '\0')
            ++ptr, ++i;
    }
    *ptr++ = '\n';
    *ptr++ = '\r';

    for (d = descriptor_list; d; d = d->next)
    {
        if (d->lookup_status != STATUS_DONE) continue;
        if (d->connected != CON_PLAYING ) continue;
        write_to_buffer( d, buf, ptr - buf );
    }
    return;
}

void avatar_message(CHAR_DATA *ch)
{
    static char * const he_she  [] = { "it", "he",  "she" };
    static char * const him_her [] = { "it", "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };

    DESCRIPTOR_DATA *d;
    char buf[400];                                          // that should be plenty.
    const char *dmess;
    const char *i;
    char *ptr2;
    char *ptr;
    int size;

    size = strlen2(ch->pcdata->avatarmessage);
    if (size > 380)
    {
        bug("Bad avatarmessage.",0);
        return;
    }

    ptr2  = "#C<- #RAvatar #C->#n ";
    ptr   = buf;
    dmess = ch->pcdata->avatarmessage;

    while ((*ptr = *ptr2) != '\0')
        ++ptr, ++ptr2;

    while (*dmess != '\0')
    {
        if ( *dmess != '$' )
        {
            *ptr++ = *dmess++;
            continue;
        }
        ++dmess;
        switch (*dmess)
        {
            default:  i = ""; break;
            case 'n': i = ch->name; break;
            case 'e': i = he_she  [URANGE(0, ch->sex, 1)]; break;
            case 'm': i = him_her [URANGE(0, ch->sex, 1)]; break;
            case 's': i = his_her [URANGE(0, ch->sex, 1)]; break;
        }
        ++dmess;
        /* copying the data into the pointer */
        while ((*ptr = *i) != '\0')
            ++ptr, ++i;
    }
    *ptr++ = '\n';
    *ptr++ = '\r';

    for (d = descriptor_list; d; d = d->next)
    {
        if (d->lookup_status != STATUS_DONE) continue;
        if (d->connected != CON_PLAYING ) continue;
        write_to_buffer( d, buf, ptr - buf );
    }
    return;
}

void recycle_descriptors()
{
    DESCRIPTOR_DATA *dclose;
    DESCRIPTOR_DATA *dclose_next;

    for (dclose = descriptor_list; dclose; dclose = dclose_next)
    {
        dclose_next = dclose->next;
        if (dclose->lookup_status != STATUS_CLOSED) continue;

        /*
         * First let's get it out of the descriptor list.
         */
        if ( dclose == descriptor_list )
        {
            descriptor_list = descriptor_list->next;
        }
        else
        {
            DESCRIPTOR_DATA *d;

            for (d = descriptor_list; d && d->next != dclose; d = d->next)
                ;
            if (d != NULL)
                d->next = dclose->next;
            else
            {
                bug( "Recycle_descriptors: dclose not found.", 0 );
                continue;
            }
        }

        /*
         * Clear out that memory
         */
        free_string( dclose->host );
        free_mem( dclose->outbuf, dclose->outsize );

        /*
         * Mccp
         */
        if (dclose->out_compress)
        {
            deflateEnd(dclose->out_compress);
            free_mem(dclose->out_compress_buf, COMPRESS_BUF_SIZE);
            free_mem(dclose->out_compress, sizeof(z_stream));
        }

        /*
         * Bye bye mr. Descriptor.
         */
        close( dclose->descriptor );

        /* 
         * And then we recycle
         */
        dclose->next        = descriptor_free;
        descriptor_free     = dclose;
    }
}

void do_startragbid(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];

    if(ragnarok || ragnarok_safe_timer == PULSE_RAGNAROK)
    {
        stc("Ragnarok is already going or bidding is begun",ch);
        return;
    }

    ragnarok_on_timer = PULSE_RAGNAROK;
    ragnarok_safe_timer = 0;
    sprintf(buf, "Bidding on Ragnarok has begun, place your bids now.");
    do_info(ch, buf);
    return;
}

void update_ragnarok()
{
    if (ragnarok_safe_timer > 0)
        ragnarok_safe_timer--;
    if (--ragnarok_on_timer <= 0 && ragnarok)
    {
        ragnarok_stop();
        ragnarok_safe_timer = 120;                          /* 120 minutes of safe after a ragnarok */
    }
}

void update_doubleexp()
{
    if (pulse_doubleexp > 0)
    {
        if (--pulse_doubleexp == 0)
        {
            global_exp = FALSE;
            pulse_doubleexp = -1;
            do_info(NULL,"#GH#Ra#Gp#Rp#Gy #GH#Ro#Gu#Rr #0(#7ENDED#0)#n");
        }
    }
}

void update_doubleqps()
{
    if (pulse_doubleqp > 0)
    {
        if ( --pulse_doubleqp == 0)
        {
            global_qp = FALSE;
            pulse_doubleqp = -1;
            do_info(NULL,"#GQ#RU#GE#RS#GT#RO#GR#RS #GD#RE#GL#RI#GG#RH#GT #0(#7ENDED#0)#n");
        }
    }
}

void update_arena()
{
    if (--pulse_arena <= 1)
    {
        if (pulse_arena > 0) open_arena();
        else if (pulse_arena == 0)
        {
            pulse_arena = 15;
            close_arena();
        }
    }
}

void recycle_dummys()
{
    DUMMY_ARG *dummy;
    DUMMY_ARG *dummy_next;

    for (dummy = dummy_list; dummy; dummy = dummy_next)
    {
        dummy_next = dummy->next;
        if (dummy->status == 1) continue;                   // being used

        if (dummy == dummy_list)
        {
            dummy_list = dummy_list->next;
        }
        else
        {
            DUMMY_ARG *prev;

            /* we find the prev dummy arg */
            for (prev = dummy_list; prev && prev->next != dummy; prev = prev->next)
                ;
            if (prev != NULL)
                prev->next = dummy->next;
            else
            {
                bug( "Recycle_dymmys: dummy not found.", 0 );
                continue;
            }

            /* recycle */
            dummy->next = dummy_free;
            dummy_free = dummy;
        }
    }
}
