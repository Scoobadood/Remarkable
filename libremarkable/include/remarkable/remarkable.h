//
// Created by Dave Durbin (Old) on 14/2/21.
//

#ifndef REMARKABLE_REMARKABLE_H
#define REMARKABLE_REMARKABLE_H

#include <vector>
#include <list>
#include <libssh/libssh.h>

#include "rm_template.h"

class remarkable {
    const std::string host;
    const std::string password;
    std::list<rm_template> installed_templates;
    ssh_session session;

    ssh_scp create_scp_session(const std::string &directory, int mode);
    bool remove_template_from_installed_list(const rm_template &tplate);
    bool add_template_to_installed_list(const rm_template & tplate);
    const char * serialise_templates_to_char_buffer( );

public:
    /**
     * Create an instance of the remarkable device with the gven host addess and password.
     */
    remarkable(const std::string & host, const std::string & password);

    /**
     * @return A vector of templates.
     */
    std::vector<rm_template> get_installed_templates();

    /**
     * Copy a template to the device by copying the SVG and PNG files and updating the
     * templates.json file.
     */
    bool install_template(const rm_template &tplate);

    /**
     * Delete a template from the device by deleting the files and
     * removing it from templates.json file.
     */
    bool uninstall_template(const rm_template &tplate);
};

#endif //REMARKABLE_REMARKABLE_H
