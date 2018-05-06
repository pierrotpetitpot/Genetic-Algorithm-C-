
#include <iostream>     // for cout etc.
#include <vector>     // for vector class
#include <string>     // for string class
#include <algorithm>    // for sort algorithm
#include <time.h>     // for random seed
#include <math.h>     // for abs()
#include <stdlib.h>                 //for rand
#include <cmath>                    //math functions
#include <algorithm>
#include <iomanip>                  //input/outut

using namespace std;
int counter = 0;            //generation counter
float best;                 //best fitness for 2nd app
float lowest_z = 0;         //best fitness for 1st app

//initialize the population

#define number_of_individuals 1000
#define maximum_number_of_iterations 20000
#define max_iteration

#define PI 3.14159265359

int c[5] = {1,2,5,2,3};     //matrices in the langermann function
int a[5] = {3,5,2,1,7};
int b[5] = {5,2,1,4,9};
// lets make the target to be x^3=y

float p[20][2]={{-10,-2615},{-9,-1876},{-8,-1291},{-7,-842},{-6,-511},{-5,-280},{-4,-131},{-3,-46},{-2,-7},{-1,4},{0,5},{1,14},{2,49},{3,128},{4,269},{5,490},{6,809},{7,1244},{8,1813},{9,2534}};
//chosen points

long double langermann(long double x, long double y)        //langermann function
{
    long double langermann =0;
    for(int i = 0; i < 5; i++)
    {
        long double variable = 0;
        variable = pow((x - a[i]),2)+pow((y - b[i]),2);         //devide to conquer
        long double e = exp(variable/PI);
        langermann = langermann + c[i] * cos(PI*variable)/e;
    }
    langermann = -langermann;
    return langermann;
}

double thePower()       //used in mutation formula
{
    double e = (rand() % 101)*0.01;
    return e;
}

