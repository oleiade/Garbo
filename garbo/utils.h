#ifndef UTILS_H
#define UTILS_H

#include <Process.h>

#include "./debug.h"

// Synchronize clock using NTP
void setClock(Process* p) {
  // Sync clock with NTP
  p->runShellCommand("ntpd -nqp 0.openwrt.pool.ntp.org");

  // Block until clock sync is completed
  while (p->running());
}

// Check if connected to the Internet
int isConnectedToInternet(Process* p) {
  // Check if ping to google.com is successful
  p->runShellCommand("ping -W 1 -c 4 www.google.com >& /dev/null && echo 1 || echo 0");

  // Wait until execution is completed, return result
  while (p->running());

  return p->parseInt();
}

#endif
