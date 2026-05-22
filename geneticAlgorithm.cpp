#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>

#include "RandomGeneration.h"

using stringPair = std::pair<std::string,std::string> ;

std::string assessTarget( ) //Get the target
{
    std::string target { " " } ;

    std::cout << "Give a target string : " ;
    std::cin >> target ;

    return target;
}
 int determineTargetSize( const std::string& target )
{
    int size = static_cast<int>( target.size() ) ;

    return size ;
}
char generateGenome()
{
    return static_cast<char>( random::getInt( 32 , 127 ) ) ;
}
std::string generateUnitOfPopulation( int chromosomeSize )
{
    std::string unit {} ;

    for( int index = 0 ; index < chromosomeSize ; ++ index )
    {
        unit.push_back( generateGenome() ) ;
    }

    return unit ;
}
std::vector<std::string> generatePopulation( int populationSize , int chromosomeSize )
{
    std::vector<std::string> population { } ;

    for( int index = 0 ; index < populationSize ; ++ index )
    {
        population.push_back( generateUnitOfPopulation( chromosomeSize ) ) ;
    }

    return population ;
}
int evaluateUnitFitness( std::string unit , const std::string& target )
{
    int unitFitness { 0 } ;

    for( std::size_t index = 0 ; index < unit.size() ; ++ index )
        {
            if( unit[index] == target[index] )
                unitFitness += 1 ;
        }

    return unitFitness ;
}
std::vector<int> evaluatePopulationFitness( const std::vector<std::string>& population , const std::string& target )
{
    std::vector<int> populationFitness { } ;

    for( const std::string &unit : population )
    {
        populationFitness.push_back( evaluateUnitFitness( unit , target) )  ;
    }

    return populationFitness ;
}
template < typename T >
T sumVector( const std::vector<T>& vec)
{
    T sum { } ;

    for( T element : vec )
    {
        sum += element ;
    }

    return sum ;
}
template < typename T >
T vectorMaxElement( const std::vector<T>& vec )
{
    T maxElement { vec.at(0) } ;

    if( !vec.empty())
    {
        for( T unit : vec )
        {
            if( maxElement < unit )
                std::swap( maxElement , unit ) ;
        }
    }
    else
        return 0;

    return maxElement ;

}
std::vector<int> chooseParents( std::vector<int>& populationFitness )
{
    std::vector<int> fitToParent ( static_cast<int>( populationFitness.size() ) , 0 ) ;

    double superiorMatingProbability { 0.75 } ;
    double inferiorMatingProbability { 0.0 } ;

    int sum { } ;
    int max { vectorMaxElement( populationFitness ) } ;

    while( sum < 2 || sum % 2 != 0 ) // At least keep two parents to keep the generation going or keep an even to mate
    {
        for( int index = 0 ; index < std::ssize( populationFitness ) ; ++ index )
        {
            superiorMatingProbability += random::getReal( 0.0 , 0.25 ) ; // Higher boost for stronger parents to be choosen
            inferiorMatingProbability += random::getReal( 0.0 , 0.25 ) ; // Lower boost for weaker parents to be chosen

            if( populationFitness[index] <= max/2 && random::getReal( 0.0 , 1.0 ) <= inferiorMatingProbability )
                fitToParent[index] = 1 ;
            else if( populationFitness[index] > max/2 && random::getReal( 0.0 , 1.0 ) <= superiorMatingProbability )
                fitToParent[index] = 1 ;

            superiorMatingProbability = 0.75 ;
            inferiorMatingProbability = 0.0 ;
        }

        sum = sumVector(fitToParent)  ;
    }

    return fitToParent ;
}
std::vector<std::string> getParents( const std::vector<int>& fitToParent , const std::vector<std::string>& population )
{
    std::vector<std::string> parents { } ;

    for( std::size_t index { } ; index < std::ssize( population ) ; ++ index )
    {
        if( fitToParent[index] == true )
            parents.push_back( population[index] );
    }

    return parents ;
}
void mutateGenomeOf( std::string& child )
{
    for( char& genome : child )
    {
        double mutationProbability { random::getReal( 0.01 , 0.05 ) } ;
        double chanceToGetMutated { random::getReal( 0.0 , 1.0 ) } ;

        if( chanceToGetMutated <= mutationProbability )
            genome = static_cast<char>( random::getInt( 32 , 127 ) ) ;
    }

}
stringPair doublePointCrossover( const std::string& parent1 , const std::string& parent2 )
{
    stringPair offsprings {} ;

    int chromosomeSize = std::ssize(parent1) ; //Determine chromosome size

    std::string child1 ( chromosomeSize , '0' ) ;
    std::string child2 ( chromosomeSize , '0' ) ;

    int randomCrossoverPoint1 { random::getInt( 1 , chromosomeSize/2  ) } ; //Always guarantee the second point is after the first
    int randomCrossoverPoint2 { random::getInt( randomCrossoverPoint1 ,chromosomeSize - 1 ) } ; //Guarantee the last genome is always available

    //Make the kids
    for (int index = 0; index < chromosomeSize ; ++ index )
    {
        if( index < randomCrossoverPoint1 || index > randomCrossoverPoint2 )
        {
            child1[index] = parent1[index] ;
            child2[index] = parent2[index] ;
        }
        else if( index > randomCrossoverPoint1 && index < randomCrossoverPoint2 )
        {
            child1[index] = parent2[index] ;
            child2[index] = parent1[index] ;
        }
    }

    //Give an equal chance to both parents to transfer the crossover point to their kids

    double fairProbability { 0.5 } ; //For both parents

    //If they win the coin flip , parent 1 gives the crossover point genome at the specified position

    for( int index { } ; index < 2 ; ++ index )
    {
        double randomProbability { random::getReal( 0.0 , 1.0 ) } ;

        if( fairProbability <= randomProbability )
        {
            child1[randomCrossoverPoint1] = parent1[randomCrossoverPoint1] ;
            child1[randomCrossoverPoint2] = parent2[randomCrossoverPoint2] ;
            child2[randomCrossoverPoint1] = parent1[randomCrossoverPoint1] ;
            child2[randomCrossoverPoint2] = parent2[randomCrossoverPoint2] ;
        }
        else
        {
            child1[randomCrossoverPoint1] = parent2[randomCrossoverPoint1] ;
            child1[randomCrossoverPoint2] = parent1[randomCrossoverPoint2] ;
            child2[randomCrossoverPoint1] = parent2[randomCrossoverPoint1] ;
            child2[randomCrossoverPoint2] = parent1[randomCrossoverPoint2] ;
        }
    }

    mutateGenomeOf( child1 ) ;
    mutateGenomeOf( child2 ) ;

    offsprings = { child1 , child2 } ;

    return offsprings ;
}
std::vector< stringPair > getOffsprings( std::vector<std::string>& parents )
{
    std::vector< stringPair > offsprings { } ;

    std::shuffle( parents.begin() , parents.end() , random::mt );

    for( int index { } ; index < std::ssize(parents) ; index += 2 )
    {
        offsprings.push_back( doublePointCrossover( parents[index] , parents[index + 1] )) ;
    }

    return offsprings ;
}
void injectChildrenInPopulation( std::vector<std::string>& population ,const std::vector<stringPair>& offsprings )
{
    for( const stringPair& children : offsprings )
    {
        population.push_back( children.first ) ;
        population.push_back( children.second ) ;
    }
}
int main()
{
    const std::string target { assessTarget() } ;
    const int chromosomeSize {  determineTargetSize( target )  } ;
    std::vector<std::string> population { generatePopulation( 10000 , chromosomeSize ) } ;
    std::vector<int> populationFitness { evaluatePopulationFitness( population , target ) } ;
    std::vector<int> parents { chooseParents( populationFitness ) } ;
    std::vector<std::string> choosenParents { getParents( parents , population) } ;
    std::vector<stringPair> kids { getOffsprings( choosenParents ) } ;

    int generation { } ;

    bool targetFound{ false } ;

    while( generation <= 1000000)
    {
        populationFitness = evaluatePopulationFitness( population , target ) ;
        parents = chooseParents( populationFitness ) ;
        choosenParents = getParents( parents , population ) ;
        kids = getOffsprings( choosenParents ) ;
        injectChildrenInPopulation( population , kids ) ;

        auto sortByFitness = [&]( const std::string& a, const std::string& b ) //Sort the population from best to worst
        {
            return evaluateUnitFitness( a, target ) > evaluateUnitFitness( b, target ) ;
        };

        std::sort( population.begin(), population.end(), sortByFitness ) ;
        population.resize( 100 ) ;

        for( const std::string& unit : population )
        {
            if( unit == target)
            {
                targetFound = true ;
                std::cout << unit << '\n' ;
                goto end ;
            }
        }

        std::cout << "Generation " << generation << " with a population of " << population.size() << '\n' ;

        generation ++ ;
    }

    std::cout << '\n' ;

    end:
        if( targetFound )
            std::cout << "Target has been found " << '\n' ;
        else
            std::cout << "Target not found " << '\n' ;

    return 0;
}
