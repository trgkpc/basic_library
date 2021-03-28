#pragma once
#include <array>
#include <cmath>
#include "static_transform.hpp"
#include "dynamic_transform.hpp"
#include "diff_transform.hpp"

namespace Transform
{

// 時間微分に対応した関数
template <unsigned N>
struct MultidiffTransform
{
    MultidiffTransform(StaticTransform static_frame, std::array<DynamicTransform, N> dynamic_frame)
        : static_frame(static_frame), dynamic_frame(dynamic_frame)
    {
        static_assert(N < 4, "MultidiffTransform dimension must less than 4;");
    }

    template <class... Args>
    MultidiffTransform(StaticTransform static_frame, Args... dynamic_frames)
        : static_frame(static_frame), dynamic_frame(std::array<DynamicTransform, N>{dynamic_frames...})
    {
        static_assert(N < 4, "MultidiffTransform dimension must less than 4;");
    }

    template <unsigned K>
    MultidiffTransform(MultidiffTransform<K> frame)
        : static_frame(frame.static_frame)
    {
        for(int i=0; i<N; i++){
            if(i<K){
                dynamic_frame[i] = frame.dynamic_frame[i];
            }else{
                dynamic_frame[i] = Transform::DynamicTransform(0., 0., 0.);
            }
        }
    }

    MultidiffTransform(StaticTransform static_frame)
        : static_frame(static_frame)
    {
        for(int i=0; i<N; i++){
            dynamic_frame[i] = Transform::DynamicTransform(0., 0., 0.);
        }
    }


    MultidiffTransform()
    {
        static_assert(N < 4, "MultidiffTransform dimension must less than 4;");
    }

    MultidiffTransform operator -() const
    {
        // static_frameのoperator -()の時間微分
        //    pos: -Rot * pos
        //    rot: -Rot

        // 返り値の初期化
        auto ret = MultidiffTransform<N>();

        // 0階微分
        ret.static_frame = -static_frame;
        if(N == 0){
            return ret;
        }
        
        // 1階微分
        ret.dynamic_frame[0].pos = rot_vec_diff1(
            ret.static_frame.pos, ret.static_frame.rot, 
            -dynamic_frame[0].pos, -dynamic_frame[0].rot);
        ret.dynamic_frame[0].rot = -dynamic_frame[0].rot;
        if(N == 1){
            return ret;
        }

        // 2階微分
        ret.dynamic_frame[1].pos = rot_vec_diff2(
            ret.static_frame.pos, ret.static_frame.rot, 
            -dynamic_frame[0].pos, -dynamic_frame[0].rot,
            -dynamic_frame[1].pos, -dynamic_frame[1].rot);
        ret.dynamic_frame[1].rot = -dynamic_frame[1].rot;
        if(N == 2){
            return ret;
        }

        // 3階微分
        ret.dynamic_frame[2].pos = rot_vec_diff3(
            ret.static_frame.pos, ret.static_frame.rot, 
            -dynamic_frame[0].pos, -dynamic_frame[0].rot,
            -dynamic_frame[1].pos, -dynamic_frame[1].rot,
            -dynamic_frame[2].pos, -dynamic_frame[2].rot);
        ret.dynamic_frame[2].rot = -dynamic_frame[2].rot;
        return ret;
    }

    MultidiffTransform operator +(const MultidiffTransform<N>& frame) const
    {
        // static_frameのoperator+(const StaticTransform&)の時間微分
        //    pos: pos + rot * frame.pos
        //    rot: rot * frame.rot

        // 返り値の初期化
        auto ret = MultidiffTransform<N>();

        // 0階微分
        ret.static_frame = static_frame + frame.static_frame;
        if(N == 0){
            return ret;
        }

        // 1階微分
        ret.dynamic_frame[0].pos = dynamic_frame[0].pos + rot_vec_diff1(
            frame.static_frame.pos, static_frame.rot,
            frame.dynamic_frame[0].pos, dynamic_frame[0].rot);
        ret.dynamic_frame[0].rot = dynamic_frame[0].rot + frame.dynamic_frame[0].rot;
        if(N == 1){
            return ret;
        }

        // 2階微分
        ret.dynamic_frame[1].pos = dynamic_frame[1].pos + rot_vec_diff2(
            frame.static_frame.pos, static_frame.rot,
            frame.dynamic_frame[0].pos, dynamic_frame[0].rot,
            frame.dynamic_frame[1].pos, dynamic_frame[1].rot);
        ret.dynamic_frame[1].rot = dynamic_frame[1].rot + frame.dynamic_frame[1].rot;
        if(N == 2){
            return ret;
        }

        // 3階微分
        ret.dynamic_frame[2].pos = dynamic_frame[2].pos + rot_vec_diff3(
            frame.static_frame.pos, static_frame.rot,
            frame.dynamic_frame[0].pos, dynamic_frame[0].rot,
            frame.dynamic_frame[1].pos, dynamic_frame[1].rot,
            frame.dynamic_frame[2].pos, dynamic_frame[2].rot);
        ret.dynamic_frame[2].rot = dynamic_frame[2].rot + frame.dynamic_frame[2].rot;
        return ret;
    }

    MultidiffTransform operator +(const StaticTransform& frame_) const
    {
        MultidiffTransform<N> frame;
        frame.static_frame = frame_;
        for(int i=0; i<N; i++){
            frame.dynamic_frame[i] = DynamicTransform(0., 0., 0.);
        }
        return (*this) + frame;
    }
    
    MultidiffTransform<N+1> append(DynamicTransform frame) const
    {
        MultidiffTransform<N+1> ret;
        ret.static_frame = static_frame;
        for(int i=0; i<N; i++){
            ret.dynamic_frame[i] = dynamic_frame[i];
        }
	    ret.dynamic_frame[N] = frame;
        return ret;
    }
        
    StaticTransform staticTransform() const
    {
        return static_frame;
    }
    
    DynamicTransform dynamicTransform(int n) const
    {
	    if(n<=0 || n>N){
	        std::cerr << "[[FATAL]] MultidiffFrame<" << N << ">.dynamicFrame(" << n << ") called." << std::endl;
	    }
        return dynamic_frame[n-1];
    }

    DynamicTransform dynamic(int n) const
    {
        // DynamicTransform dynamicTransform(int n) へのエイリアス
        return dynamicTransform(n);
    }

    template <unsigned K>
    MultidiffTransform<K> staticMultidiffTransform()
    {
        MultidiffTransform<K> ret;
        ret.static_frame = static_frame;
        for(int i=0; i<K; i++){
            ret.dynamic_frame[i] = DynamicTransform(0., 0., 0.);
        }
        return ret;
    }

    template <unsigned K>
    MultidiffTransform<K> staticMulti()
    {
        // MultidiffTransform<K> staticMultidiffTransform() へのエイリアス
        return staticMultidiffTransform<K>();
    }

    StaticTransform static_frame;
    std::array<DynamicTransform, N> dynamic_frame;
};

template <unsigned N>
inline std::ostream& operator<<(std::ostream& os, MultidiffTransform<N> frame)
{
    os << "{" << std::endl;
    os << "  " << frame.static_frame << "," << std::endl;
    for(int i=0; i<N; i++){
        os << "  " << frame.dynamic_frame[i] << "," << std::endl;
    }
    os << "}";
    return os;
}

}
