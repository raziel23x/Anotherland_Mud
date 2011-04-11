#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void Economy_List(CHAR_DATA *ch);
void Economy_List2(CHAR_DATA *ch);
void Economy_Reset();
void Economy_Totalreset();
void Economy_Set(CHAR_DATA *ch, int nBank, char *Command, char *Value);
int Economy_Info(CHAR_DATA *ch,char *cpType, int nAmount );
void Economy_Transact(CHAR_DATA *ch, int nBankNumber,char *Sell,char *Buy);
void Bank_Stat(CHAR_DATA *ch);
void Load_Economy()
{

    int i;
    FILE *fp;

    if ((fp = fopen("../txt/economy.txt", "r")) == NULL)
    {
        log_string("Fatal Error: economy.txt not found!");
        exit(1);
    }
    for (i = 0; i < MAX_ECONOMY; i++)
    {
        economy_table[i].caEconomyName = fread_string(fp);

        economy_table[i].nExpRate = fread_number(fp);
        economy_table[i].nPrimalRate = fread_number(fp);
        economy_table[i].nQuestRate = fread_number(fp);
        economy_table[i].nClassRate = fread_number(fp);

        economy_table[i].nExpAmount = fread_number(fp);
        economy_table[i].nPrimalAmount = fread_number(fp);
        economy_table[i].nQuestAmount = fread_number(fp);
        economy_table[i].nClassAmount = fread_number(fp);

        economy_table[i].nMaxAmount = fread_number(fp);

        economy_table[i].nTimeTillRegeneration = fread_number(fp);
        economy_table[i].nTimeTillReset = fread_number(fp);
        economy_table[i].caLastPerson = fread_string(fp);
        economy_table[i].nExchanges = fread_number(fp);

        economy_table[i].nExpBought = fread_number(fp);
        economy_table[i].nPrimalBought = fread_number(fp);
        economy_table[i].nQuestBought = fread_number(fp);
        economy_table[i].nClassBought = fread_number(fp);

        economy_table[i].nExpSold = fread_number(fp);
        economy_table[i].nPrimalSold = fread_number(fp);
        economy_table[i].nQuestSold = fread_number(fp);
        economy_table[i].nClassSold = fread_number(fp);
    }
    fclose(fp);
}

void Economy_Reset()
{
    int nEcon;
    for ( nEcon = 0; nEcon < MAX_ECONOMY; nEcon++ )
    {
        if ( nEcon == 0 )
            economy_table[nEcon].caEconomyName = "Normal Bank";
        else if ( nEcon == 1 )
            economy_table[nEcon].caEconomyName = "Newbie Bank";

        //economy_table[nEcon].nExpRate = 150000000;
        //economy_table[nEcon].nPrimalRate = 200;
        //economy_table[nEcon].nQuestRate = 100;
        //economy_table[nEcon].nClassRate = 150000000;

        economy_table[nEcon].nExpAmount = economy_table[nEcon].nMaxAmount*.75;
        economy_table[nEcon].nPrimalAmount = economy_table[nEcon].nMaxAmount*.75;
        economy_table[nEcon].nQuestAmount = economy_table[nEcon].nMaxAmount*.75;
        economy_table[nEcon].nClassAmount = economy_table[nEcon].nMaxAmount*.75;

        //economy_table[nEcon].nMaxAmount = 2000;

        economy_table[nEcon].nTimeTillRegeneration = 60;
        economy_table[nEcon].nTimeTillReset = 1440;
        economy_table[nEcon].caLastPerson = "No One";
        //economy_table[nEcon].nExchanges = 0;

        //economy_table[nEcon].nExpBought = 0;
        //economy_table[nEcon].nPrimalBought = 0;
        //economy_table[nEcon].nQuestBought = 0;
        //economy_table[nEcon].nClassBought = 0;

        //economy_table[nEcon].nExpSold = 0;
        //economy_table[nEcon].nPrimalSold = 0;
        //economy_table[nEcon].nQuestSold = 0;
        //economy_table[nEcon].nClassSold = 0;
    }
}

