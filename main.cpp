#define LIBSSH_STATIC 1

#include <libssh/libssh.h>
#include <cstdio>      /* printf, scanf, NULL */
#include <cstdlib>
#include <iostream>
#include <strings.h>
#include "json/json.h"



int actually_read_templates(ssh_session session, ssh_scp scp, char **bufferPtr) {
    int rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_NEWFILE) {
        fprintf(stderr, "Error receiving information about file: %s\n", ssh_get_error(session));
        return SSH_ERROR;
    }

    int size = ssh_scp_request_get_size(scp);
    char *filename = strdup(ssh_scp_request_get_filename(scp));
    int mode = ssh_scp_request_get_permissions(scp);
    printf("Receiving file %s, size %d, permissions 0%o\n", filename, size, mode);
    free(filename);

    *bufferPtr = (char *) malloc(size);
    char * buffer = *bufferPtr;
    if (buffer == nullptr) {
        fprintf(stderr, "Memory allocation error\n");
        return SSH_ERROR;
    }

    ssh_scp_accept_request(scp);
    int bytesRead = 0;
    while( bytesRead < size) {
        rc = ssh_scp_read(scp, buffer + bytesRead, size);
        if (rc == SSH_ERROR) {
            fprintf(stderr, "Error receiving file data: %s\n", ssh_get_error(session));
            free(buffer);
            return rc;
        }
        printf("read %d\n", rc);
        bytesRead += rc;
    }
    printf("Done\n");
    rc = ssh_scp_pull_request(scp);
    if (rc != SSH_SCP_REQUEST_EOF) {
        fprintf(stderr, "Unexpected request: %s\n", ssh_get_error(session));
        return SSH_ERROR;
    }

    return SSH_OK;
}

int scp_read_templates(ssh_session session, char **bufferPtr) {
    ssh_scp scp;
    int rc;

    scp = ssh_scp_new(session, SSH_SCP_READ, "/usr/share/remarkable/templates/templates.json");
    if (scp == NULL) {
        fprintf(stderr, "Error allocating scp session: %s\n",
                ssh_get_error(session));
        return SSH_ERROR;
    }

    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error initializing scp session: %s\n",
                ssh_get_error(session));
        ssh_scp_free(scp);
        return rc;
    }

    actually_read_templates(session, scp, bufferPtr);

    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return SSH_OK;
}

int authenticate_password(ssh_session session, const char *password) {
    int rc = ssh_userauth_password(session, NULL, password);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with password: %s\n",
                ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return SSH_AUTH_ERROR;
    }
    return rc;
}

int connect(ssh_session session) {
    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.0.8");
    int verbosity = SSH_LOG_WARN;
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    const char *user = "root";
    ssh_options_set(session, SSH_OPTIONS_USER, user);

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to localhost: %s\n",
                ssh_get_error(session));
    }
    return rc;
}

int main(int argc, char *argv[]) {
    ssh_session session = ssh_new();
    if (session == nullptr) {
        return 1;
    }

    if(connect(session) != SSH_OK) {
        exit(-1);
    }

    // Authenticate ourselves
    const char *password = "wg8czspbIz";
    authenticate_password(session, password);

    char * templatesData;
    scp_read_templates(session, &templatesData);

    Json::Value root;
    std::stringstream str;
    str << templatesData;
    free(templatesData);
    str >> root;

    const auto templates = root["templates"];
    for(const auto& t : templates) {
        auto name = t.get("name", Json::Value::null).asString();
        std::cout << name << std::endl;
    }


    ssh_free(session);
    return 0;
}