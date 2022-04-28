#pragma once
#include <iostream>
#include "Transport_Network_class.h"

using namespace std;

int main()
{
    Transport_Network FirstListOfFlights = Transport_Network("firstListOfFlights.txt");
    cout << "List of cities 1:\n";
    cout << FirstListOfFlights.getListCities();
    cout << "\n";
    cout << "The most cost-effective flight from London to Moscow:\n";
    cout << FirstListOfFlights.findWay("London", "Moscow");
    cout << "\nThe cost of the flight: " << FirstListOfFlights.effectiveÑostOfFlight("London", "Moscow") << "\n\n";
    cout << "The most cost-effective flight from Paris to Rome:\n";
    cout << FirstListOfFlights.findWay(2, 0);
    cout << "\nThe cost of the flight: " << FirstListOfFlights.effectiveÑostOfFlight(2, 0) << "\n\n";

    Transport_Network SecondListOfFlights = Transport_Network("secondListOfFlights.txt");
    cout << "List of cities 2:\n";
    cout << SecondListOfFlights.getListCities();
    cout << "\n";
    cout << "The most cost-effective flight from Vladivostok to Moscow:\n";
    cout << SecondListOfFlights.findWay("Vladivostok", "Moscow");
    cout << "\nThe cost of the flight: " << SecondListOfFlights.effectiveÑostOfFlight("Vladivostok", "Moscow") << "\n\n";
    cout << "The most cost-effective flight from Khabarovsk to Saint-Petersburg:\n";
    cout << SecondListOfFlights.findWay(2, 0);
    cout << "\nThe cost of the flight: " << SecondListOfFlights.effectiveÑostOfFlight(2, 0) << "\n\n";

    return 0;
}