#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.05";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 9;
	static const long BUILD = 184;
	static const long REVISION = 1040;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 450;
	#define RC_FILEVERSION 0,9,184,1040
	#define RC_FILEVERSION_STRING "0, 9, 184, 1040\0"
	static const char FULLVERSION_STRING[] = "0.9.184.1040";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 8;
	

}
#endif //VERSION_H
