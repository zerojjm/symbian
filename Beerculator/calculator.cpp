#include "calculator.h"

Calculator::Calculator()
{
}
double Calculator::calcBeer(double dl, double percent)
{
    //(dl*percent)/(3.3*4.6);
    return (dl*percent)/15.18;
}

double Calculator::calcEuroPerGram(double dl, double percent, double euros)
{
    if(dl <= 0.0 || percent <=0.0)
        return 0.0;

    //dl*100*(percent/100)*0.7894/euros
    return euros/(dl*percent*0.7894);

}

double Calculator::calcPermille(double beers, double beginTime, double weight,
                                       bool male, double &timeLeft,
                                       double limitPermilles, double &timeLeftLimit)
{
    if(weight<=0.0)
        return 0.0;

    double multiplier;
    if(male)
        multiplier = 0.75;
    else
        multiplier = 0.66;
    //grams = beers*3.3*4.6*0.7894 = beers*11.983092

    double grams = beers*11.983092;

    //calculate how much left after given hours (burns 1g/1h/10kg)
    grams = grams-beginTime*weight*0.1;

    double permillesNow;
    if(grams<0)
        permillesNow = 0.0;
    else
        permillesNow = grams/(multiplier*weight);

    //time left
    timeLeft = grams/(weight*0.1);

    //time left to given limit
    double gramsInLimit = limitPermilles*multiplier*weight;
    if(limitPermilles>=permillesNow)
        timeLeftLimit = 0.0;
    else
        timeLeftLimit = (grams-gramsInLimit)/(weight*0.1);

    return permillesNow;
}
