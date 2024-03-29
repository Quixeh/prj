/** 
* Project Menu System
* 
* This function controls the menu used in the command window of the program. 
* All user interface needs to go through this menu. 
**/

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int menu(void* unused){
		
	// Title of program
		
	cout    << endl
		<< "      SSSSSSSSSSSSSSS LLLLLLLLLLL             MMMMMMMM               MMMMMMMM\n"
		<< "    SS:::::::::::::::SL:::::::::L             M:::::::M             M:::::::M\n"
		<< "   S:::::SSSSSS::::::SL:::::::::L             M::::::::M           M::::::::M\n"
		<< "   S:::::S     SSSSSS LL:::::::LL             M:::::::::M         M:::::::::M\n"
		<< "   S:::::S              L:::::L               M::::::::::M       M::::::::::M\n"
		<< "   S:::::S              L:::::L               M:::::::::::M     M:::::::::::M\n"
		<< "    S::::SSSS           L:::::L               M:::::::M::::M   M::::M:::::::M\n"
		<< "     SS::::::SSSSS      L:::::L               M::::::M M::::M M::::M M::::::M\n"
		<< "       SSS::::::::SS    L:::::L               M::::::M  M::::M::::M  M::::::M\n"
		<< "          SSSSSS::::S   L:::::L               M::::::M   M:::::::M   M::::::M\n"
		<< "               S:::::S  L:::::L               M::::::M    M:::::M    M::::::M\n"
		<< "               S:::::S  L:::::L         LLLLLLM::::::M     MMMMM     M::::::M\n"
		<< "   SSSSSSS     S:::::SLL:::::::LLLLLLLLL:::::LM::::::M               M::::::M\n"
		<< "   S::::::SSSSSS:::::SL::::::::::::::::::::::LM::::::M               M::::::M\n"
		<< "   S:::::::::::::::SS L::::::::::::::::::::::LM::::::M               M::::::M\n"
		<< "    SSSSSSSSSSSSSSS   LLLLLLLLLLLLLLLLLLLLLLLLMMMMMMMM               MMMMMMMM\n"
		<< endl
		<< "       Spatial Light Modulator Controller Software, Andrew Blackmore 2012.   \n";
	
	vector<string> argCommands; // Vector to hold commands from starting arguments. 
		
	// Process the command line arguments: 
		
	if (args.size() > 1){                                     // If we have any arguments
		for(int i=1; i < args.size(); i++){               // for each one
			string currentArgument = args[i];         // Process one at a time
			if (currentArgument.find("run") == 0){    // If its a run command
				if (verbose) cout << "Run Argument Found...\n";
				currentArgument.erase(0,5);      // remove run="
				int n = currentArgument.size(); 
				currentArgument.erase(n-1,n);    // remove final "
				
				// Add this command to our "to-do" list.
				argCommands.push_back(currentArgument); 
			} 
			else if (currentArgument.find("file") == 0){ // If its a file command
				if (verbose) cout << "File Argument Found...\n";
				currentArgument.erase(0,6);          // Remove the file=" from the start
				int n = currentArgument.size(); 
				currentArgument.erase(n-1,n);        // Remove the final "
				
				// This means that currentArgument now contains the path to the file. 
				ifstream inputFile(currentArgument.c_str()); // Open this file. 
				string inLine; 
				if (!inputFile.fail()) { // If we opened the file OK
					while (getline(inputFile, inLine)){ // for each line in the file
						argCommands.push_back(inLine); // Add to to-do list. 
					}
					inputFile.close(); // Close the file. 
				} 
				else { // If we couldn't open the file:
					cout << "Error: Unable to open file \"" << currentArgument.c_str() << "\"\n";
				}
				
			}
			else { // The command wasn't file= or run=
				cout << "Error: Unknown command line argument.\n";
			}
		}
	}
	
	while(!close){ // Main Menu Loop, runs until we close it. 
		
		string choice; // Input text from user. 
		char *cstr_choice, *currentCommand, *currentLine; 
		string ccStr, cwStr; // String versions of above
		
		if (argCommands.size() > 0){ // If we have arguments from earlier to perform:
			choice = argCommands[0]; // Make this the input
			argCommands.erase(argCommands.begin());	// Remove from "to-do" list. 
		} else {
			cout << "\n>> "; 
			getline(cin, choice); // Capture the input text. 
		}
				
		vector<string> commandLines;
		
		currentLine = new char [choice.size()+1];
		strcpy(currentLine, choice.c_str());
					
		char * lines;
		lines = strtok (currentLine,";");
		while (lines != NULL){
		    commandLines.push_back(lines);
		    lines = strtok (NULL, ";");
		}
		
		for (int i=0;i<commandLines.size();i++){
			currentCommand = new char [commandLines[i].size()+1];
			string tmp = commandLines[i];
				
			if (tmp[0] == ' '){
				tmp = commandLines[i].erase(0,1);
				strcpy(currentCommand, tmp.c_str());
			} else {
				strcpy(currentCommand, commandLines[i].c_str());
			}
						
			if (verbose) cout << "\nCurrent Command: " << currentCommand << endl;
				
			if (!strcmp(currentCommand,"exit")){
				close = true;
			} 
			else if (!strcmp(currentCommand,"display")){
				// Place single word commands in else-if statements like this.
				view.output();
			}
			else if (!strcmp(currentCommand,"invert")){
				view.invert();
			}
			else {  // At this point, the command probably includes more than one word.
				// The remainder is self-explanitory, but some somments will be interjected. 
				vector<string> commandList;
							
				char * commands;
				commands = strtok (currentCommand," ");
				while (commands != NULL){
				    commandList.push_back(commands);
				    commands = strtok (NULL, " ");
				}
			
				/* If I type a command, it corresponds to command list in the following way:
				* set output bitmap on 123456
				*  0    1     2     3   4 
				*/ 
			
				if (commandList[0] == "apply"){ 
					if (commandList.size() >= 2){
						if (commandList[1] == "xgrad"){
							view.applyXfn(1);
						} 
						else if (commandList[1] == "ygrad"){
							view.applyYfn(1);
						}
						else if (commandList[1] == "x2"){
							view.applyXfn(2);
						}
						else if (commandList[1] == "sinx"){
							// Atoi converts a string to a number. 
							view.applySinxfn(atoi(commandList[2].c_str()));
						}
						else if (commandList[1] == "sqx"){
							view.applySqxfn(atoi(commandList[2].c_str()));
						}
						else if (commandList[1] == "y2"){
							view.applyYfn(2);
						}
						else if (commandList[1] == "black"){
							view.randomise(0);
						}
						else if (commandList[1] == "white"){
							view.randomise(0);
							view.invert();
						}
						else if (commandList[1] == "random"){
							if (commandList.size() >= 3){ // If we're given a number, use it. 
								view.randomise(atoi(commandList[2].c_str()));
							} else {
								view.randomise((double) rand() / (RAND_MAX+1));
							}
						}
						else if (commandList[1] == "grid"){
							view.applyGrid();
						}
						else if (commandList[1] == "stripes"){
							if (commandList.size() >= 3){
								if (commandList[2] == "vertical"){
									view.applyXStripes();
								}
								else if (commandList[2] == "horizontal"){
									view.applyYStripes();
								} 
								else {
									cout <<  "Unknown Command: " << commandList[0] << " " <<commandList[1] << " " <<commandList[2] << "\n\n";
								}
							} else {
								cout << "Syntax Error: Apply requires more than one arg\n\n";
							}
						}
						else {
							cout << "Unknown Command: " << commandList[0] << " " <<commandList[1] << "\n\n";
						}	
					} else {
						cout << "Syntax Error: Apply requires more than one arg\n\n";
					}
				} 
				else if (commandList[0] == "set"){
					if (commandList.size() >= 2){
						if (commandList[1] == "size"){
							if (commandList.size() > 2){ // Resize the View!
								pxGrpSize = atoi(commandList[2].c_str());
								
								Xres = int(1920/pxGrpSize);
								Yres = int(1080/pxGrpSize);
								view.resize(int(pxGrpSize));
								if (showDisplay){
									SDL_SetWindowSize(wDisp, Xres, Yres);
									SDL_DestroyRenderer(rDisp);
									rDisp = SDL_CreateRenderer(wDisp, -1, 0);
								}
								
								view.output();
							} else {
								cout << "PxGroup Size: " << pxGrpSize << "\nXres: " << Xres << "\nYres: " << Yres << endl;
							}
						}
						else if (commandList[1] == "refresh"){
							if (commandList.size() > 2){
								pictureTime = atoi(commandList[2].c_str());
								seq.updateTiming();
							} else {
								cout << "Refresh Time (us): " << pictureTime << endl;
							}							
						}
						else if (commandList[1] == "illuminate"){
							if (commandList.size() > 2){
								illuminateTime = atoi(commandList[2].c_str());
								seq.updateTiming();
							} else {
								cout << "Illuminate Time (us): " << illuminateTime << endl;
							}							
						}
						else if (commandList[1] == "verbose"){
							if (commandList.size() >= 2){
								if (commandList[2] == "on"){
									verbose = true;
								} 
								else if (commandList[2] == "off"){
									verbose = false;
								}
								else {
									cout << "Unknown Setting: " << commandList[2] << " - Try on or off.\n\n";
								}
							}
							else {
								cout << "Syntax Error: Set Output requires more than one args\n\n";
							}
						}
						else if (commandList[1] == "output"){
							if (commandList.size() >= 3){
								if (commandList[2] == "bitmap"){
									if (commandList[3] == "on"){
										outputFullBmp = true;
									} 
									else if (commandList[3] == "off"){
										outputFullBmp = false;
									}
									else {
										cout << "Unknown Setting: " << commandList[3] << " - Try on or off.\n\n";
									}
								}
								else if (commandList[2] == "sbitmap"){
									if (commandList[3] == "on"){
										outputBmp = true;
									} 
									else if (commandList[3] == "off"){
										outputBmp = false;
									}
									else {
										cout << "Unknown Setting: " << commandList[3] << " - Try on or off.\n\n";
									}
								}
								else if (commandList[2] == "control"){
									if (commandList[3] == "on"){
										showCtrl = true;
									} 
									else if (commandList[3] == "off"){
										showCtrl = false;
									}
									else {
										cout << "Unknown Setting: " << commandList[3] << " - Try on or off.\n\n";
									}
								}
								else if (commandList[2] == "display"){
									if (commandList[3] == "on"){
										showDisplay = true;
									} 
									else if (commandList[3] == "off"){
										showDisplay = false;
									}
									else {
										cout << "Unknown Setting: " << commandList[3] << " - Try on or off.\n\n";
									}
								}
								else if (commandList[2] == "map"){
									if (commandList[3] == "on"){
										showFullSize = true;
									} 
									else if (commandList[3] == "off"){
										showFullSize = false;
									}
									else {
										cout << "Unknown Setting: " << commandList[3] << " - Try on or off.\n\n";
									}
								}
								else {
									cout << "Unknown Command: " << commandList[0] << " " <<commandList[1] << " " <<commandList[2] << "\n\n";
								}
							}
							else {
								cout << "Syntax Error: Set Output requires more than two args\n\n";
							}
						}
					} 
					else {
						cout << "Syntax Error: Set requires more than one arg\n\n";
					}
				}
				else if (commandList[0] == "load"){
					if (commandList.size() >= 2){
						view.loadBmpSpecific(commandList[1]);
						view.output();	
					}
					else {
						view.loadBmp();
						view.output();	
					}
				}
				else if (commandList[0] == "loadsc"){
				
					view.loadScaledBmp();
					view.output();	
					
				}
				else if (commandList[0] == "seq"){
					if (commandList.size() >= 2){
						if (commandList[1] == "load"){
							if (commandList.size() >= 3){
								seq.loadSpecific(commandList[2]);
							} else { 
								seq.load();
							}	
						} 
						else if (commandList[1] == "dynamic"){
							#ifdef useDMD
							if (commandList.size() >= 3){
								if (commandList.size() >= 4){
									if (commandList[2] == "swtrig"){
										cout << "Preparing Alp (Software Trigger Mode)\n";
										dmd.outputDynSeqSpecific(commandList[3],1);
									}
									else if (commandList[2] == "hwtrig"){
										cout << "Preparing Alp (Hardware Trigger Mode)\n";
										dmd.outputDynSeqSpecific(commandList[3],2);
									} 
									else {	
										cout << "Error - Please check your Syntax.\n";
									}
								} 
								else { 
									if (commandList[2] == "swtrig"){
										cout << "Preparing Alp (Software Trigger Mode)\n";
										dmd.outputDynSeq(1);	
									}
									else if (commandList[2] == "hwtrig"){
										cout << "Preparing Alp (Hardware Trigger Mode)\n";
										dmd.outputDynSeq(2);
									} 
									else {	
										dmd.outputDynSeqSpecific(commandList[2],0);
									}
								}					
									
							} else {
								dmd.outputDynSeq(0);
							}
							#endif
						}
						else if (commandList[1] == "display"){
							seq.play();
						}
						else if (commandList[1] == "clear"){
							seq.clear();
						}
						else if (commandList[1] == "play"){
							if (commandList.size() >= 3){
								if (commandList[2] == "swtrig"){
									cout << "Preparing Alp (Software Trigger Mode)\n";
									#ifdef useDMD
									dmd.outputSeq(seq,1);
									#endif		
								}
								else if (commandList[2] == "hwtrig"){
									cout << "Preparing Alp (Hardware Trigger Mode)\n";
									#ifdef useDMD
									dmd.outputSeq(seq,2);
									#endif		
								}
							}
							else {
								cout << "Outputting to Alp\n";
								#ifdef useDMD
								dmd.outputSeq(seq,0);
								#endif				
								break;	
							}
						}
					}
					else {
						cout << "Syntax Error: Seq requires more than one arg\n\n";	
					}
				}
				else if (commandList[0] == "play"){
					if (commandList.size() >= 2){
						if (commandList[1] == "swtrig"){
							cout << "Preparing Alp (Software Trigger Mode)\n";
							#ifdef useDMD
							dmd.outputView(view,1);
							#endif		
						}
						else if (commandList[1] == "hwtrig"){
							cout << "Preparing Alp (Hardware Trigger Mode)\n";
							#ifdef useDMD
							dmd.outputView(view,2);
							#endif		
						}
					}
					else {
						cout << "Outputting to Alp\n";
						#ifdef useDMD
						dmd.outputView(view,0);
						#endif				
						break;	
					}
				}
				else if (commandList[0] == "dmd"){
					if (commandList.size() >= 2){
						if (commandList[1] == "off"){
							#ifdef useDMD
							dmd.quit();
							#endif						
						}
						else if (commandList[1] == "on"){
							#ifdef useDMD
							dmd.init();
							#endif								
						}
						else if (commandList[1] == "stop"){
							#ifdef useDMD
							dmd.stop();
							#endif								
						}
						else if (commandList[1] == "info"){
							#ifdef useDMD
							dmd.fullInquire();
							#endif								
						}
					}
					else {
						cout << "Syntax Error: Set requires more than one arg\n\n";
					}
				}
				else if (commandList[0] == "help"){
					if (commandList.size() >= 2){
						if (commandList[1] == "apply"){
							cout << "\n\nApply Help\n\n"
							<< "apply black              - Apply a black screen\n"
							<< "apply white              - Apply a white screen\n"
							<< "apply random <chance>    - Apply a random potential\n"
							<< "apply xgrad              - Apply a horizontal Gradient\n"
							<< "apply ygrad              - Apply a vertical Gradient\n"
							<< "apply x2                 - Apply a horizontal polynomial\n"
							<< "apply y2                 - Apply a vertical polynomial\n"
							<< "apply grid               - Apply an alternating grid, determined by pxGrpSize\n"
							<< "apply stripes vertical   - Apply vertical stripes\n"
							<< "apply stripes horizontal - Apply horizontal stripes\n";
							 
						}
						else if (commandList[1] == "set"){
							cout << "\n\nSet Help\n\n"
							<< "set size <integer>           - Display current, or set new PxGrpSize\n"
							<< "set refresh <integer>        - Display current, or set new refresh time (us)\n"
							<< "set verbose (on/off)         - Turn verbose mode on or off\n"
							<< "set output bitmap (on/off)   - Turn bitmap output on or off\n"
							<< "set output sbitmap (on/off)  - Turn scaled bitmap output on or off\n"
							<< "set output control (on/off)  - Turn the control window on or off\n"
							<< "set output display (on/off)  - Turn the display window on or off\n"
							<< "set output map (on/off)      - Turn the pixel map window on or off\n";
						}
						else if (commandList[1] == "seq"){
							cout << "\n\nSequence Help\n\n"
							<< "seq load <Path>  - Load a directory of .bmp files into a sequence\n"
							<< "seq display      - Preview the sequence on the display window\n"
							<< "seq clear        - Clear the sequence\n"
							<< "seq play <Mode>  - Send the sequence to the ALP - Mode = 0: Play now (default),\n"
						        << "                   Mode = swtrig: software trigger, Mode = hwtrig: Hardware Trigger\n";
						}
						else if (commandList[1] == "play"){
							cout << "\n\nPlay Help\n\n"
							<< "play <Mode>     - Send the current view to the ALP - Mode = 0: Play now (default),\n"
						        << "                   Mode = swtrig: software trigger, Mode = hwtrig: Hardware Trigger\n";
						}
						else if (commandList[1] == "load"){
							cout << "\n\nLoad Help\n\n"
							<< "load <Path>     - Load a single 24bit .bmp files into a sequence - File Size is required"
						        << "                  to be 6,076 kB (No compression or encoding etc)\n";
						}
						else if (commandList[1] == "dmd"){
							cout << "\n\ndmd Help\n\n"
							<< "dmd (on/off)     - Turn the connection to the DMD on or off. When off, the DMD can be"
						        << "                   safely disconnected\n";
						}
					} 
					else {
						cout << "\n\nHelp Menu. Type help <command> for more details.\n() indicates required fields, <> indicates optional\n\n"
						<< "exit                   - Quit the program\n"
						<< "display                - Update the GUI\n"
						<< "invert                 - Swap black & white pixels\n"
						<< "play <mode>            - Send sequence/image to SLM\n" 
						<< "load <filename>        - Import an image or sequence\n" 
						<< "dmd (on/off)           - Turn DMD on or off.\n"
						<< "apply (function)       - Apply a function to the image\n"
						<< "seq (command)          - Send a command to a sequence of images\n"
						<< "set (option) <value>   - Change program settings\n\n";
					}					
				}
				else {
					cout << "Unknown Command: " << commandList[0] << "\n\n";
				}					
			}
		}
	}
}
// EOF
