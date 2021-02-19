#define LIBSSH_STATIC 1

#include "main.h"
#include "main_menu.h"
#include "remarkable/file_utilities.h"
#include "remarkable/remarkable.h"

int main(int argc, char *argv[]) {
    using namespace std;

    const char *host = argv[1];
    const char *password = argv[2];

    remarkable device{host, password};
    auto candidates = find_eligible_templates_in_directory("/Users/dave/Documents/Writing");

    auto data = new app_data(device, candidates);
    main_menu(data);
    delete data;
}