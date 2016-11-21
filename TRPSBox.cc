 
#include "G4RunManager.hh"
//#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UserSteppingAction.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//#include "DetectorConstruction.hh"
#include "ZnSeDetectorConstruction.hh"
#include "TRPSPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#include "boost/program_options.hpp"

#include <iostream>
#include <string>
#include "time.h"

namespace
{
  const size_t ERROR_IN_COMMAND_LINE = 1;
  const size_t SUCCESS = 0;
  const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace

int main(int argc,char** argv)
{
 std::string input;
 std::string output;
 // Define and parse program options
 try
 {
   /** Define and parse the program options
    */
   namespace po = boost::program_options;
   po::options_description desc("Options");
   desc.add_options()
     ("help", "Print help messages")
     (",o", po::value<std::string>(&output)->required(), "The output file prefix.")
     (",i", po::value<std::string>(&input)->required(), "The path to the input macro.");

   po::variables_map vm;
   try
   {
     po::store(po::parse_command_line(argc, argv, desc),
               vm); // can throw

     /** --help option
      */
     if ( vm.count("help")  )
     {
       std::cout << "Basic Command Line Parameter App" << std::endl
                 << desc << std::endl;
       return SUCCESS;
     }

     po::notify(vm); // throws on error, so do after help in case
                     // there are any problems
   }
   catch(po::error& e)
   {
     std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
     std::cerr << desc << std::endl;
     return ERROR_IN_COMMAND_LINE;
   }

   printf("Input file is: %s \n",input.c_str());
   printf("Output file is:: %s .\n",output.c_str());

 }
 catch(std::exception& e)
 {
   std::cerr << "Unhandled Exception reached the top of main: "
             << e.what() << ", application will now exit" << std::endl;
   return ERROR_UNHANDLED_EXCEPTION;

 }  
 

 // Construct the run manager
 //
 G4RunManager * runManager = new G4RunManager;
 //G4RunManager * runManager = new G4MTRunManager;
 
//Set the random seed
CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
G4long seed = time(NULL);
CLHEP::HepRandom::setTheSeed(seed);
G4cout << "The Random seed has been set by the system time as: " << seed << G4endl;

 // Set mandatory initialization classes
 //
 ZnSeDetectorConstruction* detector = new ZnSeDetectorConstruction();
 runManager->SetUserInitialization(detector);
 //
 G4VUserPhysicsList* physics = new TRPSPhysicsList();
 physics->SetCuts();
 runManager->SetUserInitialization(physics);
    
 // Set user action classes
 G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
 runManager->SetUserAction(gen_action);
 
 RunAction* run_action = new RunAction;
 runManager->SetUserAction(run_action);
 
 SteppingAction* step_action = new SteppingAction();
 runManager->SetUserAction(step_action);
 
 EventAction* evt_action = new EventAction(run_action);
 runManager->SetUserAction(evt_action);
 
 G4String fileName;
 if(argc > 2){
	fileName=output;
	run_action->SetOutputFileName(fileName);
 }
 else{
	run_action->SetOutputFileName("TRPS");
 }

#ifdef G4VIS_USE
 // Visualization manager
 //
 G4VisManager* visManager = new G4VisExecutive;
 visManager->Initialize();
#endif
    
 // Initialize G4 kernel
 //
 runManager->Initialize();
  
 // Get the pointer to the User Interface manager
 //
 G4UImanager* UImanager = G4UImanager::GetUIpointer();  

 if (argc==1)   // Define UI session for interactive mode
 {
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
      ui->SessionStart();
      delete ui;
#endif
 }
 else           // Batch mode
 {
   G4String command = "/control/execute ";
   G4String scriptName = input;
   UImanager->ApplyCommand(command+scriptName);
 }

#ifdef G4VIS_USE
 delete visManager;
#endif
 delete runManager;

 return 0;
}


