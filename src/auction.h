AUCTION_DATA *init_auction();
void save_auction();
void load_auction();
void add_auction( OBJ_DATA *item, CHAR_DATA *ch, int minbid, int curbid, int length );
OBJ_DATA *extract_auction( AUCTION_DATA *auc );
bool auction_string( CHAR_DATA *ch, AUCTION_DATA *auc, bool found );

DECLARE_DO_FUN( do_addauction       );
DECLARE_DO_FUN( do_removeauction    );
DECLARE_DO_FUN( do_auction      );
DECLARE_DO_FUN( do_bid          );
DECLARE_DO_FUN( do_auctioninfo      );
DECLARE_DO_FUN( do_killauction      );
DECLARE_DO_FUN( do_auctionextend    );
#define AUC_RUNNING 0
#define AUC_SELLER  1
#define AUC_BUYER   2
#define AUC_RETURN  3
