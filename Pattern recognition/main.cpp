#include <iostream>
#include <vector>
#include "tinyxml2.h"
#include <string>
#include<algorithm>
#include <cmath>

using namespace std;
using namespace tinyxml2;

class Class1
{
public:
    int obraz;
    vector<vector<float>> coordinate; //image coordinate
    vector<vector<float>> difference_coordinate; //the difference of coordinates of the new object and the class kernel
    vector<vector<float>> covariance_multiply; //(x-y)^T * (S+E)^(-1)
    vector<vector<float>> covariance_matrix; //the covariance matrix then as a result we have the form (S+E)^-1
    vector<vector<float>> kernel; //the class kernel
    float final;
    Class1() {};
    ~Class1() {};
};

void E_M(vector<vector<float>>& M) //сalculating (S+E)^-1
{
    double t;
    vector<vector<float>> E(M.size(), vector<float>(M.size(), 0.0));
    for (int i = 0; i < M.size(); i++) //determine the unit matrix and add it to the covariance matrix
        for (int j = 0; j < M.size(); j++)
        {
            if (i == j)
            {
                E[i][j] = 1.0;
            }
            M[i][j] += E[i][j];
        }

    for (int a = 0; a < M.size(); a++) //set the number of the main line
    {
        t = M[a][a];
        //all the elements of the a-th row of M matrix except the a-th row and up to it divide by the resolving element
        for (int b = a + 1; b < M.size(); b++)
        {
            M[a][b] = M[a][b] / t;
        }
        //all the elements of the a-th row of E matrix divide by the resolving element
        for (int b = 0; b < M.size(); b++) {
            E[a][b] = E[a][b] / t;
        }
        //element corresponding to the permissive - divide by itself to get 1
        M[a][a] /= t;

        if (a > 0)
        {
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < M.size(); j++)
                {
                    E[i][j] = E[i][j] - E[a][j] * M[i][a]; //calculate the elements of the E matrix going from the 0th to the last column
                }
                for (int j = M.size() - 1; j >= a; j--)
                {
                    M[i][j] = M[i][j] - M[a][j] * M[i][a]; //calculate the elements of the M matrix going from the last column to the a-th
                }
            }
        }
        for (int i = a + 1; i < M.size(); i++)
        {
            for (int j = 0; j < M.size(); j++)
            {
                E[i][j] = E[i][j] - E[a][j] * M[i][a];
            }
            for (int j = M.size() - 1; j >= a; j--)
            {
                M[i][j] = M[i][j] - M[a][j] * M[i][a];
            }
        }

    }
    //in place of the original matrix we should get the unit matrix and in place of the unit matrix we should get the inverse matrix.
    for (int i = 0; i < M.size(); i++)
    {
        for (int b = 0; b < M.size(); b++)
        {
            M[i][b] = E[i][b];
        }
    }
}

void print_line()
{
    for (int m = 0; m < 100; m++)
        cout << "_";
    cout << endl;
}

class Loader
{
public:
    vector<vector<vector<int>>> examples;
    vector<string> classes; //each element of the vector is the class id of the corresponding pattern
    vector<vector<vector<int>>> tasks;
    bool load_instance(const char* fileName)
    {
        XMLDocument doc;
        if (doc.LoadFile(fileName) != XMLError::XML_SUCCESS)
        {
            cout << "Error openning input XML file." << endl;
            return false;
        }

        XMLElement* root;
        root = doc.FirstChildElement("root");
        XMLElement* objects = root->FirstChildElement("examples");
        for (auto object = objects->FirstChildElement("object"); object; object = object->NextSiblingElement("object"))
        {
            vector<vector<int>> example;
            classes.push_back(object->Attribute("class"));
            for (auto row = object->FirstChildElement("row"); row; row = row->NextSiblingElement("row"))
            {
                vector<int> line;
                string values = row->GetText();
                for (char value : values)
                {
                    if (value == '1')
                        line.push_back(1);
                    else if (value == '0')
                        line.push_back(0);
                }
                example.push_back(line);
            }
            examples.push_back(example);
        }

        XMLElement* task = root->FirstChildElement("tasks");
        for (auto object = task->FirstChildElement(); object; object = object->NextSiblingElement("object"))
        {
            vector<vector<int>> example;
            for (auto row = object->FirstChildElement("row"); row; row = row->NextSiblingElement("row"))
            {
                vector<int> line;
                string values = row->GetText();
                for (char value : values)
                {
                    if (value == '1')
                        line.push_back(1);
                    else if (value == '0')
                        line.push_back(0);
                }
                example.push_back(line);
            }
            tasks.push_back(example);
        }
        return true;
    }

    void print_examples()
    {
        for (int i = 0; i < examples.size(); i++)
        {
            cout << "\nObject " << i << " class=" << classes[i] << "\n";
            for (int j = 0; j < examples[i].size(); j++) {
                for (int k = 0; k < examples[i][j].size(); k++) {
                    if (examples[i][j][k] == 0)
                        cout << "\x0B0";
                    else
                        cout << "\x0B2";
                }
                cout << "\n";
            }
        }
    }

    void print_tasks(int i, string class1)
    {
        cout << "\nTask " << i << " supposed class = " << class1 << "\n";
        for (int j = 0; j < tasks[i].size(); j++)
        {
            for (int k = 0; k < tasks[i][j].size(); k++)
                if (tasks[i][j][k] == 0)
                    cout << "\x0B0";
                else
                    cout << "\x0B2";
            cout << "\n";
        }
    }
};

