#include <iostream>
#include <string>

#include <thread>
#include <mutex>
#include <chrono>

std::mutex m;
const std::chrono::milliseconds tick60(1000/60);
const std::chrono::milliseconds tick120(1000/120);
const std::chrono::milliseconds tick240(1000/240);
const std::chrono::milliseconds tick480(1000/480);

void test(int id) {
	while (!m.try_lock()) {
		std::this_thread::sleep_for(tick480);
		continue;
	}

	std::cout << "Starting thread: " << id << std::endl;
	m.unlock();
}

int main() {
	std::cout << "Starting Main-thread" << std::endl;

	std::cout << "Locking Main-thread..." << std::endl;
	m.lock();

	// not blocking main thread
	std::thread t4(test, 4);
	t4.detach();

	std::string s = "";
	while (s != "y" && s != "Y") {
		std::cout << "Main-thread locked..." << std::endl;
		std::cout << "Unlock (y/N):" << std::endl;
		std::getline(std::cin, s);
	}

	std::cout << "Unlocking Main-thread..." << std::endl;
	m.unlock();

	// blocks main thread
	std::thread t1(test, 1);
	std::thread t2(test, 2);
	std::thread t3(test, 3);
	t1.join();
	t2.join();
	t3.join();

	std::cout << "Sub-threads terminated." << std::endl;
	std::cout << "Main-thread stopped." << std::endl;

	std::cin.get();
	return 0;
}