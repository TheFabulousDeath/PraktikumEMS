#ifndef isensor_h
#define isensor_h

#include "Arduino_BHY2.h"
#include "Nicla_System.h"


class ISensor {
public:
	bool isCalibrated();
	bool hasLevelChange();
	int isOutside();

private:
	
};

class Activity : public ISensor {
	
};

class Magnetometer : public ISensor {
	
};

class Thermometer : public ISensor {
	
};

class AQI : public ISensor {
	
};

class Barometer : public ISensor {
	
};
#endif