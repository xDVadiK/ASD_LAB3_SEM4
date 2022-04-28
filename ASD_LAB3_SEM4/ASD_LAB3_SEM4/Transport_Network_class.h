#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include "Map_class.h"
#include "Matrix_class.h"

// —lass transport network 
class Transport_Network
{
private:

    Matrix* MatrixAdjacency; // Pointer to the adjacency matrix storing all edges of the graph
    Matrix* MatrixParent; // Pointer to the parent matrix to restore the shortest path
    string* arrayOfVertices; // List of all vertices of the graph
    Red_Black_Tree<string, int>* Map; // Pointer to an associative array that stores all the vertices of the graph and their indexes

    // Filling in the adjacency matrix that stores the weights of all existing edges in the graph
    void fillingAdjacencyMatrix(Matrix& MatrixAdjacency)
    {
        for (int i = 0; i < MatrixAdjacency.getNumberVertices(); i++)
        {
            for (int j = 0; j < MatrixAdjacency.getNumberVertices(); j++)
            {
                if (MatrixAdjacency.getEdgeValue(i, j) == 0 && i != j)
                    MatrixAdjacency.addNewEdge(i, j, INT_MAX); // Assigning "infinity" in the absence of an edge between vertices
            }
        }
    };

    // Filling in the parent matrix
    void fillingParentMatrix(Matrix& MatrixParent)
    {
        for (int i = 0; i < MatrixParent.getNumberVertices(); i++)
        {
            for (int j = 0; j < MatrixParent.getNumberVertices(); j++)
            {
                MatrixParent.addNewEdge(i, j, i);
            }
        }
    };

    // Filling in the adjacency matrix from a file
    void fillMatrixFromFile(string nameFile)
    {
        string flightInformation; // A string that stores information about flight parameters
        string firstCity; // A string that stores information about the first city
        string secondCity; // A string that stores information about the second city
        string costFlight; // A string that stores information about the cost of the flight
        ifstream in(nameFile);
        if (in.is_open())
        {
            while (getline(in, flightInformation)) // Reading a line from a file
            {
                // Extracting information about the first city from a string read from a file
                firstCity = flightInformation.substr(0, flightInformation.find(';'));
                flightInformation.erase(0, flightInformation.find(';') + 1);
                if (!Map->find(firstCity)) Map->insert(firstCity, Map->get_size_tree());
                // Extracting information about the second city from the line read from the file
                secondCity = flightInformation.substr(0, flightInformation.find(';'));
                flightInformation.erase(0, flightInformation.find(';') + 1);
                if (!Map->find(secondCity)) Map->insert(secondCity, Map->get_size_tree());
            }
        }
        in.close();
        arrayOfVertices = new string[Map->get_size_tree()]; // Creating a list of all vertices of a graph of the desired size
        MatrixAdjacency = new Matrix(Map->get_size_tree()); // Creating an adjacency matrix of the desired size
        MatrixParent = new Matrix(Map->get_size_tree()); // Creating a parent matrix of the desired size
        Map->clear();
        in.open(nameFile);
        if (in.is_open())
        {
            while (getline(in, flightInformation))
            {
                // Extracting information about the first city from a string read from a file
                firstCity = flightInformation.substr(0, flightInformation.find(';'));
                flightInformation.erase(0, flightInformation.find(';') + 1);
                if (!Map->find(firstCity))
                {
                    arrayOfVertices[Map->get_size_tree()] = firstCity;
                    Map->insert(firstCity, Map->get_size_tree());
                }
                // Extracting information about the second city from the line read from the file
                secondCity = flightInformation.substr(0, flightInformation.find(';'));
                flightInformation.erase(0, flightInformation.find(';') + 1);
                if (!Map->find(secondCity))
                {
                    arrayOfVertices[Map->get_size_tree()] = secondCity;
                    Map->insert(secondCity, Map->get_size_tree());
                }
                // Extracting information about the cost of a direct flight
                costFlight = flightInformation.substr(0, flightInformation.find(';'));
                // Entry into the adjacency matrix of the cost of the flight
                if (costFlight != "N/A")
                {
                    MatrixAdjacency->addNewEdge(*Map->Get_map_element(firstCity)->value, *Map->Get_map_element(secondCity)->value, stoi(costFlight));
                }
                else
                {
                    // If there is no data on the return flight, we assign "infinity"
                    MatrixAdjacency->addNewEdge(*Map->Get_map_element(firstCity)->value, *Map->Get_map_element(secondCity)->value, INT_MAX);
                }
                flightInformation.erase(0, flightInformation.find(';') + 1);
                // Extracting information about the cost of the reverse
                costFlight = flightInformation.substr(0, flightInformation.find(';'));
                // Entry into the adjacency matrix of the cost of the flight
                if (costFlight != "N/A")
                {
                    MatrixAdjacency->addNewEdge(*Map->Get_map_element(secondCity)->value, *Map->Get_map_element(firstCity)->value, stoi(costFlight));
                }
                else
                {
                    // If there is no data on the return flight, we assign "infinity"
                    MatrixAdjacency->addNewEdge(*Map->Get_map_element(secondCity)->value, *Map->Get_map_element(firstCity)->value, INT_MAX);
                }
            }
            in.close();
            fillingAdjacencyMatrix(*MatrixAdjacency); // Filling in the adjacency matrix that stores the weights of all existing edges in the graph 
            fillingParentMatrix(*MatrixParent); // Filling in the parent matrix 
        }
    }

