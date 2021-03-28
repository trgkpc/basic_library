#pragma once

#include <algorithm>
#include <cmath>
#include <functional>

namespace Basic
{
// 黄金探索により凸関数の極小値を求める

inline double golden_search(std::function<double(double)> f, const double& low, const double& high, int loop_num = 20)
{
    constexpr double phi = (1.0 + std::sqrt(5)) / 2.0;

    double left = low;
    double right = high;
    
    double p0 = (phi*left + right) / (1.0+phi);
    double f0 = f(p0);

    double p1 = (left + phi*right) / (1.0+phi);
    double f1 = f(p1);

    for(int i=0; i<loop_num; i++){
#ifdef DEBUG_GOLDEN_SERACH
        // デバッグモードのときに出力
        auto print = [&](double t){
            std::cout << "f(" << t << ")=" << f(t) << ", ";
        };
        print(left);
        print(p0);
        print(p1);
        print(right);
        std::cout << std::endl;
#endif
        if(f0 < f1){
            /* グラフがこんな感じ 
                     r
              l     1 <-ここ(1)をrightにする
               \_0_/
            */
            right = p1;

            p1 = p0;
            f1 = f(p1);

            p0 = (phi*left + right) / (1.0+phi);
            f0 = f(p0);
        }else{
            /* グラフがこんな感じ
              l
               0     r
                \_1_/
            */
            left = p0;

            p0 = p1;
            f0 = f(p0);

            p1 = (left + phi*right) / (1.0+phi);
            f1 = f(p1);
        }
    }

    return 0.5*(left+right);
}

}  // namespace Basic
