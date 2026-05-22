# Evolutionary String Matcher

Evolutionary String Matcher is a genetic algorithm written in C++23 that evolves random ASCII strings into a user defined target through simulated evolution.

The program creates populations of random strings and improves them generation after generation using selection, crossover, and mutation until the target is discovered.

This project was built as an experiment in evolutionary computation and modern C++ programming.

## Features

• Random ASCII genome generation  
• Fitness based evolution  
• Probabilistic parent selection  
• Double point crossover  
• Genome mutation  
• Population ranking  
• Survival of the fittest mechanics  

## Example

```text
Give a target string : HelloWorld

Generation 0 with a population of 100
Generation 1 with a population of 100
Generation 2 with a population of 100

Target has been found
HelloWorld
```

## How it works

Each member of the population is represented as a string.

Example chromosome:

```text
A8@!kL2z
```

The fitness function compares every character of a genome against the target string.The more matching characters a genome has, the greater its fitness score becomes.Individuals with higher fitness are more likely to become parents and pass their genes into the next generation.Parents produce offspring using double point crossover and random mutation introduces variation into the population.After every generation the strongest individuals survive while weaker ones are removed.
Over time the population slowly converges toward the target.

## Technologies

C++20  
STL  
Evolutionary algorithms  

## Build

Compile using a modern C++ compiler( all files , geneticAlgorithm.cpp and RandomGeneration.h must be located in the same file ).

```bash
cd FolderLocation
g++ -std=c++23 -g -o executableName geneticAlgorithm.cpp
./executableName
```

## License

MIT License