    // The Floyd-Warshell algorithm
    void algorithmFloydWarshell(Matrix& MatrixAdjacency, Matrix& MatrixParent)
    {
        for (int k = 0; k < Map->get_size_tree(); k++)
        {
            for (int i = 0; i < Map->get_size_tree(); i++)
            {
                for (int j = 0; j < Map->get_size_tree(); j++)
                {
                    if (MatrixAdjacency.getEdgeValue(i, k) < INT_MAX && MatrixAdjacency.getEdgeValue(k, j) < INT_MAX)
                    {
                        if (MatrixAdjacency.getEdgeValue(i, j) > (MatrixAdjacency.getEdgeValue(i, k) + MatrixAdjacency.getEdgeValue(k, j)))
                        {
                            MatrixAdjacency.addNewEdge(i, j, (MatrixAdjacency.getEdgeValue(i, k) + MatrixAdjacency.getEdgeValue(k, j)));
                            MatrixParent.addNewEdge(i, j, k); // Saving the way
                        }
                    }
                }
            }
        }
    }

public:

    // Constructor of the Floyd Warshell Algorithm class
    Transport_Network(string nameFile)
    {
        Map = new Red_Black_Tree<string, int>();
        fillMatrixFromFile(nameFile);
    };
    // The destructor of the Red Black Tree class
    ~Transport_Network()
    {
        MatrixAdjacency->clear();
        MatrixParent->clear();
        delete[] arrayOfVertices;
        Map->clear();
    };

    // Getting a list of all vertices of the graph (cities)
    string getListCities()
    {
        stringstream listCities;
        for (int i = 0; i < Map->get_size_tree(); i++)
        {
            listCities << i << ". " << arrayOfVertices[i] << "\n";
        }
        return listCities.str();
    }

    // Getting the adjacency matrix
    Matrix getMatrixAdjacency()
    {
        return *MatrixAdjacency;
    };

    // Getting the parent matrix
    Matrix getMatrixParent()
    {
        return *MatrixParent;
    };

