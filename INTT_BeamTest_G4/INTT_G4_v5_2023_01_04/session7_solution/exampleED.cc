/// \file exampleED.cc
/// \brief Main program of the ED example

#include <random>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "EDDetectorConstruction.hh"
#include "EDActionInitialization.hh"
//#include "OutputManager.hh"
#include "INTTMessenger.hh"

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " exampleED [-m macro ] [-p physList ] [-u UIsession]  [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode." << G4endl;
    G4cerr << G4endl;
  }
}

int main(int argc,char** argv)
{

  // Evaluate arguments
  if( argc > 9 )
    {
      PrintUsage();
      return 1;
    }
  
  G4String macro;
  G4String session;
  G4String physicsListName;
  G4int nofThreads = 0;

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Change a seed of random number
  std::random_device rnd;
  
  for ( G4int i=1; i<argc; i=i+2 )
    {
      if( G4String(argv[i]) == "-m" )
	macro = argv[i+1];
      else if ( G4String(argv[i]) == "-u" )
	session = argv[i+1];
      else if ( G4String(argv[i]) == "-p" )
	physicsListName = argv[i+1];
#ifdef G4MULTITHREADED
      else if ( G4String(argv[i]) == "-t" )
	{
	  nofThreads = G4UIcommand::ConvertToInt(argv[i+1]);
	}
#endif
      else {
	PrintUsage();
	return 1;
      }
    }
  
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if ( ! macro.size() )
    {
      ui = new G4UIExecutive(argc, argv, session);
    }
  
  // Construct the run manager
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nofThreads > 0 ) { 
    runManager->SetNumberOfThreads(nofThreads);
  }  
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // read INTT macros at first
  INTTMessenger* INTT_mess = new INTTMessenger( runManager, "", "" );
  G4String INTT_init_command = "/control/execute ";
  UImanager->ApplyCommand( INTT_init_command + "INTT_config.mac" );
  INTT_mess->Print();

  // for debugging INTTMessenger
  if( INTT_mess->GetDebugLevel() == 0 )
    {
      CLHEP::HepRandom::setTheSeed( rnd() );
    }
  else if( 1 <= INTT_mess->GetDebugLevel() )
    {
      CLHEP::HepRandom::setTheSeed( 2 ); // 2 has only 25 tracks, nice for debugging
      
      if( 2 <= INTT_mess->GetDebugLevel() )
	return 0;
    }
  
  // Set mandatory initialization classes
  // Detector construction
  auto det_const = new EDDetectorConstruction( INTT_mess );
  runManager->SetUserInitialization( det_const );

  // Physics list
  if ( physicsListName.size() == 0 )
    {
      //physicsListName = "FTFP_BERT";
      //physicsListName = "FTFP_BERT_EMZ"; // better than FTFP_BERT for electromagnetic processes, but takes more time (twice?)
      physicsListName = "FTFP_BERT_WVI"; // note : WVI model
      //physicsListName = "FTFP_BERT__SS"; // note : SS model
    }

  // set OutputManager
  //OutputManager* outout = new OutputManager();

  G4PhysListFactory physListFactory;
  if ( ! physListFactory.IsReferencePhysList(physicsListName) ) {
    G4cerr << " Physics list " << physicsListName 
           << " is not defined." << G4endl;
    return 1;
  } 

  G4VModularPhysicsList* physicsList 
    = physListFactory.GetReferencePhysList(physicsListName);
  physicsList->SetVerboseLevel(0);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization( new EDActionInitialization( INTT_mess, det_const ) );
  
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  if ( macro.size() ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand( command + macro );
  }
  else {
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
    //    UImanager->ApplyCommand("/control/execute run.mac");  // for debugging
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}
