#include "Chronometer.h"

std::chrono::steady_clock::time_point Chronometer::beginTime;
std::chrono::steady_clock::time_point Chronometer::endTime;
double Chronometer::countLap = 0;
double Chronometer::sumLap = 0;
int Chronometer::toFile = 1;

void Chronometer::initLap()
{
	countLap = 0;
	sumLap = 0;
}

void Chronometer::startLap()
{
	beginTime = std::chrono::steady_clock::now();
}


void Chronometer::endLap(std::string desc)
{
	endTime = std::chrono::steady_clock::now();
	sumLap += std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count();

	if (!toFile) {
		std::cout << "Time " << desc << " = " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count() << " units" << std::endl;
	}
	else {
		std::ofstream out("chrono.log", std::ios_base::app);
		out << "Time " << desc << " = " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime).count() << " units" << std::endl;
		//out.close();
	}
	countLap++;
}

void Chronometer::meanLap(std::string desc)
{
	if (!toFile) {
		std::cout << "Mean time " << desc << " = " << (sumLap / countLap) << " units" << std::endl;
	}
	else {
		std::ofstream out("chrono.log", std::ios_base::app);
		out << "Mean time " << desc << " = " << (sumLap / countLap) << " units" << std::endl;
		out.close();
	}
}

void Chronometer::write(std::string message)
{
	if (toFile == 1) {
		std::cout << message << std::endl;
	}
	else if (toFile == 2) {
		std::ofstream out("chrono.log", std::ios_base::app);
		out << message << std::endl;
		out.flush();
		out.close();
	}
}
