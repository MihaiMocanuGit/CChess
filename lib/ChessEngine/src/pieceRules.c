#include "pieceRules.h"
#include "piece.h"

typedef enum
{
    MOVE,
    CAPTURE,
    CASTLE,
    PROMOTE,
    EN_PASSANT

}MoveType_e;
typedef struct
{
    struct
    {
        int x;
        int y;
        MoveType_e type;

        // to make a move we might need information about another piece.
        // for example:
        //  1) With respect to the King, if we decide to castle we must
        //  know where the rook is
        //  2) With respect to the Pawn, if we use en'passant, it might
        //  help us to know what pawn was attacked.
        //  3) Maybe it would be better for every capture to remember which piece
        //  we are attacking. It is redundant information as we can extract it from
        //  the move position.

        Piece *movePartner;

    } moves[64]; //there are 64 spots on the table, so we can't have
                 //more than 64 valid spots for a piece to move in
    int noMoves;
    Piece *subjectPiece;
}LegalMoves;