void Economy_Totalreset()
{
    int nEcon;
    for ( nEcon = 0; nEcon < MAX_ECONOMY; nEcon++ )
    {
        if ( nEcon == 0 )
            economy_table[nEcon].caEconomyName = "Normal Bank";
        else if ( nEcon == 1 )
            economy_table[nEcon].caEconomyName = "Newbie Bank";

        economy_table[nEcon].nExpRate = 150000000;
        economy_table[nEcon].nPrimalRate = 200;
        economy_table[nEcon].nQuestRate = 100;
        economy_table[nEcon].nClassRate = 150000000;

        economy_table[nEcon].nExpAmount = economy_table[nEcon].nMaxAmount*.75;
        economy_table[nEcon].nPrimalAmount = economy_table[nEcon].nMaxAmount*.75;
        economy_table[nEcon].nQuestAmount = economy_table[nEcon].nMaxAmount*.75;
        economy_table[nEcon].nClassAmount = economy_table[nEcon].nMaxAmount*.75;

        //economy_table[nEcon].nMaxAmount = 2000;

        economy_table[nEcon].nTimeTillRegeneration = 60;
        economy_table[nEcon].nTimeTillReset = 1440;
        economy_table[nEcon].caLastPerson = "No One";
        economy_table[nEcon].nExchanges = 0;

        economy_table[nEcon].nExpBought = 0;
        economy_table[nEcon].nPrimalBought = 0;
        economy_table[nEcon].nQuestBought = 0;
        economy_table[nEcon].nClassBought = 0;

        economy_table[nEcon].nExpSold = 0;
        economy_table[nEcon].nPrimalSold = 0;
        economy_table[nEcon].nQuestSold = 0;
        economy_table[nEcon].nClassSold = 0;
    }
}

void Save_Economy()
{
    int i;
    FILE *fp;

    if ((fp = fopen("../txt/economy.txt","w")) == NULL)
    {
        log_string("Error writing to economy.txt");
        return;
    }
    for (i = 0; i < MAX_ECONOMY; i++)
    {
        fprintf(fp, "%s~\n", economy_table[i].caEconomyName);
        fprintf(fp, "%d %d %d %d\n",
            economy_table[i].nExpRate, economy_table[i].nPrimalRate, economy_table[i].nQuestRate, economy_table[i].nClassRate);
        fprintf(fp, "%d %d %d %d %d\n",
            economy_table[i].nExpAmount, economy_table[i].nPrimalAmount, economy_table[i].nQuestAmount, economy_table[i].nClassAmount,
            economy_table[i].nMaxAmount);
        fprintf(fp, "%d %d\n",
            economy_table[i].nTimeTillRegeneration, economy_table[i].nTimeTillReset);
        fprintf(fp, "%s~\n", economy_table[i].caLastPerson);
        fprintf(fp, "%d\n", economy_table[i].nExchanges);
        fprintf(fp, "%d %d %d %d\n",
            economy_table[i].nExpBought, economy_table[i].nPrimalBought, economy_table[i].nQuestBought, economy_table[i].nClassBought);
        fprintf(fp, "%d %d %d %d\n",
            economy_table[i].nExpSold, economy_table[i].nPrimalSold, economy_table[i].nQuestSold, economy_table[i].nClassSold);
    }
    fclose(fp);
}

void do_economytest(CHAR_DATA *ch, char *argument )
{
    char Command[MAX_INPUT_LENGTH];
    char Sell[MAX_INPUT_LENGTH];
    char Buy[MAX_INPUT_LENGTH];
    int nBank = 0;
    char caTemp[100];
    char caTemp1[100];
    char caTemp2[100];

    if ( IS_NPC(ch) ) return;
    argument = one_argument(argument,Command);
    if ( !str_cmp(Command,"Set") )
    {
        argument = one_argument(argument,caTemp);
        nBank = atoi(caTemp);
        if ( nBank >= MAX_ECONOMY ) return;
        argument = one_argument(argument,caTemp1);
        strcpy(caTemp2,argument);
        Economy_Set(ch,nBank,caTemp1,caTemp2);
        return;
    }
    else if ( !str_cmp(Command,"Bounce" ) )
    {
        send_to_char("Disabled atm",ch);
        return;
        do_bankbounce(ch,argument);
    }
    argument = one_argument(argument, Sell);
    strcpy(Buy, argument);
    if (Command[0] == '\0')
    {
        send_to_char("Syntax: Bank List/Stat or Bank <Exchange Bank Number> <Pr.Exp.Qp.Cp> <Pr.Exp.Qp.Cp> \n\r",ch);
        return;
    }
    if ( !str_cmp(Command,"List") )
    {
        Economy_List(ch);
        return;
    }
    if ( !str_cmp(Command,"ImmList") && ch->level > 4)
    {
        Economy_List2(ch);
        return;
    }
    else if ( !str_cmp(Command,"Reset") && ch->level > 12)
    {
        Economy_Reset();
        return;
    }
    else if ( !str_cmp(Command,"Totalreset") && ch->level > 12)
    {
        Economy_Totalreset();
        return;
    }
    else if ( !str_cmp(Command,"Stat") )
    {
        Bank_Stat(ch);
        return;
    }
    else if ( !str_cmp(Command,"Save" ) && ch->level > 12 )
    {
        Save_Economy();
        return;
    }
    else if ( !str_cmp(Command,"Load") && ch->level > 12)
    {
        Load_Economy();
        return;
    }
    else if ( is_number(Command) )
    {
        int nBank = atoi(Command);
        if ( str_cmp(Buy,"Pr") && str_cmp(Buy,"Qp") && str_cmp(Buy,"Exp") && str_cmp(Buy,"Cp") )
        {
            send_to_char("Syntax: Bank <Exchange Bank Number> <Pr.Exp.Qp.Cp> <Pr.Exp.Qp.Cp>\n\r",ch);
            return;
        }
        else
        {
            Economy_Transact(ch,nBank,Sell,Buy);
        }
    }
    else
    {
        send_to_char("Type Bank Please.\n\r",ch);
    }
    return;
}

