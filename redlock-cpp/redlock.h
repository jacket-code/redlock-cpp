/* RedLock implement DLM(Distributed Lock Manager) with cpp.
 *
 * Copyright (c) 2014, jacketzhong <jacketzhong at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __redlock__
#define __redlock__

#include <iostream>
#include <vector>
#include "hiredis/hiredis.h"
extern "C" {
#include "sds.h"
}

using namespace std;

class CLock {
public:
                            CLock();
                            ~CLock();
public:
    int                     m_validityTime; // 当前锁可以存活的时间, 毫秒
    sds                     m_resource;     // 要锁住的资源名称
    sds                     m_val;          // 锁住资源的进程随机名字
};

class CRedLock {
public:
                            CRedLock();
    virtual                 ~CRedLock();
public:
    bool                    Initialize();
    bool                    AddServerUrl(const char *ip, const int port);
    void                    SetRetry(const int count, const int delay);
    bool                    Lock(const char *resource, const int ttl, CLock &lock);
    bool                    ContinueLock(const char *resource, const int ttl,
                                         CLock &lock);
    bool                    Unlock(const CLock &lock);
private:
    bool                    LockInstance(redisContext *c, const char *resource,
                                         const char *val, const int ttl);
    bool                    ContinueLockInstance(redisContext *c, const char *resource,
                                                 const char *val, const int ttl);
    void                    UnlockInstance(redisContext *c, const char *resource,
                                           const char *val);
    sds                     GetUniqueLockId();
    redisReply *            RedisCommandArgv(redisContext *c, int argc, char **inargv);
private:
    static int              m_defaultRetryCount;    // 默认尝试次数3
    static int              m_defaultRetryDelay;    // 默认尝试延时200毫秒
    static float            m_clockDriftFactor;     // 电脑时钟误差0.01
private:
    sds                     m_unlockScript;         // 解锁脚本
    int                     m_retryCount;           // try count
    int                     m_retryDelay;           // try delay
    int                     m_quoRum;               // majority nums
    int                     m_fd;                   // rand file fd
    vector<redisContext *>  m_redisServer;          // redis master servers
    CLock                   m_continueLock;         // 续锁
    sds                     m_continueLockScript;   // 续锁脚本
};

#endif /* defined(__redlock__) */
