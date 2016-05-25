/*
 * =====================================================================================
 *
 *       Filename: mathfunc.hpp
 *        Created: 02/02/2016 20:50:30
 *  Last Modified: 05/24/2016 18:00:08
 *
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */
#pragma once
#include <algorithm>
#include <cmath>
#include <type_traits>

template<typename T> T LDistance2(T nfX, T nfY, T nfX1, T nfY1)
{
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");
    return (nfX - nfX1) * (nfX - nfX1) + (nfY - nfY1) * (nfY - nfY1);
}

template<typename T> T LDistance(T nfX, T nfY, T nfX1, T nfY1)
{
    return T(std::sqrt(LDistance2(nfX, nfY, nfX1, nfY1)));
}

template<typename T> bool CircleOverlap(T nfX, T nfY, T nfR, T nfX1, T nfY1, T nfR1)
{
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");
    return LDistance2(nfX, nfY, nfX1, nfY1) <= (nfR + nfR1) * (nfR + nfR1);
}

template<typename T> bool CircleRectangleOverlap(T nfCX, T nfCY, T nfCR, T nfX, T nfY, T nfW, T nfH)
{
    // there may be one pixel problem, keep it in mind
    T nfRCX = nfX + nfW / 2;
    T nfRCY = nfY + nfH / 2;

    T nfDX = std::abs(nfCX - nfRCX);
    T nfDY = std::abs(nfCY - nfRCY);

    if(nfDX > (nfW/2 + nfCR)){ return false; }
    if(nfDY > (nfH/2 + nfCR)){ return false; }

    if(nfDX <= (nfW/2)){ return true; } 
    if(nfDY <= (nfH/2)){ return true; }

    return (nfDX - nfW / 2) * (nfDX - nfW / 2) + (nfDY - nfH / 2) * (nfDY - nfH / 2) <= nfCR * nfCR;
}

template<typename T> bool RectangleCircleOverlap(T nfX, T nfY, T nfW, T nfH, T nfCX, T nfCY, T nfCR)
{
    return CircleRectangleOverlap(nfCX, nfCY, nfCR, nfX, nfY, nfW, nfH);
}

template<typename T> bool PointInSegment(T nfX, T nfStartX, T nfW)
{
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");
    return nfX >= nfStartX && nfX < nfStartX + nfW;
}

template<typename T> bool PointInRectangle(T nfX, T nfY, T nStartX, T nStartY, T nfW, T nfH)
{
    return PointInSegment(nfX, nStartX, nfW) && PointInSegment(nfY, nStartY, nfH);
}

template<typename T> bool PointInTriangle(T nfX, T nfY, T nfX1, T nfY1, T nfX2, T nfY2, T nfX3, T nfY3)
{
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");

    auto bSign = [](T p1X, T p1Y, T p2X, T p2Y, T p3X, T p3Y){
        return (p1X - p3X) * (p2Y - p3Y) - (p2X - p3X) * (p1Y - p3Y);
    };

    bool b1 = (bSign(nfX, nfY, nfX1, nfY1, nfX2, nfY2) <= (T)(0));
    bool b2 = (bSign(nfX, nfY, nfX2, nfY2, nfX3, nfY3) <= (T)(0));
    bool b3 = (bSign(nfX, nfY, nfX3, nfY3, nfX1, nfY1) <= (T)(0));

    return ((b1 == b2) && (b2 == b3));
}

template<typename T> bool PointInCircle(T nfX, T nfY, T nfCX, T nfCY, T nfR)
{
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");
    T dX = nfX - nfCX;
    T dY = nfY - nfCY;
    return dX * dX + dY * dY <= nfR * nfR;
}

template<typename T> bool RectangleOverlap(T nfX1, T nfY1, T nfW1, T nfH1, T nfX2, T nfY2, T nfW2, T nfH2)
{
    // TODO
    // finish code
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");
    return !(true
            || nfX1 >= nfX2 + nfW2
            || nfY1 >= nfY2 + nfH2
            || nfX1 + nfW1 < nfX2
            || nfY1 + nfH1 < nfY2);
}

// check whether R2 is inside R1
template<typename T> bool RectangleInside(T nfX1, T nfY1, T nfW1, T nfH1, T nfX2, T nfY2, T nfW2, T nfH2)
{
    // TODO
    // finish code
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");
    return true
        && (nfX1 <= nfX2) && (nfX1 + nfW1 >= nfX2 + nfW2)
        && (nfY1 <= nfY2) && (nfY1 + nfH1 >= nfY2 + nfH2);
}

template<typename T> bool IntervalOverlap(T nfX1, T nfW1, T nfX2, T nfW2)
{
    static_assert(std::is_arithmetic<T>::value, "Arithmetic type required...");
    // TODO
    // assume T2 is smaller
    return !(nfX1 + nfW1 <= nfX2 || nfX2 + nfW2 <= nfX1);
}

template<typename T> bool RectangleOverlapRegion(
        T nfX1, T nfY1, T nfW1, T nfH1, T *nfX2, T *nfY2, T *nfW2, T *nfH2)
{
    if(nfX2 && nfY2 && nfW2 && nfH2 && 
            RectangleOverlap(nfX1, nfY1, nfW1, nfH1, *nfX2, *nfY2, *nfW2, *nfH2)){

        T nfRX, nfRY, nfRW, nfRH;
        // TODO
        //
        // we assume W, H are always non-negative

        nfRX = std::max(nfX1, *nfX2);
        nfRY = std::max(nfY1, *nfY2);
        nfRW = std::min(nfX1 + nfW1, *nfX2 + *nfW2) - nfRX;
        nfRH = std::min(nfY1 + nfH1, *nfY2 + *nfH2) - nfRY;

        *nfX2 = nfRX;
        *nfY2 = nfRY;
        *nfW2 = nfRW;
        *nfH2 = nfRH;

        return true;
    }
    return false;
}

template<typename T> bool PowerOf2(T nParam)
{
    static_assert(std::is_integral<T>::value, "Integral type required...");
    return !(nParam & (nParam - 1));
}

// TODO & TBD
// this is portable but not optimized, gcc has builtin functions
// as __builtin_clz(), google it, however it's not portable
template<typename T> T RoundByPowerOf2(T nParam)
{
    static_assert(std::is_integral<T>::value, "Integral type required...");

    nParam |= (nParam >> 1);
    nParam |= (nParam >> 2);

    if(sizeof(T) >= 1){ nParam |= (nParam >>  4); }
    if(sizeof(T) >= 2){ nParam |= (nParam >>  8); }
    if(sizeof(T) >= 4){ nParam |= (nParam >> 16); }
    if(sizeof(T) >= 8){ nParam |= (nParam >> 32); }

    return nParam + 1;
}
