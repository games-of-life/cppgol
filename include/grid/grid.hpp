#ifndef GRID_H_
#define GRID_H_

#include <cstdint>
#include <ranges>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace grid {

enum class CellState { dead = 0, alive = 1 };

class abstract_grid {
  protected:
    const uint64_t width;
    const uint64_t height;

  public:
    abstract_grid(uint64_t w, uint64_t h);
    virtual CellState get_elem(int64_t i, int64_t j) const = 0;
    virtual void set_elem(int64_t i, int64_t j, CellState val) = 0;
    virtual void run_gol_step() = 0;
    virtual ~abstract_grid(){};
};

class vector_grid : public abstract_grid {
  private:
    std::vector<CellState> field;
    uint64_t calc_neighbors(int64_t i, int64_t j) const;

  public:
    vector_grid(uint64_t w, uint64_t h, float prob);
    CellState get_elem(int64_t i, int64_t j) const override;
    void set_elem(int64_t i, int64_t j, CellState val) override;
    void run_gol_step() override;
};

class set_grid : public abstract_grid {
  private:
    using coord_t = std::tuple<int64_t, int64_t>;

    struct coord_hash {
      public:
        uint64_t operator()(const coord_t &t) const {
            return std::hash<int64_t>()(std::get<0>(t)) ^
                   std::hash<int64_t>()(std::get<1>(t));
        }
    };

    std::unordered_set<coord_t, coord_hash> field;

    auto moore_neighborhood(const coord_t &base) const;

  public:
    set_grid(uint64_t w, uint64_t h, float prob);
    CellState get_elem(int64_t i, int64_t j) const override;
    void set_elem(int64_t i, int64_t j, CellState val) override;
    void run_gol_step() override;
};

} // namespace grid

#endif