    // Search for the path between vertices (cities) in the graph by their names
    string findWay(string firstCity, string secondCity)
    {
        if (!Map->find(firstCity) || !Map->find(secondCity)) 
        {
            throw domain_error("The name of the city that is not in the list has been entered");
        }
        else 
        {
            Matrix MatrixAdjacency = *this->MatrixAdjacency;
            Matrix MatrixParent = *this->MatrixParent;
            algorithmFloydWarshell(MatrixAdjacency, MatrixParent); // // The Floyd-Warshell algorithm
            int indexFirtsCity = *Map->Get_map_element(firstCity)->value; // Getting the index of the first vertex (city)
            int indexSecondCity = *Map->Get_map_element(secondCity)->value; // Getting the index of the second vertex (city)
            if (MatrixAdjacency.getEdgeValue(indexFirtsCity, indexSecondCity) == INT_MAX)
            {
                return "There is no information about the way between the specified cities";
            }
            else
            {
                string way = firstCity + " -> ";
                // Restoring the path using the parent matrix if necessary
                if (MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity) != indexFirtsCity)
                {
                    while (MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity) != indexFirtsCity)
                    {
                        way += arrayOfVertices[MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity)] + " -> ";
                        indexFirtsCity = MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity);
                    }
                }
                way += secondCity;
                return way;
            }
        }
    }

    // Search the way between vertices (cities) in the graph by their indexes
    string findWay(int indexFirtsCity, int indexSecondCity)
    {
        if ((indexFirtsCity < 0 || indexFirtsCity >= Map->get_size_tree()) || (indexSecondCity < 0 || indexSecondCity >= Map->get_size_tree()))
        {
            throw domain_error("An index has been entered that does not correspond to any city from the list");
        }
        else
        {
            Matrix MatrixAdjacency = *this->MatrixAdjacency;
            Matrix MatrixParent = *this->MatrixParent;
            algorithmFloydWarshell(MatrixAdjacency, MatrixParent); // The Floyd-Warshell algorithm
            if (MatrixAdjacency.getEdgeValue(indexFirtsCity, indexSecondCity) == INT_MAX)
            {
                return "There is no information about the way between the specified cities";
            }
            else
            {
                string way = arrayOfVertices[indexFirtsCity] + " -> ";
                // Restoring the path using the parent matrix if necessary
                if (MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity) != indexFirtsCity)
                {
                    while (MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity) != indexFirtsCity)
                    {
                        way += arrayOfVertices[MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity)] + " -> ";
                        indexFirtsCity = MatrixParent.getEdgeValue(indexFirtsCity, indexSecondCity);
                    }
                }
                way += arrayOfVertices[indexSecondCity];
                return way;
            }
        }
    }

    // Determining the effective cost of a flight between cities by their names
    int effective—ostOfFlight(string firstCity, string secondCity)
    {
        if (!Map->find(firstCity) || !Map->find(secondCity))
        {
            throw domain_error("The name of the city that is not in the list has been entered");
        }
        else
        {
            Matrix MatrixAdjacency = *this->MatrixAdjacency;
            Matrix MatrixParent = *this->MatrixParent;
            algorithmFloydWarshell(MatrixAdjacency, MatrixParent);
            int indexFirtsCity = *Map->Get_map_element(firstCity)->value;
            int indexSecondCity = *Map->Get_map_element(secondCity)->value;
            if (MatrixAdjacency.getEdgeValue(indexFirtsCity, indexSecondCity) == INT_MAX)
            {
                throw domain_error("There is no information about the way between the specified cities");
            }
            else
            {
                return MatrixAdjacency.getEdgeValue(indexFirtsCity, indexSecondCity);
            }
        }
    };

    // Determination of the effective cost of a flight between cities by their indexes
    int effective—ostOfFlight(int indexFirtsCity, int indexSecondCity)
    {
        if ((indexFirtsCity < 0 || indexFirtsCity >= Map->get_size_tree()) || (indexSecondCity < 0 || indexSecondCity >= Map->get_size_tree()))
        {
            throw domain_error("An index has been entered that does not correspond to any city from the list");
        }
        else
        {
            Matrix MatrixAdjacency = *this->MatrixAdjacency;
            Matrix MatrixParent = *this->MatrixParent;
            algorithmFloydWarshell(MatrixAdjacency, MatrixParent);
            if (MatrixAdjacency.getEdgeValue(indexFirtsCity, indexSecondCity) == INT_MAX)
            {
                throw domain_error("There is no information about the way between the specified cities");
            }
            else
            {
                return MatrixAdjacency.getEdgeValue(indexFirtsCity, indexSecondCity);
            }
        }
    };
};
