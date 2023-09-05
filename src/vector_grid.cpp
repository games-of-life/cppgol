#include "grid/grid.hpp"
#include <algorithm>
#include <iterator>
#include <random>

namespace grid {

vector_grid::vector_grid(uint64_t w, uint64_t h, float prob)
    : abstract_grid(w, h) {
    this->field.reserve(w * h);
    std::random_device d;
    std::mt19937 rng(d());
    std::uniform_real_distribution<> dist(0., 1.);
    std::generate_n(
        std::back_inserter(this->field), w * h, [&prob, &dist, &rng]() {
            return (dist(rng) > prob) ? CellState::dead : CellState::alive;
        });
}

CellState vector_grid::get_elem(int64_t i, int64_t j) const {
    return this->field[i * this->height + j];
}

void vector_grid::set_elem(int64_t i, int64_t j, CellState val) {
    this->field[i * this->height + j] = val;
}

uint64_t vector_grid::calc_neighbors(int64_t i, int64_t j) const {
    uint count = 0;
    for (long i_cor = i - 1; i_cor <= i + 1; i_cor++) {
        for (long j_cor = j - 1; j_cor <= j + 1; j_cor++) {
            if (!((i_cor == i && j_cor == j) || (i_cor < 0 || j_cor < 0) ||
                  (i_cor >= this->width || j_cor >= this->height)) &&
                (CellState::alive == this->get_elem(i_cor, j_cor))) {
                count++;
            }
        }
    }
    return count;
}

void vector_grid::run_gol_step() {
    std::vector<CellState> field_copy(this->width * this->height);

    for (uint i = 0; i < this->width; ++i) {
        for (uint j = 0; j < this->height; ++j) {
            uint64_t neigh = this->calc_neighbors(i, j);
            if ((3 == neigh) ||
                (2 == neigh && CellState::alive == this->get_elem(i, j))) {
                field_copy[i * this->height + j] = CellState::alive;
            } else {
                field_copy[i * this->height + j] = CellState::dead;
            }
        }
    }

    std::swap(this->field, field_copy);
}

} // namespace grid
