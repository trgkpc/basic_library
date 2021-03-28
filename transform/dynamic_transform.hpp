#pragma once
#include <Eigen/Dense>
#include <iostream>
#include <utility>

namespace Transform
{
struct DynamicTransform
{
    DynamicTransform(double dx, double dy, double rot)
        : pos(Eigen::Vector2d{dx,dy}), rot(rot)
    {
    }

    DynamicTransform(Eigen::Vector2d pos, double rot)
        : pos(pos),  rot(rot)
    {
    }

    DynamicTransform()
    {
    }

    std::pair<double, double> getPos() const
    {
        return std::make_pair<double, double>(double(pos.x()), double(pos.y()));
    }

    Eigen::Vector2d pos;
    double rot;
};

inline std::ostream& operator<<(std::ostream& os, DynamicTransform frame)
{
    os << "{{" << frame.pos.x() << ", " << frame.pos.y() << "}, {" << frame.rot << "}";
    return os;
}

}
