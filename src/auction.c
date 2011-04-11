#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "merc.h"
/*
Copyright 2001 Devik Kyrn :) of Another land,
Do not remove this.
*/
// save.c
void    fwrite_obj      args( ( CHAR_DATA *ch,  OBJ_DATA  *obj, FILE *fp, int iNest ) );
OBJ_DATA *fread_obj2    args( ( FILE *fp ) );

AUCTION_DATA *auction_list;

AUCTION_DATA *init_auction()
{
    AUCTION_DATA *auc;

    auc = alloc_perm( sizeof(*auc) );
    if ( auction_list )
        auc->next = auction_list;
    else auc->next = NULL;
    auction_list = auc;

    auc->item       = NULL;
    auc->owner      = str_dup( "" );
    auc->minbid     = 0;
    auc->curbid     = 0;
    auc->totbid     = 0;
    auc->lastbid    = str_dup( "" );
    auc->aucend     = 0;

    return auc;
}

void save_auction()
{
    AUCTION_DATA    *auc;
    FILE        *fp;

    fclose(fpReserve);

    if ( !(fp = fopen("auction.data","w")) )
    {
        bug( "save_auction: Could not open auction.data", 0 );
        fpReserve = fopen(NULL_FILE,"r");
        return;
    }

    for ( auc = auction_list; auc; auc = auc->next )
    {
        fprintf(fp,"#AUCTION\n");
        fprintf(fp,"Owner     %s~\n",auc->owner);
        fprintf(fp,"MinBid    %d\n",auc->minbid);
        fprintf(fp,"CurBid    %d\n",auc->curbid);
        fprintf(fp,"TotBid    %d\n",auc->totbid);
        fprintf(fp,"Last      %s~\n",auc->lastbid);
        fprintf(fp,"AucEnd    %d\n",auc->aucend);
        fprintf(fp,"Status    %d\n",auc->status);
        fwrite_obj(NULL,auc->item,fp,0);
    }

    fprintf(fp,"#END\n");

    fflush(fp);
    fclose(fp);
    fpReserve = fopen( NULL_FILE, "r" );
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )    if ( !str_cmp( word, literal ) ){field  = value;continue;}

void load_auction()
{
    AUCTION_DATA    *auc;
    FILE        *fp;
    char        *word;

    fclose(fpReserve);

    if ( !(fp = fopen("auction.data","r")) )
    {
        bug( "load_auction: Could not open auction.data", 0 );
        fpReserve = fopen(NULL_FILE,"r");
        return;
    }

    for ( ; ; )
    {
        if ( feof(fp) ) break;
        word = fread_word( fp );
        if ( !str_cmp(word,"#END") ) break;
        if ( str_cmp(word,"#AUCTION") )
        {
            bug("load_auction: auction.data is in illegal format", 0 );
            fclose(fp);
            fpReserve = fopen(NULL_FILE,"r");
            break;
        }

        auc = init_auction();

        for ( ; ; )
        {
            word = fread_word( fp );

            KEY("Owner",    auc->owner, fread_string(fp) );
            KEY("MinBid",   auc->minbid,    fread_number(fp) );
            KEY("CurBid",   auc->curbid,    fread_number(fp) );
            KEY("TotBid",   auc->totbid,    fread_number(fp) );
            KEY("Last",     auc->lastbid,   fread_string(fp) );
            KEY("AucEnd",   auc->aucend,    fread_number(fp) );
            KEY("Status",   auc->status,    fread_number(fp) );

            if ( !str_cmp(word,"#OBJECT") )
            {
                auc->item = fread_obj2(fp);
                object_list = object_list->next;
                auc->item->next = NULL;
                break;
            }
        }

    }

    fclose(fp);
    fpReserve = fopen( NULL_FILE, "r" );
}

void add_auction( OBJ_DATA *item, CHAR_DATA *ch, int minbid, int curbid, int length )
{
    AUCTION_DATA *auc;
    OBJ_DATA *obj;
    bool found = FALSE;

    auc = init_auction();

    auc->item = item;
    if ( auc->item == object_list )
    {
        obj = object_list;
        found = TRUE;
    }
    else for ( obj = object_list; obj; obj = obj->next )
    if (  obj->next == auc->item )
        { found = TRUE; break; }

        if ( found )
    {
        obj->next = auc->item->next;
        auc->item->next = NULL;
    }

    if ( auc->item->questowner ) free_string(auc->item->questowner);
    auc->item->questowner = str_dup( "" );

    auc->owner = str_dup(ch->name);
    auc->minbid = minbid;
    auc->curbid = curbid;
    auc->aucend = length;

    save_auction();
}

