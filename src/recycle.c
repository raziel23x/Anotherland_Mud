/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  MOBprograms for ROM 2.4 v0.98g (C) M.Nylander 1996                     *
 *  Based on MERC 2.2 MOBprograms concept by N'Atas-ha.                    *
 *  Written and adapted to ROM 2.4 by                                      *
 *          Markku Nylander (markku.nylander@uta.fi)                       *
 *  This code may be copied and distributed as per the ROM license.        *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <ctype.h>
#include "merc.h"
#include "recycle.h"

//BUFFER STUFF
BUFFER *buf_free;

BUFFER * new_buf()
{
    BUFFER *buffer;

    if (buf_free == NULL)
        buffer = alloc_perm(sizeof(*buffer));
    else
    {
        buffer = buf_free;
        buf_free = buf_free->next;
    }
    buffer->next        = NULL;
    buffer->state       = BUFFER_SAFE;
    buffer->size        = get_size(BASE_BUF);

    buffer->string      = alloc_mem(buffer->size);
    buffer->string[0]   = '\0';
    VALIDATE(buffer);

    return buffer;
}

BUFFER * new_buf_size(int size)
{
    BUFFER *buffer;

    if (buf_free == NULL)
        buffer = alloc_perm(sizeof(*buffer));
    else
    {
        buffer = buf_free;
        buf_free = buf_free->next;
    }

    buffer->next        = NULL;
    buffer->state       = BUFFER_SAFE;
    buffer->size        = get_size(size);
    if (buffer->size == -1)
    {
        bug("new_buf: buffer size %d too large.",size);
        exit(2);
    }
    buffer->string      = alloc_mem(buffer->size);
    buffer->string[0]   = '\0';
    VALIDATE(buffer);

    return buffer;
}

/* stuff for recycling wizlist structures */
WIZ_DATA *wiz_free;

WIZ_DATA *new_wiz(void)
{
    static WIZ_DATA wiz_zero;
    WIZ_DATA *wiz;

    if (wiz_free == NULL)
        wiz = alloc_perm(sizeof(*wiz));
    else
    {
        wiz = wiz_free;
        wiz_free = wiz_free->next;
    }

    *wiz = wiz_zero;
    VALIDATE(wiz);
    wiz->name = &str_empty[0];
    return wiz;
}

void free_wiz(WIZ_DATA *wiz)
{
    if (!IS_VALID(wiz))
        return;

    free_string(wiz->name);
    INVALIDATE(wiz);

    wiz->next = wiz_free;
    wiz_free = wiz;
}

void free_buf(BUFFER *buffer)
{
    if (!IS_VALID(buffer))
        return;

    free_mem(buffer->string,buffer->size);
    buffer->string = NULL;
    buffer->size   = 0;
    buffer->state  = BUFFER_FREED;
    INVALIDATE(buffer);

    buffer->next  = buf_free;
    buf_free      = buffer;
}

/* buffer sizes */
const int buf_size[MAX_BUF_LIST] =
{
    16,32,64,128,256,1024,2048,4096,8192,16384
};

int get_size (int val)
{
    int i;

    for (i = 0; i < MAX_BUF_LIST; i++)
        if (buf_size[i] >= val)
    {
        return buf_size[i];
    }

    return -1;
}

char * buf_string(BUFFER *buffer)
{
    return buffer->string;
}

void clear_buf(BUFFER *buffer)
{
    buffer->string[0] = '\0';
    buffer->state     = BUFFER_SAFE;
}

bool add_buf(BUFFER *buffer, char *string)
{
    int len;
    char *oldstr;
    int oldsize;

    oldstr = buffer->string;
    oldsize = buffer->size;

    if (buffer->state == BUFFER_OVERFLOW)                   /* don't waste time on bad strings! */
        return FALSE;
    len = strlen(buffer->string) + strlen(string) + 1;

    while (len >= buffer->size)                             /* increase the buffer size */
    {
        buffer->size    = get_size(buffer->size + 1);
        {
            if (buffer->size == -1)                         /* overflow */
            {
                buffer->size = oldsize;
                buffer->state = BUFFER_OVERFLOW;
                bug("buffer overflow past size %d",buffer->size);
                return FALSE;
            }
        }
    }

    if (buffer->size != oldsize)
    {
        buffer->string  = alloc_mem(buffer->size);

        strcpy(buffer->string,oldstr);
        free_mem(oldstr,oldsize);
    }

    strcat(buffer->string,string);
    return TRUE;
}

MPROG_LIST *mprog_free;

MPROG_LIST *new_mprog(void)
{
    static MPROG_LIST mp_zero;
    MPROG_LIST *mp;

    if (mprog_free == NULL)
        mp = alloc_perm(sizeof(*mp));
    else
    {
        mp = mprog_free;
        mprog_free=mprog_free->next;
    }

    *mp = mp_zero;
    mp->vnum             = 0;
    mp->trig_type        = 0;
    mp->code             = str_dup("");
    VALIDATE(mp);
    return mp;
}

void free_mprog(MPROG_LIST *mp)
{
    if (!IS_VALID(mp))
        return;

    INVALIDATE(mp);
    mp->next = mprog_free;
    mprog_free = mp;
}
