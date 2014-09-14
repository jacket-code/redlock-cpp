//
//  CLockExample.cpp
//
//  Created by jacketzhong on 14-9-11.
//  Copyright (c) 2014年 jacketzhong. All rights reserved.
//

#include <stdlib.h>
#include <unistd.h>
#include "redlock-cpp/redlock.h"

int main (int argc, char **argv) {
    CRedLock * dlm = new CRedLock();
    dlm->AddServerUrl("127.0.0.1", 5005);
    dlm->AddServerUrl("127.0.0.1", 5006);
    dlm->AddServerUrl("127.0.0.1", 5007);
    
    // 连续分布式锁的使用案例
    while (1) {
        CLock my_lock;
        bool flag = dlm->ContinueLock("foo", 14000, my_lock);
        if (flag) {
            printf("获取成功, Acquired by client name:%s, res:%s, vttl:%d\n",
                   my_lock.m_val, my_lock.m_resource, my_lock.m_validityTime);
            // do resource job
            sleep(10);
        } else {
            printf("获取失败, lock not acquired, name:%s\n", my_lock.m_val);
            sleep(rand() % 3);
        }
    }
    
    return 0;
}
