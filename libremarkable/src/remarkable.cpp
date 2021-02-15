//
// Created by Dave Durbin (Old) on 14/2/21.
//

#include "remarkable.h"

#include <iostream>
#include <libssh/libssh.h>

/**
 * Create an instance of the remarkable device with the gven host addess and password.
 */
remarkable::remarkable(const std::string &host, const std::string &password)
: host{host}, password{password}
{
    using namespace std;

    session = ssh_new();
    if (session == nullptr) {
        throw runtime_error("Couldn't establish ssh connection");
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(session, SSH_OPTIONS_USER, "root");

    int verbosity = SSH_LOG_WARN;
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        stringstream ss;
        ss << "Error connecting to " << host << ": " << ssh_get_error(session) << endl;
        throw runtime_error(ss.str());
    }

    rc = ssh_userauth_password(session, nullptr, password.c_str());
    if (rc != SSH_AUTH_SUCCESS) {
        stringstream ss;
        ssh_disconnect(session);
        ss << "Error authenticating with password: " << ssh_get_error(session) << endl;
        throw runtime_error(ss.str());
    }
}

ssh_scp
remarkable::create_scp_session(const std::string & file_name) {
    using namespace std;

    ssh_scp scp_session_ptr = ssh_scp_new(session, SSH_SCP_READ, file_name.c_str());
    if (scp_session_ptr == nullptr) {
        cerr << "Error allocating scp session: " << ssh_get_error(session) << endl;
        scp_session_ptr = nullptr;
    }

    int rc = ssh_scp_init(scp_session_ptr);
    if (rc != SSH_OK) {
        cerr << "Error initializing scp session: " << ssh_get_error(session) << endl;
        ssh_scp_free(scp_session_ptr);
        scp_session_ptr = nullptr;
    }
    return scp_session_ptr;
}

/**
 * Copy a template to the device by copying the SVG and PNG files and updating the
 * templates.json file.
 */
void push_template_to_device(const rm_template &tplate){}

/**
 * Delete a template from the device by deleting the files and
 * removing it from templates.json file.
 */
void remove_template_from_device(const rm_template &tplate){}

std::string
static get_template_file_name( ) {
    static std::string TEMPLATE_FILE_NAME = "/usr/share/remarkable/templates/templates.json";
    return TEMPLATE_FILE_NAME;
}

/**
 *
 */
char *
remarkable::read_templates_json(ssh_scp scp){
    using namespace std;

    int rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE) {
        cerr << "Error receiving information about file: " << ssh_get_error(session) << endl;
        return nullptr;
    }

    int size = ssh_scp_request_get_size(scp);
    char * buffer = (char *) malloc(size);
    if (buffer == nullptr) {
        cerr << "Memory allocation error" << endl;
        return nullptr;
    }

    ssh_scp_accept_request(scp);
    int bytesRead = 0;
    while (bytesRead < size) {
        rc = ssh_scp_read(scp, buffer + bytesRead, size);
        if (rc == SSH_ERROR) {
            cerr << "Error receiving file data: " << ssh_get_error(session) << endl;
            free(buffer);
            return nullptr;
        }
        bytesRead += rc;
    }

    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_EOF) {
        cerr << "Unexpected request: " << ssh_get_error(session) << endl;
        free(buffer);
        return nullptr;
    }

    return buffer;
}

/**
 * @return A vector of templates.
 */
std::vector<rm_template>
remarkable::get_installed_temnplates() {
    using namespace std;

    ssh_scp scp;
    if( ( scp = create_scp_session(get_template_file_name()) ) == nullptr) {
        cerr << "SCP Session failed" << endl;
        return vector<rm_template>{};
    }

    char *templatesData = read_templates_json(scp);
    if(templatesData == nullptr) {
        cerr << "Failed to get templates file from device." << endl;
        return vector<rm_template>{};
    }
    ssh_scp_close(scp);
    ssh_scp_free(scp);

    std::stringstream str;
    str << templatesData;
    free(templatesData);
    auto templates = Json::Value::null;
    str >> templates;

    vector<rm_template> page_templates;
    for (const auto &t : templates["templates"]) {
        auto name = t["name"].asString();
        if (!rm_template::is_built_in(name)) {
            auto pt = rm_template::from_json(t);
            page_templates.push_back(pt);
        }
    }
    return page_templates;
}


void remarkable::push_template_to_device(const rm_template &tplate) {}

bool
remarkable::remove_template_from_device(const rm_template &tplate) {
    using namespace std;
    ssh_channel channel = ssh_channel_new(session);
    if (channel == nullptr) {
        cerr << "Couldn't create channel to device" << endl;
        return false;
    }

    int rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        cerr << "Couldn't open channel to device " << rc << endl;
        return false;
    }

    std::ostringstream stringStream;
    stringStream << "rm ";
    stringStream << tplate.get_name();
    stringStream << ".*";
    stringStream << " > /dev/null 2>&1";
    auto cmd = stringStream.str();

    rc = ssh_channel_request_exec(channel, cmd.c_str());
    if (rc != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return false;
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return true;
}
