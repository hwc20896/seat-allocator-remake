#pragma once

#include <vector>
#include "algo.hpp"
#include <tuple>

class Backend final {
    public:
        explicit Backend() = default;

        const Grid& setData(const Grid& grid);

        [[nodiscard]]
        const Grid& getGrid(int index) const;

        [[nodiscard]]
        const std::tuple<uint64_t, Grid> shuffle(const std::vector<RuleFn>& rules);

        [[nodiscard]]
        RuleFn forbidSideBySideLocal(const std::string& a, const std::string& b) const;
    private:
        std::vector<Grid> datas_;
        GridShuffler shuffler_;
};

//  Class Declaration     ^^^^^^^^^^^^^
//  ======================================================
//  Class Implementation  vvvvvvvvvvvvv

inline const Grid& Backend::setData(const Grid& grid) {
    datas_.clear();
    datas_.push_back(grid);
    shuffler_.setGrid(grid);
    return grid;
}

inline const Grid& Backend::getGrid(const int index) const {
    return datas_.at(index);
}

inline const std::tuple<uint64_t, Grid> Backend::shuffle(const std::vector<RuleFn>& rules) {
    if (!shuffler_.shuffle(rules)) throw std::runtime_error("Unable to shuffle grid.");
    const auto current = datas_.size();
    const auto gd = shuffler_.getShuffledGrid();
    datas_.push_back(gd);
    return {current, gd};
}

inline RuleFn Backend::forbidSideBySideLocal(const std::string& a, const std::string& b) const {
    return shuffler_.forbidSideBySideLocal(a, b);
}

