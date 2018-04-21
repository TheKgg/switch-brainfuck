#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

#include <switch.h>

int main(int argc, char **argv) {
	gfxInitDefault();
	consoleInit(NULL);

	u32 ptr=0;
	unsigned char all[30000] = {0};

	DIR* dir;
	struct dirent* ent;
	ent=NULL;
	dir = opendir("");
	printf("\n");//its ugly all the way at the top
	if(dir==NULL) {
		printf("Failed to open dir.\n");
	} else {	//Gets the first BF file	
		while ((ent = readdir(dir))) {
			char *dot = strrchr(ent->d_name, '.');
			if (dot && !strcmp(dot, ".bf")) {
				printf("Running %s...\n", ent->d_name);
				break;
			} 
			ent=NULL;
		}
		closedir(dir);
	}
	int size=190;
	char code[100000]= "-[------->+<]>-.-[->+++++<]>++.+++++++..+++.[--->+<]>-----.---[->+++<]>.-[--->+<]>---.+++.------.--------.-[--->+<]>.-.+[->++<]>+.---------.";
	if(ent!=NULL) {//reads the file
		FILE *f;
		f=fopen(ent->d_name,"rt");
		char c;
		memset(code, 0, sizeof(code)/sizeof(code[0]));
		fseek(f,0,SEEK_END);
		size =  ftell(f);
		printf("The file is %i chars long.\n\n", size);
		fseek(f, 0, SEEK_SET);
		for(int i=0; c!=EOF && i<size; i++) {
			c=(char)fgetc(f);
			code[i]=c;
		}

	} else {//Run a test BF
		printf("Please put a .bf in the same folder as this brew. Running hello world instead...\n");
	}

	printf("Code:\n\n%s\n\nOutput:\n", code);
	int place = 10 + (size/80);//allows the input not to mess up the screen
	int bracket=0;
	int i;
	for (i=0; i<sizeof(code); i++) {//does the thing
		switch(code[i]) {
			case '+'://Increase the pointer's value
			all[ptr]++;
			break;

			case '-'://Decrease the pointer's value
			all[ptr]--;
			break;

			case '.'://Print the char
			putchar(all[ptr]);
			break;

			case '>'://move pointer to the right (+1)
			if(ptr>30000){//30,000 is a conventional max on Brainfuck
				printf("\nPointer tried going above 30,000. (char %i)\n",i+1);
				i=sizeof(code);//breaks within switch
			}
			else
				ptr++;
			break;

			case '<'://move pointer to the left (-1)
			if(ptr<1) {
				printf("\nPointer tried going below 0. (char %i)\n",i+1);
				i=sizeof(code);//breaks within switch
			}
			else
				ptr--;
			break;


			case ','://idk do a keyboard or something idk
			printf("\x1b[%i;0HWaiting for input... use the joystick to select a number(then press A):\n", 19+(size/80));
			u32 number=0;
			int numBuf=0;
			JoystickPosition pos;
			while(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A) hidScanInput();//Incase it asks for input twice the A press from last time doesn't effect the new time
			
			while(!(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_A)) {
				hidScanInput();
				hidJoystickRead(&pos, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
				if(pos.dx>200) 
					numBuf++;
				else if(pos.dx<-200)
					numBuf--;
				if(numBuf>2) {
					numBuf=0;
					number++;
				} else if(numBuf<-2) {
					numBuf=0;
					number--;
				}
				printf("\x1b[%i;0H%i | ", 20+(size/80), number);
				putchar(number);
				gfxFlushBuffers();
				gfxSwapBuffers();
				gfxWaitForVsync();
			}
			printf("\x1b[%i;0H                                                                                                   ", (size/80)+20);
			printf("\x1b[%i;0H                                                                                                    ", (size/80)+19);
			printf("\x1b[%i;0H", place);
			all[ptr]=number;
			break;


			case '['://do that close/open math bro
			if(all[ptr]==0) {
				i++;
				while(bracket>0||code[i]!=']') {
					if(code[i]=='[')
						bracket++;
					else if(code[i]==']')
						bracket--;
					i++;
				}
			}
			break;


			case ']'://do that close/open math bro
			if(all[ptr]!=0) {
				i--;
				while(bracket>0||code[i]!='[') {
					if(code[i]==']')
						bracket++;
					else if(code[i]=='[')
						bracket--;
					i--;
				}
				i--;
			}
			break;
		}
	}
	if(bracket>0) {
		printf("Program ended with an unclosed bracket.");
	}


	// Main loop
	while(appletMainLoop()) {
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		// Your code goes here

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

		gfxFlushBuffers();
		gfxSwapBuffers();
		gfxWaitForVsync();
	}

	gfxExit();
	return 0;
}

