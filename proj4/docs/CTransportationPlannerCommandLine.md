# CTransportationPlannerCommandLine

The CTTransportationPlannerCommandLine starts off with a private intialization of a stuct SImplementation and a unique pointer to it called DImplementation
class CTransportationPlannerCommandLine{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
}

Publically it has the costructor, destructor and a member function called bool ProcessCommands.

## Constructor
This interface provides functionality for processing commands related to the command line. The constructor takes in many parameters relating which are:
std::shared_ptr<CDataSource> cmdsrc --> reads commands
std::shared_ptr<CDataSink> outsink --> pointer to datasink for standard output
std::shared_ptr<CDataSink> errsink --> datasink for error output
std::shared_ptr<CDataFactory> results --> pointer to data factory for storing results
std::shared_ptr<CTransportationPlanner> planner --> pointer to transportaitonplanner used for actually processing the commands


## Destructor
~CTransportationPlannerCommandLine();
Destructor is in charge of clean up and gets rid of allocated memory when an instance goes out of scope

## bool ProcessCommands()
This function porcesses commands from our sources and outputs the corresponding actions