void Economy_List(CHAR_DATA *ch)
{
    char caBuffer[1024];
    int nEcon;
    for ( nEcon = 0; nEcon < MAX_ECONOMY; nEcon++ )
    {
        sprintf(caBuffer,"Economy Name: %s Economy ID: %d\n\r",
            economy_table[nEcon].caEconomyName, nEcon);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Regen: \t%d.\tReset: %d.\tPool Max: %d \tNote: ~Minutes.\n\r",
            number_range(1,100),economy_table[nEcon].nTimeTillReset+1,economy_table[nEcon].nMaxAmount);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Exp Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nExpRate,economy_table[nEcon].nExpAmount,economy_table[nEcon].nExpSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Primal Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nPrimalRate,economy_table[nEcon].nPrimalAmount,economy_table[nEcon].nPrimalSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Quest Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nQuestRate,economy_table[nEcon].nQuestAmount,economy_table[nEcon].nQuestSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Class Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nClassRate,economy_table[nEcon].nClassAmount,economy_table[nEcon].nClassSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Bought: Exp: \t%d, Primal: %d, Quest: %d, Class: %d: Total: %d\n\r",
            economy_table[nEcon].nExpBought,economy_table[nEcon].nPrimalBought,economy_table[nEcon].nQuestBought,economy_table[nEcon].nClassBought,economy_table[nEcon].nExchanges);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Last Person to use %s was: %s.\n\r",
            economy_table[nEcon].caEconomyName,economy_table[nEcon].caLastPerson);
        send_to_char(caBuffer,ch);
        send_to_char("\n\r",ch);
    }
    return;
}

void Economy_List2(CHAR_DATA *ch)
{
    char caBuffer[1024];
    int nEcon;
    for ( nEcon = 0; nEcon < MAX_ECONOMY; nEcon++ )
    {
        sprintf(caBuffer,"Economy Name: %s Economy ID: %d\n\r",
            economy_table[nEcon].caEconomyName, nEcon);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Regen: \t%d.\tReset: %d.\tPool Max: %d \tNote: ~Minutes.\n\r",
            economy_table[nEcon].nTimeTillRegeneration,economy_table[nEcon].nTimeTillReset+1,economy_table[nEcon].nMaxAmount);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Exp Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nExpRate,economy_table[nEcon].nExpAmount,economy_table[nEcon].nExpSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Primal Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nPrimalRate,economy_table[nEcon].nPrimalAmount,economy_table[nEcon].nPrimalSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Quest Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nQuestRate,economy_table[nEcon].nQuestAmount,economy_table[nEcon].nQuestSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Class Rate: \t%-10d. \tPool: %-10d. \tSold: %-10d\n\r",
            economy_table[nEcon].nClassRate,economy_table[nEcon].nClassAmount,economy_table[nEcon].nClassSold);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Bought: Exp: \t%d, Primal: %d, Quest: %d, Class: %d: Total: %d\n\r",
            economy_table[nEcon].nExpBought,economy_table[nEcon].nPrimalBought,economy_table[nEcon].nQuestBought,economy_table[nEcon].nClassBought,economy_table[nEcon].nExchanges);
        send_to_char(caBuffer,ch);
        sprintf(caBuffer,"Last Person to use %s was: %s.\n\r",
            economy_table[nEcon].caEconomyName,economy_table[nEcon].caLastPerson);
        send_to_char(caBuffer,ch);
        send_to_char("\n\r",ch);
    }
    return;
}

