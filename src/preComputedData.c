#include "preComputedData.h"
#include "zobrist.h"
#include "utility.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint64_t knight_moves[64];
uint64_t king_moves[64];

const uint64_t a_file = 0x0101010101010101;
const uint64_t first_rank = 0xff00000000000000;

uint64_t rank_attacks[256][8];
smsk masks;

uint64_t rook_magic_numbers[64] = {
    6665332121435906180ULL, 2323858233967513600ULL, 180152918636233344ULL, 5116093643460970752ULL, 1297046055996751904ULL, 72058693684035586ULL, 15168264832261554432ULL, 1513209758272729344ULL, 74872489842327808ULL, 4611756388513751040ULL, 5478770235550994432ULL, 2378182146981953568ULL, 36310581234171952ULL, 72902036148454144ULL, 10133167916974376ULL, 38421383715767040ULL, 4629709762790433408ULL, 18014950414876736ULL, 282575564177424ULL, 63191682295826432ULL, 4900198970275401740ULL, 9223654611947104258ULL, 288234774467913826ULL, 144152571496960324ULL, 10953950581743520358ULL, 360358341083398272ULL, 144150376744026176ULL, 22518105513136384ULL, 2252351721704449ULL, 4400194125952ULL, 580593679157386ULL, 2449972018494571073ULL, 36029071901073412ULL, 5764757881257730048ULL, 457398993031174ULL, 1706444202182656ULL, 1175580257420314624ULL, 4616752705514504208ULL, 11613248038110472ULL, 1225119837215260928ULL, 27079323198455808ULL, 156078295643979840ULL, 281750391685144ULL, 72339211017060360ULL, 4611704710259343381ULL, 2451106087462535296ULL, 4612249127697383492ULL, 40620911648440324ULL, 36100267422261504ULL, 2882451370955247744ULL, 3170569326340125440ULL, 288652966577045632ULL, 9368626320185000064ULL, 338332956634644992ULL, 72059797626168320ULL, 72339077614027008ULL, 18226054514610465ULL, 1297371357608345729ULL, 11854046244460560401ULL, 18023280770351137ULL, 3460171957490812929ULL, 576742519339493889ULL, 90073126855282692ULL, 576887380536231174ULL, };

uint64_t bishop_magic_numbers[64] = {
    2310488454833766432ULL, 585824197654094336ULL, 145267478493134994ULL, 9802089609931280384ULL, 9549322327662002432ULL, 2307268020040892449ULL, 4611765218770460672ULL, 9241599760215904328ULL, 28862318289617168ULL, 4035418789968953472ULL, 9260123384912562176ULL, 576473998007738416ULL, 4611690434190704640ULL, 36874339179888640ULL, 6919781394659415064ULL, 576478628091594752ULL, 4513512697501696ULL, 14062507583537709568ULL, 6346838546722324864ULL, 2306968914019517441ULL, 9289258884399616ULL, 1153203538197872900ULL, 6917740279920347138ULL, 618734921430733324ULL, 72796484106977792ULL, 1166437251425567744ULL, 4756083789849969154ULL, 1158552382892739712ULL, 4785143340351488ULL, 2328502299024428048ULL, 6939769953483752512ULL, 1145184360352004ULL, 5057545817511969824ULL, 38291600623801348ULL, 74878546006378496ULL, 117185966468760080ULL, 299084343443488ULL, 20274999248097287ULL, 9224502626874392710ULL, 5067258251575360ULL, 1153207388905422848ULL, 1171015360078417920ULL, 565149580745226ULL, 18014673660052992ULL, 306253609446671361ULL, 27062425756893280ULL, 2410249751631258ULL, 9426179177167454820ULL, 288302050835431937ULL, 1450724231204835328ULL, 4611723406130808834ULL, 1152979263872827456ULL, 576498755795288072ULL, 435423918817540ULL, 1173222921765667844ULL, 580559361577089ULL, 1153625364132798465ULL, 4620730622569616040ULL, 144134156866852904ULL, 2882303920456336448ULL, 5699883579871744ULL, 288336204817563904ULL, 1171252984647058470ULL, 76578854843875360ULL,
};

magic_entry rook_magics[64];
magic_entry bishop_magics[64];

