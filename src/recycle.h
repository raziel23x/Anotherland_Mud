/* stuff for providing a crash-proof buffer */

#define MAX_BUF     16384
#define MAX_BUF_LIST    10
#define BASE_BUF    1024

/* valid states */
#define BUFFER_SAFE 0
#define BUFFER_OVERFLOW 1
#define BUFFER_FREED    2

/* buffer procedures */
BUFFER  *new_buf args( (void) );
BUFFER  *new_buf_size args( (int size) );
void    free_buf args( (BUFFER *buffer) );
bool    add_buf args( (BUFFER *buffer, char *string) );
void    clear_buf args( (BUFFER *buffer) );
char    *buf_string args( (BUFFER *buffer) );

#define MPL MPROG_LIST
MPL     *new_mprog args( (void) );
void    free_mprog args( (MPROG_LIST *mp) );
#undef MPL

/* wizlist data recycling */
#define WD WIZ_DATA
WD  *new_wiz args( (void) );
void    free_wiz args( (WIZ_DATA *ban) );
#undef WD
