/*
 Mining Pool Agent

 Copyright (C) 2016  BTC.COM

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <event2/util.h>

#include "gtest/gtest.h"
#include "Utils.h"

TEST(Utils, Strings_Format) {
  for (int i = 1; i < 1024; i++) {
    string s;
    for (int j = 0; j < i; j++) {
      s += "A";
    }
    string s1 = Strings::Format("%s", s.c_str());
    ASSERT_EQ(s1, s);
  }
}

TEST(Utils, Strings_FormatIP) {
  in_addr addr;
  string ip = "123.45.67.89";
  
  ASSERT_EQ(evutil_inet_pton(AF_INET, ip.c_str(), &addr), 1);
  ASSERT_EQ(Strings::FormatIP(addr.s_addr, "{1}x{2}x{3}x{4}"), "123x45x67x89");
}

TEST(Utils, Strings_Append) {
  for (int i = 1; i < 1024; i++) {
    string s;
    for (int j = 0; j < i; j++) {
      s += "A";
    }
    string s1;
    Strings::Append(s1, "%s", s.c_str());
    ASSERT_EQ(s1, s);
  }
}

TEST(Utils, splitNotify) {
  string l1 = "{\"id\":null,\"method\":\"mining.notify\",\"params\":[\"0\",\"c4c401368c24e20edc18932587dd724bd6a54a0a00e2b0a40000000000000000\",\"01000000010000000000000000000000000000000000000000000000000000000000000000ffffffff19030083062f4254432e434f4d2f";
  string l2 = "\",\"ffffffff01ff58294d0000000017a914134468158139c8613c7677e7289443dc5b9426578700000000\",[\"e4998740e0cdbaf1e7962066261d169f43336c8844a5e1c966df22078eaf2bd7\",\"ecd413ea949531b5c7fb9cd2ffeadbacdb56ba91a66c8c30b2bfdfaf86618601\",\"83ab2807fcb97987376ba0e56f83982099b5e2c71a2b3235b6fab7ed33cf37ee\",\"90fa17b455f61fc3903b4960051388bd2cba12a45193957cff04fd32c62a1dc9\",\"ba1c74f0e4ec71327f9076fc5f8a1fa57f932b5e016a6ad8415b39f84fa13cf9\",\"82a103c8b201942d6e9ea771ee9200aec60be8ced29442b183089e5348499c96\",\"4792e27547c65fedb3b2ae83ae823df7d566c249d88df109639b88459806f939\",\"88184a3ad97590ede0af268f03b9b731005680c2715edc91f42fee9b73d54a09\",\"2681b734a05f5fcf472bc015899b52ed29fc96186a2650837a27ff02d8ce53ab\",\"33f33d7cb90cbaa99c7be7f2f9ad8f3f1e255210bac02eafa8566819bbaef95b\",\"96e6444b6b93ed0f18f46f1ca99d3a43caffb906176d22c0c56ffcfa8da6688b\"],\"20000000\",\"18050edc\",\"57be5b49\",false]}";
  string line = l1 + l2;

  const char *pch = splitNotify(line);

  ASSERT_EQ(static_cast<size_t >(pch - line.c_str()), l1.length());
  ASSERT_EQ(strlen(pch), l2.length());
}


TEST(Utils, Strings_parseConfJson) {
  {
    AgentConf conf;

    string line = R"EOF({
      "agent_listen_ip": "0.0.0.0",
      "agent_listen_port": 3333,
      "pools": [
          {
            "duration": 20,
            "pool": [
              [ "us.ss.btc.com", 1800, "kevin" ]
            ]
          }
      ]
    })EOF";

    ASSERT_EQ(parseConfJson(line, conf), true);

    ASSERT_EQ(conf.agentType_, "btc");
    ASSERT_EQ(conf.alwaysKeepDownconn_, false);
    ASSERT_EQ(conf.disconnectWhenLostAsicBoost_, true);
    ASSERT_EQ(conf.useIpAsWorkerName_, false);
    ASSERT_EQ(conf.ipWorkerNameFormat_, "{1}x{2}x{3}x{4}");
    ASSERT_EQ(conf.submitResponseFromServer_, false);

    ASSERT_EQ(conf.listenIP_, "0.0.0.0");
    ASSERT_EQ(conf.listenPort_, 3333u);
    ASSERT_EQ(conf.fixedWorkerName_, "");
    
    ASSERT_EQ(conf.pools_.size(), 1u);
    ASSERT_EQ(conf.pools_[0].duration_, 20u);
    ASSERT_EQ(conf.pools_[0].confs_[0].host_, "cn.ss.btc.com");
    ASSERT_EQ(conf.pools_[0].confs_[0].port_, 1800);
    ASSERT_EQ(conf.pools_[0].confs_[0].upPoolUserName_, "kevin");
  }

  {
    AgentConf conf;

    string line = R"EOF({
      "always_keep_downconn": true,
      "disconnect_when_lost_asicboost": false,
      "use_ip_as_worker_name": true,
      "ip_worker_name_format": "{1}.{2}.{3}.{4}",
      "submit_response_from_server": true,
      "agent_type": "btc",
      "agent_listen_ip": "127.0.0.1",
      "agent_listen_port": 1800,
      "fixed_worker_name": "myworker",
      "pools": [
        {
          "duration": 10,
          "pool": [
            [ "us.ss.btc.com", 1800, "kevin" ],
            [ "us.ss.btc.com", 443, "YourSubAccountName" ],
            [ "us.ss.btc.com", 3333, "YourSubAccountName" ]
          ]
        },
        {
          "duration": 20,
          "pool": [
            [ "us.ss.btc.com", 1801, "kevinus" ],
            [ "us.ss.btc.com", 444, "YourSubAccountName" ],
            [ "us.ss.btc.com", 3334, "YourSubAccountName" ]
          ]
        }
      ]
    })EOF";
    ASSERT_EQ(parseConfJson(line, conf), true);

    ASSERT_EQ(conf.agentType_, "btc");
    ASSERT_EQ(conf.alwaysKeepDownconn_, true);
    ASSERT_EQ(conf.disconnectWhenLostAsicBoost_, false);
    ASSERT_EQ(conf.useIpAsWorkerName_, true);
    ASSERT_EQ(conf.ipWorkerNameFormat_, "{1}.{2}.{3}.{4}");
    ASSERT_EQ(conf.submitResponseFromServer_, true);
    ASSERT_EQ(conf.listenIP_, "127.0.0.1");
    ASSERT_EQ(conf.listenPort_, 1800u);
    ASSERT_EQ(conf.fixedWorkerName_, "myworker");

    ASSERT_EQ(conf.pools_.size(), 2u);

    ASSERT_EQ(conf.pools_[0].duration_, 20u);
    ASSERT_EQ(conf.pools_[0].confs_[0].host_, "cn.ss.btc.com");
    ASSERT_EQ(conf.pools_[0].confs_[0].port_, 1800u);
    ASSERT_EQ(conf.pools_[0].confs_[0].upPoolUserName_, "kevin");

    ASSERT_EQ(conf.pools_[1].duration_, 30u);
    ASSERT_EQ(conf.pools_[1].confs_[0].host_, "cn.ss.btc.com");
    ASSERT_EQ(conf.pools_[1].confs_[0].port_, 1800u);
    ASSERT_EQ(conf.pools_[1].confs_[0].upPoolUserName_, "kevin");
  }
}
