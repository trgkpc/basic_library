#include <iostream>
#include <fstream>
#include <transform/transform.hpp>
#include <random>

using std::cout;
using std::endl;

#define print(frame)\
    cout << "===== " << #frame << " =====" << endl;\
    cout << frame << endl;\
    cout << endl;

template <long long N>
struct Polynomial
{
    explicit Polynomial(std::array<double, N> coeff)
    : coeff(coeff), dim(N-1)
    {
    }

    template <class... Args>
    explicit Polynomial(Args... coeff)
    : coeff(std::array<double, N>{coeff...}), dim(N-1)
    {
    }

    double getValue(double x) const
    {
        double ret = 0.0;
        double x_k = 1.0;
        for(int i = dim; i>=0; i--)
        {
            ret += coeff[i] * x_k;
            x_k *= x;
        }
        return ret;
    }

    double getDiff(const int& k, const double& x) const
    {
        double ret = 0.0;
        double x_k = 1.0;
        for(int i = dim; (i-k)>=0; i--)
        {
            int C = 1;
            for(int j=1;j<=k;j++){
                // x**(dim-i) <- (dim-i)+1〜(dim-i)+k
                C *= (dim-i)+j;
            }
            ret += C * coeff[i-k] * x_k;
            x_k *= x;
        }
        return ret;
    }

    template <long long k> 
    double getDiff(const double& x) const
    {
        return getDiff(k, x);
    }

private:
    int dim;
public:
    std::array<double, N> coeff;
};

#define print_func(func) \
    std::cout << func(-1.) << "," << func(0.) << "," << func(1.) << "," << func(2.) << std::endl;

template <long long A, long long B, long long C>
Transform::MultidiffTransform<3> calc(Polynomial<A> x, Polynomial<B> y, Polynomial<C> th, double t)
{
    Transform::StaticTransform s(x.getValue(t), y.getValue(t), th.getValue(t));
    Transform::DynamicTransform d1(x.getDiff(1, t), y.getDiff(1, t), th.getDiff(1, t));
    Transform::DynamicTransform d2(x.getDiff(2, t), y.getDiff(2, t), th.getDiff(2, t));
    Transform::DynamicTransform d3(x.getDiff(3, t), y.getDiff(3, t), th.getDiff(3, t));

    return Transform::MultidiffTransform<3>(s, d1, d2, d3);
}


