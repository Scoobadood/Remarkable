//
// Created by Dave Durbin (Old) on 9/2/21.
//

#include <iostream>
#include <libssh/libssh.h>
#include "scp.h"


int
connect(ssh_session session, const std::string& host, const std::string& user_id) {
    using namespace std;

    ssh_options_set(session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(session, SSH_OPTIONS_USER, user_id.c_str());

    int verbosity = SSH_LOG_WARN;
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        cerr << "Error connecting to " << host << ": " << ssh_get_error(session) << endl;
    }
    return rc;
}

int authenticate_with_password(ssh_session session, const std::string& password) {
    using namespace std;

    int rc = ssh_userauth_password(session, NULL, password.c_str());
    if (rc != SSH_AUTH_SUCCESS) {
        cerr << "Error authenticating with password: " <<ssh_get_error(session) << endl;
        ssh_disconnect(session);
        ssh_free(session);
    }
    return rc;
}

int
create_scp_session(ssh_session session, ssh_scp * scp_session_ptr ) {
    using namespace std;

    *scp_session_ptr = ssh_scp_new(session, SSH_SCP_READ, "/usr/share/remarkable/templates/templates.json");
    if (*scp_session_ptr == nullptr) {
        cerr << "Error allocating scp session: " << ssh_get_error(session) << endl;
        return SSH_ERROR;
    }

    int rc = ssh_scp_init(*scp_session_ptr);
    if (rc != SSH_OK) {
        cerr << "Error initializing scp session: " << ssh_get_error(session) << endl;
        ssh_scp_free(*scp_session_ptr);
        *scp_session_ptr = nullptr;
    }
    return rc;
}

int
read_templates_json(ssh_session session, ssh_scp scp, char **bufferPtr) {
    using namespace std;

    int rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE) {
        cerr << "Error receiving information about file: " << ssh_get_error(session) << endl;
        return SSH_ERROR;
    }

    int size = ssh_scp_request_get_size(scp);

    *bufferPtr = (char *) malloc(size);
    char * buffer = *bufferPtr;
    if (buffer == nullptr) {
        cerr << "Memory allocation error" << endl;
        return SSH_ERROR;
    }

    ssh_scp_accept_request(scp);
    int bytesRead = 0;
    while( bytesRead < size) {
        rc = ssh_scp_read(scp, buffer + bytesRead, size);
        if (rc == SSH_ERROR) {
            cerr << "Error receiving file data: " << ssh_get_error(session) << endl;
            free(buffer);
            *bufferPtr = nullptr;
            return rc;
        }
        bytesRead += rc;
    }

    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_EOF) {
        cerr << "Unexpected request: " << ssh_get_error(session) << endl;
        return SSH_ERROR;
    }

    return SSH_OK;
}


Json::Value
get_templates_json_from_device(
        ssh_session session,
        const std::string &host,
        const std::string &user_id,
        const std::string &password) {
    if (connect(session, host, user_id) != SSH_OK) {
        return Json::Value::null;
    }

    // Authenticate ourselves
    if( authenticate_with_password(session, password) != SSH_AUTH_SUCCESS) {
        return Json::Value::null;
    }

    ssh_scp scp;
    if( create_scp_session(session, &scp) ) {
        return Json::Value::null;
    }

    char *templatesData;
    auto templates = Json::Value::null;
    if( read_templates_json(session, scp, &templatesData) != SSH_OK) {
        if ( templatesData != nullptr) {
            free(templatesData);
        }
    } else {
        std::stringstream str;
        str << templatesData;
        free(templatesData);
        str >> templates;
    }

    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return templates["templates"];
}