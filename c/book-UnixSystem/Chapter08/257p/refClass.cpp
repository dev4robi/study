#include <iostream>
#include <string>
using namespace std;

class RefClass
{
private:
    string className;

public:
    void setName(string name)
    {
        className = name;
    }

    string getName()
    {
        return className;
    }
};

class UseRef
{
public:
    void useRefClass(RefClass *refClass)
    {
        refClass->setName("JSHIN");
        cout << refClass->getName() << endl;
    }
};

void main(void)
{
    RefClass refClass;
    UseRef useRef;
    useRef.useRefClass(&refClass);
}