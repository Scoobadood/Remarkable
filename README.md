# Project to allow easy installation of new templates onto reMarkable 2 devices.

Currently this is a WiP and does nothing prticularly useful. Future commits will make this more usable.

Right now it will only establish a connection to a device and recover the templates.json file, parse it and list the templates which are installed.

Next steps include:
* Adding the ability to push a PNG/SVG pair with a new template description
  * ~~Read files in a directory and get those with SVG and PNG extensions~~
  * ~~Check those against the template file to see if they are already installed~~
  * SCP both files to the device
  * Prompt for a template name
  * Is it landscape?  
  * Update the templates.json file
  * push it back to the device with blank icon.  
* Generating the SVG directly from the PNG
* Ability to specify an icon file
* Maybe some sort of UI?

## Usage
Create 1404x1720 PNG file of the template
Then Convert to SVG, ensure that the files have the same name apart from the extension

e.g. template.svg and template.png

Run this app in the same directory as the images. You'll be presented with a cli_menu.

Install templates lets you copy templates to the device. Chosing to install a template that's already installed will overwrite the current version.
You will be warned if your template clashes with a template on the device.
You will be prompted for an icon, category and whether this is a landscape template or not.

Delete template lets you remove a template *that you installed* to the device. You can't delete preinstalled templates

Change template lets you toggle whether a template is landscape or not and to edit the categories it belongs to.

