#include "driver.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main()
{
	Drone d;

	std::this_thread::sleep_for(std::chrono::seconds(2));
	d.launch();
	std::this_thread::sleep_for(std::chrono::seconds(2));

	d.stop();

	exit(1);
	return 0;
}
