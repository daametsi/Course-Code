// Greg Brandes, Steven Kneisler, and David Ametsitsi - Team1A
// Create printString, readString, readSector, and handleInterrupt21 functions
// printString
// -prints string to console
//
// readString
// -reads from keyboard
//
// readSector
// -reads from sector and prints to console	
//
// handleInterrupt21
// -handle the programmer customizable interrupt handler 21
// -calls printString, readString, readSector
//
// January 20th, 2014

 
#define AL 0xE

void printString(char*);			// Prototype for printString
void readString(char*);				// Prototype for readString
void readSector(char*, int);			// Prototype for readSector
int mod(int, int);				// Prototype for mod
int div(int, int);				// Prototype for div
void handleInterrupt21(int, int, int, int);	// Prototype for handleInterrupt21


int main(){
	char buffer[512];
	char input[80]; // Declare input string
	// PRINT STRING TEST
	printString("input data into the console using your keyboard, then press enter..\n\r\0");
	//readString(input);
	//printString(input);

	// READ SECTOR TEST
	//readSector(buffer, 30);
	//printString(buffer);
	
	// INTERRUPT 21 TEST
	makeInterrupt21();
	//interrupt(0x21, 0, 0, 0);
	
        // INTERRUPT 21 - print, read and readsector
	interrupt(0x21, 1, input, 0, 0);
	interrupt(0x21, 0, input, 0, 0);
	interrupt(0x21, 2, buffer, 0, 0);
                                 

	// program done, loop infinitely 
	while(1){
		//do nothing
	}
}

void printString(char* chars){
	int i = 0;
	int AX;
	while(chars[i] != '\0'){
		AX = AL*256 + chars[i];
		interrupt(0x10, AX, 0, 0, 0); // Print to cursor position
		i++; // Increment index
	}
}

void readString(char* string){
	int i = 0;
	char in; // declare input char

	do{
		in = interrupt(0x16, 0, 0, 0); // Capture keyboard input
		if(in == 0x8){
			if(i != 0)
				i--; // Decrement counter on backspace
		} else {
			string[i] = in; // Add to input string array
			i++; // Increment counter on keyboard input
		}
	}while(in != 0xD); // Loop until enter is pressed 

	string[i] = 0xA; // Append line feed char
	string[i+1] = 0x0; // Append end of string char
}

void readSector(char* buff, int sector){
	int Mode = 2; // 2 = Write mode
	int NumOfSec = 1; // Number of sectors to read
	int trackNum = div(sector,36); // = 0, when sector = 30
	int relSecNum = mod(sector,18) + 1; // = 13, when sector = 30
	int headNum = mod(div(sector,18),2); // = 0, when sector = 30
	int deviceNum = 0; // device number 0 for floppy
	interrupt(0x13, Mode*256 + NumOfSec, buff, trackNum*256 + relSecNum, headNum*256 + deviceNum);
}

int mod(int a,int b){
	while(a > b || a == b){
		a = a - b;
	}
	return a;
}

int div(int a, int b){
	int quot = 0;
	while((quot+1)*b < a || (quot+1)*b == a){
		quot++;
	}
	return quot;
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	// print string:
	// 	AX = 0
	// 	BX = address of the string to print
	// read string:
	// 	AX = 1
	// 	BX = address of the character array where the keys entered will go
	// read sector:
	// 	AX = 2
	// 	BX = address of the character array where the sector will go
	// 	CX = the sector number
	//
	if(ax == 0){
		printString(bx);
	} else if(ax == 1){
		readString(bx);
	} else if(ax == 2){
		readSector(bx,cx);	
	}
}
