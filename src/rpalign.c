/*************************************
 *   File by Magna - l33t!           *
 * Help from Chrono "Fear the Pies!" *
 *************************************/

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

void do_rpalignment( CHAR_DATA *ch, char *argument )
{
    char buf[MSL];
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_RPONLY))
    {
        ch->pcdata->rpalign=RPALIGN_NONE;
        return;
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Please enter 'light', 'merc', 'none', or 'dark'.\n\r",ch );
        return;
    }
    if      (!str_cmp(arg,"light"))
    {                                                       //if(ch->pcdata->quest >=10000){
        // ch->pcdata->quest -= 10000;
        ch->pcdata->rpalign = RPALIGN_LIGHT;
        send_to_char( "DDAlignment is now for the light.\n\r", ch );
        sprintf(buf,"%s has chosen to be a warrior for the light.\n\r",ch->name);
        do_info(NULL,buf);
    }
    //else
    //send_to_char("You do not have the needed 10K qps.\n\r",ch);}

    else if      (!str_cmp(arg,"dark"))
    {
        //if(ch->pcdata->quest >=10000){
        //ch->pcdata->quest -= 10000;

        ch->pcdata->rpalign = RPALIGN_DARK;
        send_to_char( "DDAlignment is now for the dark.\n\r", ch );
        sprintf(buf,"%s has chosen to be a warrior for the darkness.\n\r",ch->name);
        do_info(NULL,buf);
    }
    //   }else
    //send_to_char("You do not have the needed 10K qps.\n\r",ch);}

    else if(!str_cmp(arg,"merc"))
    {
        //if(ch->pcdata->quest >=10000){
        // ch->pcdata->quest -= 10000;

        ch->pcdata->rpalign = RPALIGN_NEUTRAL;
        send_to_char( "DDAlignment is now for neither light nor dark.\n\r", ch );
        sprintf(buf,"%s has chosen to be a warrior for themselves.\n\r",ch->name);
        do_info(NULL,buf);
    }
    // } else
    //send_to_char("You do not have the needed 10K qps.\n\r",ch);}
    else if(!str_cmp(arg,"love"))
    {
        if(ch->pcdata->quest >=1000000)
        {
            ch->pcdata->quest -= 1000000;

            ch->pcdata->rpalign = RPALIGN_LOVE;
            send_to_char( "DDAlignment is now for the heart and the soul.\n\r", ch );
            sprintf(buf,"%s has chosen to be a warrior for Love.\n\r",ch->name);
            do_info(NULL,buf);
        }
        else
            send_to_char("You do not have the needed 1000K qps.\n\r",ch);
    }

    else if(!str_cmp(arg,"none"))
    {
        //if(ch->pcdata->quest >=20000){
        //ch->pcdata->quest -= 20000;

        ch->pcdata->rpalign = RPALIGN_NONE;
        send_to_char( "Alignment is now none.\n\r", ch );
        sprintf(buf,"%s has given up in the war between the light and dark.\n\r",ch->name);
        do_info(NULL,buf);
    }
    //}else
    //send_to_char("You do not have the needed 20K qps.\n\r",ch);}

    else
    {
        send_to_char( "Please enter 'light', 'merc', 'none', or 'dark'.\n\r", ch );
        return;
    }
    return;
}