OBJ_DATA *extract_auction( AUCTION_DATA *auc )
{
    AUCTION_DATA *iAuc;
    OBJ_DATA *obj;
    bool found = FALSE;

    if ( auc == auction_list )
    {
        auction_list = auc->next;
        found = TRUE;
    }
    else
    {
        for ( iAuc = auction_list; iAuc; iAuc = iAuc->next )
            if ( iAuc->next && iAuc->next == auc )
        {
            found = TRUE;
            break;
        }
        if ( found )
            iAuc->next = auc->next;
    }

    if ( !found )
    {
        bug("extract_auction: auction not in auction_list",0);
        return NULL;
    }

    free_string(auc->owner);
    free_string(auc->lastbid);

    obj = auc->item;

    free_mem(auc,sizeof(*auc));

    save_auction();

    return obj;
}

void do_addauction( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MIL];
    char arg2[MIL];
    char arg3[MIL];
    char arg4[MIL];
    char buf[MAX_STRING_LENGTH];
    char buf2[MSL];
    int time;
    int start;
    int bid;
    AUCTION_DATA *auc = auction_list;
    int i;
    if(ch->played<18000)
    {
        stcf(ch,"#CYou have not been here long enougth to add an auction.\n\r");
        return;
    }
    for ( i = 0; auc; auc = auc->next, i++ )
        if ( i == 14 ) return stcf(ch,"#RThere are too many items being auctioned right now.  Please wait. :)#n\n\r");

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );

    if (!arg[0] || !arg2[0] || !arg3[0] || !arg4[0]
        || !is_number( arg ) || !is_number( arg2 ) || !is_number( arg3 ) )
    {
        send_to_char( "Syntax: Auction add <Min Bid Increment> <Starting bid> <Time in minutes> <object>.\n\r",ch );
        return;
    }

    bid = atoi( arg );
    start = atoi( arg2 );
    time = atoi( arg3 );

    if ( bid < 100 || bid > 1000 )
        return stcf(ch,"Bid increment must be at least 100 and no more than 1000 qps.\n\r");

    if ( start < 100 )
        return stcf(ch,"Starting bid must be at least 100.\n\r");

    if ( time < 5 || time > 750 )
        return stcf(ch,"Auction length must be at least 5 minutes and no more than 30 minutes.\n\r");

    if ( ( obj = get_obj_carry( ch, arg4 ) ) == NULL )
        return stcf(ch,"You do not have that item.\n\r");

    if (str_cmp(ch->pcdata->switchname,obj->questowner))
    {
        send_to_char("But you don't own it!\n\r",ch);
        return;
    }

    if ( ( IS_SET(obj->quest, QUEST_ANLA) ||
        IS_SET(obj->item_type, ITEM_PIECE) ||
        ( IS_SET(obj->extra_flags, ITEM_ENGRAVED) && str_cmp(obj->questowner, ch->pcdata->switchname) ) )
        && ch->level < 13 && ch->trust < 13)
    {
        stc("Can't sell that!\n\r",ch);
        return;
    }

    if ( ch->pcdata->quest < ( (start/15) + (bid/10) + time ) && ( ch->level < 6 ))
    {
        sprintf( buf2, " You do not have the required %d quest point listing fee.\n\r", ( (start/15) + (bid/10) + time ) );
        send_to_char(buf2,ch);
        return;
    }

    if ( ch->level < 6 )
    {
        ch->pcdata->quest -= (start/15) + (bid/10) + time;
    }

    sprintf( buf2, " You paid the : %d quest point auction processing fee.\n\r",( (start/15) + (bid/10) + time ) );
    stc(buf2,ch);

    obj_from_char(obj);
    add_auction( obj, ch, bid, start, time );

    sprintf(buf,"%s has placed '%s#n' on the auction block.",
        ch->name, obj->short_descr );
    do_info(ch,buf);
    sprintf(buf,"Bidding starts at %d quest points.",
        start );
    do_info(ch,buf);
    save_auction();
}

