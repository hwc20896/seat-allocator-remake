#pragma once

#include "rule_factory.hpp"
#include <ranges>
#include <algorithm>

class GridShuffler {
    Grid original_grid_, shuffled_grid_;
    std::vector<Position> non_empty_positions_;
    std::unordered_map<std::string, Position> original_positions_;
    std::unordered_map<Position, std::vector<Position>, PositionHash> neighbors_map_;
    std::unordered_map<std::string, std::unordered_set<std::string>> forbidden_neighbors_;

    bool isInitialized_ = false;

    uint64_t rows = 0, cols = 0;

    public:
        explicit GridShuffler() = default;

        void setGrid(const Grid& g);

        bool shuffle(const std::vector<RuleFn>& rules);

        [[nodiscard]]
        const Grid& getShuffledGrid() const;

        //  Function Factory
        [[nodiscard]]
        RuleFn forbidSideBySideLocal(const std::string& a, const std::string& b) const;
    private:
        void initFromGrid();

        [[nodiscard]]
        bool isValid(const Position& pos, const std::string& digit, const AssignmentMap& current, const std::unordered_set<std::string>& used, const std::vector<RuleFn>& extra_rules) const;

        bool backtrack(AssignmentMap& current, std::unordered_set<std::string>& used, const std::vector<RuleFn>& extra_rules);
};

inline void GridShuffler::setGrid(const Grid& g) {
    this->original_grid_ = g;
    isInitialized_ = true;
    initFromGrid();
}

inline void GridShuffler::initFromGrid() {
    this->rows = original_grid_.size();
    if (rows == 0) return;
    cols = original_grid_[0].size();

    shuffled_grid_ = original_grid_;

    // 清空舊資料
    non_empty_positions_.clear();
    original_positions_.clear();
    neighbors_map_.clear();
    forbidden_neighbors_.clear();

    // 收集非空位置
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!original_grid_[r][c].empty()) {
                Position p{r, c};
                non_empty_positions_.push_back(p);
                original_positions_[original_grid_[r][c]] = p;
            }
        }
    }

    // 建立鄰居與 forbidden_neighbors
    std::vector<Position> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    for (auto [r, c] : non_empty_positions_) {
        std::string cur = original_grid_[r][c];
        std::vector<Position> neighbors;
        for (auto [dr, dc] : dirs) {
            if (int nc = c + dc, nr = r + dr;
                nr >= 0 && nr < rows &&
                nc >= 0 && nc < cols &&
                !original_grid_[nr][nc].empty()
            ) {
                neighbors.emplace_back(nr, nc);
                forbidden_neighbors_[cur].insert(original_grid_[nr][nc]);
            }
        }
        neighbors_map_[{r, c}] = neighbors;
    }
}

inline bool GridShuffler::isValid(
    const Position& pos,
    const std::string& digit,
    const AssignmentMap& current,
    const std::unordered_set<std::string>& used,
    const std::vector<RuleFn>& extra_rules
) const {
    if (used.contains(digit)) return false;

    // 原始 forbidden_neighbors 檢查
    for (const auto& nb : neighbors_map_.at(pos)) {
        if (auto it = current.find(nb); it != current.end()) {
            if (forbidden_neighbors_.at(digit).contains(it->second))
                return false;
        }
    }

    return std::ranges::all_of(extra_rules, [pos,digit,current](const RuleFn& rule){return rule(pos, digit, current);});
}

inline bool GridShuffler::backtrack(AssignmentMap& current,
                                    std::unordered_set<std::string>& used,
                                    const std::vector<RuleFn>& extra_rules) {
    if (current.size() == non_empty_positions_.size()) return true;

    // MRV: choose the position with the fewest valid options
    auto remaining_positions = non_empty_positions_ | std::views::filter([&](const auto& pos){return !current.contains(pos);});

    const auto best_it = std::ranges::min_element(
        remaining_positions,
        {},
        [&](const auto& pos) {
            return std::ranges::count_if(
                forbidden_neighbors_ | std::views::keys,
                [&](const auto& digit) {
                    return !used.contains(digit)
                        && isValid(pos, digit, current, used, extra_rules);
                });
        });

    if (best_it == std::ranges::end(remaining_positions))
        return false;

    const Position& pos = *best_it;

    // Count options once
    const size_t min_options = std::ranges::count_if(
        forbidden_neighbors_ | std::views::keys,
        [&](const auto& digit) {
            return !used.contains(digit)
                && isValid(pos, digit, current, used, extra_rules);
        });

    if (min_options == 0)
        return false;

    // Try each possible digit
    for (const auto& digit : forbidden_neighbors_ | std::views::keys) {
        if (used.contains(digit) || !isValid(pos, digit, current, used, extra_rules)) continue;

        current[pos] = digit;
        used.insert(digit);

        if (backtrack(current, used, extra_rules)) return true;

        current.erase(pos);
        used.erase(digit);
    }

    return false;
}

inline bool GridShuffler::shuffle(const std::vector<RuleFn>& rules) {
    if (!isInitialized_) throw std::runtime_error("The internal grid has to be initialized before it can be shuffled.");
    AssignmentMap current;
    std::unordered_set<std::string> used;
    if (backtrack(current, used, rules)) {
        // 填入 shuffled_grid_
        for (int r = 0; r < this->rows; ++r)
            for (int c = 0; c < this->cols; ++c)
                shuffled_grid_[r][c] = "";
        for (const auto& [pos, digit] : current) {
            shuffled_grid_[pos.first][pos.second] = digit;
        }
        return true;
    }
    return false;
}

inline const Grid& GridShuffler::getShuffledGrid() const {
    if (!isInitialized_) throw std::runtime_error("The internal grid has to be initialized before the value can be retrieved.");
    return shuffled_grid_;
}

inline RuleFn GridShuffler::forbidSideBySideLocal(const std::string& a, const std::string& b) const {
    if (!isInitialized_) throw std::runtime_error("The internal grid hasn't yet initialized. Consider initialize it first.");
    return forbid_side_by_side(a, b, cols);
}


