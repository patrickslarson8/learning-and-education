#include <iostream>

class MyArray
{
private:
        int* data;
        int size;
public: 
        MyArray(int sz) { data = new int[sz]; size = sz; }
        // … any other methods you want/need …

        MyArray(MyArray& arrayObj) //Copy constructor to perform deep copy
        {
            std::cout << "Copy Constructor Called\n";

            //Creating new location in memory to store data
            size = arrayObj.getSize();
            data = new int[size];

            //Iterate through old object to update values in new object
            for (int i = 0; i < size; i++)
            {
                data[i] = arrayObj.getDataAtLocation(i);
            }
        }

        ~MyArray() //Destructor because constructor used new
        {
            std::cout << "Destructor called.\n";
            delete[] data;
        }

        //Getters
        const int getSize() { return size; }
        const int* getDataAddress() { return data; }
        const int getDataAtLocation(int location) 
        {
            if ((location > size - 1) || (location < 0))
            {
                std::cout << "Location requested is out of bounds.\n";
                return 0;
            }

            else
            {
                return data[location];
            }
        }

        //Setter
        void setArrayValue( int location, int value )
        {
            if ((location > size - 1) || (location < 0))
            {
                std::cout << "Out of bounds of array. Value not set.\n";
            }
            else
            {
                data[location] = value;
                std::cout << "Array location set.\n";
            }
        }
};

int main() //Simple program to show the constructors and destructors working
{
    {
        std::cout << "Create a new object and set values.\n";
        MyArray existingObj = { 3 };
        existingObj.setArrayValue(0, 1);
        existingObj.setArrayValue(1, 2);
        existingObj.setArrayValue(2, 3);

        std::cout << "Copy object to new object.\n";
        MyArray newObj = existingObj;

        std::cout << "First object location: " << existingObj.getDataAddress() << " of size: " << existingObj.getSize() << "\n";
        std::cout << "First object values: " << existingObj.getDataAtLocation(0) << 
            ", " << existingObj.getDataAtLocation(1) << ", " << existingObj.getDataAtLocation(2) << "\n";

        std::cout << "Second object location: " << newObj.getDataAddress() << " of size: " << newObj.getSize() << "\n";
        std::cout << "Second object values: " << newObj.getDataAtLocation(0) << 
            ", " << newObj.getDataAtLocation(1) << ", " << newObj.getDataAtLocation(2) << "\n";

        std::cout << "Clean up...\n";
    }
    std::cout << "Clean up complete, closing.\n";
}