uint64_t diagonalMask(int sq) {
    const uint64_t maindia = 0x8040201008040201;
    int diag  = (sq&7) - (sq>>3);
    return diag >= 0 ? maindia >> diag*8 : maindia << -diag*8;
}

uint64_t antiDiagMask(int sq) {
    const uint64_t maindia = 0x0102040810204080;
    int diag  = 7 - (sq&7) - (sq>>3);
    return diag >= 0 ? maindia >> diag*8 : maindia << -diag*8;
}

uint64_t diagonalAttacks(uint64_t occ, int sq) {
    uint64_t forward, reverse;
    forward = occ & masks[sq].diagonalMaskEx;
    reverse  = _byteswap_uint64(forward);
    forward -= masks[sq].bitMask;
    reverse -= _byteswap_uint64(masks[sq].bitMask);
    forward ^= _byteswap_uint64(reverse);
    forward &= masks[sq].diagonalMaskEx;
    return forward;
}

uint64_t antiDiagAttacks(uint64_t occ, int sq) {
    uint64_t forward, reverse;
    forward  = occ & masks[sq].antidiagMaskEx;
    reverse  = _byteswap_uint64(forward);
    forward -= masks[sq].bitMask;
    reverse -= _byteswap_uint64(masks[sq].bitMask);
    forward ^= _byteswap_uint64(reverse);
    forward &= masks[sq].antidiagMaskEx;
    return forward;
}

uint64_t fileAttacks(uint64_t occ, int sq) {
    uint64_t forward, reverse;
    forward  = occ & masks[sq].fileMaskEx;
    reverse  = _byteswap_uint64(forward);
    forward -= masks[sq].bitMask;
    reverse -= _byteswap_uint64(masks[sq].bitMask);
    forward ^= _byteswap_uint64(reverse);
    forward &= masks[sq].fileMaskEx;
    return forward;
}

uint64_t rankAttacks(uint64_t occ, int sq)
{
    return rank_attacks[occ >> (8 * (sq >> 3)) & 0b11111111][sq & 7] << (8 * (sq >> 3));
}

uint64_t allAttacks(uint64_t occ, int sq)
{
    return diagonalAttacks(occ, sq) +
           antiDiagAttacks(occ, sq) +
           fileAttacks(occ, sq) +
           rankAttacks(occ, sq);
}

uint64_t project_bits(uint64_t mask, uint64_t bits){
    int idx = 0;
    uint64_t projected_bb = 0;
    while (bits){
        bool set = bits & (1ULL << idx);
        bits &= ~(1Ull << idx);
        int n = poplsb(&mask);
        if (set){
            projected_bb |= 1Ull << n;
        }
        idx++;
    }
    return projected_bb;
}

uint64_t rook_attack(uint64_t occ, int sq)
{
    occ &= rook_magics[sq].mask;
    return rook_magics[sq].table[(occ * rook_magic_numbers[sq]) >> rook_magics[sq].shift];
}

uint64_t bishop_attack(uint64_t occ, int sq)
{
    occ &= bishop_magics[sq].mask;
    return bishop_magics[sq].table[(occ * bishop_magic_numbers[sq]) >> bishop_magics[sq].shift];
}