int main()
{
    /*
    // Step1. Polynomialのデバッグ
    Polynomial<3> f(1., -2, 1.);
    {
        std::function<double(double)> df = [&](double t){ return f.getValue(t); };
        print_func(df);
    }
    {
        std::function<double(double)> df = [&](double t){ return f.getDiff<0>(t); };
        print_func(df);
    }
    {
        std::function<double(double)> df = [&](double t){ return f.getDiff<1>(t); };
        print_func(df);
    }
    {
        std::function<double(double)> df = [&](double t){ return f.getDiff<2>(t); };
        print_func(df);
    }
    {
        std::function<double(double)> df = [&](double t){ return f.getDiff<3>(t); };
        print_func(df);
    }
    */

    /*
    // Step2. MultidiffTransformのoperator+のデバッグ
    std::ofstream fs1("theoretical.log");
    std::ofstream fs2("numerical.log");
    std::function<void(std::ofstream& fs, const double& t, const Transform::MultidiffTransform<3>& frame)> writer
        = [](std::ofstream& fs, const double& t, const Transform::MultidiffTransform<3>& frame){
            fs << t << " ";
            for(int i=0; i<3; i++){
                fs << frame.dynamic_frame[i].pos.x() << " ";
                fs << frame.dynamic_frame[i].pos.y() << " ";
                fs << frame.dynamic_frame[i].rot << " ";
            }
            fs << endl;
        };
    
    
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> dist1(-1.0, 1.0);

    Polynomial<5> x(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    Polynomial<5> y(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    Polynomial<5> th(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    std::function<Transform::MultidiffTransform<3>(double)> base = [&](double t){ return calc(x, y, th, t); };

    Polynomial<5> X(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    Polynomial<5> Y(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    Polynomial<5> Th(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    std::function<Transform::MultidiffTransform<3>(double)> add = [&](double t){ return calc(X, Y, Th, t); };
    
    std::function<void(double)> callback = [&](double t){
        constexpr double dt = 0.0001;
        auto frame = base(t) + add(t);
        auto dframe = base(t+dt) + add(t+dt);
        writer(fs1, t, frame);

        Transform::MultidiffTransform<3> dfdt;
        dfdt.static_frame = Transform::StaticTransform(0., 0.,0.);
        dfdt.dynamic_frame[0].pos = (dframe.static_frame.pos - frame.static_frame.pos) / dt;
        dfdt.dynamic_frame[0].rot = (dframe.static_frame.rot.angle() -  frame.static_frame.rot.angle()) / dt;

        dfdt.dynamic_frame[1].pos = (dframe.dynamic_frame[0].pos - frame.dynamic_frame[0].pos) / dt;
        dfdt.dynamic_frame[1].rot = (dframe.dynamic_frame[0].rot -  frame.dynamic_frame[0].rot) / dt;

        dfdt.dynamic_frame[2].pos = (dframe.dynamic_frame[1].pos - frame.dynamic_frame[1].pos) / dt;
        dfdt.dynamic_frame[2].rot = (dframe.dynamic_frame[1].rot -  frame.dynamic_frame[1].rot) / dt;


        writer(fs2, t, dfdt);
    };

    for(double t=0.0; t<1.0; t+=0.01){
        callback(t);
    }
    */

    // Step3. MultidiffTransformのoperator-のデバッグ
    std::ofstream fs1("theoretical.log");
    std::ofstream fs2("numerical.log");
    std::function<void(std::ofstream& fs, const double& t, const Transform::MultidiffTransform<3>& frame)> writer
        = [](std::ofstream& fs, const double& t, const Transform::MultidiffTransform<3>& frame){
            fs << t << " ";
            for(int i=0; i<3; i++){
                fs << frame.dynamic_frame[i].pos.x() << " ";
                fs << frame.dynamic_frame[i].pos.y() << " ";
                fs << frame.dynamic_frame[i].rot << " ";
            }
            fs << endl;
        };
    
    
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> dist1(-1.0, 1.0);

    Polynomial<5> x(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    Polynomial<5> y(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    Polynomial<5> th(dist1(engine), dist1(engine), dist1(engine), dist1(engine), dist1(engine));
    std::function<Transform::MultidiffTransform<3>(double)> get = [&](double t){ return calc(x, y, th, t); };
    
    std::function<void(double)> callback = [&](double t){
        constexpr double dt = 0.0001;
        auto frame = -get(t);
        auto dframe = -get(t+dt);
        writer(fs1, t, frame);

        Transform::MultidiffTransform<3> dfdt;
        dfdt.static_frame = Transform::StaticTransform(0., 0.,0.);
        dfdt.dynamic_frame[0].pos = (dframe.static_frame.pos - frame.static_frame.pos) / dt;
        dfdt.dynamic_frame[0].rot = (dframe.static_frame.rot.angle() -  frame.static_frame.rot.angle()) / dt;

        dfdt.dynamic_frame[1].pos = (dframe.dynamic_frame[0].pos - frame.dynamic_frame[0].pos) / dt;
        dfdt.dynamic_frame[1].rot = (dframe.dynamic_frame[0].rot -  frame.dynamic_frame[0].rot) / dt;

        dfdt.dynamic_frame[2].pos = (dframe.dynamic_frame[1].pos - frame.dynamic_frame[1].pos) / dt;
        dfdt.dynamic_frame[2].rot = (dframe.dynamic_frame[1].rot -  frame.dynamic_frame[1].rot) / dt;


        writer(fs2, t, dfdt);
    };

    for(double t=0.0; t<1.0; t+=0.01){
        callback(t);
    }

}

