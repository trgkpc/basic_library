#pragma once
#include <iostream>
#include <Eigen/Dense>

namespace Transform
{
struct StaticTransform
{
    StaticTransform(double x, double y, double theta)
        : pos(Eigen::Vector2d{x,y}), rot(Eigen::Rotation2Dd{theta})
    {
    }

    StaticTransform(Eigen::Vector2d pos, double theta)
        : pos(pos),  rot(Eigen::Rotation2Dd{theta})
    {
    }

    StaticTransform(Eigen::Vector2d pos, Eigen::Rotation2Dd rot)
        : pos(pos),  rot(rot)
    {
    }
    
    StaticTransform()
    {
    }

    StaticTransform operator -() const
    {
        Eigen::Rotation2Dd Rot = rot.inverse();
	    Eigen::Vector2d Pos = Rot * (-pos);
	    return StaticTransform{Pos, Rot};
    }

    StaticTransform operator +(const StaticTransform& frame) const
    {
        return StaticTransform(pos + rot * frame.pos, rot * frame.rot);
    }

    Eigen::Vector2d pos;
    Eigen::Rotation2Dd rot;
};

inline std::ostream& operator<<(std::ostream& os, StaticTransform frame)
{
    os << "{{" << frame.pos.x() << ", " << frame.pos.y() << "}, {" << frame.rot.angle() << "}";
    return os;
}

}
