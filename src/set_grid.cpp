#include "grid/grid.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/cartesian_product.hpp"
#include "range/v3/view/chunk_by.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include <random>

namespace grid {

using namespace ranges;

set_grid::set_grid(uint64_t w, uint64_t h, float prob)
    : abstract_grid(w, h) {
    std::random_device d;
    std::mt19937 rng(d());
    std::uniform_int_distribution<> w_dist(0, w);
    std::uniform_int_distribution<> h_dist(0, h);
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
    auto moore_neighborhood = [](const coord_t &coord) {
        auto [i, j] = coord;
        return views::cartesian_product(view::iota(-1, 2), view::iota(-1, 2)) |
               views::filter([](const std::tuple<int, int> &x) {
                   return !(0 == std::get<0>(x) && 0 == std::get<1>(x));
               }) |
               views::transform([i, j](const std::tuple<int, int> &x) {
                   auto [dx, dy] = x;
                   return coord_t{i + dx, j + dy};
               });
    };

    auto valuable_points =
        this->field |
        views::transform([&moore_neighborhood](const coord_t &cur) {
          return moore_neighborhood(cur);
        }) |
        views::join;

    std::vector<coord_t> in_bound_points =
        valuable_points | views::filter([*this](const coord_t &point) {
            return !(std::get<0>(point) < 0 || std::get<1>(point) < 0 ||
                     std::get<0>(point) >= this->width ||
                     std::get<1>(point) >= this->height);
        }) | ranges::to<std::vector>;

    std::sort(in_bound_points.begin(), in_bound_points.end());

    auto frequencies = in_bound_points |
                       views::chunk_by([](const coord_t &a, const coord_t &b) {
                           return std::get<0>(a) == std::get<0>(b) &&
                                  std::get<1>(a) == std::get<1>(b);
                       }) |
                       views::transform([](const auto &gr) {
                           return std::tuple{gr.front(), ranges::distance(gr)};
                       });

    std::unordered_set<coord_t, coord_hash> new_field =
        frequencies | views::filter([*this](const auto &coord_freq) {
            auto [coord, freq] = coord_freq;
            return (3 == freq) ||
                   ((2 == freq) &&
                    (CellState::alive ==
                     this->get_elem(std::get<0>(coord), std::get<1>(coord))));
        }) |
        views::transform(
            [](const auto &coord_freq) { return std::get<0>(coord_freq); }) |
        ranges::to<std::unordered_set<coord_t, coord_hash>>;

    std::swap(this->field, new_field);
}

} // namespace grid
