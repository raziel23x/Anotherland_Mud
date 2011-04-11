/*
01234567890123456789012345678901234567890123456789012345678901234567890123456789
Psycho v2 for ROM based muds. Robert Schultz, Sembiance  -  bert@ncinter.net
Snippets of mine can be found at http://www.ncinter.net/~bert/mud/
This file (psycho.c) contains all the psycho thingie functions.
*/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"
#include "psycho.h"

char *psycho_message[] =
{
    "<---------------------------------------------------------------------->",
    "If I could be an animal, I'd be a ameoba, yeah....  oohhh yeah...",
    "Rock me amadeus... OOOH... Rock me amadeus...",
    "I'm naked and defenseless and the cityguards are whipping me..",
    "Ever had a dream where you wore nothing but underwear and clown shoes?.. guess its just me",
    "Don't hate me because I'm beautiful.",
    "Help! I've fallen and I can't get up!",
    "If it was christmas all year round, I'd have 5 nipples",
    "Oh great gods of computer monitors, take me to your WORLD!!!!",
    "Partridge Family ROCKS!",
    "I could kill a man with a stick of string cheese!",
    "I'm all drunk on cough syrup, so don't mind me...",
    "CD's make great earrings",
    "The clouds.... they're  BLEEDING  on me... make 'em stop!",
    "I wanna have an orgy in the smurf villiage.. MUAHAHAHA..",
    "I wanna run naked through the midgaard fountain",
    "My heart is filled with lemon pudding...",
    "Someday, in the future we'll have.... Oh never mind...",
    "The internet sucks",
    "If I had a virus on my computer, that would RULE",
    "I wish I was an oscar meyer weiner",
    "#YYou #0wouldn't do that if #GNixon#0 was in the #7White House#n",
    "#RI AM #LTHE #GWALRUS#7!!!!!!{x",
    "#GWherever you go... #gThere you are....#n",
    "I could pkill everyone on this mud with a ball of protoplasm!!!",
    "Hey everyone, I just dropped 2000 bones in Market Square!!!",
    "Anyone ever seen a Lag Beast IRL?",
    "This is the best MUD I've ever played!!!!",
    "Do your shields change based on the variable wind speeds here?",
    "Can someone please spell me up anytime I ask?",
    "Hey I just saw myself!!!",
    "I bet no one on this mud has the guts to come pkill me!!",
    "Clans, Schmlans",
    "Hey, how do I get to Jellyfish Heaven?",
    "There are 4 me's living all together!",
    "I love lag!!!",
    "I could just live on water and pot pies!",
    "The Immortals RULE!",
    "If there was a food of the gods, it would have to be BEEF JERKY!",
    "Ever stick cockroaches in your underpants?",
    "I ammmm schtinking drunk!",
    "May a horny rhino find a liking to your leg",
    "I can't find my corpse!!! Oh wait, I ate it already",
    "I LOVE EVERYBODY!!!EXCEPT HANZO MAGUS AND MAGNUS OF COURSE!!!",
    "I Hate RESTORES!!!!",
    "IRL I have a clan called 'Fast Food Workers for EVIL'",
    "This whole internet thing is a JOKE!",
    "I LOVE LINUX!!!",
    "A rolling stone plays the guitar",
    "I love Macintoshes!!!... PC's Suck!!!",
    "I just stabbed myself with my newbie sword",
};

void do_psycho(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    if(argument[0] == '\0')
    {
        send_to_char("Syntax: psycho <character>\n\r", ch);
        return;
    }

    if( (victim = get_char_world(ch, argument)) == NULL)
    {
        send_to_char("That player is not online.\n\r", ch);
        return;
    }

    if(IS_NPC(victim))
    {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    if( (victim == ch) && (ch->level != MAX_LEVEL) )
    {
        send_to_char("You can not make yourself a Psycho!\n\r", ch);
        return;
    }

    if(IS_SET(victim->affected_by2, PLR_PSYCHO))
    {
        sprintf(buf, "%s is no longer Psycho!\n\r", victim->name);
        send_to_char(buf, ch);
        REMOVE_BIT(victim->affected_by2, PLR_PSYCHO);
        send_to_char("You now feel mentally stable!\n\r", victim);
    }
    else
    {
        sprintf(buf, "%s is now a Psycho!\n\r", victim->name);
        send_to_char(buf, ch);
        SET_BIT(victim->affected_by2, PLR_PSYCHO);
        send_to_char("You feel mentally unstable\n\r", victim);
    }

    return;
}

void psycho_update(void)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *wch;
    int chance1, chance2, psychoNum;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected != CON_PLAYING )
            continue;

        wch   = ( d->original != NULL ) ? d->original : d->character;

        if(!IS_SET(wch->affected_by2, PLR_PSYCHO))
            continue;

        chance1 =1;                                         //number_range(1, 5);  /* Increase 25 for messages to  */
        chance2 =0;                                         //number_range(1, 5);  /* appear less often. Both 25's */
        if(chance1 == chance2)
        {
            psychoNum = number_range(1, MAX_PSYCHO);
            do_chat(wch, psycho_message[psychoNum]);
        }
    }

    return;
}
