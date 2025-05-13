#include <behaviour-ai.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <format>
#include "piece-api.h"
#include "notation.h"
#include "piece-type.h"
#include "piece-update.h"
#include "iterator-bitfield.h"

using Bitmap = int;  // Define bitmap type

// ==== Pieces

Piece::Piece()
    : attackable_fields(0), movable_fields(0), color(true), position(0) {}

void Piece::set_position(int pos) {
    position = pos;
}

void Piece::die() {
    // Mock die function
}

bool Piece::operator==(const Piece& other) const {
    // Mock
    return position == other.position;
}

bool Piece::operator!=(const Piece& other) const {
    // Mock
    return position != other.position;
}

// ==== Behaviour



std::vector<Piece> PlayerBehaviourAI::get_all_pieces() {
    // Mock function to return all pieces
    return std::vector<Piece>{};
}

std::vector<Piece> PlayerBehaviourAI::filter(const std::vector<Piece>& pieces,
                                             bool color) {
    std::vector<Piece> result;
    for (const auto& piece : pieces) {
        if (piece.color == color) {
            result.push_back(piece);
        }
    }
    return result;
}

Bitmap PlayerBehaviourAI::create_attack_map(const std::vector<Piece>&  /*pieces*/,
                                            bool  /*color*/) {
    return 0;
}

bool PlayerBehaviourAI::is_under_attack(const Piece&  /*piece*/,
                                        Bitmap  /*attackMap*/) {
    return false;
}

void PlayerBehaviourAI::make_move(piece::army::Army& my_army) {

    struct Move{
        piece::Piece* piece;
        board::bitmap::Squares dest;
    };


    // === SELECTION 
    auto moves_all = piece::api::calc_possible_moves(my_army, this->_board, this->_army_list);

    std::vector<Move> moves;
    std::vector<Move> attack;
    piece::aggregator::PositionAggregator aggr{_army_list};
    auto positions = aggr.positions();

    for (auto [src, destinations]: moves_all) {
        IteratorBitmap dest{destinations};

        // just a temp. solution - iteration everytime is rather ugly
        piece::Piece* ptr = nullptr;
        for (auto i=0; i < my_army.size(); ++i) {
            if (my_army.pieces[i].position == src) {
                ptr = &my_army.pieces[i];
                break;
            }
        }

        if (ptr != nullptr) {

            while (*dest) {
                if (ptr->attackable & positions) {
                    attack.emplace_back(ptr, *dest);
                } else {
                    moves.emplace_back(ptr, *dest);
                }
                ++dest;
            }
        }

    }

    std::cout << "- Found " << moves.size() << " possible moves for Player " << my_army.color() << std::endl;

    // MOVEMENT
    if (attack.size() > 0) {
        //auto idx = static_cast<int>(rand() * attack.size()) % attack.size();
        auto [piece, dest] = attack[0];
        auto src = piece->position;

        piece::api::move_piece(src, dest, this->_board, this->_army_list);
        std::cout << "-> Attack with " << piece->type << " from square " << board::notation::ChessNotation{src, this->_board} << " to " << board::notation::ChessNotation{dest, this->_board} << std::endl;
    }

    else if (moves.size() > 0) {
        auto idx = static_cast<int>(rand() * moves.size()) % moves.size();
        auto [piece, dest] = moves[idx];
        auto src = piece->position;


        piece::api::move_piece(src, dest, this->_board, this->_army_list);
        std::cout << "-> Move " << piece->type << " from square " << board::notation::ChessNotation{src, this->_board} << " to " << board::notation::ChessNotation{dest, this->_board} << std::endl;
    }
    else {
        std::cout << "-> No moves left" << std::endl;
    }

    piece::update::display_all_pieces(_board, _army_list);
}

bool PlayerBehaviourAI::flip_a_coin() {
    return rand() % 2 == 0;
}

Piece PlayerBehaviourAI::get_piece_on_dest(Bitmap  /*dest*/) {
    return {};
}

