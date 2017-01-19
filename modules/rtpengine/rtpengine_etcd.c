

#include <json-c/json.h>

#include "cetcd.h"
#include "rtpengine.h"
#include "rtpengine_etcd.h"

str rtpp_etcd_url = {NULL, 0};

// TO DO
// sudo apt-get install libyajl-dev  libcurl4-openssl-dev libjson-c-dev
// curl -L 'http://127.0.0.1:2379/v2/keys/config/kamailio/'
// curl -L -X PUT http://127.0.0.1:2379/v2/keys/config/kamailio/running/54.243.19.79 -d value="{ \"ip\":\"54.243.19.79\" }"
// kamctl fifo nh_show_rtpp all

int init_rtpproxy_etcd(void)
{
    LM_INFO("init_rtpproxy_etcd\n");
    return rtpp_load_etcd();
}

int rtpp_load_etcd(void)
{
//    struct rtpp_set *rtpp_list = NULL;

    LM_INFO("\n etcd connecting ... \n\n");

    rtpengine_delete_node_all();

    if ((rtpp_etcd_url.s == NULL) || (rtpp_etcd_url.len == 0)) {
        LM_ERR("etcd params failed\n");
        return -1;
    }

    cetcd_array addrs;
    cetcd_array_init(&addrs, 1);
    cetcd_array_append(&addrs, "http://127.0.0.1:2379");

    cetcd_client cli;
    cetcd_client_init(&cli, &addrs);

    cetcd_response *resp;
    resp = cetcd_lsdir(&cli, "/config/kamailio/running/", 0, 0);
    if(resp->err) {
        LM_ERR("etcd reading failed :%d, %s (%s)\n", resp->err->ecode, resp->err->message, resp->err->cause);
        return -1;
    }

//    cetcd_response_print(resp);
    cetcd_response_node *serv_node = 0;

    int i, count;
    if (resp->node->nodes) {
        count = cetcd_array_size(resp->node->nodes);
        for (i = 0; i < count; ++i) {
            serv_node = cetcd_array_get(resp->node->nodes, i);
            char const* value = serv_node->value;
            LM_INFO("rtpengine etcd config %s\n", value);
            struct json_object *jobj;
            char* str_ip = 0;
            jobj = json_tokener_parse(value);
            json_object* returnObj;
            json_object_object_get_ex(jobj, "ip", &returnObj);
            str_ip = (char *) json_object_get_string(returnObj);

            // TO DO add to etcd schema and port to etcd
            // udp:127.0.0.1:22222
            char url[80];
            memset(url,0,sizeof(url));
            strcat(url, "udp:");
            strcat(url, str_ip);
            strcat(url, ":22222");
            if(rtpengine_add_rtpengine_set(&url[0], 1, 0, 0) !=0)
                return -1;
        }
    }

    cetcd_response_release(resp);
    cetcd_client_destroy(&cli);

    return 0;
}