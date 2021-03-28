#pragma once
#include <utility>

namespace Basic::Feedback
{
inline double Kp(double time_const)
{
    return 1.0 / time_const;
}

inline std::pair<double, double> KpKd(double p_time_const, double v_time_const)
{
    return std::make_pair(double(1.0 / p_time_const / p_time_const), double(2.0 / v_time_const));
}

template <class T>
inline T pRegulator(T error, double time_constant)
{
    return -Kp(time_constant) * error;
}

template <class T> 
inline T pdRegulator(T error, T d_error, double p_time_const, double v_time_const)
{
    auto [Kp, Kd] = KpKd(p_time_const, v_time_const);
    return -Kp * error - Kd * d_error;
}

}  // namespace Basic::Feedback
