#include "grid/grid.hpp"
#include <algorithm>
#include <array>
#include <random>
#include <unordered_map>
#include <unordered_set>

namespace grid {

set_grid::set_grid(int w, int h, float prob) {
    std::random_device d;
    std::mt19937 rng(d());
    std::uniform_int_distribution<> w_dist(0, w);
    std::uniform_int_distribution<> h_dist(0, h);
    this->width = w;
    this->height = h;
    for (uint64_t i = 0; i < w * h * prob; ++i) {
        this->field.insert({w_dist(rng), h_dist(rng)});
    }
}

CellState set_grid::get_elem(int64_t i, int64_t j) const {
    return this->field.contains({i, j}) ? CellState::alive : CellState::dead;
}

void set_grid::set_elem(int64_t i, int64_t j, CellState val) {
    switch (val) {
    case CellState::alive:
        this->field.insert({i, j});
        break;
    case CellState::dead:
        this->field.erase({i, j});
    }
}

void set_grid::run_gol_step() {
    std::array<coord_t, 8> moore_neighbors{
        coord_t{-1, -1}, coord_t{-1, 0}, coord_t{-1, 1}, coord_t{0, -1},
        coord_t{0, 1},   coord_t{1, -1}, coord_t{1, 0},  coord_t{1, 1}};

    std::vector<coord_t> valuable_points;
    std::for_each(
        this->field.begin(), this->field.end(),
        [*this, &valuable_points, &moore_neighbors](const coord_t &cur) {
            for (const coord_t &neigh :
                 moore_neighbors |
                     std::views::transform(
                         [&cur](const coord_t &offset) -> const coord_t {
                             return {std::get<0>(offset) + std::get<0>(cur),
                                     std::get<1>(offset) + std::get<1>(cur)};
                         })) {
                valuable_points.push_back(neigh);
            }
        });

    std::unordered_map<coord_t, uint64_t, coord_hash> frequencies;
    for (const coord_t &cur :
         valuable_points | std::views::filter([*this](const coord_t &point) {
             return !(std::get<0>(point) < 0 || std::get<1>(point) < 0 ||
                      std::get<0>(point) >= this->width ||
                      std::get<1>(point) >= this->height);
         })) {
        if (frequencies.contains(cur)) {
            frequencies[cur] += 1;
        } else {
            frequencies[cur] = 1;
        }
    }

    std::unordered_set<coord_t, coord_hash> new_field;
    for (const auto &[coord, _] :
         frequencies |
             std::views::filter(
                 [*this](const std::pair<coord_t, uint64_t> &cur) {
                     return (3 == cur.second) ||
                            (2 == cur.second &&
                             CellState::alive ==
                                 this->get_elem(std::get<0>(cur.first),
                                                std::get<1>(cur.first)));
                 })) {
        new_field.insert(coord);
    }

    std::swap(this->field, new_field);
}

} // namespace grid