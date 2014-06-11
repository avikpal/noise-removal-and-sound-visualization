#include "/usr/include/stk/FileRead.h"
#include "/usr/include/stk/FileLoop.h"
#include "/usr/include/stk/FileWvOut.h"

using namespace stk;

int main(){

	Stk::setSampleRate(44100.0);

	FileWvIn input;
	FileWvOut output;

	input.openFile("mic.wav",true);

	StkFrames frames(input.getSize(),2);
	

	std::cout<<input.getSize()<<'\n';
	output.openFile("hellowave.wav", 2, stk::FileWrite::FILE_WAV ,Stk::STK_SINT32);
	
	input.setRate(44100.0);

	output.tick(input.tick(frames));

	return 0;
}
