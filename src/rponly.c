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

void do_rptoken( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int value;

    argument = one_argument( argument, arg );
    argument = one_argument(argument, arg2);

    if (IS_NPC(ch)) return;
    if (arg[0] == '\0')
    {
        send_to_char("Syntax: rptoken <hp|mana|move|status|upgrade> <amount>.\n\r",ch);
        return;
    }
    if (!str_cmp(arg, "hp"))
    {
        value = atoi(arg2);
        if (((value = atoi(arg2)) < 1 || value > 5000000) && str_cmp(arg2,"all"))
        {
            send_to_char("From 1 to 5000000 hp only please.\n\r",ch);
            return;
        }
        if (!is_number(arg2) && str_cmp(arg2,"all"))
        {
            send_to_char("Please enter a numeric value.\n\r",ch);
            return;
        }
        if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
        {
            send_to_char( "Error...missing object, please inform Joshua.\n\r",ch );
            return;
        }

        obj = create_object( pObjIndex, value );
        obj->value[1] = value;
        obj->level = 69;
        obj->cost = value;
        obj->item_type = ITEM_XPTOKEN;
        obj_to_char(obj,ch);
        if (obj->questmaker != NULL) free_string(obj->questmaker);
        obj->questmaker = str_dup(ch->name);
        free_string( obj->name );
        obj->name = str_dup( "hp token" );
        sprintf(buf,"a %d hp token",value);
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );
        sprintf(buf,"A %d hp token lies on the floor.",value);
        free_string( obj->description );
        obj->description = str_dup( buf );
        act( "You pick your nose and reveal $p.", ch, obj, NULL, TO_CHAR );
        act( "$n picks $s nose and reveals $p.", ch, obj, NULL, TO_ROOM );
        return;
    }
    else if (!str_cmp(arg, "mana"))
    {
        value = atoi(arg2);

        if (((value = atoi(arg2)) < 1 || value > 5000000 ) && str_cmp(arg2,"all"))
        {
            send_to_char("From 1 to 5,000,000 mana please.\n\r",ch);
            return;
        }
        if (!is_number(arg2) && str_cmp(arg2,"all"))
        {
            send_to_char("Please enter a numeric value.\n\r",ch);
            return;
        }
        victim = get_char_room( ch, arg2 );

        if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
        {
            send_to_char( "Error...missing object, please inform Joshua.\n\r",ch );
            return;
        }

        obj = create_object( pObjIndex, value );
        obj->value[2] = value;
        obj->level = 69;
        obj->cost = value;
        obj->item_type = ITEM_PRTOKEN;
        obj_to_char(obj,ch);
        if (obj->questmaker != NULL) free_string(obj->questmaker);
        obj->questmaker = str_dup(ch->name);
        free_string( obj->name );
        obj->name = str_dup( "mana token" );
        sprintf(buf,"a %d mana token",value);
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );
        sprintf(buf,"A %d mana token lies on the floor.",value);
        free_string( obj->description );
        obj->description = str_dup( buf );
        if (victim != NULL && victim != ch)
        {
            act( "You blow into $N's ear and produce $p.", ch, obj, victim, TO_CHAR );
            act( "$n blows into $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT );
            act( "$n blows into your ear and produces $p.", ch, obj, victim, TO_VICT );
        }
        else
        {
            act( "You fart reveal $p.", ch, obj, NULL, TO_CHAR );
            act( "$n farts and out $s butt comes $p.", ch, obj, NULL, TO_ROOM );
        }
        return;
    }
    else if (!str_cmp(arg, "move"))
    {
        value = atoi(arg2);
        if (((value = atoi(arg2)) < 1 || value > 5000000) && str_cmp(arg2,"all"))
        {
            send_to_char("From 1 to 5,000,000 move please.\n\r",ch);
            return;
        }
        if (!is_number(arg2) && str_cmp(arg2,"all"))
        {
            send_to_char("Please enter a numeric value.\n\r",ch);
            return;
        }
        if ( !str_cmp(arg2,"all" ) ) value = ch->pcdata->store;
        victim = get_char_room( ch, arg2 );

        if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
        {
            send_to_char( "Error...missing object, please inform Trace.\n\r",ch );
            return;
        }

        obj = create_object( pObjIndex, value );
        obj->value[3] = value;
        obj->level = 69;
        obj->cost = value;
        obj->item_type = ITEM_STORETOKEN;
        obj_to_char(obj,ch);
        if (obj->questmaker != NULL) free_string(obj->questmaker);
        obj->questmaker = str_dup(ch->name);
        free_string( obj->name );
        obj->name = str_dup( "move token" );
        sprintf(buf,"a %d move token",value);
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );
        sprintf(buf,"A %d move token lies on the floor.",value);
        free_string( obj->description );
        obj->description = str_dup( buf );
        if (victim != NULL && victim != ch)
        {
            act( "You sneeze onto $N's face and out flies $p.", ch, obj, victim, TO_CHAR );
            act( "$n sneezes onto $N's face and out flies $p.", ch, obj, victim, TO_NOTVICT );
            act( "$n sneezes onto your face and out comes $p.", ch, obj, victim, TO_VICT );
        }
        else
        {
            act( "You sneeze and $p flies out.", ch, obj, NULL, TO_CHAR );
            act( "$n sneezes and $p flies out.", ch, obj, NULL, TO_ROOM );
        }
        return;
    }
    else if (!str_cmp(arg, "Status"))
    {
        value = atoi(arg2);

        if (((value = atoi(arg2)) < 1 || value > 2 ) && str_cmp(arg2,"all"))
        {
            send_to_char("From 1 to 2 status please.\n\r",ch);
            return;
        }
        if (!is_number(arg2) && str_cmp(arg2,"all"))
        {
            send_to_char("Please enter a numeric value.\n\r",ch);
            return;
        }
        if ( !str_cmp(arg2,"all" ) ) value = ch->pcdata->quest;
        victim = get_char_room( ch, arg2 );

        if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
        {
            send_to_char( "Error...missing object, please inform Trace.\n\r",ch );
            return;
        }

        obj = create_object( pObjIndex, value );
        obj->value[0] = value;
        obj->level = 69;
        obj->cost = value*1000;
        obj->item_type = ITEM_QUEST;
        obj_to_char(obj,ch);
        obj->questowner = str_dup(ch->pcdata->switchname);
        if (obj->questmaker != NULL) free_string(obj->questmaker);
        obj->questmaker = str_dup(ch->name);
        free_string( obj->name );
        obj->name = str_dup( "status token" );
        sprintf(buf,"a %d point status token",value);
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );
        sprintf(buf,"A %d point status token lies on the floor.",value);
        free_string( obj->description );
        obj->description = str_dup( buf );
        if (victim != NULL && victim != ch)
        {
            act( "You reach behind $N's ear and produce $p.", ch, obj, victim, TO_CHAR );
            act( "$n reaches behind $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT );
            act( "$n reaches behind your ear and produces $p.", ch, obj, victim, TO_VICT );
        }
        else
        {
            act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR );
            act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM );
        }
        return;
    }
    else
    {
        do_token(ch,"");
    }
    return;
}
