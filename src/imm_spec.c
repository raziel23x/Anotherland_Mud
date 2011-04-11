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
#include <unistd.h>

void do_isharmlocal(CHAR_DATA * pChar, char * pArgument)
{
    CHAR_DATA * pVictim;
    int iDamage;
    char pArg1[MAX_INPUT_LENGTH];
    char pArg2[MAX_INPUT_LENGTH];

    pArgument = one_argument(pArgument, pArg1);
    pArgument = one_argument(pArgument, pArg2);

    pVictim = get_char_room(pChar, pArg1);

    if(pVictim == NULL)
    {
        stc("Can not find target.\n\r", pChar);
        return;
    }

    iDamage = atoi(pArg2);

    if(iDamage <= 0)
    {
        return;
    }

    stcf(pChar, "%d damage done to %s.\n\r", iDamage, pVictim->name);
    hurt_person(pChar, pVictim, iDamage);

    return;
}

void do_isharmglobal(CHAR_DATA * pChar, char * pArgument)
{
    CHAR_DATA * pVictim;
    int iDamage;
    char pArg1[MAX_INPUT_LENGTH];
    char pArg2[MAX_INPUT_LENGTH];

    pArgument = one_argument(pArgument, pArg1);
    pArgument = one_argument(pArgument, pArg2);

    pVictim = get_char_world(pChar, pArg1);

    if(pVictim == NULL)
    {
        stc("Can not find target.\n\r", pChar);
        return;
    }

    iDamage = atoi(pArg2);

    if(iDamage <= 0)
    {
        return;
    }

    stcf(pChar, "%d damage done to %s.\n\r", iDamage, pVictim->name);
    hurt_person(pChar, pVictim, iDamage);

    return;
}

void do_isharmroom(CHAR_DATA * pChar, char * pArgument)
{
    CHAR_DATA * pVictim;
    CHAR_DATA * pVictimNext;
    int iDamage;
    char pArg1[MAX_INPUT_LENGTH];

    pArgument = one_argument(pArgument, pArg1);
    iDamage = atoi(pArg1);

    if(iDamage <= 0)
    {
        return;
    }

    for(pVictim = pChar->in_room->people; pVictim != NULL; pVictim = pVictimNext)
    {
        pVictimNext = pVictim->next_in_room;

        if(pChar != pVictim)
        {
            hurt_person(pChar, pVictim, iDamage);
        }
    }

    stc("Done.\n\r", pChar);
    return;
}

