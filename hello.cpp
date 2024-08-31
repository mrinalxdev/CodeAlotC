#include <iostream>

class HelloWorld {
    public :
        void printHello(){
            std::cout << "Hello, World !!" << std::endl;
        }
};

int main() {
    HelloWorld obj;
    obj.printHello();
    return 0;
}