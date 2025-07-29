#include "TickSchedulerManager.h"
#include <boost/asio/post.hpp>
#include "../Session/Manager/UserSessionShardManager.h"
#include "../World/ZoneManager/ZoneControllerManager.h"

TickSchedulerManager::TickSchedulerManager(IoContextPool& ioPool, int zoneThreadCount)
    : _ioPool(ioPool)
{    
    _zoneThreadCount = zoneThreadCount;
    for (int i = 0; i < _zoneThreadCount; ++i)
    {
        boost::asio::io_context& ctx = _ioPool.GetNextIoContext();
        _userContexts.push_back(&ctx);
    }
}
void TickSchedulerManager::Start() 
{    
    StartZoneTickLoop();
}



void TickSchedulerManager::StartZoneTickLoop()
{
    for (int shardId = 0; shardId < _zoneThreadCount; ++shardId)
    {
        ScheduleNextZoneTick(shardId);
    }
}

void TickSchedulerManager::ScheduleNextZoneTick(int shardId)
{
    auto timer = std::make_shared<boost::asio::steady_timer>(*_userContexts[shardId]);
    timer->expires_after(std::chrono::milliseconds(6000)); // 100ms 주기

    timer->async_wait([this, shardId, timer](const boost::system::error_code& ec) {
        if (ec) return;

        ZoneControllerManager::Instance().Tick(shardId, [](auto zone) {
            zone->Tick();
            });

        ScheduleNextZoneTick(shardId); // 재귀 호출로 루프
        });
}