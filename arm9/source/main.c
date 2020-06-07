/*

			Copyright (C) 2017  Coto
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
USA

*/

#include "main.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dswnifi_lib.h"
#include "keypadTGDS.h"
#include "TGDSLogoLZSSCompressed.h"
#include "fileBrowse.h"	//generic template functions from TGDS: maintain 1 source, whose changes are globally accepted by all TGDS Projects.
#include "biosTGDS.h"
#include "ipcfifoTGDSUser.h"
#include "dldi.h"
#include "global_settings.h"
#include "posixHandleTGDS.h"
#include "TGDSMemoryAllocator.h"
#include "consoleTGDS.h"
#include "soundTGDS.h"
#include "nds_cp15_misc.h"

char curChosenBrowseFile[MAX_TGDSFILENAME_LENGTH+1];

static inline void menuShow(){
	clrscr();
	printf("     ");
	printf("     ");
	printf("toolchaingenericds-argvtest: ");
	printf("(Start): Shutdown DS. >%d", TGDSPrintfColor_Red);
	printf("(Select): GDB Debugging. >%d", TGDSPrintfColor_Green);
	printf("Available heap memory: %d >%d", getMaxRam(), TGDSPrintfColor_Cyan);
	printarm7DebugBuffer();
}

int main(int argc, char argv[10][256]) {
	
	/*			TGDS 1.5 Standard ARM9 Init code start	*/
	bool isTGDSCustomConsole = false;	//set default console or custom console: default console
	GUI_init(isTGDSCustomConsole);
	GUI_clear();
	setTGDSMemoryAllocator(getProjectSpecificMemoryAllocatorSetup());
	sint32 fwlanguage = (sint32)getLanguage();
	
	printf("              ");
	printf("              ");
	#ifdef ARM7_DLDI
	setDLDIARM7Address((u32 *)TGDSDLDI_ARM7_ADDRESS);	//Required by ARM7DLDI!
	#endif
	int ret=FS_init();
	if (ret == 0)
	{
		printf("FS Init ok.");
	}
	else if(ret == -1)
	{
		printf("FS Init error.");
	}
	switch_dswnifi_mode(dswifi_idlemode);
	asm("mcr	p15, 0, r0, c7, c10, 4");
	flush_icache_all();
	flush_dcache_all();
	/*			TGDS 1.5 Standard ARM9 Init code end	*/
	
	/*
	printf("argc addr: %x", &__system_argv->argc);
	printf("argv addr: %x", &__system_argv->argv);
	
	
	*/
	
	
	
	//Show logo
	RenderTGDSLogoMainEngine((uint8*)&TGDSLogoLZSSCompressed[0], TGDSLogoLZSSCompressed_size);
	
	menuShow();
	
	printf("argc: %x", argc);
	printf("argv: %x", argv);
	
	//ARGV Test
	if (0 != argc ) {
		int i;
		for (i=0; i<argc; i++) {
			if (argv[i]) {
				printf("[%d] %s ", i, argv[i]);
			}
		}
		
	} else {
		printf("No arguments passed!");
	}
	
	static bool GDBEnabled = false;
	while(1) 
	{
		scanKeys();
		if (keysPressed() & KEY_START){
			
			menuShow();
			while(keysPressed() & KEY_START){
				scanKeys();
			}
		}
		
		if (keysPressed() & KEY_SELECT){
			
			if(GDBEnabled == false){
				GDBEnabled = true;
			}
			else{
				GDBEnabled = false;
			}
			
			while(keysPressed() & KEY_SELECT){
				scanKeys();
			}
		}
		
		//GDB Debug Start
		
		//GDB Stub Process must run here
		if(GDBEnabled == true){
			int retGDBVal = remoteStubMain();
			if(retGDBVal == remoteStubMainWIFINotConnected){
				if (switch_dswnifi_mode(dswifi_gdbstubmode) == true){
					clrscr();
					//Show IP and port here
					printf("    ");
					printf("    ");
					printf("[Connect to GDB]: NDSMemory Mode!");
					char IP[16];
					printf("Port:%d GDB IP:%s",remotePort, print_ip((uint32)Wifi_GetIP(), IP));
					remoteInit();
				}
				else{
					//GDB Client Reconnect:ERROR
				}
			}
			else if(retGDBVal == remoteStubMainWIFIConnectedGDBDisconnected){
				setWIFISetup(false);
				clrscr();
				printf("    ");
				printf("    ");
				printf("Remote GDB Client disconnected. ");
				printf("Press A to retry this GDB Session. ");
				printf("Press B to reboot NDS GDB Server ");
				
				int keys = 0;
				while(1){
					scanKeys();
					keys = keysPressed();
					if (keys&KEY_A){
						break;
					}
					if (keys&KEY_B){
						break;
					}
					IRQVBlankWait();
				}
				
				if (keys&KEY_B){
					main(0, (sint8**)"");
				}
				
				if (switch_dswnifi_mode(dswifi_gdbstubmode) == true){ // gdbNdsStart() called
					reconnectCount++;
					clrscr();
					//Show IP and port here
					printf("    ");
					printf("    ");
					printf("[Re-Connect to GDB]: NDSMemory Mode!");
					char IP[16];
					printf("Retries: %d",reconnectCount);
					printf("Port:%d GDB IP:%s", remotePort, print_ip((uint32)Wifi_GetIP(), IP));
					remoteInit();
				}
			}
		}
		//GDB Debug End
		
		handleARM9SVC();	/* Do not remove, handles TGDS services */
		IRQVBlankWait();
	}

	return 0;
}