#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

namespace Basic
{
inline double overshoot(double x0, double v0, double Kp, double Kd)
{
    double D = (Kd * Kd) - 4.0 * Kp;
    double m = std::min(0.0, x0);
    if (std::abs(D) < 1e-4) {
        // 臨界制動
        double k = 0.5 * Kd;
        double A = x0;
        double B = v0 + k * x0;
        if (B != 0.0 && k != 0) {
            double t = v0 / (B * k);
            if (t > 0) {
                double x = A * std::exp(-k * t) + B * t * std::exp(-k * t);
                m = std::min(x, m);
            }
        }
    } else if (D > 0) {
        // 過減衰
        double sqrtd = std::sqrt(D);
        double alpha = 0.5 * (Kd + sqrtd);
        double beta = 0.5 * (Kd - sqrtd);
        double A = (-beta * x0 - v0) / (alpha - beta);
        double B = (alpha * x0 + v0) / (alpha - beta);
        if ((A * B) < 0) {
            double t = std::log(-(B * beta) / (A * alpha)) / (beta - alpha);
            if (t > 0) {
                double x = A * std::exp(-alpha * t) + B * std::exp(-beta * t);
                m = std::min(x, m);
            }
        }
    } else {
        // 減衰振動
        double k = 0.5 * Kd;
        double omega = 0.5 * std::sqrt(-D);
        double A = x0;
        double B = (x0 * k + v0) / omega;
        double phi = std::atan((A * omega + B * k) / (B * omega - A * k));
        double t = (0.5 * M_PI - phi) / omega;
        {
            double x = (A * std::cos(omega * t) + B * std::sin(omega * t)) * std::exp(-k * t);
            m = std::min(x, m);
        }
        t += M_PI / omega;
        {
            double x = (A * std::cos(omega * t) + B * std::sin(omega * t)) * std::exp(-k * t);
            m = std::min(x, m);
        }
    }
    return m;
}
}  // namespace Basic
