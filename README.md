# NeoBMS_Master

This is the firmware of the NeoBMS Master. Watch out for the battery definition files. Bad things could happen if you misconfigure the BMS!


## Setup the Eclipse project
First you need to rename these battery definition files:

/inc/Battery_config_example.h

/src/Battery_config_example.c.bak

To:

/inc/Battery_config.h

/src/Battery_config.c

You can now start eclipse and edit these config files to suit your Battery und BMS configuration. Next you will need to clean the project (Project -> Clean) and rebuild the Index (Project -> C/C++ Index -> Rebuild). Otherwise the project will not compile or Eclipse will show erratic syntax errors.
