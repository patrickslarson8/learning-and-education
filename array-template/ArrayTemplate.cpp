#include <utility>
#include <memory>
#include <iostream>
#include <string>
#include "MyDate.h"

// Forward declare friend functions
template <typename> class Array;
//Unary
template <typename T> Array<T> operator-(const Array<T>&);
//Binary
template <typename T> Array<T> operator+(const Array<T>&, const Array<T>&);
template <typename T> Array<T> operator-(const Array<T>&, const Array<T>&);
template <typename T> Array<T> operator*(const Array<T>&, const Array<T>&);
template <typename T> Array<T> operator/(const Array<T>&, const Array<T>&);


template <typename T>
class Array
{
private:
	// Buffer to be used to ensure never going out of memory
	static const int AllocSize{ 1 };

	// function used to allocate more memory
	void resize(int newSize);

	// pointer to container for elements
	T* arrayElements{ nullptr };

	// Tracker for how much memory is being used
	int size{ 0 };

public:
	// Ctor creates default size that will grow as required
	Array();
	virtual ~Array();

	// Disallow assignment and pass-by value to make
	// memory management easier
	Array& operator=(const Array& rhs) = delete;
	Array(const Array& src) = delete;

	// Move constructor and move assignment operator
	Array(Array&& src) noexcept;
	Array& operator=(Array&& rhs) noexcept;

	// returns value at specific index
	const T& getElementAt(int x) const;

	// sets value at specific index
	void setElementAt(int x, const T& element);

	// adds element to end of array
	void append(const T& value);

	// returns num of element in array
	int getSize() const noexcept;

	// subscript operator
	T& operator[](int index);

	//Unary arithmetic
	friend Array<T> (::operator-<T>)(const Array<T>&);

	// Binary arthimetic
	// Addition: Will add arrays elementwise starting with both
	// arrays at index 0. If one array is longer, the extra
	// elements are left unchanged.
	friend Array<T> operator+ <>(const Array<T>&, const Array<T>&);

	// subtraction occurs similiar to addition
	friend Array<T> operator- <>(const Array<T>&, const Array<T>&);

	// multiplication occurs similiar to addition
	friend Array<T> operator* <>(const Array<T>&, const Array<T>&);

	// Division occurs similiar to addition. Care
	// must be taken not to divide by zero
	friend Array<T> operator/ <>(const Array<T>&, const Array<T>&);

	// Shorthand
	// += will perform element wise addition similiar to binary artithmetic
	Array<T>& operator+=( const Array<T>&);
	Array<T>& operator-=(const Array<T>&);
	Array<T>& operator*=(const Array<T>&);
	Array<T>& operator/=(const Array<T>&);

	// Equality operators
	bool operator==(const Array<T>&) const;
	bool operator!=(const Array<T>&) const;

	// Specialization for Dates
	Array(N::Date);
	Array<N::Date> operator+(const int);

	// Specialization for strings
	Array(std::string);
	// appends the input to each member of the array
	Array<std::string> operator+(const std::string&);

};

// Implementation
template <typename T> Array<T>::Array()
{
	size = AllocSize;
	arrayElements = new T[size]{};
}

template <typename T> Array<T>::~Array()
{
	delete[] arrayElements;
	arrayElements = nullptr;
}

template <typename T> Array<T>::Array(Array&& src) noexcept
	: arrayElements {std::exchange(src.arrayElements, nullptr)}
	, size {std::exchange(src.size, 0)}
{}

template <typename T> void Array<T>::resize(int newSize)
{
	// create new bigger array
	auto newArray{ std::make_unique<T[]>(newSize) };
	// New array is always larger
	for (int i{ 0 }; i < size; i++)
	{
		newArray[i] = std::move(arrayElements[i]);
	}

	// Delete old array and set new array
	delete[] arrayElements;
	size = newSize;
	arrayElements = newArray.release();
}

template <typename T> const T& Array<T>::getElementAt(int index) const
{
	if (index >= size) { std::cout << "Error: Index out of range." << std::endl; }
	return arrayElements[index];
}

template <typename T> void Array<T>::setElementAt(int index, const T& element)
{
	if (index >= size) { resize(size + AllocSize); }
	arrayElements[index] = element;
}