bool PlayerBehaviourAI::is_defeated(piece::army::Army &my_army) const {
    auto valid_moves = piece::api::calc_possible_moves(my_army, this->_board, this->_army_list);
    bool are_there_valid_moves = valid_moves.size() > 0;
    bool is_king_alive = my_army.king().is_alive();
    bool have_multiple_pieces = my_army.size() > 1;
    return !(are_there_valid_moves & is_king_alive & have_multiple_pieces);
}

Bitmap PlayerBehaviourAI::get_movable_fields_as_bitmap(bool color) {
    Bitmap movable_fields     = 0;
    std::vector<Piece> const pieces = get_all_pieces();
    auto my_pieces            = filter(pieces, color);
    for (const auto& piece : my_pieces) {
        movable_fields |= piece.movable_fields;
    }
    return movable_fields;
}

void PlayerBehaviourAI::evaluate_and_update_all_movable_fields(
    Piece& piece, const Piece& king, const std::vector<Piece>& pieces) {
    Bitmap tmp_bitmap_dest_fields = piece.attackable_fields;
    while (tmp_bitmap_dest_fields != 0) {
        Bitmap const target       = tmp_bitmap_dest_fields & -tmp_bitmap_dest_fields;
        bool const is_field_empty = true;  // Mock check
        Piece piece_on_target;       // Mock piece from field
        Piece const save_status_src_piece  = piece;
        Piece const save_statuc_dest_piece = piece_on_target;

        piece.set_position(target);
        if (&piece_on_target != nullptr) {
            piece_on_target.die();
        }

        Bitmap const tmp_attack_map = create_attack_map(pieces, !piece.color);
        if (!is_under_attack(king, tmp_attack_map)) {
            piece.movable_fields |= target;
        }
        piece = save_status_src_piece;
        if (&piece_on_target != nullptr) {
            piece_on_target = save_statuc_dest_piece;
        }

        tmp_bitmap_dest_fields -= target;
    }
}

void PlayerBehaviourAI::update_movable_fields(bool color) {
    std::vector<Piece> const pieces = get_all_pieces();
    auto my_pieces            = filter(pieces, color);
    auto other_pieces         = filter(pieces, !color);
    Bitmap const my_positions    = create_attack_map(my_pieces, color);
    Bitmap const other_positions = create_attack_map(other_pieces, !color);

    Bitmap const attack_map = create_attack_map(pieces, !color);

    Piece king                  = get_king(my_pieces);
    bool const is_king_under_attack   = is_under_attack(king, attack_map);
    bool is_king_forced_to_move = false;

    if (is_king_under_attack) {
        auto attackers          = get_attacking_pieces(king, other_pieces);
        int const number_of_attackers = count_bits(attackers);

        if (number_of_attackers > 1) {
            is_king_forced_to_move = true;
        }
    }

    reset_movable_fields(my_pieces);

    if (is_king_forced_to_move) {
        evaluate_and_update_all_movable_fields(king, king, pieces);
    } else {
        for (auto& piece : my_pieces) {
            if (piece.attackable_fields == 0) {
                piece.movable_fields = 0;
            } else if (piece == king) {
                evaluate_and_update_all_movable_fields(king, king, pieces);
            } else {
                if (is_under_attack(piece, attack_map) && piece != king) {
                    Bitmap const tmp_attack_vector =
                        create_attack_map(pieces, !color); // remove piece and let re-run

                    if (is_under_attack(king, tmp_attack_vector)) {
                        piece.movable_fields = 0;
                        break;
                    }

                    if (is_king_under_attack) {
                        evaluate_and_update_all_movable_fields(piece, king,
                                                               pieces);
                    } else {
                        piece.movable_fields = piece.attackable_fields;
                    }
                }
            }
        }
    }
}

Piece PlayerBehaviourAI::get_king(const std::vector<Piece>&  /*pieces*/) {
    return {};
}

Bitmap PlayerBehaviourAI::get_attacking_pieces(
    const Piece&  /*king*/, const std::vector<Piece>&  /*other_pieces*/) {
    return 0;
}

void PlayerBehaviourAI::reset_movable_fields(std::vector<Piece>& pieces) {
    for (auto& piece : pieces) {
        piece.movable_fields = 0;
    }
}

int PlayerBehaviourAI::count_bits(Bitmap  /*bitmap*/) {
    return 0;
}