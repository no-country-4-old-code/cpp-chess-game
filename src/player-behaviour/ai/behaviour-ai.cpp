#include <behaviour-ai.h>
#include <cstdlib>
#include <ctime>
#include <vector>

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

PlayerBehaviourAI::PlayerBehaviourAI(bool color) : myColor(color) {}

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

void PlayerBehaviourAI::make_move() const {
    std::vector<Piece> const pieces = get_all_pieces();
    auto my_pieces            = filter(pieces, myColor);
    update_movable_fields(myColor);

    Piece selected_piece;
    Bitmap dest = 0;
    for (const auto& piece : pieces) {
        if (piece.movable_fields > 0) {
            auto tmp_mv_fieldst = piece.movable_fields;
            while (tmp_mv_fieldst != 0) {
                dest = tmp_mv_fieldst & -tmp_mv_fieldst;
                if (flip_a_coin()) {
                    break;
                }
                tmp_mv_fieldst -= dest;
            }
            selected_piece = piece;
        }
    }

    Piece piece_on_dest = get_piece_on_dest(dest);
    selected_piece.set_position(dest);
    if (&piece_on_dest != nullptr) {
        piece_on_dest.die();
    }
}

bool PlayerBehaviourAI::flip_a_coin() {
    return rand() % 2 == 0;
}

Piece PlayerBehaviourAI::get_piece_on_dest(Bitmap  /*dest*/) {
    return {};
}

bool PlayerBehaviourAI::has_valid_moves() const {
    update_movable_fields(myColor);
    Bitmap const bitmap = get_movable_fields_as_bitmap(myColor);
    return bitmap > 0;
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
                        create_attack_map(pieces, !color);

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