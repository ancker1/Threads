#include <thread>
#include <chrono>
#include <iostream>
#include <random>
#include <mutex>
using namespace std;

int primes[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47 };
mutex arr_mutex;

void printContent(vector<int> data)
{
	for (int i = 0; i < data.size(); i++)
	{
		cout << data[i] << " ";
	}
	cout << endl;
}

bool notPrime(int num)
{
	for (int i = 0; i < sizeof(primes)/sizeof(primes[0]); i++)
	{
		if (num == primes[i])
			return false;
	}
	return true;
}

bool carryon(vector<int> arr)
{
	for (int i = 0; i < arr.size(); i++)
	{
		if (notPrime(arr[i]))
			return true;
	}
	bool isPrimeInArray = false;
	for (int i = 0; i < sizeof(primes)/sizeof(primes[0]); i++)
	{
		for (int j = 0; j < arr.size(); j++)
		{
			if (primes[i] == arr[j])
				isPrimeInArray = true;
		}
		if (!isPrimeInArray)
			return true;
		isPrimeInArray = false;
	}
	return false;
}

void insertNum(vector<int>& arr)
{
	while (carryon(arr))
	{
		this_thread::sleep_for(std::chrono::milliseconds(100));
		arr_mutex.lock();
		arr.push_back(rand() % 50 + 1);
		cout << "Added random numbers: ";
		printContent(arr);
		arr_mutex.unlock();
		if (!carryon(arr))
			break;
	}
}
void removeNums(vector<int>& arr)
{
	while (carryon(arr))
	{
		this_thread::sleep_for(std::chrono::milliseconds(500));
		arr_mutex.lock();
		for (int i = 0; i < arr.size(); i++)
		{
			if (notPrime(arr[i]))
			{
				arr.erase(arr.begin() + i, arr.begin() + i + 1);
				--i;
			}
		}
		cout << "Erased non primes: ";
		printContent(arr);
		arr_mutex.unlock();
	}
}



int main()
{

	vector<int> arr;
	thread producer{ insertNum,ref(arr) };
	thread consumer{ removeNums,ref(arr) };

	producer.join();
	consumer.join();

	cout << "size of vector: " << arr.size() << endl;
	cout << "Final" << endl;
	printContent(arr);
	
	return 0;
}