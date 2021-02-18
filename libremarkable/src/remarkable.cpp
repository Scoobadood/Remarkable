//
// Created by Dave Durbin (Old) on 14/2/21.
//

#include "remarkable.h"

#include <iostream>
#include <fstream>
#include <libssh/libssh.h>

/**
 * Create an instance of the remarkable device with the gven host addess and password.
 */
remarkable::remarkable(const std::string &host, const std::string &password)
        : host{host}, password{password} {
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
remarkable::create_scp_session(const std::string &directory, int mode) {
    using namespace std;

    ssh_scp scp_session_ptr = ssh_scp_new(session, mode, directory.c_str());
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

std::string
static get_template_directory_name() {
    static std::string TEMPLATE_DIR_NAME = "/usr/share/remarkable/templates";
    return TEMPLATE_DIR_NAME;
}

std::string
static get_template_file_name() {
    static std::string TEMPLATE_FILE_NAME = "/usr/share/remarkable/templates/templates.json";
    return TEMPLATE_FILE_NAME;
}

/**
 * Read a file from the device via scp and return an array of bytes.
 */
char *
read_file_from_device(ssh_session session, ssh_scp scp) {
    using namespace std;

    int rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE) {
        cerr << "Error receiving information about file: " << ssh_get_error(session) << endl;
        return nullptr;
    }

    int size = ssh_scp_request_get_size(scp);
    char *buffer = (char *) malloc(size);
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


const char *
remarkable::serialise_templates_to_char_buffer() {
    using namespace std;

    Json::Value root;
    for (const auto &tplate : installed_templates) {
        root.append(tplate.to_json());
    }

    stringstream ss;
    ss << root;
    const auto tplate_string = ss.str();
    const size_t len = tplate_string.length();
    char *serialised_templates = new char[len + 1];

    tplate_string.copy(serialised_templates, len + 1, 0);
    return serialised_templates;
}

bool
load_file(const std::string &file_name,
          char **file_data_ptr,
          size_t *file_length_ptr) {
    using namespace std;
    ifstream input{file_name, ios::in | ios::binary | ios::ate};
    if (!input.is_open()) {
        *file_length_ptr = 0;
        *file_data_ptr = nullptr;
        return false;
    }

    auto size = input.tellg();
    auto memblock = new char[size];
    input.seekg(0, ios::beg);
    input.read(memblock, size);
    input.close();
    *file_length_ptr = size;
    *file_data_ptr = memblock;
    return true;
}

bool
write_file_to_device(ssh_scp scp, const std::string &file_name, const char *data, size_t data_length) {
    using namespace std;

    int rc = ssh_scp_push_file(scp, file_name.c_str(), data_length, 0644);
    if (rc != SSH_OK) {

        cerr << "Can't create remote file: " << endl;
        return false;
    }
    rc = ssh_scp_write(scp, data, data_length);
    if (rc != SSH_OK) {
        cerr << "Can't write to remote file: " << endl;
        return false;
    }

    return true;
}

bool
copy_file_to_device(ssh_scp scp, const std::string & file_name) {
    using namespace std;
    char *file_data = nullptr;
    size_t file_length = 0;
    if (!load_file(file_name, &file_data, &file_length)) {
        cerr << "Couldn't read file " << file_name << endl;
        delete[] file_data;
        return false;
    }
    if (!write_file_to_device(scp, file_name, file_data, file_length)) {
        delete[] file_data;
        cerr << "Failed to install file " << file_name << endl;
        return false;
    }
    delete[] file_data;
    return true;
}

bool
remarkable::remove_template_from_installed_list(const rm_template &tplate) {
    using namespace std;

    // Check that it's actually there.
    const auto &found = find(begin(installed_templates), end(installed_templates), tplate.get_name());
    if (found == end(installed_templates)) {
        cerr << "Template " << tplate.get_name() << " is not installed." << endl;
        return false;
    }
    installed_templates.erase(found);
    return true;
}

bool
remarkable::add_template_to_installed_list(const rm_template & tplate){
    using namespace std;

    // Check that it's not there already
    const auto &found = find(begin(installed_templates), end(installed_templates), tplate.get_name());
    if (found != end(installed_templates)) {
        cerr << "Template " << tplate.get_name() << " is already installed." << endl;
        return false;
    }
    installed_templates.push_back(tplate);
    return true;
}


bool
execute_command_on_device_silently(ssh_session session, const std::string &cmd) {
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

/* *****
 * * User surfaced functions
 * *****/

/**
 * @return A vector of templates.
 */
std::vector<rm_template>
remarkable::get_installed_templates() {
    using namespace std;

    // Establish SCP connection for read
    ssh_scp scp;
    if ((scp = create_scp_session(get_template_file_name(), SSH_SCP_READ)) == nullptr) {
        cerr << "SCP Session failed" << endl;
        return vector<rm_template>{};
    }

    // Get the file data
    char *templatesData = read_file_from_device(session, scp);
    if (templatesData == nullptr) {
        cerr << "Failed to get templates file from device." << endl;
        return vector<rm_template>{};
    }
    ssh_scp_close(scp);
    ssh_scp_free(scp);

    // Turn it into a Json object
    std::stringstream str;
    str << templatesData;
    free(templatesData);
    auto templates = Json::Value::null;
    str >> templates;

    // Pipe that into local value objects
    vector<rm_template> page_templates;
    for (const auto &t : templates["templates"]) {
        page_templates.emplace_back(t);
    }
    return page_templates;
}


/**
 * Uninstall a template from the device by:
 * removing from the templates list
 * pushing the new templates.json
 * running `rm` remotely to delete the SVG and PNG files.
 */
bool
remarkable::uninstall_template(const rm_template &tplate) {
    using namespace std;

    if (tplate.is_built_in()) {
        cerr << "Can't delete built-in template " << tplate.get_name() << endl;
        return false;
    }

    if (!remove_template_from_installed_list(tplate)) {
        return false;
    }

    // Establish SCP connection for write
    ssh_scp scp;
    if ((scp = create_scp_session(get_template_directory_name(), SSH_SCP_WRITE)) == nullptr) {
        cerr << "SCP Session failed" << endl;
        return false;
    }

    const char *template_bytes = serialise_templates_to_char_buffer();

    bool wrote_ok = write_file_to_device(scp, "templates.json", template_bytes, strlen(template_bytes));
    delete[] template_bytes;
    ssh_scp_close(scp);
    ssh_scp_free(scp);

    if (!wrote_ok) {
        cerr << "Failed to update templates.json" << endl;
        return false;
    }

    // Now delete files from the devices
    std::ostringstream stringStream;
    stringStream << "rm ";
    stringStream << tplate.get_name();
    stringStream << ".*";
    stringStream << " > /dev/null 2>&1";
    auto cmd = stringStream.str();

    return (execute_command_on_device_silently(session, cmd));
}

/**
 * Copy a template to the device by copying the SVG and PNG files and updating the
 * templates.json file.
 */
bool
remarkable::install_template(const rm_template &tplate) {
    using namespace std;

    if (tplate.is_built_in()) {
        cerr << "Can't overwrite built-in template " << tplate.get_name() << endl;
        return false;
    }

    if (!add_template_to_installed_list(tplate)) {
        return false;
    }

    // Establish SCP connection for write
    ssh_scp scp;
    if ((scp = create_scp_session(get_template_directory_name(), SSH_SCP_WRITE)) == nullptr) {
        cerr << "SCP Session failed" << endl;
        return false;
    }


    // Copy SVG and PNG files
    const auto svg_file_name = tplate.get_name() + ".svg";
    if( !copy_file_to_device(scp, svg_file_name)) {
        cerr << "Failed to install SVG file" << endl;
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        return false;
    }

    const auto png_file_name = tplate.get_name() + ".png";
    if( !copy_file_to_device(scp, png_file_name)) {
        cerr << "Failed to install PNG file" << endl;
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        return false;
    }

    // Copy template file
    const char *template_bytes = serialise_templates_to_char_buffer();
    bool wrote_ok = write_file_to_device(scp, "templates.json", template_bytes, strlen(template_bytes));
    delete[] template_bytes;

    ssh_scp_close(scp);
    ssh_scp_free(scp);

    // Now copy template files to the device
    return wrote_ok;
}