int theSign()           //used in mutation formula
{
    int w = rand() % 2;
    if (w == 1)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

double tournamentComparing(double a, double b, double c) // compares three individuals
{
    
    double lowest;
    
    lowest = a;
    
    if(b<a)
        
    {lowest = b;}
    
    if(lowest > c)
        
    {lowest = c;}
    
    
    
    if(lowest == a)
        
    {return 0;}
    
    if(lowest == b)
        
    {return 1;}
    
    else{return 2;}
    
}

double alphaGenerator() //used in corss over formula
{
    double u = (rand() % 151 + (-25))*0.01;
    return u;
}

class individual //attributes of each individual
{
public:
    float array[4];         //array of 4 chaacteristics
    float fitness;
    void randomizer();
    void evaluateapp1();
    void evaluateapp2();
    friend bool termination_criteria(vector <individual>:: iterator);
};

vector<individual> population;
vector<individual> parent_pool;
vector<individual> offspring_pool;
vector<individual> joint_pool;
vector<individual> tournament;

bool cmd(const individual &first, const individual &second)
{
    return first.fitness < second.fitness;
}

void initialize_populationapp1()
{
    for (int i = 0; i < number_of_individuals; i++)
    {
        individual temp;
        temp.randomizer();
        temp.evaluateapp1();
        population.push_back(temp);
    }
}

void initialize_populationapp2()
{
    for (int i = 0; i < number_of_individuals; i++)
    {
        individual temp;
        temp.randomizer();
        temp.evaluateapp2();
        population.push_back(temp);
    }
}



void individual::randomizer()
{
    bool criteria = false;
    while(criteria != true)
    {
        for (int i = 0; i < 4; i++)
            array[i] = rand() % 101 *0.12345; // random decimal number between zero and 10 for each characteristic
        
        if(array[0] == 0) criteria = false;         //to avoid getting a 0
        else criteria = true;
    }
}


void individual::evaluateapp1() // fitness coordinates of the point for langermann
{
    long double z = langermann(array[0],array[1]);///////
    fitness = z;
}

void individual::evaluateapp2() // fitness coordinates of the point for MSE
{
    float y =0;
    float ybar=0;
    float sub = 0;
    float variable = 0;
    
    for(int i = 0; i < 20; i++) // since there are ten sets
    {
        y = array[0]*pow(p[i][0],3)+array[1]*pow(p[i][0],2)+array[2]*p[i][0]+array[3]; // should be expecting 0 for the first loop
        ybar = p[i][1];
        sub = ybar - y;
        variable = pow(sub,2) + variable;
    }
    variable = variable / 20;
    fitness = variable;
}

bool termination_criteria(vector<individual>::iterator it)      //termination criteria
{
    if(it->fitness == best && counter == 100) {return true;}
    if(it->fitness != best){counter = 0; return false;}
    else {counter++; return false;}
}

int main()
{
    srand(time(NULL));
    int input=0;
    while(input != 1 && input != 2)     //choice of application
    {
        cout << "Please enter a 1 or a 2 for the application number 1 or 2 respectively" << endl;
        cin >> input;
        if(input != 1 && input != 2)
        {
            cout << "You have not entered either a 1 or 2, please enter again" << endl;
        }
    }
    if(input == 2)
    {
        initialize_populationapp2();
        int randomIndex = 0;
        individual fittest;
        vector<individual>:: iterator it;
        int i = 0;
        do
        {
            if(i>0)
                //beginning of 2nd application
            {
                cout << "Generation " << i << endl << "Fittest individual " << fittest.fitness << endl;
                best = fittest.fitness;
            }
            for(int ab = 0; ab < number_of_individuals/10; ab++)
            {
                for (int i = 0; i < 3; i++)
                {
                    randomIndex = rand() % population.size();  //selecting random invidividuals from the population pool and putting them inside tournament
                    tournament.push_back(population[randomIndex]);
                }
                int z = tournamentComparing(tournament[0].fitness, tournament[1].fitness,tournament[2].fitness); //comparing individuals inside the tournament
                parent_pool.push_back(tournament[z]);   //inserting the fitness individual among the tournament
                
                tournament.clear(); //making new space to compare individuals
            }
            
            for(int ab = 0; ab < 10 * number_of_individuals; ab++) //making 10 times the number of individuals in population pool of offsprings
            {
                individual offSpring1;
                individual offSpring2;
                float a;
                //following the crossover formula
                randomIndex = rand() % parent_pool.size();
                int randomIndex1 = rand() % parent_pool.size();
                a = alphaGenerator();
                offSpring1.array[0] = parent_pool[randomIndex].array[0] + a*(parent_pool[randomIndex1].array[0] - parent_pool[randomIndex].array[0]);
                
                a = alphaGenerator();
                offSpring1.array[1] = parent_pool[randomIndex].array[1] + a*(parent_pool[randomIndex1].array[1] - parent_pool[randomIndex].array[1]);
                
                a = alphaGenerator();
                offSpring1.array[2] = parent_pool[randomIndex].array[2] + a*(parent_pool[randomIndex1].array[2] - parent_pool[randomIndex].array[2]);
                
                a = alphaGenerator();
                offSpring1.array[3] = parent_pool[randomIndex].array[3] + a*(parent_pool[randomIndex1].array[3] - parent_pool[randomIndex].array[3]);
                
                a = alphaGenerator();
                offSpring2.array[0] = parent_pool[randomIndex].array[0] + a*(parent_pool[randomIndex1].array[0] - parent_pool[randomIndex].array[0]);
                a = alphaGenerator();
                offSpring2.array[1] = parent_pool[randomIndex].array[1] + a*(parent_pool[randomIndex1].array[1] - parent_pool[randomIndex].array[1]);
                
                a = alphaGenerator();
                offSpring2.array[2] = parent_pool[randomIndex].array[2] + a*(parent_pool[randomIndex1].array[2] - parent_pool[randomIndex].array[2]);
                
                a = alphaGenerator();
                offSpring2.array[3] = parent_pool[randomIndex].array[3] + a*(parent_pool[randomIndex1].array[3] - parent_pool[randomIndex].array[3]);
                
                for (int j = 0; j < 2; j++)
                {
                    int thesign = theSign();
                    double thepower = thePower();
                    offSpring1.array[j] = offSpring1.array[j] + thesign * 0.1 * 10 * pow(2, -thepower * 10);
                }
                
                for (int k = 0; k < 2; k++)
                {
                    int thesign = theSign();
                    double thepower = thePower();
                    offSpring2.array[k] = offSpring2.array[k] + thesign * 0.1 * 10 * pow(2, -thepower * 10);
                }
                
                offSpring1.evaluateapp2();
                offSpring2.evaluateapp2();
                offspring_pool.push_back(offSpring1);
                offspring_pool.push_back(offSpring2);
            }
            //inserting parent_pool and offspring_pool togethr and sorting them after
            vector<individual> joint_pool;
            
            joint_pool.insert(joint_pool.end(),parent_pool.begin(),parent_pool.end());
            joint_pool.insert(joint_pool.end(),offspring_pool.begin(),offspring_pool.end());
            
            sort(joint_pool.begin(),joint_pool.end(),cmd);
            
            
            
            population.clear();
            
            population.insert(population.end(),joint_pool.begin(),(joint_pool.begin()+number_of_individuals));
            
            it = population.begin();
            fittest.fitness = it->fitness;
            i++;
            
        } while(termination_criteria(population.begin()) != true);
        
        cout << "The fittest individual is " << endl;
        
        it = population.begin();
        
        cout << it->fitness << endl;
        
        cout << "a " << it->array[0] << endl;
        cout << "b " << it->array[1] << endl;
        cout << "c " << it-> array[2] << endl;
        cout << "d " << it->array[3] << endl;
        
        cout << "Therefore the function is y = " << it->array[0] << "x^3 + " << it->array[1] << "x^2 + " << it->array[2] << "x + " << it->array[3] << endl;
    }
    //and of 2nd applicatin, beginning of 1st application
    
    if(input == 1)
    {
        initialize_populationapp1();
        
        sort(population.begin(),population.end(),cmd);
        
        vector<individual>:: iterator it;
        
        int gf = 0;
        
        int randomIndex = 0;
        
        individual fittest;
        
        while(termination_criteria(population.begin()) != true)
            
        {
            
            for(int ab = 0; ab < number_of_individuals/10; ab++)
                
            {
                
                for (int i = 0; i < 3; i++)
                    
                {
                    
                    randomIndex = rand() % population.size();
                    
                    tournament.push_back(population[randomIndex]);
                    
                }
                
                int z = tournamentComparing(tournament[0].fitness, tournament[1].fitness, tournament[2].fitness);
                
                parent_pool.push_back(tournament[z]);
                
                
                
                tournament.clear();
                
            }
            
            
            
            for(int ab = 0; ab <10 * number_of_individuals; ab++)
                
            {
                
                individual offSpring1;
                
                individual offSpring2;
                
                double a;
                
                
                
                randomIndex = rand() % parent_pool.size();
                
                int randomIndex1 = rand() % parent_pool.size();
                
                
                
                a = alphaGenerator();
                
                offSpring1.array[0] = parent_pool[randomIndex].array[0] + a*(parent_pool[randomIndex1].array[0] - parent_pool[randomIndex].array[0]);
                
                
                
                a = alphaGenerator();
                
                offSpring1.array[1] = parent_pool[randomIndex].array[1] + a*(parent_pool[randomIndex1].array[1] - parent_pool[randomIndex].array[1]);
                
                
                
                a = alphaGenerator();
                
                offSpring2.array[0] = parent_pool[randomIndex].array[0] + a*(parent_pool[randomIndex1].array[0] - parent_pool[randomIndex].array[0]);
                
                
                
                a = alphaGenerator();
                
                offSpring2.array[1] = parent_pool[randomIndex].array[1] + a*(parent_pool[randomIndex1].array[1] - parent_pool[randomIndex].array[1]);
                
                
                
                for (int j = 0; j < 2; j++)
                    
                {
                    
                    int thesign = theSign();
                    
                    double thepower = thePower();
                    
                    offSpring1.array[j] = offSpring1.array[j] + thesign * 0.1 * 10 * pow(2, -thepower * 10);
                    
                }
                
                
                
                for (int k = 0; k < 2; k++)
                    
                {
                    
                    int thesign = theSign();
                    
                    double thepower = thePower();
                    
                    offSpring2.array[k] = offSpring2.array[k] + thesign * 0.1 * 10 * pow(2, -thepower * 10);
                    
                }
                
                
                
                offSpring1.evaluateapp1();
                
                offSpring2.evaluateapp1();
                
                offspring_pool.push_back(offSpring1);
                
                offspring_pool.push_back(offSpring2);
                
            }
            
            
            
            joint_pool.insert(joint_pool.end(),parent_pool.begin(),parent_pool.end());
            
            joint_pool.insert(joint_pool.end(),offspring_pool.begin(),offspring_pool.end());
            
            
            
            sort(joint_pool.begin(),joint_pool.end(),cmd);
            
            
            
            population.clear();
            
            
            
            population.insert(population.end(),joint_pool.begin(),(joint_pool.begin()+number_of_individuals));
            
            
            
            it = population.begin();
            
            fittest.fitness = it->fitness;
            
            parent_pool.clear();
            offspring_pool.clear();
            
            
            gf++;
            
            if(gf >= 100)
                
            {
                
                it->fitness = 0;
                
            }
            
            cout << "Generation " << gf << endl;
            
            cout << "Best individual is " << fittest.fitness << endl;
            
            lowest_z = it->fitness;
        }
        it = population.begin();
        
        fittest.fitness = it->fitness;
        for(int i = 0; i < 2; i++)
        {   
            fittest.array[i] = it->array[i];   
        }
        cout << "The best individual has a fitness of " << fittest.fitness << endl;
        cout << "The x and y components are " << endl;
        for(int i = 0; i < 2; i++)   
        {
            cout << fittest.array[i] << setprecision(8) << endl;   
        }
    }
    //end of 1st application
}