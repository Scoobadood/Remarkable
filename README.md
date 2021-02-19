# Project to allow easy installation of new templates onto reMarkable 2 devices.

## History
Currently this is a WiP and does nothing particularly useful. Future commits will make this more usable.

Right now it will only establish a connection to a device and recover the templates.json file, parse it and list the templates which are installed.

Next steps include:
* Adding the ability to push a PNG/SVG pair with a new template description
  * ~~Read files in a directory and get those with SVG and PNG extensions~~
  * ~~Check those against the template file to see if they are already installed~~
  * ~~SCP both files to the device~~
  * ~~Prompt for a template name~~
  * Is it landscape?  
  * ~~Update the templates.json file~~
  * ~~push it back to the device with blank icon.~~  
* Generating the SVG directly from the PNG
* Ability to specify an icon file
* Maybe some sort of UI?

## Usage
### Create template
Create 1404x1720 PNG file of the template
Then Convert to SVG, ensure that the files have the same name apart from the extension

e.g. template.svg and template.png


### Launch app
Menu Options:

Connect > 
  Connect to the device. You will be prompted for an IP address and a Password.
These will be saved for use the next time. After connection you will see a split pane with
LHS The templates which are installed on the device
Black = Built in templates, can't be changed
Blue  = User installed templates, can be edited or removed.

icons indicting whether they are landscape or portrait and
(for blue) an edit icon and a delete icon

RHS:
A list of files 
Non template files are grey
Singular template files are red (ie svg withou png) 
Template files are clustered and blue with an install icon


You can:
Click an edit icon and a popup will appear allowing you to toggle landscape mode or rename a template.
Click on a delete icon and you will be prompted whether or not to delete the template

Click on an install icon and you will be prompted for a name and landscapiness


