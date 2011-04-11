#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "merc.h"

#define OBJ_VNUM_RUNE       83020
#define OBJ_VNUM_RUNEWEAPON 83021
#define OBJ_VNUM_RUNEROBE   83022
#define OBJ_VNUM_RUNESCROLL 83023

char const *runes[10] = { "none", "ice", "fire", "lightning", "earth", "chaos", "poison", "dark", "light", "universal" };
RUNE_TYPE rune_global = {0,0,0,0,0,0,0,0,0,0};

void do_runecreate( CHAR_DATA *ch, char *argument )
{
    const RUNE_TYPE costs = { 0, 500, 500, 500, 500, 1000, 250, 100, 100, 5000 };
    int i;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if ( !IS_CLASS(ch,CLASS_RUNEMASTER) )
        return stcf(ch,"Huh?\n\r");

    for ( i = 1; i <= 9; i++ )
        if ( !str_cmp(arg,runes[i]) )
    {
        // They want a rune.
        OBJ_DATA *rune;
        char buf[MAX_STRING_LENGTH];

        if ( i == 9 )
        {
            // Universal rune costs qps
            if ( ch->pcdata->quest < costs[9] )
                return stcf(ch,"#RYou do not have the required #r%d#R quest points.#n\n\r",costs[9]);
            ch->pcdata->quest -= costs[9];
        }
        else
        {
            if ( ch->practice < costs[i] )
                return stcf(ch,"#RYou do not have the required #r%d#R primal.#n\n\r",costs[i]);
            ch->practice -= costs[i];
        }
        if ( !(rune = create_object(get_obj_index(OBJ_VNUM_RUNE),100)) )
            return stcf(ch,"Error.  Could not create object.  Please report this to an immortal.\n\r");
        sprintf(buf,rune->name,runes[i]);
        free_string(rune->name); rune->name = str_dup(buf);
        sprintf(buf,rune->short_descr,runes[i]);
        free_string(rune->short_descr); rune->short_descr = str_dup(buf);
        free_string(rune->questmaker); rune->questmaker = str_dup(ch->name);
        free_string(rune->questowner); rune->questowner = str_dup(ch->name);
        rune->value[0] = i;
        obj_to_char(rune,ch);
        stcf(ch,"#PA %s rune appears in your hands.#n\n\r",runes[i]);
        act("#PA rune appears in $n's hands.#n\n\r",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( !str_cmp(arg,"robe") )
    {
        // They want a robe.
        OBJ_DATA *robe;

        if ( ch->practice < 1000 )
            return stcf(ch,"#RYou do not have the required #r1000#R primal.#n\n\r");
        ch->practice -= 1000;
        if ( !(robe = create_object(get_obj_index(OBJ_VNUM_RUNEROBE),100)) )
            return stcf(ch,"Error.  Could not create object.  Please report this to an immortal.\n\r");
        free_string(robe->questmaker); robe->questmaker = str_dup(ch->name);
        free_string(robe->questowner); robe->questowner = str_dup(ch->name);
        free_string(robe->chpoweruse); robe->chpoweruse = str_dup("-000000000");
        obj_to_char(robe,ch);
        stcf(ch,"#PA robe appears in your hands.#n\n\r");
        act("#PA robe appears in $n's hands.#n\n\r",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( !str_cmp(arg,"weapon") )
    {
        // They want a weapon.
        OBJ_DATA *weapon;
        int weapon_type = 0;
        char buf[MAX_STRING_LENGTH];
        char arg2[MAX_INPUT_LENGTH];
        char ing[MAX_STRING_LENGTH];
        int last;

        one_argument(argument,arg2);

        /*if ( (weapon_type = flag_value(weapon_flags,arg2)) == NO_FLAG )
            return stcf(ch,"#RThat is not a valid weapon type.#n\n\r");*/
        if ( ch->practice < 1500 )
            return stcf(ch,"#RYou do not have the required #r1500#R primal.#n\n\r");
        if ( (weapon_type == 9 || weapon_type == 10) && ch->pcdata->quest < 1000 )
            return stcf(ch,"#RCreating grep or bite weapons costs 1000 qps.#n\n\r");
        else ch->pcdata->quest -= 1000;
        ch->practice -= 1500;
        if ( !(weapon = create_object(get_obj_index(OBJ_VNUM_RUNEWEAPON),100)) )
            return stcf(ch,"Error.  Could not create object.  Please report this to an immortal.\n\r");
        last = strlen(ing) - 1;
        if ( ing[last] == 'e' ) ing[last] = '\0';
        strcat(ing,"ing");
        sprintf(buf,weapon->name,arg2);
        free_string(weapon->name); weapon->name = str_dup(buf);
        sprintf(buf,weapon->short_descr,arg2);
        free_string(weapon->short_descr); weapon->short_descr = str_dup(buf);
        sprintf(buf,weapon->description,ing);
        free_string(weapon->description); weapon->description = str_dup(buf);
        free_string(weapon->questmaker); weapon->questmaker = str_dup(ch->name);
        free_string(weapon->questowner); weapon->questowner = str_dup(ch->name);
        free_string(weapon->chpoweruse); weapon->chpoweruse = str_dup("-000000000");
        weapon->value[3] = weapon_type;
        obj_to_char(weapon,ch);
        stcf(ch,"#PA %s weapon appears in your hands.#n\n\r",ing);
        act("#PA weapon made of #Lblue #7silver #Pappears in $n's hands.#n\n\r",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( !str_cmp(arg,"scroll") )
    {
        // They want a scroll.
        OBJ_DATA *scroll;

        if ( ch->pcdata->quest < 5000 )
            return stcf(ch,"#RYou do not have the required #r5000#R quest points.#n\n\r");
        ch->pcdata->quest -= 5000;
        if ( !(scroll = create_object(get_obj_index(OBJ_VNUM_RUNESCROLL),100)) )
            return stcf(ch,"Error.  Could not create object.  Please report this to an immortal.\n\r");
        free_string(scroll->questmaker); scroll->questmaker = str_dup(ch->name);
        free_string(scroll->questowner); scroll->questowner = str_dup(ch->name);
        free_string(scroll->chpoweruse); scroll->chpoweruse = str_dup("-000000000");
        obj_to_char(scroll,ch);
        stcf(ch,"#PA scroll appears in your hands.#n\n\r");
        act("#PA scroll appears in $n's hands.#n\n\r",ch,NULL,NULL,TO_ROOM);
        return;
    }

    // They don't know what they want, or they didn't ask.
    stcf(ch,"#RRunecreate:#n\n\r  #rRunes: #Rrunecreate <rune>#n\n\r    ");
    for ( i = 1; i <= 3; i++ )
        stcf(ch,"#P%-9s #p%4d primal  ",runes[i],costs[i]);
    stcf(ch,"\n\r");
    for ( i = 4; i <= 6; i++ )
        stcf(ch,"#P%-9s #p%4d primal  ",runes[i],costs[i]);
    stcf(ch,"\n\r");
    for ( i = 7; i <= 8; i++ )
        stcf(ch,"#P%-9s #p%4d primal  ",runes[i],costs[i]);
    stcf(ch,"#P%-9s #p%4d quest points\n\r",runes[9],costs[9]);
    stcf(ch,"  #rRobes: #Rrunecreate robe#n - #p1000 primal#n\n\r");
    stcf(ch,"  #rWeapon: #Rrunecreate weapon <type>#n - #p1500 primal#n\n\r");
    stcf(ch,"    #PGrep and bite weapons costs an additional 1000 quest points.#n\n\r");
    stcf(ch,"  #rScroll: #Rrunecreate scroll#n - #p5000 quest points#n\n\r");
}

void do_runetrain( CHAR_DATA *ch, char *argument )
{
    int cost = 0;

    if ( !IS_CLASS(ch,CLASS_RUNEMASTER) )
        return stcf(ch,"Huh?\n\r");

    if ( !str_cmp(argument,"forge") )
    {
        if ( ch->pcdata->powers[RUNEM_FORGE] == 10 )
            return stcf(ch,"#RYou have learned all you can about rune forging.#n\n\r");
        cost = 1000 * (ch->pcdata->powers[RUNEM_FORGE] + 1);
        if ( ch->practice < cost )
            return stcf(ch,"#RYou do not have the required #r%d#R primal.#n\n\r",cost);
        ch->practice -= cost;
        ch->pcdata->powers[RUNEM_FORGE]++;
        return stcf(ch,"#PYou have increased your rune forging ability.#n\n\r");
    }

    if ( !str_cmp(argument,"weave") )
    {
        if ( ch->pcdata->powers[RUNEM_WEAVE] == 10 )
            return stcf(ch,"#RYou have learned all you can about rune weaving.#n\n\r");
        cost = 1000 * (ch->pcdata->powers[RUNEM_WEAVE] + 1);
        if ( ch->practice < cost )
            return stcf(ch,"#RYou do not have the required #r%d#R primal.#n\n\r",cost);
        ch->practice -= cost;
        ch->pcdata->powers[RUNEM_WEAVE]++;
        return stcf(ch,"#PYou have increased your rune weaving ability.#n\n\r");
    }

    stcf(ch,"#RRunetrain:#n\n\r  #RForge #r- #P%d  #RWeave #r- #P%d#n\n\r",
        ch->pcdata->powers[RUNEM_FORGE], ch->pcdata->powers[RUNEM_WEAVE] );
    stcf(ch,"#RType #rrunetrain <forge/weave>#R to improve.#n\n\r");
}

void do_runeforge( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *runeobj;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int i;
    bool found = FALSE;

    if ( !IS_CLASS(ch,CLASS_RUNEMASTER) )
        return stcf(ch,"Huh?\n\r");

    argument = one_argument(argument,arg);
    argument = one_argument(argument,arg2);

    if ( !*arg )
        return stcf(ch,"#RRuneforge: #rruneforge <item> <rune>#n\n\r");

    if ( !(obj = get_obj_carry(ch,arg)) )
        return stcf(ch,"#RYou don't have that.#n\n\r");

    if ( obj->pIndexData->vnum != OBJ_VNUM_RUNEROBE &&
        obj->pIndexData->vnum != OBJ_VNUM_RUNEWEAPON )
        return stcf(ch,"#RYou can't forge runes onto that!#n\n\r");

    for ( i = 1; i <= 9; i++ )
        if ( !str_cmp(arg2,runes[i]) )
            { found = TRUE; break; }

            if ( !found )
                return stcf(ch,"#RThat's not a valid rune!");

    for ( runeobj = ch->carrying; runeobj; runeobj = runeobj->next )
        if ( runeobj->item_type == ITEM_RUNE && runeobj->value[0] == i )
            break;

    if ( !runeobj ) return stcf(ch,"#RYou aren't carrying that rune!#n\n\r");

    rune_count(obj);
    if ( rune_global[0] >= ch->pcdata->powers[RUNEM_FORGE] )
        return stcf(ch,"#RYou have forged as many runes as you can on this item.#n\n\r");
    rune_global[i]++;
    set_runes(obj);
    extract_obj(runeobj);

    if (obj->item_type == ITEM_WEAPON) switch(i)
    {
        case 1:     oset_affect(ch,obj,15,APPLY_HITROLL,FALSE);
        SET_BIT(obj->weapflags,WEAPON_FROST);       break;
        case 2:     oset_affect(ch,obj,15,APPLY_DAMROLL,FALSE);
        SET_BIT(obj->weapflags,WEAPON_FLAMING);     break;
        case 3:     oset_affect(ch,obj, 5,APPLY_HITROLL,FALSE);
        oset_affect(ch,obj, 5,APPLY_DAMROLL,FALSE);
        SET_BIT(obj->weapflags,WEAPON_SHOCKING);    break;
        case 4:     oset_affect(ch,obj,15,APPLY_AC,     FALSE);
        SET_BIT(obj->weapflags,WEAPON_VORPAL);      break;
        case 5:     if (rune_global[i]==1) obj->value[0] += 15000;  break;
        case 6:     SET_BIT(obj->weapflags,WEAPON_POISON);      break;
        case 7:     if (rune_global[i]==1) obj->value[0] += 4;  break;
        default:
        case 8:
        case 9:     break;
    }

    if (obj->item_type == ITEM_ARMOR) switch(i)
    {
        case 1:     oset_affect(ch,obj,15,APPLY_HITROLL,FALSE); break;
        case 2:     oset_affect(ch,obj,15,APPLY_DAMROLL,FALSE); break;
        case 3:     oset_affect(ch,obj, 5,APPLY_HITROLL,FALSE);
        oset_affect(ch,obj, 5,APPLY_DAMROLL,FALSE); break;
        case 4:     oset_affect(ch,obj,15,APPLY_AC,     FALSE); break;
        case 5:     if (rune_global[i]==1) obj->value[3] += 15; break;
        default:
        case 6:
        case 7:
        case 8:
        case 9:     break;
    }

    stcf(ch,"#PYou forge #L%s#P into %s#P.#n\n\r",runes[i],obj->short_descr);
    act("#P$n touches $p and it glows with energy.#n",ch,obj,NULL,TO_ROOM);
}

void do_runeweave( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *runeobj;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int i;
    bool found = FALSE;

    if ( !IS_CLASS(ch,CLASS_RUNEMASTER) )
        return stcf(ch,"Huh?\n\r");

    argument = one_argument(argument,arg);
    argument = one_argument(argument,arg2);

    if ( !*arg )
        return stcf(ch,"#RRuneweave: #rruneweave <scroll> <rune>#n\n\r");

    if ( !(obj = get_obj_carry(ch,arg)) )
        return stcf(ch,"#RYou don't have that.#n\n\r");

    if ( obj->item_type != ITEM_RUNESCROLL )
        return stcf(ch,"#RYou can't weave runes onto that!#n\n\r");

    for ( i = 1; i <= 9; i++ )
        if ( !str_cmp(arg2,runes[i]) )
            { found = TRUE; break; }

            if ( !found )
                return stcf(ch,"#RThat's not a valid rune!");

    for ( runeobj = ch->carrying; runeobj; runeobj = runeobj->next )
        if ( runeobj->item_type == ITEM_RUNE && runeobj->value[0] == i )
            break;

    if ( !runeobj ) return stcf(ch,"#RYou aren't carrying that rune!#n\n\r");

    rune_count(obj);
    if ( rune_global[0] >= ch->pcdata->powers[RUNEM_WEAVE] )
        return stcf(ch,"#RYou have weaved as many runes as you can on this scroll.#n\n\r");
    rune_global[i]++;
    set_runes(obj);
    extract_obj(runeobj);

    stcf(ch,"#PYou weave #L%s#P onto %s#P.#n\n\r",runes[i],obj->short_descr);
    act("#P$n touches $p and it glows with energy.#n",ch,obj,NULL,TO_ROOM);
}

void do_runecount( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int i, count = 0;

    if ( !*argument )
        return stcf(ch,"#RRunecount: #rrunecount <object>#n\n\r");

    if ( !(obj = get_obj_carry(ch,argument)) )
        return stcf(ch,"#RYou are not carrying that.#n\n\r");

    if ( obj->item_type != ITEM_RUNESCROLL &&
        obj->pIndexData->vnum != OBJ_VNUM_RUNEWEAPON &&
        obj->pIndexData->vnum != OBJ_VNUM_RUNEROBE )
        return stcf(ch,"#RThat item cannot have runes!#n\n\r");

    rune_count(obj);
    stcf(ch,"#PRunes on %s#P:\n\r",obj->short_descr);
    for ( i = 1, count = -1; i <= 9; i++ )
        if ( rune_global[i] )
    {
        stcf(ch,"%s#7%d #L%s rune%s", (count<1)?"":", ",
            rune_global[i], runes[i], (rune_global[i]>1)?"s":"" );
        if ( count == -1 ) count = 0;
        if ( (++count)%5 == 4 ) stcf(ch,"\n\r");
    }
    if ( count == -1 ) return stcf(ch," None.\n\r");
    if ( count%5 != 0 ) stcf(ch,"\n\r");
}

void do_runeread( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *scroll;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg);
    argument = one_argument(argument, arg2);

    if ( !(scroll = get_obj_carry(ch,arg)) )
        return stcf(ch,"#RYou don't have that.#n\n\r");

    if ( scroll->item_type != ITEM_RUNESCROLL )
        return stcf(ch,"#RThat is not a rune scroll!#n\n\r");

    strcpy(buf,scroll->chpoweruse);
    if ( !IS_CLASS(ch,CLASS_RUNEMASTER) && buf[9] != '1' && buf[9] != 'x')
        return stcf(ch,"#RThat is not a universal scroll.#n\n\r");

    if (rune_spell(ch,arg2,scroll))
    {
        obj_from_char(scroll);
        extract_obj(scroll);
    }
}

void rune_count( OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    sprintf(buf,"%s",obj->chpoweruse);

    rune_global[0] = 0;
    for ( i = 1; i <= 9; i++ )
    {
        if ( buf[i] == 'x' )
            rune_global[i] = 10;
        else rune_global[i] = (buf[i] - '1') + 1;
        rune_global[0] += rune_global[i];
    }
}

void set_runes( OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    buf[0] = '-';
    for ( i = 1; i <= 9; i++ )
        if ( rune_global[i] == 10 ) buf[i] = 'x';
    else buf[i] = (rune_global[i] + '1') - 1;

    free_string(obj->chpoweruse);
    obj->chpoweruse = str_dup(buf);
}

RUNEA_TYPE  rune_attack_table[] =
{
    {   "200000000",     1, ra_ice_slash    },
    {   "020000000",     1, ra_fire_slash   },
    {   "300000000",     2, ra_ice_slash    },
    {   "030000000",     2, ra_fire_slash   },
    {   "230010000",     1, ra_fi_slash },
    {   "450010000",     2, ra_fi_slash },
    {   "090010000",    -1, ra_fire_slash   },
    {   "0000x0000",     1, ra_chaos_slash  },
    {   "331400000",     1, ra_elem_slash   },
    {   "222220000",     2, ra_elem_slash   },
    {   "000010900",     1, ra_demon_slash  },
    {   "000060400",     2, ra_demon_slash  },
    {   "005500000",     1, ra_shatter_slash},
    {   "004420000",     2, ra_shatter_slash},
    {   "010010800",     1, ra_dark_slash   },
    {   "000000x00",    -1, ra_dark_slash   },
    {   "000400000",     1, ra_ltn_slash    },
    {   "000800000",     2, ra_ltn_slash    },

    // End of list.
    {   "",     0,  NULL        }
};

RUNES_TYPE  rune_spell_table[]  =
{
    {   "020000000",     1, 1, RS_TAR,  rs_fire_wave    },
    {   "040000000",     1, 1, RS_TAR,  rs_fire_ball    },
    {   "060000000",     1, 1, RS_TAR,  rs_fire_blast   },
    {   "080020000",     3, 2, RS_TAR,  rs_chaos_flames },
    {   "200000000",     1, 1, RS_TAR,  rs_ice_shot },
    {   "400000000",     1, 1, RS_TAR,  rs_ice_blast    },
    {   "600000000",     1, 1, RS_TAR,  rs_ice_storm    },
    {   "800020000",     3, 2, RS_TAR,  rs_chaos_ice    },
    {   "002000000",     1, 1, RS_TAR,  rs_lit_bolt },
    {   "004000000",     1, 1, RS_TAR,  rs_lit_strike   },
    {   "006000000",     1, 1, RS_TAR,  rs_lit_blast    },
    {   "008020000",     4, 2, RS_TAR,  rs_lit_chain    },
    {   "000200000",     1, 1, RS_NTAR, rs_earth_shake  },
    {   "000600000",     3, 2, RS_NTAR, rs_earth_shake  },
    {   "000000400",     2, 2, RS_TAR,  rs_daemon_flare },
    {   "000020800",     4, 3, RS_TAR,  rs_daemon_flare },
    {   "000000040",     1, 1, RS_DEF,  rs_holy_touch   },
    {   "000000080",     2, 2, RS_DEF,  rs_holy_touch   },
    {   "222200000",     4, 2, RS_TAR,  rs_elemental    },
    {   "222220000",     2, 3, RS_NTAR, rs_elem_room    },
    {   "333010000",     4, 3, RS_TAR,  rs_chaos_blast  },
    {   "330000000",     1, 2, RS_TAR,  rs_fire_ice },
    {   "303000000",     2, 3, RS_TAR,  rs_ice_charge   },
    {   "033000000",     2, 3, RS_TAR,  rs_fire_charge  },
    {   "000000109",    10, 5, RS_TAR,  rs_darkmat  },
    {   "00000000x",     2, 4, RS_NTAR, rs_wasteqp  },
    {   "550000000",    -2, 2, RS_TAR,  rs_fire_ice },
    {   "055000000",     3, 3, RS_NTAR, rs_lit_storm    },
    {   "000000x00",     1, 4, RS_NTAR, rs_demon_conv   },
    {   "0000000x0",     9, 4, RS_TAR,  rs_angel_conv   },
    {   "000000019",     4, 5, RS_TAR,  rs_disintegrate },

    // End of list.
    {   "",      0, 0, 0,       NULL        }
};

void rune_attack( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *weapon1;
    OBJ_DATA *weapon2;
    bool weap1 = FALSE;
    bool weap2 = FALSE;
    int i, j;

    if ( (weapon1 = get_eq_char(ch,WEAR_WIELD)) &&
        weapon1->pIndexData->vnum == OBJ_VNUM_RUNEWEAPON )
        weap1 = TRUE;
    if ( (weapon2 = get_eq_char(ch,WEAR_HOLD)) &&
        weapon2->pIndexData->vnum == OBJ_VNUM_RUNEWEAPON )
        weap2 = TRUE;

    if ( !weap1 && !weap2 ) return;

    if ( weap1 && (i = find_rune_attack( weapon1->chpoweruse )) != -1 )
    {
        if ( rune_attack_table[i].number > 0 )
            for ( j = 0; j < rune_attack_table[i].number; j++ )
                (*rune_attack_table[i].function) (ch, victim);
        else if ( rune_attack_table[i].number < 0 )
            for ( j = 0; j > rune_attack_table[i].number; j-- )
                (*rune_attack_table[i].function) (ch, ch);
    }

    if ( weap2 && (i = find_rune_attack( weapon2->chpoweruse )) != -1 )
    {
        if ( rune_attack_table[i].number > 0 )
            for ( j = 0; j < rune_attack_table[i].number; j++ )
                (*rune_attack_table[i].function) (ch, victim);
        else if ( rune_attack_table[i].number < 0 )
            for ( j = 0; j > rune_attack_table[i].number; j-- )
                (*rune_attack_table[i].function) (ch, ch);
    }
}

bool is_rune_weapon( CHAR_DATA *ch, char *rune )
{
    OBJ_DATA *weapon1;
    OBJ_DATA *weapon2;
    bool weap1 = FALSE;
    bool weap2 = FALSE;

    if ( (weapon1 = get_eq_char(ch,WEAR_WIELD)) &&
        weapon1->pIndexData->vnum == OBJ_VNUM_RUNEWEAPON )
        weap1 = TRUE;
    if ( (weapon2 = get_eq_char(ch,WEAR_HOLD)) &&
        weapon2->pIndexData->vnum == OBJ_VNUM_RUNEWEAPON )
        weap2 = TRUE;

    if ( !weap1 && !weap2 ) return FALSE;

    if ( weap1 && !str_cmp(weapon1->chpoweruse+1,rune) )
        return TRUE;

    if ( weap2 && !str_cmp(weapon2->chpoweruse+1,rune) )
        return TRUE;

    return FALSE;
}

bool is_rune_robe( CHAR_DATA *ch, char *rune )
{
    OBJ_DATA *robe;
    bool found = FALSE;

    if ( (robe = get_eq_char(ch,WEAR_BODY)) &&
        robe->pIndexData->vnum == OBJ_VNUM_RUNEROBE )
        found = TRUE;

    if ( !found ) return FALSE;

    if ( robe && robe->chpoweruse &&
        !str_cmp(robe->chpoweruse+1,rune) )
        return TRUE;

    return FALSE;
}

bool is_rune_robe_obj( OBJ_DATA *robe, char *rune )
{
    if ( robe->pIndexData->vnum != OBJ_VNUM_RUNEROBE )
        return FALSE;

    if ( robe && robe->chpoweruse &&
        !str_cmp(robe->chpoweruse+1,rune) )
        return TRUE;

    return FALSE;
}

int find_rune_attack( char *rune )
{
    int i;

    for ( i = 0; ; i++ )
        if ( !rune_attack_table[i].runes[0] ) break;
    else if ( !str_cmp(rune_attack_table[i].runes,rune+1) )
        return i;

    return -1;
}

bool rune_spell( CHAR_DATA *ch, char *nvict, OBJ_DATA *scroll )
{
    int i, j;
    CHAR_DATA *victim;

    if ( (i = find_rune_spell( scroll->chpoweruse )) != -1 )
    {
        if ( rune_spell_table[i].type == RS_NTAR ) victim = NULL;
        else if ( !(victim = get_char_room(ch,nvict)) )
        {
            if ( rune_spell_table[i].type == RS_TAR && ch->fighting )
                victim = ch->fighting;
            else if (rune_spell_table[i].type == RS_DEF )
                victim = ch;
            else
            {
                stcf(ch,"#RThey aren't here.#n\n\r");
                return FALSE;
            }
        }

        if ( ++scroll->value[0] < rune_spell_table[i].times )
        {
            act("#LYou begin reading $p.#n",ch,scroll,NULL,TO_CHAR);
            act("#L$n begins reading $p.#n",ch,scroll,NULL,TO_ROOM);
            scroll->timer = 1;
            return FALSE;
        }

        if ( rune_spell_table[i].number > 0 )
            for ( j = 0; j < rune_spell_table[i].number; j++ )
                (*rune_spell_table[i].function) (ch, victim);
        else if ( rune_spell_table[i].number < 0 )
            for ( j = 0; j > rune_spell_table[i].number; j-- )
                (*rune_spell_table[i].function) (ch, ch);
        return TRUE;
    }
    act("#G$p#G burns away in a cloud of green smoke.#n",ch,scroll,NULL,TO_CHAR);
    act("#G$p#G burns away in a cloud of green smoke.#n",ch,scroll,NULL,TO_ROOM);
    return TRUE;
}

int find_rune_spell( char *rune )
{
    int i;

    for ( i = 0; ; i++ )
        if ( !rune_spell_table[i].runes[0] ) break;
    else if ( !str_cmp(rune_spell_table[i].runes,rune+1) )
        return i;

    return -1;
}

/*
 * Damage messages.
 */
void dam_word( int dam, char *pvs, char *pvp )
{
    const char * vs;
    const char * vp;

    if ( dam ==   0 ) { vs = " misses";           vp = " misses";             }
    else if ( dam <=  10 ) { vs = " tickles";           vp = " tickles ";           }
    else if ( dam <= 20 ) { vs = " maims";          vp = " maims";              }
    else if ( dam <= 30 ) { vs = " scratchs";           vp = " scratches";          }
    else if ( dam <= 40 ) { vs = " nicks";          vp = " nicks";              }
    else if ( dam <= 50 ) { vs = " grazes";         vp = " grazes";             }
    else if ( dam <= 60) { vs = " strikes";         vp = " strikes";            }
    else if ( dam <= 70 ) { vs = " injures";            vp = " injures";            }
    else if ( dam <= 80 ) { vs = " wounds";         vp = " wounds";             }
    else if ( dam <= 90 ) { vs = " mauls";          vp = " mauls";              }
    else if ( dam <= 100 ) { vs = " pummels";           vp = " pummels";            }
    else if ( dam <= 120 ) { vs = " decimates";         vp = " decimates";          }
    else if ( dam <= 140 ) { vs = " devastates";            vp = " devastes";           }
    else if ( dam <= 160 ) { vs = " nicks";         vp = " nicks";              }
    else if ( dam <= 180 ) { vs = " shreds";            vp = " shreds";             }
    else if ( dam <= 220 ) { vs = " smashs";            vp = " smashs";             }
    else if ( dam <= 240 ) { vs = " mutilates";         vp = " mutilates";          }
    else if ( dam <= 260 ) { vs = " eliminates";            vp = " eliminates";         }
    else if ( dam <= 280 ) { vs = " slams";         vp = " slams";              }
    else if ( dam <= 300 ) { vs = " disembowels";       vp = " disembowels";            }
    else if ( dam <= 320 ) { vs = " eviscerates";       vp = " eviscerates";            }
    else if ( dam <= 340 ) { vs = " cripples";          vp = " cripples";           }
    else if ( dam <= 360 ) { vs = " massacres";         vp = " massacres";          }
    else if ( dam <= 380 ) { vs = " extrudinites";      vp = " extrudinites";           }
    else if ( dam <= 400 ) { vs = " implodes";          vp = " implodes";           }
    else if ( dam <= 450 ) { vs = " levels";            vp = " levels";             }
    else if ( dam <= 500 ) { vs = " superimplodes";     vp = " superimplodes";          }
    else if ( dam <= 550 ) { vs = " #yDeMoLiShEs#n";                            vp = " #yDeMoLiShEs#n";         }
    else if ( dam <= 600 ) { vs = " #LDiSmEmBeRs#n";                            vp = " #LDiSmEmBeRs#n";         }
    else if ( dam <= 700 ) { vs = " #CDeStRoYs#n";                          vp = " #CDeStRoYs#n";           }
    else if ( dam <= 750 ) { vs = " #RAnNiHiLaTeS#n";                           vp = " #RAnNiHiLaTeS#n";            }
    else if ( dam <= 800 ) { vs = " #0BrUtaLiZeS#n";                            vp = " #0BrUtaLiZeS#n";         }
    else if ( dam <= 850 ) { vs = " #oSlAuGhTeRs#n";                            vp = " #oSlAuGhTeRs#n";         }
    else if ( dam <= 900 ) { vs = " #y#bFOSSILIZES#n";                          vp = " #y#bFOSSILIZES#n";           }
    else if ( dam <= 950 ) { vs = " #L#bLACERATES#n";                           vp = " #L#bLACERATES#n";            }
    else if ( dam <= 1000 ) { vs = " #C#bPULVERIZES#n";                         vp = " #C#bPULVERIZES#n";           }
    else if ( dam <= 1500 ) { vs = " #R#bVAPORIZES#n";                          vp = " #R#bVAPORIZES#n";            }
    else if ( dam <= 2000 ) { vs = " #0#bDISINTEGRATES#n";                      vp = " #0#bDISINTEGRATES#n";            }
    else if ( dam <= 2500 ) { vs = " #o*#0#bERADICATES#n#o*#n";                     vp = " #o*#0#bERADICATES#n#o*#n";       }
    else if ( dam <= 3000 ) { vs = " #y**#R#bULTRAIMPLODES#n#y**#n";                    vp = " #y**#R#bULTRAIMPLODES#n#y**#n";      }
    else if ( dam <= 3500 ) { vs = " #L***#C#bCRUCIFIES#n#L***#n";                  vp = " #L***#C#bCRUCIFIES#n#L***#n";        }
    else if ( dam <= 4000 ) { vs = " #C****#L#bRUPTURES#n#C****#n";                 vp = " #C****#L#bRUPTURES#n#C****#n";       }
    else if ( dam <= 4500 ) { vs = " #R*****#y#bINCINERATES#n#R*****#n";                vp = " #R*****#y#bINCINERATES#n#R*****#n";  }
    else if ( dam <= 5000 ) { vs = " #0******#C#bSHATTERS#n#0******#n";             vp = " #0******#C#bSHATTERS#n#0******#n";       }
    else if ( dam <= 10000 ) { vs = " #R(*)*(*)*(*)#y#bFRIES#n#R(*)*(*)*(*)#n";         vp = " #R(*)*(*)*(*)#y#bFRIES#n#R(*)*(*)*(*)#n";        }
    else                     { vs = " #R**-#C**-#L**-#R*#0#bEXTERMINATES#n#R*#L-**#C-**#R-**#n";    vp = " #R**-#C**-#L**-#R*#0#bEXTERMINATES#n#R*#L-**#C-**#R-**#n";   }

    strcpy(pvs,vs);
    strcpy(pvp,vp);
}

void dam_msg( const char * type, int dam, CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    char vs[MAX_STRING_LENGTH];
    char vp[MAX_STRING_LENGTH];

    dam_word(dam,vs,vp);

    sprintf(buf,"Your %s%s %s!#r[#l**#R%d#l**#r]#n\n\r",
        type, vs, (victim != ch) ? PERS(victim,ch) : "yourself!!", dam );
    stcf(ch,buf);
    sprintf(buf,"$n's %s%s you!#r[#l**#R%d#l**#r]#n",
        type, vs, dam );
    if (victim != ch) act(buf,ch,NULL,victim,TO_VICT);
    sprintf(buf,"$n's %s%s %s!",
        type, vp, (victim != ch) ? PERS(victim,ch) : "$mself!!" );
    act(buf,ch,NULL,victim,TO_ROOM);
}

/*
 * Rune Spells.
 */

void ra_ice_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,100+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#Cice slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_fire_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,100+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rfire slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_fi_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(300 + rgen*10,300+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rfire#0-#Cice #Rsl#Cash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_chaos_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(300 + rgen*10,300+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#ychaos slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_elem_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(5000 + rgen*250,5500+rgen*400);

    cap_dam(ch,victim,dam);
    dam_msg( "#Re#pl#Ye#Gm#Le#pt#Pa#7l #0slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_demon_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(2000 + rgen*100,2500+rgen*100);
    if ( IS_CLASS(victim,CLASS_DEMON) || IS_CLASS(victim,CLASS_TANARRI) )
        dam *= 0.2;
    cap_dam(ch,victim,dam);
    dam_msg( "#Rdemon #0slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);

    dam = number_range(2000 + rgen*100,2500+rgen*100);
    if ( IS_CLASS(victim,CLASS_DEMON) || IS_CLASS(victim,CLASS_TANARRI) )
        dam *= 0.1;
    cap_dam(ch,victim,dam);
    dam_msg( "#Rdemon #0slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_shatter_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(4000 + rgen*75,4500+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#os#Rh#oa#Rt#ot#Re#or #Rslash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_dark_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(5000 + rgen*100,7500+rgen*250);

    cap_dam(ch,victim,dam);
    dam_msg( "#0dark #7m#0a#lt#Lt#0er #7slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void ra_ltn_slash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,100+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#7lightning slash#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_fire_wave( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,100+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rfire wave#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_fire_ball( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(200 + rgen*25,200+rgen*150);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rfire ball#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_fire_blast( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(300 + rgen*50,350+rgen*200);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rblast of flame#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_chaos_flames( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,500+rgen*200);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rc#yh#Ra#yo#Rt#yi#Rc #yf#Rl#ya#Rm#ye#Rs#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_ice_shot( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,100+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#Cice shot#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_ice_blast( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(200 + rgen*25,200+rgen*150);

    cap_dam(ch,victim,dam);
    dam_msg( "#Cblast of ice#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_ice_storm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(300 + rgen*50,350+rgen*200);

    cap_dam(ch,victim,dam);
    dam_msg( "#Cice storm#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_chaos_ice( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,500+rgen*200);

    cap_dam(ch,victim,dam);
    dam_msg( "#Cc#yh#Ca#yo#Ct#yi#Cc #yf#Cr#ye#Ce#yz#Ce#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_lit_bolt( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(100 + rgen*10,100+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#Llightning bolt#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_lit_strike( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(200 + rgen*25,200+rgen*150);

    cap_dam(ch,victim,dam);
    dam_msg( "#Llightning strike#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_lit_blast( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(300 + rgen*50,350+rgen*200);

    cap_dam(ch,victim,dam);
    dam_msg( "#Lblast of lightning#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_lit_chain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;
    CHAR_DATA *rvict;

    dam = number_range(250 + rgen*50,300+rgen*150);

    if ( number_range(1,3) == 2 )
        for ( rvict = ch->in_room->people; rvict;  )
    {
        if ( number_range(1,10) == 5 )
            break;
        if ( !rvict ) rvict = ch->in_room->people;
        else rvict = rvict->next;
    }
    else rvict = victim;

    cap_dam(ch,rvict,dam);
    dam_msg( "#Lchain lightning#n", dam, ch, rvict );
    hurt_person(ch,rvict,dam);
}

void rs_earth_shake( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    int rdam = 0;
    sh_int rgen = 6 - ch->generation;
    CHAR_DATA *rvict;

    dam = number_range(100 + rgen*10,100+rgen*100);

    for ( rvict = ch->in_room->people; rvict; rvict = rvict->next )
    {
        rdam = number_fuzzy(number_fuzzy(dam));
        if ( rvict == ch )
        {
            if ( number_range(1,4) == 3 )
            {
                stcf(ch,"#oAnticipating the quake, you jump at just the right time to avoid damage.#n\n\r");
                continue;
            }
            else
            {
                stcf(ch,"#oAnticipating the quake, you jump in the air, lessening the damage.#n\n\r");
                rdam /= 2;
            }
        }
        cap_dam(ch,rvict,rdam);
        dam_msg( "#oea#0r#ot#0hqu#oak#0e#n", rdam, ch, rvict );
        hurt_person(ch,rvict,rdam);
    }
}

void rs_daemon_flare( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(1000 + rgen*100,2000+rgen*300);
    if ( IS_CLASS(victim,CLASS_DEMON) || IS_CLASS(victim,CLASS_TANARRI) )
        dam *= 0.2;

    cap_dam(ch,victim,dam);
    dam_msg( "#0daemon #Rflare#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_holy_touch( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int heal = 0;
    sh_int rgen = 6 - ch->generation;

    heal = number_range(2500 + rgen*100,3000+rgen*200);

    stcf(victim,"#LA gentle breeze revives you. *(#7%d#L)*#n\n\r",heal);
    victim->hit += heal;
    if ( victim->hit > victim->max_hit ) victim->hit = victim->max_hit;
    act("#LA gentle breeze swirls around $n.#n",victim,NULL,NULL,TO_ROOM);
}

void rs_elemental( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(500 + rgen*50,750+rgen*100);

    cap_dam(ch,victim,dam);
    dam_msg( "#7elemental blast#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_elem_room( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    int rdam = 0;
    sh_int rgen = 6 - ch->generation;
    CHAR_DATA *rvict;

    dam = number_range(500 + rgen*50,750+rgen*100);

    for ( rvict = ch->in_room->people; rvict; rvict = rvict->next )
    {
        rdam = number_fuzzy(number_fuzzy(dam));
        if ( rvict == ch ) continue;
        cap_dam(ch,rvict,rdam);
        dam_msg( "#7elemental blast#n", rdam, ch, rvict );
        hurt_person(ch,rvict,rdam);
    }
}

void rs_chaos_blast( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(2000 + rgen*200,2500+rgen*300);

    cap_dam(ch,victim,dam);
    dam_msg( "#ychaos blast#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_fire_ice( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(1000 + rgen*100,1000+rgen*200);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rfire #Ca#7n#Rd #Cice #0blast#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_ice_charge( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(1000 + rgen*100,1500+rgen*250);

    cap_dam(ch,victim,dam);
    dam_msg( "#Cice charge#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_fire_charge( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(1000 + rgen*100,1500+rgen*250);

    cap_dam(ch,victim,dam);
    dam_msg( "#Rfire charge#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_darkmat( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(2000 + rgen*500,3000+rgen*750);

    dam_msg( "#7#bULTIMATE#n #0dark matter charge#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_wasteqp( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;

    dam = 5000;

    cap_dam(ch,ch,dam);
    dam_msg( "#750000 quest point attack#n", dam, ch, ch );
    hurt_person(ch,ch,dam);
}

void rs_lit_storm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    int rdam = 0;
    sh_int rgen = 6 - ch->generation;
    CHAR_DATA *rvict;

    dam = number_range(1000 + rgen*100,1000+rgen*250);

    for ( rvict = ch->in_room->people; rvict; rvict = rvict->next )
    {
        rdam = number_fuzzy(number_fuzzy(dam));
        cap_dam(ch,rvict,dam);
        dam_msg( "#Llightning storm#n", dam, ch, rvict );
        hurt_person(ch,rvict,dam);
    }
}

void rs_demon_conv( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;

    dam = ch->hit/2;

    dam_msg( "#Rdemonic convulsions#n", dam, ch, ch );
    hurt_person(ch,ch,dam);
}

void rs_angel_conv( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(1000 + rgen*200,1200+rgen*240);

    cap_dam(ch,victim,dam);
    dam_msg( "#yangelic #Cconvulsions#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}

void rs_disintegrate( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int dam = 0;
    sh_int rgen = 6 - ch->generation;

    dam = number_range(7500 + rgen*1000,10000+rgen*1500);

    dam_msg( "#7#bULTIMATE#n #Rdistintegration#n", dam, ch, victim );
    hurt_person(ch,victim,dam);
}