template <typename T> void Array<T>::append(const T& element)
{
	setElementAt(size, element);
}

template <typename T> int Array<T>::getSize() const noexcept
{
	return size;
}

// Operators
template <typename T> Array<T>& Array<T>::operator=(Array<T>&& rhs) noexcept
{
	if (this == &rhs) { return *this; }
	delete[] arrayElements;
	arrayElements = std::exchange(rhs.arrayElements, nullptr);
	size = std::exchange(rhs.size, 0);
	return *this;
}

template <typename T> T& Array<T>::operator[](int index)
{
	if (index >= size)
	{
		resize(index + AllocSize);
	}
	// allow for reverse indexing starting from back of array
	if (index < 0)
	{
		int uint = index * -1;
		if (uint >= size)
		{
			resize(uint + AllocSize);
		}
		return arrayElements[size + index];
	}
	return arrayElements[index];
}
// Friends
//unary
template <typename T> Array<T> operator-(const Array<T>& arrayIn)
{
	Array<T> temp{};
	for (int i{0}; i < arrayIn.getSize(); i++)
	{
		temp.setElementAt(i,(arrayIn.getElementAt(i) * -1));
	}
	return temp;
}

//Binary
template <typename T> Array<T> operator+(const Array<T>& lhs, const Array<T>& rhs)
{
	// If one array is smaller, only add up the indices that exist in both
	if (lhs.getSize() > rhs.getSize())
	{
		Array<T> temp{};
		for (int i{ 0 }; i < lhs.getSize(); i++)
		{
			if (i < rhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) + rhs.getElementAt(i));
			}
			else { temp.setElementAt(i, lhs.getElementAt(i)); }
		}
		return temp;
	}
	// if rhs is larger or both equal
	else
	{
		Array<T> temp{};
		for (int i{ 0 }; i < rhs.getSize(); i++)
		{
			if (i < lhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) + rhs.getElementAt(i));
			}
			else { temp.setElementAt(i, rhs.getElementAt(i)); }
		}
		return temp;
	}
}

template <typename T> Array<T> operator-(const Array<T>& lhs, const Array<T>& rhs)
{
	// If one array is smaller, only add up the indices that exist in both
	if (lhs.getSize() > rhs.getSize())
	{
		Array<T> temp{};
		for (int i{ 0 }; i < lhs.getSize(); i++)
		{
			if (i < rhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) + (rhs.getElementAt(i) * -1));
			}
			else { temp.setElementAt(i, lhs.getElementAt(i)); }
		}
		return temp;
	}
	// if rhs is larger or both equal
	else
	{
		Array<T> temp{};
		for (int i{ 0 }; i < rhs.getSize(); i++)
		{
			if (i < lhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) + (rhs.getElementAt(i) * -1));
			}
			else { temp.setElementAt(i, rhs.getElementAt(i)); }
		}
		return temp;
	}
}

template <typename T> Array<T> operator*(const Array<T>& lhs, const Array<T>& rhs)
{
	// If one array is smaller, only add up the indices that exist in both
	if (lhs.getSize() > rhs.getSize())
	{
		Array<T> temp{};
		for (int i{ 0 }; i < lhs.getSize(); i++)
		{
			if (i < rhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) * rhs.getElementAt(i));
			}
			else { temp.setElementAt(i, lhs.getElementAt(i)); }
		}
		return temp;
	}
	// if rhs is larger or both equal
	else
	{
		Array<T> temp{};
		for (int i{ 0 }; i < rhs.getSize(); i++)
		{
			if (i < lhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) * rhs.getElementAt(i));
			}
			else { temp.setElementAt(i, rhs.getElementAt(i)); }
		}
		return temp;
	}
}

template <typename T> Array<T> operator/(const Array<T>& lhs, const Array<T>& rhs)
{
	// If one array is smaller, only add up the indices that exist in both
	if (lhs.getSize() > rhs.getSize())
	{
		Array<T> temp{};
		for (int i{ 0 }; i < lhs.getSize(); i++)
		{
			if (i < rhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) / rhs.getElementAt(i));
			}
			else { temp.setElementAt(i, lhs.getElementAt(i)); }
		}
		return temp;
	}
	// if rhs is larger or both equal
	else
	{
		Array<T> temp{};
		for (int i{ 0 }; i < rhs.getSize(); i++)
		{
			if (i < lhs.getSize())
			{
				temp.setElementAt(i, lhs.getElementAt(i) / rhs.getElementAt(i));
			}
			else { temp.setElementAt(i, rhs.getElementAt(i)); }
		}
		return temp;
	}
}

