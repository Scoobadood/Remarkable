//
// Created by Dave Durbin (Old) on 9/2/21.
//

#ifndef REMARKABLE_SCP_H
#define REMARKABLE_SCP_H


#include <libssh/libssh.h>

#include <json/json.h>

Json::Value
get_templates_json_from_device(
        ssh_session session,
        const std::string & host, const std::string & user_id, const std::string & password );

#endif //REMARKABLE_SCP_H
