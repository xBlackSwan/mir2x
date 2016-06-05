/*
 * =====================================================================================
 *
 *       Filename: servicecoreop.cpp
 *        Created: 05/03/2016 21:29:58
 *  Last Modified: 06/05/2016 12:15:25
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
#include <string>

#include "player.hpp"
#include "actorpod.hpp"
#include "monoserver.hpp"
#include "servicecore.hpp"

void ServiceCore::On_MPK_DUMMY(const MessagePack &, const Theron::Address &)
{
    // do nothing since this is message send by anyomous SyncDriver
    // to drive the anyomous trigger
}

// ServiceCore accepts net packages from *many* sessions and based on it to create
// the player object for a one to one map
//
// So servicecore <-> session is 1 to N, means we have to put put pointer of session
// in the net package otherwise we can't find the session even we have session's 
// address, session is a sync-driver, even we have it's address we can't find it
//
void ServiceCore::On_MPK_NETPACKAGE(const MessagePack &rstMPK, const Theron::Address &)
{
    AMNetPackage stAMNP;
    std::memcpy(&stAMNP, rstMPK.Data(), sizeof(stAMNP));

    OperateNet(stAMNP.SessionID, stAMNP.Type, stAMNP.Data, stAMNP.DataLen);
}

// this can only come from MonoServer layer, monster should be add internally
// void ServiceCore::On_MPK_ADDMONSTER(const MessagePack &rstMPK, const Theron::Address &rstFromAddr)
// {
//     AMAddMonster stAMAM;
//     std::memcpy(&stAMAM, rstMPK.Data(), sizeof(stAMAM));
//
//     // hummm, different map may have different size of RM
//     auto stRMAddr = GetRMAddress(stAMAM.MapID, stAMAM.X, stAMAM.Y);
//     if(stRMAddr == Theron::Address::Null()){
//         if(rstMPK.ID()){
//             m_ActorPod->Forward(MPK_ERROR, rstAddr, rstMPK.ID());
//         }
//     }
//
//
//
//     if(m_MapRecordMap.find(stAMAM.MapID) == m_MapRecordMap.end()){
//         LoadMap(stAMAM.MapID);
//     }
//
//     auto fnOPR = [this, rstFromAddr](const MessagePack &rstRMPK, const Theron::Address &){
//         switch(rstRMPK.Type()){
//             case MPK_OK:
//                 {
//                     m_ActorPod->Forward(MPK_OK, rstFromAddr);
//                     break;
//                 }
//             default:
//                 {
//                     m_ActorPod->Forward(MPK_ERROR, rstFromAddr);
//                     break;
//                 }
//         }
//     };
//     m_ActorPod->Forward({MPK_ADDMONSTER, rstMPK.Data(),
//             rstMPK.DataLen()}, m_MapRecordMap[stAMAM.MapID].PodAddress, fnOPR);
// }

// TODO
// based on the information of coming connection
// do IP banning etc. currently only activate this session
void ServiceCore::On_MPK_NEWCONNECTION(const MessagePack &rstMPK, const Theron::Address &)
{
    uint32_t nSessionID = *((uint32_t *)rstMPK.Data());
    if(nSessionID){
        extern NetPodN *g_NetPodN;
        g_NetPodN->Activate(nSessionID, GetAddress());
    }
}

// void ServiceCore::On_MPK_LOGIN(const MessagePack &rstMPK, const Theron::Address &)
// {
//     AMLogin stAML;
//     std::memcpy(&stAML, rstMPK.Data(), sizeof(stAML));
//
//     extern MonoServer *g_MonoServer;
//     auto pNewPlayer = new Player(m_CurrUID++,
//             g_MonoServer->GetTimeTick(), stAML.GUID, stAML.SID);
//
//     // ... add all dress, inventory, weapon here
//     // ... add all position/direction/map/state here
//
//     AMNewPlayer stAMNP;
//     stAMNP.Data = (void *)pNewPlayer;
//
//     if(m_MapRecordMap.find(stAML.MapID) == m_MapRecordMap.end()){
//         // load map
//     }
//     m_ActorPod->Forward({MPK_NEWPLAYER, stAMNP}, m_MapRecordMap[stAML.MapID].PodAddress);
// }

void ServiceCore::On_MPK_ADDCHAROBJECT(
        const MessagePack &rstMPK, const Theron::Address &rstFromAddr)
{
    AMAddCharObject stAMACO;
    std::memcpy(&stAMACO, rstMPK.Data(), sizeof(stAMACO));

    uint32_t nMapID = stAMACO.Common.MapID;
    int nMapX = stAMACO.Common.MapX;
    int nMapY = stAMACO.Common.MapY;

    if(!ValidP(nMapID, nMapX, nMapY)){
        m_ActorPod->Forward(MPK_ERROR, rstFromAddr, rstMPK.ID());
        return;
    }

    const auto &rstRMRecord = GetRMRecord(nMapID, nMapX, nMapY, false);
    switch(rstRMRecord.Query){
        case QUERY_OK:
            {
                // merely happen
                if(!rstRMRecord.PodAddress){
                    m_ActorPod->Forward(MPK_ERROR, rstFromAddr, rstMPK.ID());
                    return;
                }

                auto fnOnR = [stAMACO, this](const MessagePack &rstRMPK, const Theron::Address &){
                    // TODO: do nothing, but put a callback
                    // reason is for RM it can accept MPK_ADDCHAROBJECT from many actors
                    // besides servicecore, those all expect a callback, so here we keep
                    // the interface consistant
                    extern MonoServer *g_MonoServer;
                    if(rstRMPK.Type() != MPK_OK){
                        g_MonoServer->AddLog(LOGTYPE_WARNING,
                                "add monster failed: MonsterID = %d, MapID = %d, X = %d, Y = %d",
                                stAMACO.Monster.MonsterID,
                                stAMACO.Common.MapID, stAMACO.Common.MapX, stAMACO.Common.MapY);
                        return;
                    }

                    // ok we succeed
                    g_MonoServer->AddLog(LOGTYPE_INFO,
                            "add monster succeed: MonsterID = %d, MapID = %d, X = %d, Y = %d",
                            stAMACO.Monster.MonsterID,
                            stAMACO.Common.MapID, stAMACO.Common.MapX, stAMACO.Common.MapY);
                };
                m_ActorPod->Forward({MPK_ADDCHAROBJECT, stAMACO}, rstRMRecord.PodAddress, fnOnR);
                m_ActorPod->Forward(MPK_PENDING, rstRMRecord.PodAddress, rstMPK.ID());
                return;
            }
        case QUERY_PENDING:
            {
                // the RM address is on the way
                // we need to adding the player when SC get this RM address
                auto fnOnGetRMAddress = [stAMACO, nMapID, nMapX, nMapY, this]() -> bool{
                    // since this trigger will be driven by itself, we don't need
                    // to install a trigger in GetRMRecord()
                    const auto &rstRMRecord = GetRMRecord(nMapID, nMapX, nMapY, false);

                    // 1. still waiting
                    if(rstRMRecord.Query == QUERY_PENDING){
                        // ok we have to continue to drive this trigger
                        extern EventTaskHub *g_EventTaskHub;
                        g_EventTaskHub->Add(200, [stAddr = m_ActorPod->GetAddress()](){
                            SyncDriver().Forward(MPK_DUMMY, stAddr);
                        });

                        return false;
                    }

                    // 2. otherwise we won't need this trigger handler anymore
                    if(!rstRMRecord.Valid()){
                        // 1. report it
                        extern MonoServer *g_MonoServer;
                        g_MonoServer->AddLog(LOGTYPE_INFO,
                                "RM is not valid corresponding to (map, x, y) = (%d, %d, %d)", nMapID, nMapX, nMapY);
                        // 2. delete this opertion handler
                        return true;
                    }

                    // 3. ok ready to add the object
                    auto fnOnR = [stAMACO, this](const MessagePack &rstRMPK, const Theron::Address &){
                        // TODO: do nothing, but put a callback
                        // reason is for RM it can accept MPK_ADDCHAROBJECT from many actors
                        // besides servicecore, those all expect a callback, so here we keep
                        // the interface consistant
                        extern MonoServer *g_MonoServer;
                        if(rstRMPK.Type() != MPK_OK){
                            g_MonoServer->AddLog(LOGTYPE_WARNING,
                                    "add monster error: MonsterID = %d, MapID = %d, X = %d, Y = %d",
                                    stAMACO.Monster.MonsterID,
                                    stAMACO.Common.MapID, stAMACO.Common.MapX, stAMACO.Common.MapY);
                            return;
                        }

                        // ok we succeed
                        g_MonoServer->AddLog(LOGTYPE_INFO,
                                "add monster ok: MonsterID = %d, MapID = %d, X = %d, Y = %d",
                                stAMACO.Monster.MonsterID,
                                stAMACO.Common.MapID, stAMACO.Common.MapX, stAMACO.Common.MapY);
                    };

                    m_ActorPod->Forward({MPK_ADDCHAROBJECT, stAMACO}, rstRMRecord.PodAddress, fnOnR);
                    return true;
                };

                m_Hook.Install(fnOnGetRMAddress);
                m_ActorPod->Forward(MPK_PENDING, rstFromAddr, rstMPK.ID());
                return;
            }
        default:
            {
                m_ActorPod->Forward(MPK_ERROR, rstFromAddr, rstMPK.ID());
                return;
            }
    }
}

void ServiceCore::On_MPK_PLAYERPHATOM(const MessagePack &rstMPK, const Theron::Address &)
{
    AMPlayerPhantom stAMPP;
    std::memcpy(&stAMPP, rstMPK.Data(), sizeof(stAMPP));

    if(m_PlayerRecordMap.find(stAMPP.GUID) != m_PlayerRecordMap.end()){
    }
}

// don't try to find its sender, it's from a temp SyncDriver in the lambda
void ServiceCore::On_MPK_LOGINQUERYDB(const MessagePack &rstMPK, const Theron::Address &)
{
    AMLoginQueryDB stAMLQDB;
    std::memcpy(&stAMLQDB, rstMPK.Data(), sizeof(stAMLQDB));

    uint32_t nMapID = stAMLQDB.MapID;
    int nMapX = stAMLQDB.MapX;
    int nMapY = stAMLQDB.MapY;

    if(!ValidP(nMapID, nMapX, nMapY)){ return; }

    const auto &rstRMRecord = GetRMRecord(nMapID, nMapX, nMapY, false);
    switch(rstRMRecord.Query){
        case QUERY_NA:
            {
                // this RM is not walkable
                // TODO: this is a serious error actually
                //       we need take action rather than just report failure
                extern NetPodN *g_NetPodN;
                g_NetPodN->Send(stAMLQDB.SessionID, SM_LOGINFAIL,
                        [nSID = stAMLQDB.SessionID](){g_NetPodN->Shutdown(nSID);});
                return;
            }
        case QUERY_OK:
            {
                // valid RM, try to login
                if(rstRMRecord.PodAddress == Theron::Address::Null()){ return; }

                AMAddCharObject stAMACO;
                stAMACO.Type = OBJECT_PLAYER;
                stAMACO.Common.MapID     = stAMLQDB.MapID;
                stAMACO.Common.MapX      = stAMLQDB.MapX;
                stAMACO.Common.MapY      = stAMLQDB.MapY;
                stAMACO.Player.GUID      = stAMLQDB.GUID;
                stAMACO.Player.Level     = stAMLQDB.Level;
                stAMACO.Player.JobID     = stAMLQDB.JobID;
                stAMACO.Player.Direction = stAMLQDB.Direction;
                stAMACO.Player.SessionID = stAMLQDB.SessionID;

                // we don't need the response
                // if adding succeeds, the player will report itself to SC
                m_ActorPod->Forward({MPK_ADDCHAROBJECT, stAMACO}, rstRMRecord.PodAddress);
                return;
            }
        case QUERY_PENDING:
            {
                // the RM address is on the way
                // we need to adding the player when SC get this RM address
                auto fnOnGetRMAddress = [stAMLQDB, nMapID, nMapX, nMapY, this]() -> bool{
                    const auto &rstRMRecord = GetRMRecord(nMapID, nMapX, nMapY, false);

                    // still pending
                    if(rstRMRecord.Query == QUERY_PENDING){
                        // drive this anyomous trigger
                        extern EventTaskHub *g_EventTaskHub;
                        g_EventTaskHub->Add(200, [stAddr = m_ActorPod->GetAddress()](){
                            SyncDriver().Forward(MPK_DUMMY, stAddr);
                        });

                        return false;
                    }

                    if(!rstRMRecord.Valid()){
                        // 1. report it
                        extern MonoServer *g_MonoServer;
                        g_MonoServer->AddLog(LOGTYPE_INFO,
                                "RM is not valid corresponding to (map, x, y) = (%d, %d, %d)", nMapID, nMapX, nMapY);
                        // 2. delete this opertion handler
                        return true;
                    }

                    // OK we get the valid address
                    AMAddCharObject stAMACO;
                    stAMACO.Type = OBJECT_PLAYER;
                    stAMACO.Common.MapID     = stAMLQDB.MapID;
                    stAMACO.Common.MapX      = stAMLQDB.MapX;
                    stAMACO.Common.MapY      = stAMLQDB.MapY;
                    stAMACO.Player.GUID      = stAMLQDB.GUID;
                    stAMACO.Player.Level     = stAMLQDB.Level;
                    stAMACO.Player.JobID     = stAMLQDB.JobID;
                    stAMACO.Player.Direction = stAMLQDB.Direction;
                    stAMACO.Player.SessionID = stAMLQDB.SessionID;

                    // 3. ok ready to add the object
                    auto fnOnR = [stAMACO, this](const MessagePack &rstRMPK, const Theron::Address &){
                        // TODO: do nothing, but put a callback
                        // reason is for RM it can accept MPK_ADDCHAROBJECT from many actors
                        // besides servicecore, those all expect a callback, so here we keep
                        // the interface consistant
                        extern MonoServer *g_MonoServer;
                        if(rstRMPK.Type() != MPK_OK){
                            g_MonoServer->AddLog(LOGTYPE_WARNING,
                                    "add player failed: GUID = %d, JobID = %d, MapID = %d, X = %d, Y = %d",
                                    stAMACO.Player.GUID, stAMACO.Player.JobID, stAMACO.Common.MapID, stAMACO.Common.MapX, stAMACO.Common.MapY);
                            return;
                        }

                        // ok we succeed
                            g_MonoServer->AddLog(LOGTYPE_INFO,
                                    "add player succeed: GUID = %d, JobID = %d, MapID = %d, X = %d, Y = %d",
                                    stAMACO.Player.GUID, stAMACO.Player.JobID, stAMACO.Common.MapID, stAMACO.Common.MapX, stAMACO.Common.MapY);
                    };

                    // when adding succeed, the new object will respond
                    m_ActorPod->Forward({MPK_ADDCHAROBJECT, stAMACO}, rstRMRecord.PodAddress, fnOnR);

                    // don't forget it
                    return true;
                };

                m_Hook.Install(fnOnGetRMAddress);
                // don't response this message, it's send by temp SyncDriver()
                return;
            }
        default:
            {
                extern MonoServer *g_MonoServer;
                g_MonoServer->AddLog(LOGTYPE_WARNING, "invalid query state");
                return;
            }
    }
}