// Shorthand
template <typename T> Array<T>& Array<T>::operator+=(const Array<T>& rhs)
{
	*this = *this + rhs;
	return *this;
}

template <typename T> Array<T>& Array<T>::operator-=(const Array<T>& rhs)
{
	*this = *this - rhs;
	return *this;
}

template <typename T> Array<T>& Array<T>::operator*=(const Array<T>& rhs)
{
	*this = *this * rhs;
	return *this;
}

template <typename T> Array<T>& Array<T>::operator/=(const Array<T>& rhs)
{
	*this = *this / rhs;
	return *this;
}

// Equality
template <typename T> bool Array<T>::operator==(const Array<T>& rhs) const
{
	if (this->getSize() != rhs.getSize()) { return false; }
	else
	{
		for (int i{}; i < this->getSize(); i++)
		{
			if (this->getElementAt(i) != rhs.getElementAt(i)) { return false; }
		}
	}
	return true;
}

template <typename T> bool Array<T>::operator!=(const Array<T>& rhs) const
{
	if (*this == rhs) { return false; }
	else { return true; }
}

// Date Specialization
template <> Array<N::Date>::Array(N::Date dateIn)
{
	size = AllocSize;
	arrayElements = new N::Date[size]{};
	arrayElements[0].set_day(dateIn.get_day());
	arrayElements[0].set_month(dateIn.get_month());
	arrayElements[0].set_year(dateIn.get_year());
}
// Adds an integer number of days
// increasing the month and year as required
template <> Array<N::Date> Array<N::Date>::operator+(const int daysToAdd)
{
	Array<N::Date> tempArray{};
	// loop through each date in the array
	for (int i{ 0 }; i < size; i++)
	{
		int day = arrayElements[i].get_day();
		int month = arrayElements[i].get_month();
		int year = arrayElements[i].get_year();
		// Iterate through number of days to properly increment months and years
		for (int j{ 1 }; j <= daysToAdd; j++)
		{
			day = day + 1;
			// Specific logic depending on the month so we know when to roll to the
			// next month. 31 day months
			if ((month == 1) || (month == 3) || (month == 5) ||
				(month == 7) || (month == 8) || (month == 10) || (month == 12))
			{
				// Increment month if greater than 31
				if (day > 31)
				{
					day = 1;
					month = month + 1;
					// Increment year if greater than 12
					if (month > 12)
					{
						month = 1;
						year = year + 1;
					}
				}
			}
			// 30 day months
			else if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
			{
				if (day > 30)
				{
					day = 1;
					month = month + 1;
					// Increment year if greater than 12
					if (month > 12)
					{
						month = 1;
						year = year + 1;
					}
				}
			}
			else if (month == 2)
			{
				if (arrayElements[i].get_leap_year())
				{
					if (day > 29)
					{
						day = 1;
						month = month + 1;
						// Increment year if greater than 12
						if (month > 12)
						{
							month = 1;
							year = year + 1;
						}
					}
				}
				else
				{
					if (day > 28)
					{
						day = 1;
						month = month + 1;
						// Increment year if greater than 12
						if (month > 12)
						{
							month = 1;
							year = year + 1;
						}
					}
				}
			}
		}

		tempArray[i].set_day(day);
		tempArray[i].set_month(month);
		tempArray[i].set_year(year);
	}
	return tempArray;
}

// String Specialization
template <> Array<std::string>::Array(std::string stringIn)
{
	size = AllocSize;
	arrayElements = new std::string[size]{};
	arrayElements[0] = stringIn;
}

template <> Array<std::string> Array<std::string>::operator+(const std::string& stringIn)
{
	Array<std::string> tempArray{};
	for (int i{ 0 }; i < size; i++)
	{
		tempArray[i] = arrayElements[i].append(stringIn);
	}
	return tempArray;
}