void find_magics(){
    uint64_t seed = 966479893026083835;
    for (int sq = 0; sq < 64; sq++){
        iteration_rook:
        uint64_t ent = 1ULL << (64 - rook_magics[sq].shift);
        uint64_t magic = PseudorandomNumber(&seed) & PseudorandomNumber(&seed) & PseudorandomNumber(&seed);
        memset(rook_magics[sq].table, 0, sizeof(uint64_t) * 4096);
        for (uint64_t i = 0; i < ent; i++){
            uint64_t blocker_bb = project_bits(rook_magics[sq].mask, i);
            uint64_t idx = (magic * blocker_bb) >> rook_magics[sq].shift;
            uint64_t* entry = &rook_magics[sq].table[idx];
            uint64_t attacks = rankAttacks(blocker_bb, sq) + fileAttacks(blocker_bb, sq);

            if (*entry != 0 && *entry != attacks)
            {
                goto iteration_rook;
            }
            if (*entry == 0 || *entry == attacks)
            {
                *entry = attacks;
            }
        }
        rook_magics[sq].magic = magic;
    }
    for (int sq = 0; sq < 64; sq++){
        iteration_bishop:
        uint64_t ent = 1ULL << (64 - bishop_magics[sq].shift);
        uint64_t magic = PseudorandomNumber(&seed) & PseudorandomNumber(&seed) & PseudorandomNumber(&seed);
        memset(bishop_magics[sq].table, 0, sizeof(uint64_t) * 4096);
        for (uint64_t i = 0; i < ent; i++){
            uint64_t blocker_bb = project_bits(bishop_magics[sq].mask, i);
            uint64_t idx = (magic * blocker_bb) >> bishop_magics[sq].shift;
            uint64_t* entry = &bishop_magics[sq].table[idx];
            uint64_t attacks = diagonalAttacks(blocker_bb, sq) + antiDiagAttacks(blocker_bb, sq);

            if (*entry != 0 && *entry != attacks)
            {
                goto iteration_bishop;
            }
            if (*entry == 0 || *entry == attacks)
            {
                *entry = attacks;
            }
        }
        bishop_magics[sq].magic = magic;
    }

    printf("uint64_t rook_magic_numbers[64] = {\n    ");
    for (int i = 0; i < 64; i++)
    {
        printf("%lluULL, ", rook_magics[i].magic);
    }
    printf("};\n\n");

    printf("uint64_t bishop_magic_numbers[64] = {\n    ");
    for (int i = 0; i < 64; i++)
    {
        printf("%lluULL, ", bishop_magics[i].magic);
    }
    printf("\n};\n\n");
}

void fill_tables()
{
    for (int sq = 0; sq < 64; sq++)
    {
        uint64_t ent = 1ULL << (64 - rook_magics[sq].shift);
        rook_magics[sq].table = malloc(sizeof(uint64_t) * ent);

        ent = 1ULL << (64 - bishop_magics[sq].shift);
        bishop_magics[sq].table = malloc(sizeof(uint64_t) * ent);
    }

    for (int sq = 0; sq < 64; sq++){
        uint64_t ent = 1ULL << (64 - rook_magics[sq].shift);
        memset(rook_magics[sq].table, 0, sizeof(uint64_t) * ent);
        for (uint64_t i = 0; i < ent; i++){
            uint64_t blocker_bb = project_bits(rook_magics[sq].mask, i);
            uint64_t idx = (rook_magic_numbers[sq] * blocker_bb) >> rook_magics[sq].shift;
            uint64_t* entry = &rook_magics[sq].table[idx];
            uint64_t attacks = rankAttacks(blocker_bb, sq) + fileAttacks(blocker_bb, sq);
            *entry = attacks;
        }
    }
    for (int sq = 0; sq < 64; sq++){
        uint64_t ent = 1ULL << (64 - bishop_magics[sq].shift);
        memset(bishop_magics[sq].table, 0, sizeof(uint64_t) * ent);
        for (uint64_t i = 0; i < ent; i++){
            uint64_t blocker_bb = project_bits(bishop_magics[sq].mask, i);
            uint64_t idx = (bishop_magic_numbers[sq] * blocker_bb) >> bishop_magics[sq].shift;
            uint64_t* entry = &bishop_magics[sq].table[idx];
            uint64_t attacks = diagonalAttacks(blocker_bb, sq) + antiDiagAttacks(blocker_bb, sq);
            *entry = attacks;
        }
    }
}

void verify_magics()
{
    for (int sq = 0; sq < 64; sq++)
    {
        uint64_t magic = rook_magic_numbers[sq];
        uint64_t ent = 1ULL << (64 - rook_magics[sq].shift);
        for (uint64_t i = 0; i < ent; i++)
        {
            uint64_t blocker_bb = project_bits(rook_magics[sq].mask, i);
            uint64_t idx = (magic * blocker_bb) >> rook_magics[sq].shift;
            uint64_t attacks = rankAttacks(blocker_bb, sq) + fileAttacks(blocker_bb, sq);
            if (rook_magics[sq].table[idx] != attacks)
            {
                printf("hmm\n");
            }
        }
    }
    for (int sq = 0; sq < 64; sq++)
    {

        uint64_t magic = bishop_magic_numbers[sq];
        uint64_t ent = 1ULL << (64 - bishop_magics[sq].shift);
        for (uint64_t i = 0; i < ent; i++)
        {
            uint64_t blocker_bb = project_bits(bishop_magics[sq].mask, i);
            uint64_t idx = (magic * blocker_bb) >> bishop_magics[sq].shift;
            uint64_t attacks = diagonalAttacks(blocker_bb, sq) + antiDiagAttacks(blocker_bb, sq);
            if (bishop_magics[sq].table[idx] != attacks)
            {
                printf("hmm");
            }
        }
    }
}

