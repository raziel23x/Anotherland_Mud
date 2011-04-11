/*
 * who_html.c
 *
 * This piece of code allows the mud to create a html file, that looks
 * a bit like the 'who screen'. By calling this function every few ticks,
 * the mud can keep an updated listing which can be accessed from the web.
 *
 * Code by Brian Graversen aka Jobo
 */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define HTML_DIR   "../../public_html/"

char * plr_class_name(int arg)
{
  static char buf[512];

  buf[0] = '\0';

  switch(arg)
  {
  case CLASS_DEMON:
    strcat(buf, " <font color=\"#FF0000\">[</font><font 
color=\"#666666\">Demon</font><font color=\"#FF0000\">]</font>");
    break;
  case CLASS_MAGE:
    strcat(buf, " <font color=\"#666666\">{{</font><font 
color=\"#33CCFF\">Battlemage</font><font color=\"#666666\">}}</font>");
    break;
  case CLASS_WEREWOLF:
    strcat(buf, " <font color=\"#FFFF00\">((</font><font 
color=\"#3333FF\">Werewolf</font><font color=\"#FFFF00\">))</font>");
    break;
  case CLASS_VAMPIRE:
    strcat(buf, " <font color=\"#666666\">&lt;&lt;</font><font 
color=\"#FF0000\">Vampire</font><font color=\"#666666\">>></font>");
    break;
  case CLASS_HIGHLANDER:
    strcat(buf, " <font color=\"#33CCFF\">-=</font><font 
color=\"#FF0000\">Samu</font><font color=\"#FFFF00\">rai</font><font 
color=\"#33CCFF\">=-</font>");
    break;
  case CLASS_DROW:
    strcat(buf, " <font color=\"#CC33CC\">.o0</font><font 
color=\"#666666\">Drow</font><font color=\"#CC33CC\">0o.</font>");
    break;
  case CLASS_MONK:
    strcat(buf, " <font color=\"#666666\">.x[</font><font 
color=\"#3333FF\">Monk</font><font color=\"#666666\">]x.</font>");
    break;
  case CLASS_NINJA:
    strcat(buf, " <font color=\"#33CCFF\">***</font><font 
color=\"#FFFF00\">Ninja</font><font color=\"#33CCFF\">***</font>");
    break;
  case CLASS_POWER_LICH:
    strcat(buf, " <font color=\"#33FF33\">>*&lt;</font><font 
color=\"#FFFFFF\">Lich</font><font color=\"#33FF33\">>*&lt;</font>");
    break;
  case CLASS_SHAPESHIFTER:
    strcat(buf, " <font color=\"#666666\">[</font><font 
color=\"#CC33CC\">*</font><font color=\"#666666\">]</font><font 
color=\"#FF0000\">Shapeshifter</font><font color=\"#666666\">[</font><font 
color=\"#CC33CC\">*</font><font color=\"#666666\">]</font>");
    break;
  case CLASS_TANARRI:
    strcat(buf, " <font color=\"#FFFF00\">{</font><font 
color=\"#FF0000\">Tanar'ri</font><font color=\"#FFFF00\">}</font>");
    break;
  case CLASS_ANGEL:
    strcat(buf, " <font color=\"#FFFF00\">.x</font><font 
color=\"#666666\">(</font><font color=\"#FFFFFF\">Angel</font><font 
color=\"#666666\">)</font><font color=\"#FFFF00\">x.</font>");
    break;
  case CLASS_UNDEAD_KNIGHT:
    strcat(buf, " <font color=\"#666666\">|[</font><font 
color=\"#3366FF\">Undead Knight</font><font color=\"#666666\">]|</font>");
    break;
  case CLASS_DROID:
    strcat(buf, " <font color=\"#CC33CC\">{</font><font 
color=\"#666666\">-</font><font color=\"#CC33CC\">}</font><font 
color=\"#666666\">Spider
Droid</font><font color=\"#CC33CC\">{</font><font 
color=\"#666666\">-</font><font color=\"#CC33CC\">}</font><font 
color=\"#FFFFFF\"></font>");
    break;
  }
  return (buf[0] != '\0') ? buf+1 : "none";
}

char * plr_kingdom_name(int arg)
{
  static char buf[512];

  buf[0] = '\0';

  switch(arg)
  {
  case 1:
    strcat(buf, " Kingdom 1");
    break;
  case 2:
    strcat(buf, " Kingdom 2");
    break;

    /*
     * etc.
     */

  }
  return (buf[0] != '\0') ? buf+1 : " ";
}

void parse_who()
{
  CHAR_DATA *ch;
  DESCRIPTOR_DATA *d;
  FILE *fp;
  char whofile[200];
  char buf[MAX_STRING_LENGTH];
  char fadein_data[MAX_STRING_LENGTH];
  char fadeout_data[MAX_STRING_LENGTH];
  char body_data[4 * MAX_STRING_LENGTH]; // should be enough

  sprintf(whofile, "%s%s", HTML_DIR, "who.html");
  if ((fp = fopen(whofile, "w")) == NULL)
  {
    log_string("Error writing to who.html");
    return;
  }

  /*
   * Setting up the header and foot of the file
   */
  sprintf(fadein_data, "<html>\n\r");
  strcat(fadein_data, "<body text=\"#FFFFFF\" bgcolor=\"#000000\" 
link=\"#33CCFF\" vlink=\"#33FF33\" alink=\"#FF0000\">\n\r");
  strcat(fadein_data, "<b><font color=\"#33FF33\">Online 
Players</font></b><font color=\"#FFFFFF\"></font>\n\r");
  strcat(fadein_data, "<p><table WIDTH=\"100%\">\n\r");
  strcat(fadein_data, "\n\r");
  strcat(fadein_data, "<tr>\n\r");
  strcat(fadein_data, "<td><b><u>Name</u></b></td>\n\r");
  strcat(fadein_data, "<td><b><u>Class</u></b></td>\n\r");
  strcat(fadein_data, "<td><b><u>Kingdom</u></b></td>\n\r");
  strcat(fadein_data, "</tr>\n\r");

  sprintf(fadeout_data, "\n\r</table>\n\r");
  strcat(fadeout_data, "</body>\n\r");
  strcat(fadeout_data, "</html>\n\r");

  /*
   * Now let's fill out the body of the file.
   */
  sprintf(body_data, "\n\r");
  for (d = descriptor_list; d; d = d->next)
  {
    if (d->connected != CON_PLAYING) continue;
    if ((ch = d->character) == NULL) continue;
    if (ch->level > 6) continue; // we don't list the immortals.

    strcat(body_data, "<tr>\n\r");
    sprintf(buf, "<td>%s</td>\n\r", ch->name);
    strcat(body_data, buf);
    sprintf(buf, "<td>%s</td>\n\r", plr_class_name(ch->class));
    strcat(body_data, buf);
    sprintf(buf, "<td>%s</td>\n\r", 
plr_kingdom_name(ch->pcdata->kingdom));
    strcat(body_data, buf);
    strcat(body_data, "</tr>\n\r");
  }

  /*
   * Write it all to the file, and return.
   */
  fprintf(fp, fadein_data);
  fprintf(fp, body_data);
  fprintf(fp, fadeout_data);

  fclose (fp);
  return;
}