void do_removeauction( CHAR_DATA *ch, char *argument )
{
    AUCTION_DATA *auc;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];
    bool found = FALSE;
    int count = 0;
    int number;

    if (argument[0] == '\0')
    {
        send_to_char("Syntax: Auction remove <item>\n\r",ch);
        return;
    }

    number = number_argument(argument, arg);

    for ( auc = auction_list; auc; auc = auc->next )
        if ( (IS_IMMORTAL(ch) || !str_cmp(auc->owner,ch->name) )
        && is_name(argument,auc->item->name) )
            if ( ++count == number )
                {found = TRUE; break;}

                if ( !found ) return stcf(ch,"But you're not auctioning that!\n\r");

    if ( auc->status != AUC_RUNNING )
        return stcf(ch,"That auction has already ended.\n\r");

    sprintf(buf,"%s has removed '%s#n' from the auction block.",ch->name,auc->item->short_descr);
    do_info(ch,buf);

    if ( !str_cmp(ch->name,auc->owner) )
    {
        obj_to_char(auc->item,ch);
        stcf(ch,"%s#n appears in your hands.\n\r",auc->item->short_descr);
        extract_auction(auc);
        return;
    }

    found = FALSE;
    for ( d = descriptor_list; d; d = d->next )
        if ( !str_cmp(d->character->name,auc->owner) )
            {found = TRUE; break;}

            if ( found )
            {
                auc->item->next = object_list;
                object_list = auc->item;
                obj_to_char(auc->item,ch);
                stcf(ch,"%s appears in your hands.\n\r",auc->item->short_descr);
                extract_auction(auc);
                return;
            }
    else auc->status = AUC_RETURN;
    save_auction();
}

void do_auction( CHAR_DATA *ch, char *argument )
{
    AUCTION_DATA *auc;
    bool found = FALSE;
    char caTemp[25];

    if ( IS_NPC(ch) ) return;

    if (argument[0] == '\0')
    {
        send_to_char("What would you like to do? add, remove, extend, info, list, chat\n\r",ch);
        return;
    }
    argument = one_argument(argument,caTemp);

    if ( !str_cmp(caTemp,"add") )
        do_addauction(ch,argument);
    else if ( !str_cmp(caTemp,"chat") )
        do_auctalk(ch,argument);

    else if ( !str_cmp(caTemp,"remove") )
        do_removeauction(ch,argument);

    else if ( !str_cmp(caTemp,"extend") )
        do_auctionextend(ch,argument);

    else if ( !str_cmp(caTemp,"info") )
        do_auctioninfo(ch,argument);

    else if ( !str_cmp(caTemp,"bid") )
        do_bid(ch,argument);

    else if ( !str_cmp(caTemp,"list"))
    {
        stcf(ch,"Items on the auction block:\n\r");
        for ( auc = auction_list; auc; auc = auc->next )
            found = auction_string(ch,auc,found);
        if ( !found ) stcf(ch,"None.\n\r");
        return;
    }
    else
    {
        stcf(ch,"Type in a command. You typed in %s.",caTemp);
        return;
    }
}

bool auction_string( CHAR_DATA *ch, AUCTION_DATA *auc, bool found )
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char buf5[MAX_STRING_LENGTH];
    char *head = "#0[#LItem    #0][#LShort        #0][#LOwner    #0][#LLast Bidder     #0][#LInc #0][#LStatus            #0]#n\n\r";
    char *fmt = "#0[#7%-8s#0][#n%-13s#0][#7%-9s#0][#7%-9s#0|#G%6d#0][#G%4d#0][#C%-20s#0]#n\n\r";
    bool bShow = TRUE;
    //    int i, count = 0;

    if ( !found )
        stcf(ch,head);

    strcpy(buf1,auc->item->name); buf1[8] = '\0';
    strcpy(buf2,auc->item->short_descr);                    //buf2[13] = '\0';
    //    for ( i = 0; ; i++ )
    //    {
    //	if ( buf2[i] == '\0' ) break;
    //	if ( buf2[i] == '#') { count+=2; i++; continue; }
    //	if ( (i - count) >= 13 ) { buf2[i] = '\0'; break; }
    //    }
    strcutpad(buf2,13);
    strcpy(buf3,auc->owner); buf3[9] = '\0';
    strcpy(buf4,auc->lastbid); buf4[9] = '\0';
    switch ( auc->status )
    {
        case AUC_RUNNING:   sprintf(buf5,"#PRunning: %d mins",
            auc->aucend); break;
        case AUC_BUYER:     strcpy(buf5,"#pEnded: Buyer"); bShow = FALSE; break;
        case AUC_SELLER:    strcpy(buf5,"#pEnded: Seller"); bShow = FALSE; break;
        case AUC_RETURN:    strcpy(buf5,"#pEnded: Return"); bShow = FALSE; break;
        default:        strcpy(buf5,"#RUNKNOWN"); bShow = FALSE; break;
    }
    buf5[19] = '\0';
    if ( bShow || ch->level > LEVEL_BUILDER )
        stcf(ch,fmt,buf1,buf2,buf3,buf4,auc->curbid,auc->minbid,buf5);
    return TRUE;
}

