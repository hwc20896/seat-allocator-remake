#pragma once

#include <vector>
#include "algo.hpp"

class Backend final {
    public:
        explicit Backend() = default;

        void setData(const Grid& grid);

        const Grid& getGrid(int index) const;
    private:
        std::vector<Grid> datas_;
};

//  Class Declaration     ^^^^^^^^^^^^^
//  ======================================================
//  Class Implementation  vvvvvvvvvvvvv

inline void Backend::setData(const Grid& grid) {
    datas_.clear();
    datas_.push_back(grid);
}

inline const Grid& Backend::getGrid(const int index) const {
    return datas_.at(index);
}

