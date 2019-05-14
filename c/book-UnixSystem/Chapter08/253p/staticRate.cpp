#include <iostream>
using namespace std;

class UseCard
{
private:
    static float rate;

public:
    void setRate(float newRate)
    {
        UseCard::rate = newRate;
    }

    float getRate()
    {
        return UseCard::rate;
    }
};

float UseCard::rate = 0.0f;

int main(void)
{
    UseCard *aCard = new UseCard();
    UseCard *bCard = new UseCard();

    aCard->setRate(4.0f);
    cout << "<B> 객체의 요율: " << bCard->getRate() << endl;
    
    bCard->setRate(3.5f);
    cout << "<A> 객체의 요율: " << aCard->getRate() << endl;

    delete aCard, bCard;
    return 0;
}