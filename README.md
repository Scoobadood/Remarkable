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

## Usage
Create 1404x1720 PNG file of the template
Then Convert to SVG, ensure that the files have the same name apart from the extension

e.g. template.svg and template.png

Run this app in the same directory as the images. You'll be presented with a menu like this:

Templates
---------
1. Install a template
2. Uninstall a template
3. Make a new category
X. exit
   
If (1) is selected
1. Template1
2. Template2
3. Template3 [installed]

Select (1)
What category?
1. Category 1
2. Category 2
3. Category 3
X. Back