__attribute__((constructor))  // runs before main()
static void init_table(void) {
    // right-down, right-up, up-right, up-left, left-up, left-down, down-left, down-right
    const int knight_rank_changes[] = {1, -1, -2, -2, -1, 1, 2, 2};
    const int knight_file_changes[] = {2, 2, 1, -1, -2, -2, -1, 1};

    // Right, left, up, down, right-up, left-up, right-down, left-down
    const int king_rank_changes[] = {0, 0, -1, 1, -1, -1, 1, 1};
    const int king_file_changes[] = {1, -1, 0, 0, -1, 1, -1, 1};

    uint64_t files[8] = {0};

    for (int i = 0; i < 8; i++)
    {
        files[i] = a_file << i;
    }

    for (int i = 0; i < 64; i++)
    {
        masks[i].bitMask = 1ULL << i;
        masks[i].fileMaskEx = files[i % 8] - (1ULL << i);
        masks[i].antidiagMaskEx = antiDiagMask(i) - (1ULL << i);
        masks[i].diagonalMaskEx = diagonalMask(i) - (1ULL << i);
    }

    for (uint32_t i = 0; i < 256; i++)
    {
        for (int sq = 0; sq < 8; sq++)
        {
            int file = sq;
            uint64_t attacks = 0;
            while (1)
            {
                file++;
                if (file >= 8)
                    break;

                attacks |= 1ULL << file;

                if (1ULL << file & i)
                    break;
            }

            file = sq;
            while (1)
            {
                file--;
                if (file < 0)
                    break;

                attacks |= 1ULL << file;

                if (1ULL << file & i)
                    break;
            }

            rank_attacks[i][sq] = attacks;
        }
    }

    for (int i = 0; i < 64; i++) {
        const int file = i % 8;
        const int rank = i / 8;

        unsigned long long knight_bitboard = 0;
        unsigned long long king_bitboard = 0;
        for (int direction = 0; direction < 8; direction++){
            int new_file = file + knight_file_changes[direction];
            int new_rank = rank + knight_rank_changes[direction];
            if (!(new_rank >= 8 || new_rank < 0 || new_file >= 8 || new_file < 0)){
                int target_square = new_rank * 8 + new_file;

                knight_bitboard |= 1ULL << target_square;
            }


            new_file = file + king_file_changes[direction];
            new_rank = rank + king_rank_changes[direction];
            if (!(new_rank >= 8 || new_rank < 0 || new_file >= 8 || new_file < 0)){
                int target_square = new_rank * 8 + new_file;

                king_bitboard |= 1ULL << target_square;
            }
        }
        knight_moves[i] = knight_bitboard;
        king_moves[i] = king_bitboard;
    }

    const uint64_t h_file = a_file << 7;
    const uint64_t last_rank = first_rank >> 56;

    for (int sq = 0; sq < 64; sq++){
        const int file = sq % 8;
        const int rank = sq / 8;
        uint64_t rook_attacks = fileAttacks(0, sq) + rankAttacks(0, sq);
        uint64_t bishop_attacks = diagonalAttacks(0, sq) + antiDiagAttacks(0, sq);

        if (file > 0){
            rook_attacks &= ~a_file;
            bishop_attacks &= ~a_file;
        }
        if (file < 7){
            rook_attacks &= ~h_file;
            bishop_attacks &= ~h_file;
        }
        if (rank > 0){
            rook_attacks &= ~last_rank;
            bishop_attacks &= ~last_rank;
        }
        if (rank < 7){
            rook_attacks &= ~first_rank;
            bishop_attacks &= ~first_rank;
        }

        rook_magics[sq].mask = rook_attacks;
        rook_magics[sq].shift = 64 - __builtin_popcountll(rook_attacks);

        bishop_magics[sq].mask = bishop_attacks;
        bishop_magics[sq].shift = 64 - __builtin_popcountll(bishop_attacks);
    }

    fill_tables();
    verify_magics();
}