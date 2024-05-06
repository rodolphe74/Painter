#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <string>

class Chronometer
{
private:
	static std::chrono::steady_clock::time_point beginTime;
	static std::chrono::steady_clock::time_point endTime;
	static double countLap ;
	static double sumLap;
public:
	static int toFile;
	static void initLap();
	static void startLap();
	static void endLap(std::string desc);
	static void meanLap(std::string desc);
	static void write(std::string message);
};

