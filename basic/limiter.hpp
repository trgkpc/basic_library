#pragma once
#include <Eigen/Dense>
#include <algorithm>
#include <cmath>

namespace Basic
{
inline double limiter(double value, double lim)
{
    return std::clamp(value, -lim, lim);
}

inline Eigen::Vector2d limiter(const Eigen::Vector2d& value, const double& lim)
{
    double ratio = std::sqrt(value.dot(value) / (lim*lim));
    if(ratio > 1.0){
        return value / ratio;
    }
    return value;
}
}  // namespace Basic::Limit
