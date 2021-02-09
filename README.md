# Project to allow easy installation of new templates onto reMarkable 2 devices.

Currently this is a WiP and does nothing prticularly useful. Future commits will make this more usable.

Right now it will only establish a connection to a device and recover the templates.json file, parse it and list the templates which are installed.

Next steps include:
* Adding the ability to push a PNG/SVG pair with a new template description
  * ~~Read files in a directory and get those with SVG and PNG extensions~~
  * Check those against the template file to see if they are already installed
  * SCP both files to the device
  * Prompt for a template name
  * Is it landscape?  
  * Update the templates.json file
  * push it back to the device with blank icon.  
* Generating the SVG directly from the PNG
* Ability to specify an icon file
* Maybe some sort of UI?