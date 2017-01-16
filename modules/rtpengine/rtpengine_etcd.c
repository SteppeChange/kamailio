
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "../../dprint.h"

#include "etcd-api.h"
#include <json-c/json.h>
#include "rtpengine_etcd.h"


void test_etcd()
{
	LM_DBG("etcd .................... \n");
	// sudo apt-get install libyajl-dev  libcurl4-openssl-dev libjson-c-dev
	etcd_session     sess;
	char *value; 
	// curl -L 'http://127.0.0.1:2379/v2/keys/config/b2b/?recursive=true'
	// kamctl fifo nh_show_rtpp all
	sess = etcd_open_str("127.0.0.1:2379'");
	if (!sess)
	  {
	    LM_ERR("etcd reading failed\n");
	  }
	else
	  {

	    value = etcd_get(sess,"config//kamailio//running//54.243.19.78");
	    if (!value) {
	      LM_ERR("etcd_get failed\n");
	    }
	    else
	    {
	      LM_DBG("etcd val %s \n", value);
	      struct json_object *jobj;
	      char* str;
	      jobj = json_tokener_parse(value);
	      LM_DBG("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));


	      json_object* returnObj;
	      json_object_object_get_ex(jobj, "ip", &returnObj);
	      
	      str = (char *) json_object_get_string(returnObj);
	      LM_DBG("ip %s\n", str);
	      free(value);
	    }
	    
	    etcd_close_str(sess);
	  }
 }
