#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <functional>
#include <string>

using Grid = std::vector<std::vector<std::string>>;
using Position = std::pair<int,int>;

struct PositionHash {
    uint64_t operator()(const Position& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

using AssignmentMap = std::unordered_map<Position, std::string, PositionHash>;
using RuleFn = std::function<bool(const Position&, const std::string&, const AssignmentMap&)>;

inline RuleFn no_staying(const std::unordered_map<std::string, Position>& orig_pos) {
    return [&orig_pos](const Position& pos, const std::string& digit, const AssignmentMap&) {
        const auto it = orig_pos.find(digit);
        return !(it != orig_pos.end() && pos == it->second);
    };
}

inline RuleFn forbid_digit_in_row(const std::string& digit, int row) {
    return [digit, row](const Position& pos, const std::string& d, const AssignmentMap&) {
        return !(d == digit && pos.first == row);
    };
}

inline RuleFn forbid_side_by_side(const std::string& a, const std::string& b, int col) {
    return [a, b, col](const Position& pos, const std::string& digit, const AssignmentMap& current) {
        if (digit != a && digit != b) return true;
        const std::string other = digit == a ? b : a;
        // 左鄰
        if (pos.second > 0) {
            const Position left{pos.first, pos.second - 1};
            if (const auto it = current.find(left); it != current.end() && it->second == other)
                return false;
        }

        if (pos.second < col-1) {
            const Position right{pos.first, pos.second + 1};
            if (const auto it = current.find(right); it != current.end() && it->second == other)
                return false;
        }
        return true;
    };
}