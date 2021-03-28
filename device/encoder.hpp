#pragma once
#include <iostream>
#include <cmath>
#include <string>

namespace Device
{
struct Encoder
{
    Encoder(const std::string& name, const bool& reverse, const double& factor)
     : name(name), reverse(reverse), factor(factor), pos(0.0), vel(0.0)
    {
    }

    void callback(const double& raw, const double& dt)
    {
        // 毎周期の初めに(エンコーダーのステップ値, 制御周期)で呼ぶ 
        double new_pos = reverse ? -factor*raw : factor*raw;
        estimateVel(new_pos-pos, dt);
        pos = new_pos;
    }

    virtual void estimateVel(const double& diff, const double& dt)
    {
        // オーバーライドによりローパスを挟むもあはれなり[趣深い]。
        vel = diff / dt;
    }
    
    double getPos() const
    {
        return pos;
    }

    double getVel() const
    {
        return vel;
    }

public:
    std::string name;

private:
    bool reverse;
    double factor;

    double pos;
    double vel;
};

inline std::ostream& operator<<(std::ostream& os, const Encoder& enc)
{
    os << "[Encoder] name=" << enc.name << ", pos=" << enc.getPos() << ", vel=" << enc.getVel();
    return os;
}

}  // namespace Device