int main()
{
	// Allocate arrays to demonstrate template
	Array<int> intarray;
	Array<float> fltarray;
	Array<std::string> strarray;
	Array<N::Date> datearray;

	// Demonstrate integer usage
	// assignable and callable by subscript
	for (int i{}; i <= 12; i++)
	{
		intarray[i] = 2;
	}

	Array<int> intarray2;
	// Make the second array smaller to show how operators work
	for (int i{}; i < intarray.getSize()-1; i++)
	{
		intarray2[i] = intarray[i];
	}

	std::cout << "Integer arrays: ";
	for (int i{}; i < intarray.getSize(); i++)
	{
		std::cout<<intarray[i];
	}
	std::cout << " : ";
	for (int i{}; i < intarray2.getSize(); i++)
	{
		std::cout << intarray2[i];
	}

	//Demonstrate addition
	// Each shorthand arithmetic uses the binary operators
	std::cout << std::endl << "Addition: ";
	intarray2 += intarray;
	for (int i{}; i < intarray2.getSize(); i++)
	{
		std::cout << intarray2[i];
	}

	//Demonstrate subtraction
	std::cout << std::endl << "Subtraction: ";
	intarray2 -= intarray;
	for (int i{}; i < intarray2.getSize(); i++)
	{
		std::cout << intarray2[i];
	}

	//Demonstrate multiplication
	std::cout << std::endl << "Multiplication: ";
	intarray2 *= intarray;
	for (int i{}; i < intarray2.getSize(); i++)
	{
		std::cout << intarray2[i];
	}

	//Demonstrate division
	std::cout << std::endl << "Division: ";
	intarray2 /= intarray;
	for (int i{}; i < intarray2.getSize(); i++)
	{
		std::cout << intarray2[i];
	}
	
	//Demonstrate Inequality (which uses == operator)
	std::cout << std::endl << "Inequality: " << (intarray != intarray2) << std::endl;

	//Demonstrate floats
	for (int i{}; i < intarray.getSize(); i++)
	{
		fltarray[i] = (float)intarray[i] + 0.5;
	}
	std::cout << "Floats: ";
	for (int i{}; i < fltarray.getSize(); i++)
	{
		std::cout  << fltarray[i] << " ";
	}
	std::cout << std::endl;

	// Demonstrate Strings
	// First string stores hello world
	strarray[0] = "Hello";
	strarray[1] = ",";
	strarray[2] = " ";
	strarray[3] = "World";
	strarray[4] = "!";

	std::cout << "Strings: ";
	for (int i{}; i < strarray.getSize(); i++)
	{
		std::cout << strarray[i];
	}
	std::cout << " : ";

	// second string stores a little extra
	Array<std::string> strarray2;
	strarray2[0] = "a";
	strarray2[1] = "little";
	strarray2[2] = "extra";

	for (int i{}; i < strarray2.getSize(); i++)
	{
		std::cout << strarray2[i];
	}
	std::cout << std::endl;

	// demonstrate addition
	Array<std::string> strarray3 = strarray + strarray2;
	std::cout << "Addition: ";
	for (int i{}; i < strarray3.getSize(); i++)
	{
		std::cout << strarray3[i];
	}
	std::cout << std::endl;
	
	// Demonstrate dates
	N::Date myDate{ 9, 21, 1992 };
	N::Date myDate2{ 7, 25, 2022 };
	datearray[0] = myDate;
	datearray[1] = myDate2;

	std::cout << "Dates: " << 
		datearray[0].get_month() << datearray[0].get_day() << datearray[0].get_year()
		<< " : " <<
		datearray[1].get_month() << datearray[1].get_day() << datearray[1].get_year()
		<< std::endl;

	// Demonstrate addition
	datearray = datearray + 365;

	// show contents
	std::cout << "Addition: " <<
		datearray[0].get_month() << datearray[0].get_day() << datearray[0].get_year()
		<< " : " <<
		datearray[1].get_month() << datearray[1].get_day() << datearray[1].get_year()
		<< std::endl;
	std::cout << "1992 was a leap year so added date is one less day in the next year." << std::endl;
}