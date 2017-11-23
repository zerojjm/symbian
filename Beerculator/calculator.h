#ifndef CALCULATOR_H
#define CALCULATOR_H


class Calculator
{
public:
    Calculator();

    static double calcBeer(double dl, double percent);
    static double calcEuroPerGram(double dl, double percent, double euros);
    static double calcPermille(double beers, double beginTime, double weight,
                               bool male, double &timeLeft,
                               double limitPermilles, double &timeLeftLimit);
    };

#endif // CALCULATOR_H
