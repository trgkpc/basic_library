#pragma once
#include <Eigen/Dense>
#include <algorithm>

namespace Transform
{
// Rot * Posの微分を計算する関数
inline Eigen::Vector2d rot_vec_diff1(const Eigen::Vector2d& r, const Eigen::Rotation2Dd Rot,
    const Eigen::Vector2d& v, const double omega)
{
    Eigen::Matrix2d R;
    R << 0,-1,
         1, 0;

    return Rot * (omega * R * r + v);
}

inline Eigen::Vector2d rot_vec_diff2(const Eigen::Vector2d& r, const Eigen::Rotation2Dd Rot,
    const Eigen::Vector2d& v, const double omega,
    const Eigen::Vector2d& a, const double alpha)
{
    Eigen::Matrix2d R;
    R << 0,-1,
         1, 0;
    Eigen::Matrix2d R2;
    R2 << -1, 0,
        0, -1;
    return Rot *
       (std::pow(omega, 2) * R2 * r
           + alpha * R * r
           + 2 * omega * R * v
           + a);
}

inline Eigen::Vector2d rot_vec_diff3(const Eigen::Vector2d& r, const Eigen::Rotation2Dd Rot,
    const Eigen::Vector2d& v, const double omega,
    const Eigen::Vector2d& a, const double alpha,
    const Eigen::Vector2d& da, const double dalpha)
{
    Eigen::Matrix2d R;
    R << 0,-1,
         1, 0;
    Eigen::Matrix2d R2;
    R2 << -1, 0,
        0, -1;
    Eigen::Matrix2d R3;
    R3 << 0, 1,
       -1, 0;

    return Rot * 
        (std::pow(omega, 3) * (R3 * r) + 3 * alpha * omega * (R2 * r) + dalpha * (R * r)
            + 3 * std::pow(omega, 2) * (R2 * v) + 3 * alpha * (R * v)
            + 3 * omega * (R * a)
            + da);
}

}
