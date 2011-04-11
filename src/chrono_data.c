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

void load_best()
{

    int i;
    FILE *fp;

    if ((fp = fopen("../txt/best.txt", "r")) == NULL)
    {
        log_string("Error: best.txt not found!");
        exit(1);
    }
    best[0].name = " ";
    best[0].status = 0;
    best[0].pkscore = 0;
    for (i = 0; i < 19; i++)
    {
        best[i].name = fread_string(fp);
        best[i].status = fread_number(fp);
        best[i].pkscore = fread_number(fp);
    }
    fclose(fp);

}

void save_best()
{

    FILE *fp;
    int i;

    if ((fp = fopen("../txt/best.txt","w")) == NULL)
    {
        log_string("Error writing to best.txt");
        return;
    }
    for (i=0;i<19;i++)
    {
        fprintf(fp, "%s~\n", best[i].name);
        fprintf(fp, "%d\n", best[i].status);
        fprintf(fp, "%d\n", best[i].pkscore);
    }
    fclose (fp);

}

void update_best( CHAR_DATA *ch )
{
    int j = 0;
    char buf[MAX_STRING_LENGTH];
    char class[MAX_STRING_LENGTH];
    bool changed = FALSE;

    if (IS_NPC(ch)) return;
    if (ch->level > 6) return;
    if (ch->pcdata->security == 9) return;
    if (ch->level < 3) return;
    if (ch->class == 0) return;

    if (ch->class == CLASS_DEMON) {j=0; sprintf(class,"Demon");}
    if (ch->class == CLASS_WEREWOLF) {j=1; sprintf(class,"Werewolf");}
    //  if (ch->class == CLASS_DROW) {j=2; sprintf(class,"Drow");}
    if (ch->class == CLASS_NINJA) {j=3; sprintf(class,"Ninja");}
    if (ch->class == CLASS_VAMPIRE) {j=4; sprintf(class,"Vampire");}
    if (ch->class == CLASS_MONK) {j=5; sprintf(class,"Monk");}
    if (ch->class == CLASS_MAGE) {j=6; sprintf(class,"Mage");}
    if (ch->class == CLASS_TANARRI) {j=7; sprintf(class,"Tanarri");}
    if (ch->class == CLASS_SHAPESHIFTER) {j=8; sprintf(class,"Shapeshifter");}
    if (ch->class == CLASS_WIZARD) {j=9; sprintf(class,"WIZARD");}
    if (ch->class == CLASS_SAMURAI) {j=10; sprintf(class,"Samurai");}
    if (ch->class == CLASS_UNDEAD_KNIGHT) {j=11; sprintf(class,"Undead Knight");}
    if (ch->class == CLASS_ANGEL) {j=12; sprintf(class,"Angel");}
    if (ch->class == CLASS_LICH) {j=13; sprintf(class,"Lich");}
    if (ch->class == CLASS_KAHN) {j=14; sprintf(class,"Kahn");}
    if (ch->class == CLASS_MILITARY) {j=15; sprintf(class,"Doomtrooper");}
    if (ch->class == CLASS_PHOENIX) {j=16; sprintf(class,"Phoenix");}
    if (ch->class == CLASS_ELF) {j=17; sprintf(class,"Elf");}
    if (ch->class == CLASS_DRAGON) {j=18; sprintf(class,"Dragon");}
    //if (ch->class == CLASS_DROID) {j=19; sprintf(class,"Spider Droid");}

    if (ch->race == best[j].status)
    {
        if (!str_cmp(ch->pcdata->switchname,best[j].name) && getMight(ch) != best[j].pkscore)
        {
            best[j].pkscore = getMight(ch);
            changed = TRUE;
        }

        if (str_cmp(ch->pcdata->switchname,best[j].name) && ch->race >= best[j].status && getMight(ch) > best[j].pkscore)
        {
            sprintf(buf,"#L%s#0 has taken the ranking of strongest #G%s#0 from #L%s#n.",ch->pcdata->switchname,class,best[j].name);
            do_info(NULL,buf);

            best[j].status = ch->race;
            free_string(best[j].name);
            best[j].name = str_dup(ch->name);
            best[j].pkscore = getMight(ch);
            changed = TRUE;
        }
    }

    if (ch->race != best[j].status)
    {
        if (!str_cmp(ch->pcdata->switchname,best[j].name) && ch->race != best[j].status)
        {
            best[j].status = ch->race;
            best[j].pkscore = getMight(ch);
            changed = TRUE;
        }

        else if (str_cmp(ch->pcdata->switchname,best[j].name) && ch->race > best[j].status)
        {
            sprintf(buf,"#L%s#0 has taken the ranking of strongest #G%s#0 from #L%s#n.",ch->pcdata->switchname,class,best[j].name);
            do_info(NULL,buf);

            best[j].status = ch->race;
            free_string(best[j].name);
            best[j].name = str_dup(ch->name);
            best[j].pkscore = getMight(ch);
            changed = TRUE;
        }
    }

    if (changed) save_best();
    return;
}

void do_classleader(CHAR_DATA *ch, char *argument)
{
    int i;
    char buf[MAX_STRING_LENGTH];
    char class[MAX_STRING_LENGTH];
    send_to_char("#CClass#0              :    #GName      #0->   #RStatus    #yMightrate#n\n\r",ch);
    send_to_char("\n\r",ch);
    for (i=0;i<19;i++)
    {

        if (i==0) sprintf(class,"Demon           ");
        if (i==1) sprintf(class,"Werewolf        ");
        //if (i==2) sprintf(class,"Drow            ");
        if (i==3) sprintf(class,"Ninja           ");
        if (i==4) sprintf(class,"Vampire         ");
        if (i==5) sprintf(class,"Monk            ");
        if (i==6) sprintf(class,"Mage            ");
        if (i==7) sprintf(class,"Tanarri         ");
        if (i==8) sprintf(class,"Shapeshifter    ");
        if (i==9) sprintf(class,"Wizard          ");
        if (i==10) sprintf(class,"Samurai         ");
        if (i==11) sprintf(class,"Undead Knight   ");
        if (i==12) sprintf(class,"Angel           ");
        if (i==13) sprintf(class,"Lich            ");
        if (i==14) sprintf(class,"Kahn            ");
        //if (i==15) sprintf(class,"Doomtrooper     ");
        if (i==16) sprintf(class,"Phoenix         ");
        if (i==17) sprintf(class,"Elf             ");
        if (i==18) sprintf(class,"Dragon          ");
        //if (i==19) sprintf(class,"Spider Droid    ");
        if(i!=2&&i!=15&&i!=19)
        {
            sprintf(buf,"%-18s #0: #G%-12s #0->     #R%-2d        #y%-5d\n\r",
                class,best[i].name,best[i].status,best[i].pkscore);
            send_to_char(buf,ch);
        };
    }

}

void do_resetclassleader(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int x,i;
    argument = one_argument(argument, arg);

    if (!str_cmp(arg,"all"))
    {
        for (i=0;i<19;i++)
        {
            best[i].status = 0;
            best[i].pkscore = 0;
            free_string(best[i].name);
            best[i].name = str_dup("None");
        }
        return;
    }

    else if (!is_number(arg))
    {
        send_to_char("Please enter a number to which top line to reset\n\r",ch);
        return;
    }

    if (is_number(arg))
    {
        x = atoi(arg);

        best[x].status = 0;
        best[x].pkscore = 0;
        free_string(best[x].name);
        best[x].name = str_dup("None");
        return;
    }
}
