/*
 * =====================================================================================
 *
 *       Filename: hero.cpp
 *        Created: 9/3/2015 3:49:00 AM
 *  Last Modified: 06/13/2016 16:52:03
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

#include "hero.hpp"

Hero::Hero(uint32_t nGUID, uint32_t nUID, uint32_t nGenTime, bool bMale)
    : Creature(nUID, nGenTime)
    , m_GUID(nGUID)
    , m_Male(bMale)
{}

Hero::~Hero()
{}

void Hero::Draw(int, int)
{
}

void Hero::Update()
{
}
