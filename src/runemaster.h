#define RUNE_ICE    1
#define RUNE_FIRE   2
#define RUNE_LIGHTNING  3
#define RUNE_EARTH  4
#define RUNE_CHAOS  5
#define RUNE_POISON 6
#define RUNE_DARK   7
#define RUNE_LIGHT  8
#define RUNE_UNIVERSAL  9

#define RUNEM_FORGE 1
#define RUNEM_WEAVE 2

#define RS_TAR      1
#define RS_NTAR     2
#define RS_DEF      3

typedef int RUNE_TYPE[10];

typedef struct  runea_type
{
    char    * runes;
    sh_int    number;
    void (*   function)(CHAR_DATA*,CHAR_DATA*);
} RUNEA_TYPE;

typedef struct  runes_type
{
    char    * runes;
    sh_int    number;
    sh_int    times;
    sh_int    type;
    void (*   function)(CHAR_DATA*,CHAR_DATA*);
} RUNES_TYPE;

extern RUNE_TYPE    rune_global;
extern RUNEA_TYPE   rune_attack_table[];
extern RUNES_TYPE   rune_spell_table[];

void rune_count( OBJ_DATA *obj );
void set_runes( OBJ_DATA *obj );

void rune_attack( CHAR_DATA *ch, CHAR_DATA *victim );
bool is_rune_weapon( CHAR_DATA *ch, char *rune );
bool is_rune_robe( CHAR_DATA *ch, char *rune );
bool is_rune_robe_obj( OBJ_DATA *robe, char *rune );
int find_rune_attack( char *rune );

bool rune_spell( CHAR_DATA *ch, char *nvict, OBJ_DATA *scroll );
int find_rune_spell( char *rune );

void dam_word( int dam, char *pvs, char *pvp );
void dam_msg( const char *type, int dam, CHAR_DATA *ch, CHAR_DATA *victim );

// Rune Attacks
void ra_ice_slash   ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_fire_slash  ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_fi_slash    ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_ice_slash   ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_chaos_slash ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_elem_slash  ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_demon_slash ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_shatter_slash   ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_dark_slash  ( CHAR_DATA *ch, CHAR_DATA *victim );
void ra_ltn_slash   ( CHAR_DATA *ch, CHAR_DATA *victim );

// Rune Spells
void rs_fire_wave   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_fire_ball   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_fire_blast  ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_chaos_flames    ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_ice_shot    ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_ice_blast   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_ice_storm   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_chaos_ice   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_lit_bolt    ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_lit_strike  ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_lit_blast   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_lit_chain   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_earth_shake ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_daemon_flare    ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_holy_touch  ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_elemental   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_elem_room   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_chaos_blast ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_fire_ice    ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_ice_charge  ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_fire_charge ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_darkmat     ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_wasteqp     ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_lit_storm   ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_demon_conv  ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_angel_conv  ( CHAR_DATA *ch, CHAR_DATA *victim );
void rs_disintegrate    ( CHAR_DATA *ch, CHAR_DATA *victim );

DECLARE_DO_FUN( do_runecreate   );
DECLARE_DO_FUN( do_runetrain    );
DECLARE_DO_FUN( do_runeforge    );
DECLARE_DO_FUN( do_runeweave    );
DECLARE_DO_FUN( do_runecount    );
DECLARE_DO_FUN( do_runeread );