void do_bid( CHAR_DATA *ch, char *argument )
{
    AUCTION_DATA *auc;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int bid;
    int count = 0;
    int number;
    bool found = FALSE;

    argument = one_argument(argument, arg);
    number = number_argument(arg, arg);

    if ( !arg[0] || !is_number(argument) )
        return stcf(ch,"Syntax: bid <item> [amount]\n\rOmitting amount will bid default amount.\n\r");

    bid = atoi(argument);

    for ( auc = auction_list; auc; auc = auc->next )
        if ( is_name(arg,auc->item->name) )
            if ( ++count == number )
                {found = TRUE; break;}

                if ( !found )
                    return stcf(ch,"That item is not on the auction block.\n\r");

    if ( auc->status != AUC_RUNNING )
        return stcf(ch,"That auction has already ended.\n\r");

    if ( bid == 0 )
        bid = auc->curbid + auc->minbid;
    if ( !str_cmp(auc->lastbid,ch->name) )
        return stcf(ch,"You are already the current winner of this auction.\n\r");

    if ( bid < (auc->curbid + auc->minbid) )
        return stcf(ch,"You must bid at least %d quest points for that item.\n\r",(auc->curbid + auc->minbid));

    if ( bid > ch->pcdata->quest )
        return stcf(ch,"You don't have %d quest points!\n\r",bid);

    free_string(auc->lastbid);
    auc->lastbid = str_dup(ch->name);
    auc->curbid = bid;
    auc->totbid++;
    ch->pcdata->quest -= 10;
    sprintf(buf,"%s has bid %d qps for '%s#n'.\n\r",
        ch->name, bid, auc->item->short_descr );
    do_info(ch,buf);
    save_auction();
}

void do_auctioninfo( CHAR_DATA *ch, char *argument )
{
    AUCTION_DATA *auc;
    int count = 0;
    int number;
    bool found = FALSE;

    number = number_argument(argument,argument);

    if ( !argument[0] )
        return stcf(ch,"Syntax: Auction info <item>\n\r");

    for ( auc = auction_list; auc; auc = auc->next )
        if ( is_name(argument,auc->item->name) )
            if ( ++count == number )
                { found = TRUE; break; }

                if ( !found )
                    return stcf(ch,"That item is not on the auction block.\n\r");

    stcf(ch,"Item '%s#n', owned by %s.\n\r"
        "Last bid by %s for %d quest points.\n\r"
        "This auction ends in %d minutes.\n\r"
        "%d total bids have been made for this item.\n\r"
        "Minimum bid increment is set at %d quest points.\n\r",
        auc->item->short_descr, auc->owner,
        auc->lastbid, auc->curbid,
        auc->aucend,
        auc->totbid,
        auc->minbid );
    spell_identify( skill_lookup("identify"), ch->level, ch, auc->item );
}

void do_killauction( CHAR_DATA *ch, char *argument )
{
    AUCTION_DATA *auc;
    char arg[MAX_STRING_LENGTH];
    bool found = FALSE;
    int count = 0;
    int number;

    if ( !*argument )
        return do_auction(ch,"");

    number = number_argument(argument, arg);

    for ( auc = auction_list; auc; auc = auc->next )
        if ( is_name(argument,auc->item->name) )
            if ( ++count == number )
                {found = TRUE; break;}

                if ( !found ) return stcf(ch,"But you're not auctioning that!\n\r");

    if ( !auc->item->next )
    {
        auc->item->next = obj_free;
        obj_free = auc->item;
    }
    extract_auction(auc);
}

void do_auctionextend( CHAR_DATA *ch, char *argument )
{
    AUCTION_DATA *auc;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int bid;
    int count = 0;
    int number;
    bool found = FALSE;

    argument = one_argument(argument, arg);
    number = number_argument(arg, arg);

    if ( !arg[0] || !is_number(argument) )
        return stcf(ch,"Syntax:auction extend <item> [amount].\n\r");

    bid = atoi(argument);

    for ( auc = auction_list; auc; auc = auc->next )
        if ( is_name(arg,auc->item->name) )
            if ( ++count == number )
                {found = TRUE; break;}

                if ( !found )
                    return stcf(ch,"That item is not on the auction block.\n\r");

    if ( auc->status != AUC_RUNNING )
        return stcf(ch,"That auction has already ended.\n\r");

    if ( bid <= 0 )
    {
        send_to_char( " You cannot extend a auction by 0 minutes.\n\r",ch );
        return;
    }

    if ( str_cmp(auc->owner,str_dup(ch->name) ) )
    {
        send_to_char( " That is not your auction!\n\r",ch );
        return;
    }

    if ( (bid*100) > ch->pcdata->quest )
        return stcf(ch,"You don't have %d quest points!\n\r",bid*100);

    auc->aucend += bid;
    ch->pcdata->quest -= (bid*100);
    sprintf(buf,"%s has extended %d minutes for '%s#n'.\n\r",
        ch->name, bid, auc->item->short_descr );
    do_info(ch,buf);
    save_auction();
}
