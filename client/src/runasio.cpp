/*
 * =====================================================================================
 *
 *       Filename: runasio.cpp
 *        Created: 02/23/2016 00:09:59
 *  Last Modified: 04/18/2016 00:06:17
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

#include "game.hpp"
#include "xmlconf.hpp"

void Game::RunASIO()
{
    // this function will run in another thread
    // make sure there is no data race

    // TODO
    // may need lock here since g_XMLConf may used in main thread also
    std::string szIP;
    std::string szPort;

    extern XMLConf *g_XMLConf;
    auto p1 = g_XMLConf->GetXMLNode("/Root/Network/Server/IP"  );
    auto p2 = g_XMLConf->GetXMLNode("/Root/Network/Server/Port");

    if(p1 && p2 && p1->GetText() && p2->GetText()){
        szIP   = p1->GetText();
        szPort = p2->GetText();
    }else{
        szIP   = "127.0.0.1";
        szPort = "5000";
    }
    m_NetIO.RunIO(szIP.c_str(), szPort.c_str(), [this](uint8_t nHC){ OperateHC(nHC); });
}

// you can use 
// std::future<void> stFuture = std::async(std::launch::async, [this](){ RunASIO(); });
// as asynchroized method, which setup and run ASIO as one function
//
// but I wanna put them in one thead
//
//      InitASIO();
//
//      while(!bExit){
//          PollASIO();
//          ...
//
//          Update();
//          Draw();
//      }
//
void Game::InitASIO()
{
    // this function will run in another thread
    // make sure there is no data race

    // TODO
    // may need lock here since g_XMLConf may used in main thread also
    std::string szIP;
    std::string szPort;

    extern XMLConf *g_XMLConf;
    auto p1 = g_XMLConf->GetXMLNode("/Root/Network/Server/IP"  );
    auto p2 = g_XMLConf->GetXMLNode("/Root/Network/Server/Port");

    if(p1 && p2 && p1->GetText() && p2->GetText()){
        szIP   = p1->GetText();
        szPort = p2->GetText();
    }else{
        szIP   = "127.0.0.1";
        szPort = "5000";
    }
    m_NetIO.InitIO(szIP.c_str(), szPort.c_str(), [this](uint8_t nHC){ OperateHC(nHC); });
}

void Game::PollASIO()
{
    m_NetIO.PollIO();
}