void Economy_Transact(CHAR_DATA *ch, int nBankNumber,char *Sell,char *Buy)
{
    if(IS_CLASS(ch, CLASS_RPONLY))return;
    if ( !str_cmp(Sell,Buy) ) return stcf(ch,"You can't bank the same thing...\n\r");
    if ( nBankNumber >= MAX_ECONOMY )
    {
        send_to_char("That bank doesn't even exist.\n\r",ch);
        return;
    }
    if ( str_cmp(Sell,"exp" ) && str_cmp(Sell,"pr" ) && str_cmp(Sell,"qp" ) && str_cmp(Sell,"cp" ) )
    {
        send_to_char("Bank <Bank ID Number> *<exp.pr.qp.cp> <exp.pr.qp.cp> \n\r",ch);
        return;
    }
    if ( str_cmp(Buy,"exp" ) && str_cmp(Buy,"pr" ) && str_cmp(Buy,"qp" ) && str_cmp(Buy,"cp" ) )
    {
        send_to_char("Bank <Bank ID Number> <exp.pr.qp.cp> *<exp.pr.qp.cp>\n\r",ch);
        return;
    }
    if ( !str_cmp(Sell,"exp" ) )
    {
        if ( ch->exp < economy_table[nBankNumber].nExpRate )
        {
            stcf(ch,"You only have %d out of the required %d exp to bank with, you're low %d.\n\r",
                ch->exp,economy_table[nBankNumber].nExpRate,economy_table[nBankNumber].nExpRate-ch->exp);
            return;
        }
        if ( economy_table[nBankNumber].nExpAmount >= economy_table[nBankNumber].nMaxAmount )
        {
            stcf(ch,"Please come back in %d minutes, as the exp pool has too much in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration+1);
            return;
        }
    }
    else if ( !str_cmp(Sell,"pr" ) )
    {
        if ( ch->practice < economy_table[nBankNumber].nPrimalRate )
        {
            stcf(ch,"You only have %d out of the required %d primal to bank with, you're low %d.\n\r",
                ch->practice,economy_table[nBankNumber].nPrimalRate,economy_table[nBankNumber].nPrimalRate-ch->practice);
            return;
        }
        if ( economy_table[nBankNumber].nPrimalAmount >= economy_table[nBankNumber].nMaxAmount )
        {
            stcf(ch,"Please come back in %d minutes, as the primal pool has too much in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration+1);
            return;
        }
    }
    else if ( !str_cmp(Sell,"qp" ) )
    {
        if ( ch->pcdata->quest < economy_table[nBankNumber].nQuestRate )
        {
            stcf(ch,"You only have %d out of the required %d quest points to bank with, you're low %d.\n\r",
                ch->pcdata->quest,economy_table[nBankNumber].nQuestRate,economy_table[nBankNumber].nQuestRate-ch->pcdata->quest);
            return;
        }
        if ( economy_table[nBankNumber].nQuestAmount >= economy_table[nBankNumber].nMaxAmount )
        {
            stcf(ch,"Please come back in %d minutes, as the quest point pool has too much in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration+1);
            return;
        }
    }
    else if ( !str_cmp(Sell,"cp" ) )
    {
        if ( Economy_Info(ch,"Class Points",0) == -1 )
        {
            send_to_char("You don't have class points.\n\r",ch);
            return;
        }
        if ( Economy_Info(ch,"Class Points",0) < economy_table[nBankNumber].nClassRate )
        {
            stcf(ch,"You only have %d out of the required %d class points to bank with, you're low %d.\n\r",
                Economy_Info(ch,"Class Points",0),economy_table[nBankNumber].nExpRate,economy_table[nBankNumber].nExpRate-Economy_Info(ch,"Class Points",0));
            return;
        }
        if ( economy_table[nBankNumber].nClassAmount >= economy_table[nBankNumber].nMaxAmount )
        {
            stcf(ch,"Please come back in %d minutes, as the class point pool has too much in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration+1);
            return;
        }
    }
    if ( !str_cmp(Buy,"exp" ) )
    {
        if ( ch->exp > ( 2100000000 - economy_table[nBankNumber].nExpRate )  )
        {
            send_to_char("Please lose some experience points first.\n\r",ch);
            return;
        }
        if ( economy_table[nBankNumber].nExpAmount <= 0 )
        {
            stcf(ch,"Please come back in %d minutes, as the exp pool ran out in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration);
            return;
        }
    }
    else if ( !str_cmp(Buy,"pr" ) )
    {
        if ( ( ch->practice + economy_table[nBankNumber].nPrimalAmount ) > 10000 )
        {
            send_to_char("Please lose some primal first.\n\r",ch);
            return;
        }
        if ( economy_table[nBankNumber].nPrimalAmount <= 0 )
        {
            stcf(ch,"Please come back in %d minutes, as the primal pool has run out in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration);
            return;
        }
    }
    else if ( !str_cmp(Buy,"qp" ) )
    {
        if ( ch->pcdata->quest > ( 2100000000 - economy_table[nBankNumber].nQuestRate ) )
        {
            send_to_char("Please lose some quest points first.\n\r",ch);
            return;
        }
        if ( economy_table[nBankNumber].nQuestAmount <= 0 )
        {
            stcf(ch,"Please come back in %d minutes, as the quest point pool ran out in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration);
            return;
        }
    }
    else if ( !str_cmp(Buy,"cp" ) )
    {
        if ( Economy_Info(ch,"Class Points",0) == -1 )
        {
            send_to_char("You don't have class points.\n\r",ch);
            return;
        }
        if ( Economy_Info(ch,"Class Points",0 ) > ( 2100000000-economy_table[nBankNumber].nClassRate ) )
        {
            send_to_char("Please lose some class points first.\n\r",ch);
            return;
        }
        if ( economy_table[nBankNumber].nClassAmount <= 0 )
        {
            stcf(ch,"Please come back in %d minutes, as the class point pool has ran out in this bank.\n\r",
                economy_table[nBankNumber].nTimeTillRegeneration);
            return;
        }
    }
    if ( !str_cmp(Sell,"exp" ) )
    {
        ch->exp -= economy_table[nBankNumber].nExpRate;
        economy_table[nBankNumber].nExpAmount++;
        economy_table[nBankNumber].nExpSold++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    else if ( !str_cmp(Sell,"pr" ) )
    {
        ch->practice -= economy_table[nBankNumber].nPrimalRate;
        economy_table[nBankNumber].nPrimalAmount++;
        economy_table[nBankNumber].nPrimalSold++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    else if ( !str_cmp(Sell,"qp" ) )
    {
        ch->pcdata->quest -= economy_table[nBankNumber].nQuestRate;
        ch->pcdata->questtotal -= economy_table[nBankNumber].nQuestRate/2;
        economy_table[nBankNumber].nQuestAmount++;
        economy_table[nBankNumber].nQuestSold++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    else if ( !str_cmp(Sell,"cp" ) )
    {
        Economy_Info(ch,"Subtract Class Points",economy_table[nBankNumber].nClassRate);
        economy_table[nBankNumber].nClassAmount++;
        economy_table[nBankNumber].nClassSold++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    if ( !str_cmp(Buy,"exp" ) )
    {
        ch->exp += economy_table[nBankNumber].nExpRate;
        economy_table[nBankNumber].nExpAmount--;
        economy_table[nBankNumber].nExpBought++;
        economy_table[nBankNumber].nExchanges++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    else if ( !str_cmp(Buy,"pr" ) )
    {
        ch->practice += economy_table[nBankNumber].nPrimalRate;
        economy_table[nBankNumber].nPrimalAmount--;
        economy_table[nBankNumber].nPrimalBought++;
        economy_table[nBankNumber].nExchanges++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    else if ( !str_cmp(Buy,"qp" ) )
    {
        ch->pcdata->quest += economy_table[nBankNumber].nQuestRate;
        ch->pcdata->questtotal += economy_table[nBankNumber].nQuestRate/2;
        economy_table[nBankNumber].nQuestAmount--;
        economy_table[nBankNumber].nQuestBought++;
        economy_table[nBankNumber].nExchanges++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    else if ( !str_cmp(Buy,"cp" ) )
    {
        Economy_Info(ch,"Add Class Points",economy_table[nBankNumber].nClassRate);
        economy_table[nBankNumber].nClassAmount--;
        economy_table[nBankNumber].nClassBought++;
        economy_table[nBankNumber].nExchanges++;
        economy_table[nBankNumber].caLastPerson = str_dup(ch->name);
    }
    WAIT_STATE(ch,3);
    send_to_char("Transaction Complete.\n\r",ch);
}

int Economy_Info(CHAR_DATA *ch,char *cpType, int nAmount )
{
    if ( !IS_CLASS( ch, CLASS_DEMON) && !IS_CLASS( ch, CLASS_DROID) &&
        !IS_CLASS( ch, CLASS_TANARRI) && !IS_CLASS( ch, CLASS_DROW ) && !IS_CLASS( ch, CLASS_KAHN ))
        return -1;
    if ( !str_cmp(cpType,"Class Points") )
    {
        if ( IS_CLASS( ch, CLASS_DEMON) )
            return ch->pcdata->stats[DEMON_CURRENT];
        else if ( IS_CLASS( ch, CLASS_DROID) )
            return ch->pcdata->stats[DROID_POWER];
        else if ( IS_CLASS( ch, CLASS_KAHN) )
            return ch->pcdata->stats[KAHN_CURRENT];
        else if ( IS_CLASS( ch, CLASS_TANARRI) )
            return ch->pcdata->stats[TPOINTS];
        else if ( IS_CLASS( ch, CLASS_DROW) )
            return ch->pcdata->stats[DROW_POWER];
    }
    else if ( !str_cmp(cpType,"Subtract Class Points" ) )
    {
        if ( IS_CLASS( ch, CLASS_DEMON) )
            ch->pcdata->stats[DEMON_CURRENT] -= nAmount;
        else if ( IS_CLASS( ch, CLASS_DROID) )
            ch->pcdata->stats[DROID_POWER] -= nAmount;
        else if ( IS_CLASS( ch, CLASS_KAHN) )
            ch->pcdata->stats[KAHN_CURRENT] -= nAmount;
        else if ( IS_CLASS( ch, CLASS_TANARRI) )
            ch->pcdata->stats[TPOINTS] -= nAmount;
        else if ( IS_CLASS( ch, CLASS_DROW) )
            ch->pcdata->stats[DROW_POWER] -= nAmount;
    }
    else if ( !str_cmp(cpType,"Add Class Points" ) )
    {
        if ( IS_CLASS( ch, CLASS_DEMON) )
            ch->pcdata->stats[DEMON_CURRENT] += nAmount;
        else if ( IS_CLASS( ch, CLASS_KAHN) )
            ch->pcdata->stats[KAHN_CURRENT] += nAmount;
        else if ( IS_CLASS( ch, CLASS_DROID) )
            ch->pcdata->stats[DROID_POWER] += nAmount;
        else if ( IS_CLASS( ch, CLASS_TANARRI) )
            ch->pcdata->stats[TPOINTS] += nAmount;
        else if ( IS_CLASS( ch, CLASS_DROW) )
            ch->pcdata->stats[DROW_POWER] += nAmount;
    }
    return 0;
}

void Economy_Set(CHAR_DATA *ch, int nBank, char *Command, char *Value)
{
    //	char caTemp[100];
    if ( str_cmp(ch->name,"Seth" ) ) return;
    if ( Command[0] == '\0')
    {
        send_to_char(" Initiating Economy Editing System...... Complete.\n\r",ch);
        send_to_char(" The command is: Economy Set <All the variables in the struct is possible>.\n\r",ch);
        return;
    }
    if ( nBank >= MAX_ECONOMY ) return;
    if ( !str_cmp(Command, "caEconomyName" ) )
        economy_table[nBank].caEconomyName = str_dup(Value);
    else if ( !str_cmp(Command, "nExpRate" ) )
        economy_table[nBank].nExpRate = atoi(Value);
    else if ( !str_cmp(Command, "nPrimalRate" ) )
        economy_table[nBank].nPrimalRate = atoi(Value);
    else if ( !str_cmp(Command, "nQuestRate" ) )
        economy_table[nBank].nQuestRate = atoi(Value);
    else if ( !str_cmp(Command, "nClassRate" ) )
        economy_table[nBank].nClassRate = atoi(Value);
    else if ( !str_cmp(Command, "nExpAmount" ) )
        economy_table[nBank].nExpAmount = atoi(Value);
    else if ( !str_cmp(Command, "nQuestAmount" ) )
        economy_table[nBank].nQuestAmount = atoi(Value);
    else if ( !str_cmp(Command, "nPrimalAmount" ) )
        economy_table[nBank].nPrimalAmount = atoi(Value);
    else if ( !str_cmp(Command, "nClassAmount" ) )
        economy_table[nBank].nClassAmount = atoi(Value);
    else if ( !str_cmp(Command, "nMaxAmount" ) )
        economy_table[nBank].nMaxAmount = atoi(Value);
    else if ( !str_cmp(Command, "nTimeTillRegeneration" ) )
        economy_table[nBank].nTimeTillRegeneration = atoi(Value);
    else if ( !str_cmp(Command, "nTimeTillReset" ) )
        economy_table[nBank].nTimeTillReset = atoi(Value);
    else if ( !str_cmp(Command, "caLastPerson" ) )
        economy_table[nBank].caLastPerson = str_dup(Value);
    else if ( !str_cmp(Command, "nExchanges" ) )
        economy_table[nBank].nExchanges = atoi(Value);
    else if ( !str_cmp(Command, "nExpBought" ) )
        economy_table[nBank].nExpBought = atoi(Value);
    else if ( !str_cmp(Command, "nPrimalBought" ) )
        economy_table[nBank].nPrimalBought = atoi(Value);
    else if ( !str_cmp(Command, "nQuestBought" ) )
        economy_table[nBank].nQuestBought = atoi(Value);
    else if ( !str_cmp(Command, "nClassBought" ) )
        economy_table[nBank].nClassBought = atoi(Value);
    else if ( !str_cmp(Command, "nExpSold" ) )
        economy_table[nBank].nExpSold = atoi(Value);
    else if ( !str_cmp(Command, "nPrimalSold" ) )
        economy_table[nBank].nPrimalSold = atoi(Value);
    else if ( !str_cmp(Command, "nQuestSold" ) )
        economy_table[nBank].nQuestSold = atoi(Value);
    else if ( !str_cmp(Command, "nClassSold" ) )
        economy_table[nBank].nClassSold = atoi(Value);
    else
    {
        stcf(ch,"You fucked something else.You typed: %d %s %s\n\r",nBank,Command,Value);
        return;
    }
    Save_Economy();
    send_to_char("Done\n\r",ch);
    return;
}

void Bank_Stat(CHAR_DATA *ch)
{
    unsigned long int nTotalExp = 0;
    long int nTotalQuest = 0;
    long int nTotalPrimal = 0;
    unsigned long int nTotalClass = 0;
    unsigned long int nCurrentExp = 0;
    long int nCurrentQuest = 0;
    long int nCurrentPrimal = 0;
    unsigned long int nCurrentClass = 0;
    int nBank = 0;
    for ( nBank = 0; nBank < MAX_ECONOMY; nBank++ )
    {
        nTotalExp += economy_table[nBank].nExpBought*(economy_table[nBank].nExpRate/1000000);
        nTotalQuest += economy_table[nBank].nQuestBought*economy_table[nBank].nQuestRate;
        nTotalPrimal += economy_table[nBank].nPrimalBought*economy_table[nBank].nPrimalRate;
        nTotalClass += economy_table[nBank].nClassBought*(economy_table[nBank].nClassRate/1000000);
        nCurrentExp += economy_table[nBank].nExpAmount*(economy_table[nBank].nExpRate/1000000);
        nCurrentQuest += economy_table[nBank].nQuestAmount*economy_table[nBank].nQuestRate;
        nCurrentPrimal += economy_table[nBank].nPrimalAmount*economy_table[nBank].nPrimalRate;
        nCurrentClass += economy_table[nBank].nClassAmount*(economy_table[nBank].nClassRate/10000000);
    }

    stcf(ch,"Total amount of exp sold        : %lu0000000\n\r",nTotalExp );
    stcf(ch,"Total amount of quest sold      : %ld\n\r",nTotalQuest );
    stcf(ch,"Total amount of primal sold     : %ld\n\r",nTotalPrimal );
    stcf(ch,"Total amount of class sold      : %lu0000000\n\r",nTotalClass );
    stcf(ch,"Total amount of exp available   : %lu0000000\n\r",nCurrentExp );
    stcf(ch,"Total amount of quest available : %ld\n\r",nCurrentQuest );
    stcf(ch,"Total amount of primal available: %ld\n\r",nCurrentPrimal );
    stcf(ch,"Total amount of class available : %lu0000000\n\r",nCurrentClass );
    return;
}