void do_istransfer(CHAR_DATA * pChar, char * pArgument)
{
    char pArg1[MAX_INPUT_LENGTH];
    char pArg2[MAX_INPUT_LENGTH];
    char pBuf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA * pLocation;
    DESCRIPTOR_DATA * pDesc;
    CHAR_DATA * pVictim;
    CHAR_DATA * pMount;

    sprintf(pBuf,"%s: Transfer %s",pChar->name,pArgument);
    if (pChar->level < NO_WATCH) do_watching(pChar,pBuf);

    pArgument = one_argument( pArgument, pArg1 );
    pArgument = one_argument( pArgument, pArg2 );

    if ( pArg1[0] == '\0' )
    {
        send_to_char( "Transfer whom (and where)?\n\r", pChar );
        return;
    }

    if ( !str_cmp( pArg1, "all" ) )
    {
        for ( pDesc = descriptor_list; pDesc != NULL; pDesc = pDesc->next )
        {
            if ( pDesc->connected == CON_PLAYING
                &&   pDesc->character != pChar
                &&   pDesc->character->in_room != NULL
                &&   can_see( pChar, pDesc->character ) )
            {
                char pBuf[MAX_STRING_LENGTH];
                sprintf( pBuf, "%s %s", pDesc->character->name, pArg2 );
                do_istransfer( pChar, pBuf );
            }
        }
        return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( pArg2[0] == '\0' )
    {
        pLocation = pChar->in_room;
    }
    else
    {
        if ( ( pLocation = find_location( pChar, pArg2 ) ) == NULL )
        {
            send_to_char( "No such location.\n\r", pChar );
            return;
        }

        if ( room_is_private( pLocation ) )
        {
            send_to_char( "That room is private right now.\n\r", pChar );
            return;
        }
    }

    if ( ( pVictim = get_char_world( pChar, pArg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", pChar );
        return;
    }

    if ( pVictim->in_room == NULL )
    {
        send_to_char( "They are in limbo.\n\r", pChar );
        return;
    }

    if ( pVictim->fighting != NULL )
    {
        stop_fighting( pVictim, TRUE );
    }

    char_from_room( pVictim );
    char_to_room( pVictim, pLocation );
    send_to_char( "Done.\n\r", pChar );

    if ( (pMount = pVictim->mount) == NULL) return;
    char_from_room( pMount );
    char_to_room( pMount, get_room_index(pVictim->in_room->vnum) );
}

void do_isotransfer(CHAR_DATA * ch, char * argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Otransfer which object?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' ) victim = ch;
    else if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
        return;
    }

    if (obj->carried_by != NULL)
    {
        obj_from_char(obj);
    }
    else if (obj->in_obj     != NULL) obj_from_obj(obj);
    else if (obj->in_room != NULL)
    {
        chroom = ch->in_room;
        objroom = obj->in_room;
        char_from_room(ch);
        char_to_room(ch,objroom);
        char_from_room(ch);
        char_to_room(ch,chroom);
        obj_from_room(obj);
    }
    else
    {
        send_to_char( "You were unable to get it.\n\r", ch );
        return;
    }
    obj_to_char(obj,victim);
    stc("Done.\n\r", ch);
    return;
}

void do_ismload(CHAR_DATA * ch, char * argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;

    if (IS_NPC(ch) ) return;

    if ( ch->pcdata->security < 4 )
    {
        stc( "You do not have the required security 4 to load mobiles\n\r",ch);
        return;
    }

    sprintf(buf,"%s: Mload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
        send_to_char( "Syntax: mload <vnum>.\n\r", ch );
        return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
        send_to_char( "No mob has that vnum.\n\r", ch );
        return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    stc("Done.\n\r", ch);
    return;
}

void do_isoload(CHAR_DATA * ch, char * argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}
    if ( ch->pcdata->security < 4 )
    {
        stc( "You do not have the required security 4 to load objects\n\r",ch);
        return;
    }

    sprintf(buf,"%s: Oload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
        send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' )
    {
        level = get_trust( ch );
    }
    else
    {
        /*
         * New feature from Alander.
         */
        if ( !is_number( arg2 ) )
        {
            send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
            return;
        }
        level = atoi( arg2 );
        if ( level < 0 || level > get_trust( ch ) )
        {
            send_to_char( "Limited to your trust level.\n\r", ch );
            return;
        }
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
        send_to_char( "No object has that vnum.\n\r", ch );
        return;
    }

    obj = create_object( pObjIndex, level );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
    {
        obj_to_char( obj, ch );
    }
    else
    {
        obj_to_room( obj, ch->in_room );
    }

    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    stc("Done.\n\r", ch);
    return;
}

void do_ispurge(CHAR_DATA * ch, char * argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        /* 'purge' */
        CHAR_DATA *vnext;

        for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
        {
            vnext = victim->next_in_room;
            if (IS_NPC(victim) && victim->desc == NULL && (mount = victim->mount) == NULL)
                extract_char( victim, TRUE );
        }

        send_to_char( "Ok.\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( !IS_NPC(victim) )
    {
        send_to_char( "Not on PC's.\n\r", ch );
        return;
    }
    if ( victim->desc != NULL )
    {
        send_to_char( "Not on switched players.\n\r", ch );
        return;
    }

    extract_char( victim, TRUE );
    return;
}

void do_isslay(CHAR_DATA * pChar, char * pArgument)
{
    CHAR_DATA * pVictim;
    char pArg1[MAX_INPUT_LENGTH];

    pArgument = one_argument(pArgument, pArg1);

    pVictim = get_char_room(pChar, pArg1);

    if(pVictim == NULL)
    {
        stc("Can not find target.\n\r", pChar);
        return;
    }

    raw_kill(pVictim);
    stc("Done.\n\r", pChar);

    return;
}

void do_isgoto( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *in_room;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Goto where?\n\r", ch );
        return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
        stc("No such location.\n\r",ch);
        return;
    }

    in_room = ch->in_room;
    if ( ch->fighting )
        stop_fighting( ch, TRUE );

    char_from_room( ch );
    char_to_room( ch, location );

    do_look( ch, "auto" );

    return;
}

void do_snatch(CHAR_DATA * ch, char * argument)
{
    CHAR_DATA * pVictim;
    OBJ_DATA * pObj;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if((pVictim = get_char_room(ch, arg1)) == NULL)
    {
        send_to_char("Can not find victim.\n\r", ch);
        return;
    }

    if(!IS_NPC(pVictim) && (pVictim->level > ch->level))
    {
        send_to_char("I don't think so.\n\r", ch);
        return;
    }

    if(((pObj = get_obj_carry(pVictim, arg2)) != NULL) || ((pObj = get_obj_wear(pVictim, arg2)) != NULL))
    {
        obj_from_char(pObj);
        obj_to_char(pObj, ch);
        act("You snatch $p from $N!", ch, pObj, pVictim, TO_CHAR);
        act("$n suddenly snatches $p from $N's hands!", ch, pObj, pVictim, TO_NOTVICT);
        act("$n suddenly snatches $p from your hands!", ch, pObj, pVictim, TO_VICT);
        return;
    }

    send_to_char("Can not find that object.\n\r", ch);
    return;
}

void do_ktimerlist(CHAR_DATA * ch, char * argument)
{
    CHAR_DATA * gch;
    DESCRIPTOR_DATA * d;
    char buf[MAX_STRING_LENGTH];

    for(d = descriptor_list; d != NULL; d = d->next)
    {
        if (IS_PLAYING(d)
            && (gch = d->character) != NULL
            && !IS_NPC(gch)
            && gch->in_room != NULL)
        {
            if(can_see(ch, gch))
            {
                sprintf(buf, "#7%s#n\t#L%d #Rseconds#n\n\r", gch->name, gch->pcdata->kdecap_timer * 4);
                send_to_char(buf, ch);
            }
        }
    }

    return;
}

void do_resetktimer(CHAR_DATA * ch, char * argument)
{
    CHAR_DATA * pVictim;

    if((pVictim = get_char_room(ch, argument)) == NULL)
    {
        send_to_char("You can not find them.\n\r", ch);
        return;
    }

    if(IS_NPC(pVictim))
    {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    pVictim->pcdata->kdecap_timer = 0;

    send_to_char("Your KDecap timer has been reset.\n\r", pVictim);
    send_to_char("Ok.\n\r", ch);

    return;
}

void do_groupgoto(CHAR_DATA * ch, char * argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * vch;
    CHAR_DATA * vch_next;
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *in_room;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Goto where?\n\r", ch );
        return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
        stc("No such location.\n\r",ch);
        return;
    }

    in_room = ch->in_room;
    if ( ch->fighting )
        stop_fighting( ch, TRUE );

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
        act( "$n $T", ch, NULL,
            (ch->pcdata && ch->pcdata->bamfout[0] != '\0')
            ? ch->pcdata->bamfout : "leaves in a swirling mist.",  TO_ROOM );
    }

    char_from_room( ch );
    char_to_room( ch, location );

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
        act( "$n $T", ch, NULL,
            (ch->pcdata && ch->pcdata->bamfin[0] != '\0')
            ? ch->pcdata->bamfin : "appears in a swirling mist.",  TO_ROOM );
    }

    do_look(ch, "auto");

    for (vch = in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;

        if(vch == ch)
        {
            continue;
        }

        if(!is_same_group(ch, vch))
        {
            continue;
        }

        act("You are carried away with $N.", vch, NULL, ch, TO_CHAR);

        if ( !IS_SET(ch->act, PLR_WIZINVIS) && !IS_SET(vch->act, PLR_WIZINVIS))
        {
            act( "$n is carried away with $N.", vch, NULL, ch, TO_NOTVICT);
        }

        char_from_room( vch );
        char_to_room( vch, location );

        if ( !IS_SET(ch->act, PLR_WIZINVIS) && !IS_SET(vch->act, PLR_WIZINVIS))
        {
            act( "$n arrives with $N.", vch, NULL, ch, TO_NOTVICT);
            act( "$n arrives following you.", vch, NULL, ch, TO_VICT);
        }

        do_look(vch, "auto");
    }

    return;
}

void do_affremove(CHAR_DATA * ch, char * argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA * paf;
    OBJ_DATA * pObj;
    int nIndex;
    int iRemoveNum;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if(arg1[0] == '\0')
    {
        send_to_char("Remove affect from what object?\n\r", ch);
        return;
    }

    pObj = get_obj_world(ch, arg1);

    if(pObj == NULL)
    {
        send_to_char("Could not find that object.\n\r", ch);
        return;
    }

    if(arg2[0] == '\0')
    {

        send_to_char("Object has the following affects:\n\r", ch);

        for(nIndex = 1, paf = pObj->affected; paf != NULL; ++nIndex, paf = paf->next)
        {
            sprintf( buf, "%d. Affects %s by %d.\n\r", nIndex, affect_loc_name( paf->location ), paf->modifier );
            send_to_char(buf, ch);
        }

        return;
    }

    iRemoveNum = atoi(arg2);

    if(iRemoveNum <= 0)
    {
        send_to_char("Invalid affect number.\n\r", ch);
        return;
    }

    for(nIndex = 1, paf = pObj->affected; (nIndex < iRemoveNum) && (paf != NULL); ++nIndex, paf = paf->next)
    {
        continue;
    }

    if(paf == NULL)
    {
        send_to_char("Invalid affect number.\n\r", ch);
        return;
    }

    affect_remove_obj(pObj, paf);
    send_to_char("Affect removed.\n\r", ch);

    return;
}
