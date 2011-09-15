#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.09";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 8;
	static const long BUILD = 183;
	static const long REVISION = 1036;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 444;
	#define RC_FILEVERSION 0,8,183,1036
	#define RC_FILEVERSION_STRING "0, 8, 183, 1036\0"
	static const char FULLVERSION_STRING[] = "0.8.183.1036";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 7;
	

}
#endif //VERSION_H
