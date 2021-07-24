#ifndef SRC_ALILANG_ALILANG_VPN_CONFIG_H_
#define SRC_ALILANG_ALILANG_VPN_CONFIG_H_

namespace biz
{
namespace mj
{
namespace alilang
{
    //alilang.ipc
    #define SERVICE_HOST_NAME_ALILANG       "alilang_dingtalk_service_host"
    #define APP_NAME_ALILANG                "dingtalk"
    #define HANDSHAKE_SESSION_ID_ALILANG    "{6D65991B-A801-4194-8013-8B6D411BD4F3}"
    #define SERVER_PORT_START_ALILANG       56604

    #define SERVICE_NAME_ALILANG            "alilang"

    //vpn.logname
    #define LOGGER_NAME_ALILANG_VPN         "[alilang_vpn]"

    //vpn.apis
    //api:notify_status_change
    #define VPN_STATUS_CHANGED_NOTIFY       SERVICE_NAME_ALILANG##".vpn.notify_status_change"
    //vpn.status
    #define VPN_STATUS                      "vpn_status"
    #define VPN_CONNECTED_STATUS            "connected"
    #define VPN_DISCONNECTED_STATUS         "disconnected"
    //vpn.iconId
    #define VPN_CONNECTED_STATUS_ICON_ID    "e9d1"
    #define VPN_DISCONNECTED_STATUS_ICON_ID "e9d0"

    //api:xxx
    //xxxx

    //meeting.logname
    #define LOGGER_NAME_ALILANG_MEETING         "[alilang_meeting]"

    //meeting.apis
    //apis:xxx
}
}
}
#endif //SRC_ALILANG_ALILANG_VPN_CONFIG_H_