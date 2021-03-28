#pragma once
#include <iostream>
#include <cmath>
#include <string>

namespace Device
{
struct Motor
{
    Motor(const std::string& name, const bool& reverse, const double& limit) 
     : name(name), reverse(reverse), limit(limit), is_free(false), value(0.0)
    {
    }
    
    void setPower(const double& target_)
    {
        double target = reverse ? -target_ : target_;
        if(std::abs(target) > limit){
            std::cerr << "[[WARNING]] " << name << " limit exceeded(" << target_ << ")" << std::endl;
            if(target > 0.0){
                target = limit;
            }else{
                target = -limit;
            }
        }

        is_free = false;
        value = target;
    }

    void free()
    {
        is_free = true;
        value = 0.0;
    }

    double getValue()
    {
        return value;
    }

    bool isFree()
    {
        return is_free;
    }

public:
    std::string name;
private:
    bool reverse;
    double limit;

    bool is_free;
    double value;
};

}  // namespace Device