int main(int argc, char* argv[]) //argc - argumутt counter, argv - argument values
{
    for (int i = 0; i < argc; i++)
        cout << argv[i] << "\n";
    if (argc < 2)
    {
        cout << "Name of the input XML file is not specified." << endl;
        return 1;
    }
    Loader loader;
    loader.load_instance(argv[1]);
    loader.print_examples();

    int clauses_amount = 4;
    int image_amount = 4;
    int task_image_amount = 4;

    int n = 10, m = 10;

    vector<vector<Class1>> area(clauses_amount, vector<Class1>(image_amount)); //a list of all images (the first key is the class, the second is the image)

    for (int i = 0; i < clauses_amount; i++) //set the matrix size and reset them to zero
    {
        for (int j = 0; j < image_amount; j++)
        {
            area[i][j].coordinate.resize(n);
            area[i][j].kernel.resize(n);
            area[i][j].covariance_matrix.resize(n * m);
            area[i][j].difference_coordinate.resize(n);
            area[i][j].covariance_multiply.resize(n);
            area[i][j].final = 0;

            for (int i1 = 0; i1 < n; i1++)
            {
                area[i][j].coordinate[i1].resize(m);
                area[i][j].kernel[i1].resize(m);
                area[i][j].difference_coordinate[i1].resize(m);
                area[i][j].covariance_multiply[i1].resize(m);

                for (int j1 = 0; j1 < m; j1++)
                {
                    area[i][j].coordinate[i1][j1] = 0;
                    area[i][j].kernel[i1][j1] = 0;
                    area[i][j].covariance_multiply[i1][j1] = 0;
                    area[i][j].difference_coordinate[i1][j1] = 0;
                }
            }
            for (int i1 = 0; i1 < n * m; i1++)
            {
                area[i][j].covariance_matrix[i1].resize(n * m);
                for (int j1 = 0; j1 < n * m; j1++)
                {
                    area[i][j].covariance_matrix[i1][j1] = 0;
                }
            }
        }
    }

    //read the images, fill in the coordinate matrices and output the images to the console
    for (int step = 0; step < clauses_amount; step++)
    {
        for (int obr = 0; obr < image_amount; obr++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    area[step][obr].coordinate[i][j] = loader.examples[step * clauses_amount + obr][i][j];
                }
            }
        }
    }

    for (int step = 0; step < clauses_amount; step++) //calculate the kernel classes (mathematical expectation)
    {
        cout << "Core " << step + 1 << ":\n";
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                for (int obr = 0; obr < image_amount; obr++)
                {
                    area[step][0].kernel[i][j] += area[step][obr].coordinate[i][j];
                }
                area[step][0].kernel[i][j] /= (n * m);
                cout << area[step][0].kernel[i][j] << "\t";
            }
            cout << endl;
        }
        print_line();
    }

    for (int step = 0; step < clauses_amount; step++) //calculating the covariance matrix
    {
        for (int i = 0; i < n * m; i++)
        {
            for (int j = 0; j < n * m; j++)
            {
                for (int obr = 0; obr < image_amount; obr++)
                {
                    area[step][0].covariance_matrix[i][j] += (area[step][obr].coordinate[i / n][i % n] - area[step][0].kernel[i / n][i % n]) * (area[step][0].coordinate[j / m][j % m] - area[step][0].kernel[j / m][j % m]);
                }
                area[step][0].covariance_matrix[i][j] /= (n * m - 1);
            }
        }

        E_M(area[step][0].covariance_matrix); //convert S into (S+E)^-1
    }

    for (int st = 0; st < task_image_amount; st++) //read the new images and calculate (x - y)^T
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                for (int step = 0; step < clauses_amount; step++)
                {
                    area[step][0].difference_coordinate[i][j] = loader.tasks[st][i][j] - area[step][0].kernel[i][j];
                }
            }
        }

        for (int step = 0; step < clauses_amount; step++)
        {
            for (int i = 0; i < area[step][0].covariance_multiply.size(); i++)
            {
                for (int j = 0; j < area[step][0].covariance_multiply[i].size(); j++)
                {
                    area[step][0].covariance_multiply[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < n * m; i++)  //calculating (x - y)^T * (S+E)^-1
        {
            for (int j = 0; j < n * m; j++)
            {
                for (int step = 0; step < clauses_amount; step++)
                {
                    area[step][0].covariance_multiply[i / m][i % m] += (area[step][0].difference_coordinate[j / m][j % m] * area[step][0].covariance_matrix[i][j]);
                }
            }
        }

        for (int i = 0; i < n; i++) //calculating (x - y)^T * (S+E)^-1 * (x - y)
        {
            for (int j = 0; j < m; j++)
            {
                for (int step = 0; step < clauses_amount; step++)
                {
                    area[step][0].final += (area[step][0].covariance_multiply[i][j] * area[step][0].difference_coordinate[i][j]);
                }
            }
        }

        vector<pair<float, int>> best;
        for (int step = 0; step < clauses_amount; step++)
        {
            area[step][0].final = sqrt(area[step][0].final);
            best.push_back(make_pair(area[step][0].final, step));
        }
        sort(best.begin(), best.end());

        loader.print_tasks(st, loader.classes[best[0].second * 4]);
        for (int step = 0; step < clauses_amount; step++)
        {
            cout << "Distance " << loader.classes[step * 4] << " : ";
            cout << area[step][0].final << endl;
        }
    }

    return 0;
}