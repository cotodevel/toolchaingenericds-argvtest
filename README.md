![ToolchainGenericDS](img/TGDS-Logo.png)

NTR/TWL SDK: TGDS1.65

master: Development branch. Use TGDS1.65: branch for stable features.

This is the ToolchainGenericDS Woopsi argvtest project:

1.	Compile Toolchain:
To compile this project you will need to follow the steps at https://bitbucket.org/Coto88/toolchaingenericds :
Then simply extract the project somewhere.

2.	Compile this project: 
Open msys, through msys commands head to the directory your extracted this project.
Then write:
make clean <enter>
make <enter>

After compiling, run the example in NDS. 

Project Specific description:
A Woopsi UI + TGDS SDK template for UI control handling on the NintendoDS. 
Draw the pop up windows across the screen or scroll other windows to test touchscreen. 
Also plays WAV/AD-PCM (Intel) files from the directory!

Reads arguments (libnds ARGV format) if any, received from former loader. 
Button (Start): Shutdown NDS
Button (Select): GDBGDB Debugging

Usage:
	//Send TGDS/libnds ARGuments 
	char thisArgv[3][MAX_TGDSFILENAME_LENGTH];
	memset(thisArgv, 0, sizeof(thisArgv));
	strcpy(&thisArgv[0][0], "ToolchainGenericDS-multiboot.nds");				//Arg0:	Loader used
	strcpy(&thisArgv[1][0], "0:/directory/filename.txt");						//Arg1: NDS Binary loaded
	addARGV(2, (char*)&thisArgv);
	
	//Receive TGDS/libnds ARGuments:
	See main.c


____Remoteboot____
Also, it's recommended to use the remoteboot feature. It allows to send the current TGDS Project over wifi removing the necessity
to take out the SD card repeteadly and thus, causing it to wear out and to break the SD slot of your unit.

Usage:
- Make sure the wifi settings in the NintendoDS are properly set up, so you're already able to connect to internet from it.

- Get a copy of ToolchainGenericDS-multiboot: https://bitbucket.org/Coto88/ToolchainGenericDS-multiboot/get/TGDS1.65.zip
Follow the instructions there and get either the TWL or NTR version. Make sure you update the computer IP address used to build TGDS Projects, 
in the file: toolchaingenericds-multiboot-config.txt of said repository before moving it into SD card.

For example if you're running NTR mode (say, a DS Lite), you'll need ToolchainGenericDS-multiboot.nds, tgds_multiboot_payload_ntr.bin
and toolchaingenericds-multiboot-config.txt (update here, the computer's IP you use to build TGDS Projects) then move all of them to root SD card directory.

- Build the TGDS Project as you'd normally would, and run these commands from the shell.
<make clean>
<make>

- Then if you're on NTR mode:
<remoteboot ntr_mode computer_ip_address>

- Or if you're on TWL mode:
<remoteboot twl_mode computer_ip_address>

- And finally boot ToolchainGenericDS-multiboot, and press (X), wait a few seconds and TGDS Project should boot remotely.
  After that, everytime you want to remoteboot a TGDS Project, repeat the last 2 steps. ;-)



/release folder has the latest binary precompiled for your convenience.

Latest stable release:
https://bitbucket.org/Coto88/ToolchainGenericDS-argvtest/get/TGDS1.65.zip

Note:
Libnds compatibility is kept. The only thing required, is, if you send a single ARGV in the slot 1 (see above), you need to fill ARGV in slot0 as well. Otherwise ARGV won't be sent.
In libnds, this doesn't happen. But hey, we've got these awesome binaries supported anyway. ;-)


Note: UI Controls experimental.

Coto