/**************************************************************************
**
** 	Copyright 2010 Duke Inc.
**
**************************************************************************/

#ifndef _NB_NET_TOOL_H_
#define _NB_NET_TOOL_H_

// Posix header files
#include <sys/types.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <vector>
#include <fcntl.h>

//#include <boost/noncopyable.hpp>

class auto_ifaddrs //: private boost::noncopyable
{
public:
    auto_ifaddrs(struct ifaddrs* ifa) : m_ifaddr(ifa)
    { }
    ~auto_ifaddrs()
    {
        if (m_ifaddr != NULL)
        {
            freeifaddrs(m_ifaddr);
            m_ifaddr = NULL;
        }
    }
private:
    auto_ifaddrs(const auto_ifaddrs&)
    {
    }
    auto_ifaddrs& operator=(const auto_ifaddrs&)
    {
        return *this;
    }

    struct ifaddrs* m_ifaddr;
};

inline int nb_getnameinfo(sockaddr* sa, std::string& hostname)
{
    char hbuf[NI_MAXHOST] = { 0 };
    socklen_t len = (sa->sa_family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    int ret = getnameinfo(sa, len, hbuf, sizeof(hbuf), NULL, 0, NI_NUMERICHOST);
    if (ret == 0)
    {
        hostname = hbuf;
    }
    return ret;
}

inline bool nb_getifaddrs_v4(std::vector<in_addr_t>& hosts)
{
    struct ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) == -1)
        return false;
    auto_ifaddrs autoifa(ifaddr);

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET && !(ifa->ifa_flags & IFF_LOOPBACK) && (ifa->ifa_flags & IFF_UP))
        {
            struct sockaddr_in *sin = reinterpret_cast<struct sockaddr_in *>(ifa->ifa_addr);
            hosts.push_back(sin->sin_addr.s_addr);
        }
    }

    return true;
}

inline bool nb_getifaddrs_v4(std::vector<std::string>& hosts)
{
    struct ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) == -1)
        return false;
    auto_ifaddrs autoifa(ifaddr);

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET && !(ifa->ifa_flags & IFF_LOOPBACK) && (ifa->ifa_flags & IFF_UP))
        {
            std::string strhost;
            if (nb_getnameinfo(ifa->ifa_addr, strhost) != 0)
                return false;
            hosts.push_back(strhost);
        }
    }

    return true;
}

// only get the first addr
inline std::string nb_getifaddr_v4()
{
    std::vector<std::string> hosts;
    bool result = nb_getifaddrs_v4(hosts);

    if(result && !hosts.empty())
    {        
        return hosts.front();
    }
    else
    {
        return std::string();
    }    
}

//add by wuxiang
int setnoblock(int fd)
{
    int flags = 0;
    flags = fcntl(fd, F_GETFL);
    flags = flags | O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}


#endif /* _NB_NET_TOOL_H_ */
