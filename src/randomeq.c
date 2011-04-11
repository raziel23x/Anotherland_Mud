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

void do_randomeq(CHAR_DATA *ch, char *argument)
{
}

void do_damtest(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int dam;
    int ac=0;
    ac-=(number_range(4,10)*500);
    dam = 0;
    dam+=number_range((ch->damcap[DAM_CAP]/2)+ac,ch->damcap[DAM_CAP]);
    dam +=ch->damroll;
    if(dam>ch->damcap[DAM_CAP])dam=ch->damcap[DAM_CAP];
    dam +=ac/10;
    sprintf(buf,"%d damage",dam);
    stc(buf,ch);
    sprintf(buf," aginst %d AC",ac);
    stc(buf,ch);
}

void do_averagedamtest(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int dam;
    int ac=0;
    int dam2=0;
    int i;
    ac-=(number_range(4,10)*500);
    for(i=0;i<100;i++)
    {
        dam=0;
        dam+=number_range((ch->damcap[DAM_CAP]/2)+ac,ch->damcap[DAM_CAP]);
        dam +=ch->damroll;
        if(dam>ch->damcap[DAM_CAP])dam=ch->damcap[DAM_CAP];
        dam +=ac/10;
        dam2+=dam;
    }
    sprintf(buf,"%d damage",dam2);
    stc(buf,ch);
    sprintf(buf," aginst %d AC",ac);
    stc(buf,ch);
    sprintf(buf," for %d average damage",dam2/100);
    stc(buf,ch);
}
