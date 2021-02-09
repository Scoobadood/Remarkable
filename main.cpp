#define LIBSSH_STATIC 1

#include <iostream>
#include <libssh/libssh.h>
#include "json/json.h"
#include "scp.h"
#include "page_template.h"

int main(int argc, char *argv[]) {
    using namespace std;

    const char *host = argv[1];
    const char *password = argv[2];
    const char * user_id = "root";

    ssh_session session = ssh_new();
    if (session == nullptr) {
        cerr << "Failed to start SSH session" << endl;
        exit(-1);
    }

    const auto templates = get_templates_json_from_device(session, host, user_id, password);
    for (const auto &t : templates) {
        auto pt = page_template::from_json(t);
        auto name = t.get("name", Json::Value::null).asString();
        std::cout << pt.name << std::endl;
    }

    ssh_free(session);
}