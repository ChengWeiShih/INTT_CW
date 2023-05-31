/// \file EDChamberSD.cc
/// \brief Implementation of the EDChamberSD class
//
#include "EDChamberSD.hh"

namespace { 
   G4Mutex myGlobalValueMutex = G4MUTEX_INITIALIZER;
}

G4double* myGlobalValue = new G4double(1.);

EDChamberSD::EDChamberSD(const G4String& name, 
                         const G4String& hitsCollectionName,
                         G4int ntupleId
                         )
 : G4VSensitiveDetector(name),
   fHitsCollection(0),
   fNtupleId(ntupleId)
   //HCID1(-1)
{
  collectionName.insert(hitsCollectionName);
  //VEncoder*theEncoder = new Encoder64Cal();
  
}

EDChamberSD::~EDChamberSD()
{}

void EDChamberSD::Initialize(G4HCofThisEvent* hce)
{

  // store the verbose level for debugging
  auto ui_manager = G4UImanager::GetUIpointer();
  verbose = ui_manager->GetCurrentIntValue( "/run/verbose", 1, true );

  fHitsCollection 
    = new EDChamberHitsCollection(SensitiveDetectorName, collectionName[0]);
    //G4cout<<"the name test"<<collectionName[0]<<G4endl;

  // Add this collection in hce
  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection); 

  G4int upordown = 2;
  G4int xposition = 13;
  G4int yposition = 256;
  G4int zposition = 4;
  G4int silicon_type = 2;
  G4int copyNo_counting = 0;

  for (G4int i1=0; i1<zposition; i1++) // loop over z-position, in another words, ladders
    {
      for (G4int i2=0; i2<yposition; i2++) // loop over y-position, it means the vertical direction
        {

	        G4int up_or_down = i2<128 ? 0 : 1 ; // 0 is for the down side (U14 - U26), 1 is for the up side
	  
          for (G4int i3=0; i3<xposition; i3++) // loop over x-position, the horizontal direction
            {
              
              G4int silicon_type = i3 < 8 ? 0 : 1; // 0 is for type-A, 1 is for type-B
              
              EDChamberHit *newHit = new EDChamberHit();
              G4int ypos = i2 < 128 ? i2 : i2 - 128;
              newHit->SetEncoderID( up_or_down, silicon_type, i1, i3, ypos ); // up or down, silicon_type, z, x, y
              fHitsCollection->insert(newHit);
              //G4cout<<"TESTChmaberSD : "<<copyNo_counting<<" "<<i1<<" "<<i2<<" "<<i3<<" up : "<<0<<" type : "<<0<<G4endl;
              copyNo_counting+=1;

            } // end of the loop over x-position
        } // end of the loop over y-position
    } // end  of the loop over z-position

  
  
  /* // saved in 2022/June/20 by G. Nukazuka: It can be better
  for (G4int i1=0; i1<zposition; i1++) // loop over z-position, in another words, ladders
    {
      for (G4int i2=0; i2<yposition; i2++) // loop over y-position, it means the vertical direction
        {
          if (i2<128) //Down, U14 ~ U26
            {
              for (G4int i3=0; i3<xposition; i3++) // loop over x-position, the horizontal direction
                {
                  if (i3<8)
                    {
                      EDChamberHit *newHit = new EDChamberHit();
                      newHit->SetEncoderID(0,0,i1,i3,i2); // up or down, silicon_type, z, x, y
                      fHitsCollection->insert(newHit);
                      //G4cout<<"TESTChmaberSD : "<<copyNo_counting<<" "<<i1<<" "<<i2<<" "<<i3<<" up : "<<0<<" type : "<<0<<G4endl;
                      copyNo_counting+=1;
		      
                    }
                  else 
                    {
                      EDChamberHit *newHit = new EDChamberHit();
                      newHit->SetEncoderID(0,1,i1,i3,i2);
                      fHitsCollection->insert(newHit);
                      //G4cout<<"TESTChmaberSD : "<<copyNo_counting<<" "<<i1<<" "<<i2<<" "<<i3<<" up : "<<0<<" type : "<<1<<G4endl;
                      copyNo_counting+=1;
                    }  
                } // end of the loop over x-position
            } // end of the loop over y-position
          else //upper, U1 ~ U13 
            {
              for (G4int i3=0; i3<xposition; i3++)
                {
                  if (i3<8)
                    {
                      EDChamberHit *newHit = new EDChamberHit();
                      newHit->SetEncoderID(1,0,i1,i3,i2-128);
                      fHitsCollection->insert(newHit);
                      //G4cout<<"TESTChmaberSD : "<<copyNo_counting<<" "<<i1<<" "<<i2<<" "<<i3<<" up : "<<1<<" type : "<<0<<G4endl;
                      copyNo_counting+=1;
                    }
                  else 
                    {
                      EDChamberHit *newHit = new EDChamberHit();
                      newHit->SetEncoderID(1,1,i1,i3,i2-128);
                      fHitsCollection->insert(newHit);
                      //G4cout<<"TESTChmaberSD : "<<copyNo_counting<<" "<<i1<<" "<<i2<<" "<<i3<<" up : "<<1<<" type : "<<1<<G4endl;
                      copyNo_counting+=1;
                    }
                }
            }  

        }
	}
  */ // saved in 2022/June/20 by G. Nukazuka: It can be better
  
}

G4bool EDChamberSD::ProcessHits(G4Step* step, 
                                G4TouchableHistory* /*history*/)
{

  G4int eID_1 = 0;
  const G4Event* evt_1 = G4MTRunManager::GetRunManager()->GetCurrentEvent();
  if (evt_1)
    eID_1 = evt_1->GetEventID();

  G4AutoLock myLock(&myGlobalValueMutex); 
  // simulate data race
  if ( (*myGlobalValue) > 0.) {
    delete myGlobalValue;
    myGlobalValue = new G4double(-1);
  }
  else  {
    delete myGlobalValue;
    myGlobalValue = new G4double(1);
  }
  
  myLock.unlock();



  // note : it doesn't be classified as prePosition or posPosition
  G4double edep = step->GetTotalEnergyDeposit(); 
  if ( edep == 0. ) { return false; }



  // note : get touchable
  const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
  // note : mother 
  G4VPhysicalVolume* motherPhysical = touchable->GetVolume(0); 
  // note : get the mother volumn name
  G4String motherPhysical_name_test = motherPhysical->GetName();
  // note : get copy number 
  G4int copyNo = motherPhysical->GetCopyNo();
  


  // note : get track
  auto track = step->GetTrack();
  // note : G4track getname
  G4String volume_test= track->GetVolume()->GetName(); // note : !!maybe!! it is from poststepposition

  G4int post_copyNo = track->GetVolume()->GetCopyNo();

  G4int PDG = track->GetDefinition()->GetPDGEncoding();

  G4int particle_order = track -> GetTrackID();
  // note : G4track hit information
  G4double track_posX = track->GetPosition().x();
  G4double track_posY = track->GetPosition().y();
  G4double track_posZ = track->GetPosition().z();    
  G4double secondaryParticleKineticEnergy =  track->GetKineticEnergy(); 
  G4double time = track->GetGlobalTime() ;

  

  //if( verbose == 2 )
  if( false )
  {
    G4cout<<"!!!! pre_TESTnameTEST : "<<motherPhysical_name_test<<G4endl;
    G4cout << "!!!! pre_getcopynumberTEST : " << copyNo << G4endl;
    // if (eID_1%10000==0)
    // G4cout<<"!!!! pre_getcopynumberTEST position: " << preStepPoint_object_position[0] << " "
    // <<  preStepPoint_object_position[1]<<" "<<preStepPoint_object_position[2]<<" "
    // <<G4endl;
    
    G4cout << "!!!! pos_TESTnameTEST : " << volume_test << G4endl;
    //G4cout<<"!!!! pos_TESTnameTEST2 : "<<volume_test_2<<G4endl;
    G4cout << "!!!! pos_getcopynumberTEST : " << post_copyNo << G4endl;
    G4cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << G4endl;
    G4cout << "Pos event ID : " << eID_1
      << "  particle order : " << particle_order << "        PID " << PDG
      << "         energy : " << secondaryParticleKineticEnergy
      << "     volume : " << volume_test
      << "  position : " << track_posX << "||" << track_posY << "||" << track_posZ
      << " copy number : " << copyNo << G4endl;
  }


  

  // note : the GetPreStepPoint is the first (starting), and the GetPostStepPoint is the end(ending) position
  G4ThreeVector touchposition = step->GetPreStepPoint()->GetPosition(); //note : this is the postion when particle hit on the material
  G4ThreeVector the_post_step_position = step->GetPostStepPoint()->GetPosition();  

  // average of the starting end the ending position of this step
  G4ThreeVector thePosition =(step->GetPreStepPoint()->GetPosition()+step->GetPostStepPoint()->GetPosition())*0.5;
  G4ThreeVector columnPosition;
  G4double      phiposition_S1;
  G4double Zseparation, Zseparation_1; 
  G4int material, side;
  G4int xposition, yposition, zposition;
  G4int n_hit = fHitsCollection->entries();
  G4int silicon_type, upordown;
  G4int volume_test_ID = -1;
  G4bool found = false;
  
  //  if (volume_test=="INTT_siLV_all_typeA") {
  if( volume_test=="INTT_siLV_all_typeA_odd" || volume_test=="INTT_siLV_all_typeA_even" )  {
    volume_test_ID=0;
  }
  //else if (volume_test=="INTT_siLV_all_typeB") {
  else if(volume_test=="INTT_siLV_all_typeB_odd" || volume_test=="INTT_siLV_all_typeB_even" ) {
    volume_test_ID=1;
  }

  auto pre_step_point = step->GetPreStepPoint();
  auto post_step_point = step->GetPostStepPoint();
  auto p_in  = pre_step_point->GetMomentum();
  auto p_out = post_step_point->GetMomentum();

  auto phys_in  = pre_step_point->GetProcessDefinedStep();
  auto phys_out = post_step_point->GetProcessDefinedStep();

  auto process_name_out = phys_out->GetProcessName();
  G4int process_id = phys_out->GetProcessSubType();


  
  G4double theta_in  = p_in.theta();
  G4double theta_out = p_out.theta();
  G4double dtheta = p_out.theta( p_in ); // std::acos(cosTheta(q));
  G4double phi_in  = p_in.phi();
  G4double phi_out = p_out.phi();

  G4String procName_pre = "Nan";
  G4String procName_post = "Nan";

  if (step->GetPreStepPoint()->GetProcessDefinedStep())
  {
    procName_pre = step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
  }

  if (step->GetPostStepPoint()->GetProcessDefinedStep())
  {
    procName_post = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  }

  G4int process_ID_pre  = EDChamberSD::ProcessID(procName_pre);
  G4int process_ID_post = EDChamberSD::ProcessID(procName_post);


  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

  // ntuple ID = 2: event_particle
  G4int num = 0;
  analysisManager->FillNtupleIColumn(2, num++, particle_order);                 // note : track -> GetTrackID();
  analysisManager->FillNtupleIColumn(2, num++, PDG);                            // note : track->GetDefinition()->GetPDGEncoding();
  analysisManager->FillNtupleDColumn(2, num++, secondaryParticleKineticEnergy); // note : track->GetKineticEnergy();
  analysisManager->FillNtupleDColumn(2, num++, process_id );                    // note : post_step_point->GetProcessDefinedStep()->GetProcessSubType();
  analysisManager->FillNtupleDColumn(2, num++, track_posX);      // note : G4track
  analysisManager->FillNtupleDColumn(2, num++, track_posY);      // note : G4track
  analysisManager->FillNtupleDColumn(2, num++, track_posZ);      // note : G4track
  analysisManager->FillNtupleDColumn(2, num++, theta_in);        // note : pre, theta_in
  analysisManager->FillNtupleDColumn(2, num++, theta_out);       // note : post, theta_out
  analysisManager->FillNtupleDColumn(2, num++, dtheta);          // note : dtheta, calculated by pre and post, 
  analysisManager->FillNtupleDColumn(2, num++, phi_in);          // note : pre, phi_in
  analysisManager->FillNtupleDColumn(2, num++, phi_out);         // note : post, phi_out
  analysisManager->FillNtupleIColumn(2, num++, volume_test_ID);
  analysisManager->FillNtupleIColumn(2, num++, eID_1); // event ID

  analysisManager->FillNtupleDColumn(2, num++, edep);// note : edep
  analysisManager->FillNtupleDColumn(2, num++, touchposition.getX());// note : pre_pos X
  analysisManager->FillNtupleDColumn(2, num++, touchposition.getY());// note : pre_pos Y
  analysisManager->FillNtupleDColumn(2, num++, touchposition.getZ());// note : pre_pos Z
  analysisManager->FillNtupleDColumn(2, num++, the_post_step_position.getX());// note : post_pos X
  analysisManager->FillNtupleDColumn(2, num++, the_post_step_position.getY());// note : post_pos Y
  analysisManager->FillNtupleDColumn(2, num++, the_post_step_position.getZ());// note : post_pos Z

  analysisManager->FillNtupleIColumn(2, num++, process_ID_pre);  // note : pre step process name
  analysisManager->FillNtupleIColumn(2, num++, process_ID_post); // note : post step process name

  analysisManager->AddNtupleRow(2);

  //==========================================keep========================================================
  //G4int PID = Control::GetControl()->GetPIDForCalHit(step);
  //assert(PID!=-1);
  //G4int PDG= step->GetTrack()->GetDefinition()->GetPDGEncoding();
  //==========================================keep========================================================

  //columnPosition[0] = r
  //columnPosition[1] = phi
  //columnPosition[2] = z

  //Hit_object_number=(zposition-1)*subX*subY+ (xposition-1)*subY   +(yposition-1);
  EDChamberHit* HitThisUnit = (*fHitsCollection)[copyNo];
  HitThisUnit->AddEdep(edep);
  HitThisUnit->SetPosition( thePosition );
  
  if( step->IsFirstStepInVolume() )
    HitThisUnit->SetStepIn( step );

  if( step->IsLastStepInVolume() )
    HitThisUnit->SetStepOut( step );
  
  // to keep infomation of the track that give the higherst energy to this strip, check dE before
  if( edep > HitThisUnit->GetEdepStep() )
    {
      if( HitThisUnit->GetTrack() != nullptr && verbose == 2 )
	      G4cout << "track overwritten: "
	       << HitThisUnit->GetTrack()->GetTrackID() << "(" << HitThisUnit->GetEdepStep() << ")"
	       << " --> "
	       << track->GetTrackID() << " (" << edep << ")"
	       << G4endl;
      
      HitThisUnit->SetTime( track->GetGlobalTime() ); // this is the hit timing
      HitThisUnit->SetTrack( track );
      HitThisUnit->SetEdepStep( edep );

      if( track->GetTrackID() == 1 ) // in the case of the beam track, set the flag
      {
        HitThisUnit->SetBeamIn( true );
        HitThisUnit->SetBeamOut( true );


      }
    }

  //  if( verbose == 2 )
  if( false )
    {
      G4cout << " GetPreStepPointTEST : " << touchposition[0] << " " << touchposition[1] << " " << touchposition[2] << G4endl;
      G4cout << " GetPostStepPointTEST " << the_post_step_position[0] << " " << the_post_step_position[1] << " " << the_post_step_position[2] << " " << G4endl;
      G4cout << "Mid event ID : " << eID_1 << "  particle order : " << particle_order << "        PID " << PDG << "         edep : " << edep << "     volume : " << volume_test << "  position : " << thePosition[0] << "||" << thePosition[1] << "||" << thePosition[2] << G4endl;
      G4cout << "=======================================" << G4endl;
      
      //G4cout << "GGGGGGGGGGGGGGGGGG " << thePosition[0] << " " << thePosition[1] << " " << thePosition[2] << " ENERGY : " << edep << " PID " << PDG << G4endl;
    }
  
  return false;
}

void EDChamberSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
  //  std::cerr << "void EDChamberSD::EndOfEvent(G4HCofThisEvent* /*hce*/)";
  
  G4int eID = 0;
  const G4Event* evt = G4MTRunManager::GetRunManager()->GetCurrentEvent();
  if (evt)
    eID = evt->GetEventID();

  G4ThreeVector sensorposition;
  //G4double theoffset_chamber = 
  //G4cout << " EDChamberSD offset TEST : " << public_variable::theoffset << G4endl;

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  G4int n_hit = fHitsCollection->entries();
  //G4cout << "TESTTESTTEST number of total hits" << n_hit << G4endl;
  G4ThreeVector the_edep_position;

  for (G4int i = 0; i < n_hit; i++) {
    auto hit = (*fHitsCollection)[i];
    
      G4double fEdep = hit->GetEnergyDeposit();
      //G4cout << "whoareyou~~~~~~ " << i << " " << fEdep << G4endl;
      if (fEdep > 0.) {

        G4int fupordown = hit->Getupordown();
        G4int fxposition = hit->Getxposition();
        G4int fyposition = hit->Getyposition();
        G4int fzposition = hit->Getzposition();
        G4int fsilicon_type = hit->Getsilicon_type();
        //the_edep_position[0]=hit -> GetObjectTranslation (0);
        //the_edep_position[1]=hit -> GetObjectTranslation (1);
        //the_edep_position[2]=hit -> GetObjectTranslation (2);

        if (fupordown==1 && fsilicon_type ==1)
          {
               sensorposition[0] = 25.1+(fxposition-8)*20.;
               sensorposition[1] = 0.055+(fyposition-0)*0.078;
               sensorposition[2] = 100.+(fzposition-0)*35.;             
          }
        else if (fupordown==1 && fsilicon_type ==0)
          {
               sensorposition[0] = -107.1+(fxposition-0)*16.;
               sensorposition[1] = 0.055+(fyposition-0)*0.078;
               sensorposition[2] = 100.+(fzposition-0)*35.;
          } 
        else if (fupordown==0 && fsilicon_type ==1)
          {
               sensorposition[0] = 25.1+(fxposition-8)*20.;
               sensorposition[1] = -9.961+(fyposition-0)*0.078;
               sensorposition[2] = 100.+(fzposition-0)*35.;
          }   
        else if (fupordown==0 && fsilicon_type ==0) 
          {
               sensorposition[0] = -107.1+(fxposition-0)*16.;
               sensorposition[1] = -9.961+(fyposition-0)*0.078;
               sensorposition[2] = 100.+(fzposition-0)*35.;            
          } 
	
        //G4cout << "particle hit ID 1:      " << i << " " << fupordown << " " << fsilicon_type << G4endl;
        //G4cout << "particle hit ID 2:      " << fxposition << " " << fyposition << " " << fzposition << G4endl;
        // G4cout << "hit" << std::setw(4) << i << " "
	      //        << "(" << sensorposition[0] << ", " << sensorposition[1] << ", " << sensorposition[2] << ") "
        //        << " edep = " << fEdep << G4endl;
        // hit->Print(); 

        analysisManager->FillNtupleIColumn(fNtupleId, 0, eID);
        analysisManager->FillNtupleIColumn(fNtupleId, 1, fupordown);
        analysisManager->FillNtupleIColumn(fNtupleId, 2, fxposition);
        analysisManager->FillNtupleIColumn(fNtupleId, 3, fyposition);
        analysisManager->FillNtupleIColumn(fNtupleId, 4, fzposition);
        analysisManager->FillNtupleIColumn(fNtupleId, 5, fsilicon_type);
        analysisManager->FillNtupleDColumn(fNtupleId, 6, fEdep);        
        analysisManager->AddNtupleRow(fNtupleId);

        //if ( eID % 10000 == 0)//if (1==1)//
	      if( verbose == 2 )
          {
	    
            G4cout << "EventID: " << std::setw(5) << eID << " ";
            hit->Print();
	    
            //G4cout << "Event Process: " << eID << G4endl;
            //G4cout << "particle hit ID 1:      " << i << " " << " positionID : " <<   fxposition << " " << fyposition << " " << fzposition << " up : "   << fupordown << " type : " << fsilicon_type << G4endl;
            //G4cout << "particle hit ID 2:      " << fxposition << " " << fyposition << " " << fzposition << G4endl;
            G4cout << "particle hit position : " << sensorposition[0] << " " << sensorposition[1] << " " << sensorposition[2] << " ENERGY deposit : " << fEdep << G4endl;
            //G4cout << "positoin cout test : " <<  the_edep_position[0] << " " << the_edep_position[1] << " " << the_edep_position[2] << endl;
            G4cout << " " << G4endl;
          }
        
      }
  }
  
  //std::cerr << "     ------> ends"  << std::endl;
}

G4int EDChamberSD::ProcessID(G4String Process_name_in)
{

  G4int return_process_id;

  if (Process_name_in == "Transportation")
  { return_process_id = 1;  }

  else if (Process_name_in == "phot")
  { return_process_id = 2;  }

  else if (Process_name_in == "compt")
  { return_process_id = 3;  }

  else if (Process_name_in == "conv")
  { return_process_id = 4;  }

  else if (Process_name_in == "Rayl")
  { return_process_id = 5;  }

  else if (Process_name_in == "msc")
  { return_process_id = 6;  }

  else if (Process_name_in == "eIoni")
  { return_process_id = 7;  }

  else if (Process_name_in == "eBrem")
  { return_process_id = 8;  }

  else if (Process_name_in == "CoulombScat")
  { return_process_id = 9;  }

  else if (Process_name_in == "annihil")
  { return_process_id = 10;  }

  else if (Process_name_in == "muIoni")
  { return_process_id = 11;  }

  else if (Process_name_in == "muBrems")
  { return_process_id = 12;  }

  else if (Process_name_in == "muPairProd")
  { return_process_id = 13;  }

  else if (Process_name_in == "hIoni")
  { return_process_id = 14;  }

  else if (Process_name_in == "hBrems")
  { return_process_id = 15;  }

  else if (Process_name_in == "hPairProd")
  { return_process_id = 16;  }

  else if (Process_name_in == "ionIoni")
  { return_process_id = 17;  }

  else if (Process_name_in == "photonNuclear")
  { return_process_id = 18;  }

  else if (Process_name_in == "electronNuclear")
  { return_process_id = 19;  }

  else if (Process_name_in == "muonNuclear")
  { return_process_id = 20;  }

  else if (Process_name_in == "Decay")
  { return_process_id = 21;  }

  else if (Process_name_in == "hadElastic")
  { return_process_id = 22;  }

  else if (Process_name_in == "neutronInelastic")
  { return_process_id = 23;  }

  else if (Process_name_in == "nCapture")
  { return_process_id = 24;  }

  else if (Process_name_in == "protonInelastic")
  { return_process_id = 25;  }

  else if (Process_name_in == "pi+Inelastic")
  { return_process_id = 26;  }

  else if (Process_name_in == "pi-Inelastic")
  { return_process_id = 27;  }

  else if (Process_name_in == "kaon+Inelastic")
  { return_process_id = 28;  }

  else if (Process_name_in == "kaon-Inelastic")
  { return_process_id = 29;  }

  else if (Process_name_in == "kaon0LInelastic")
  { return_process_id = 30;  }

  else if (Process_name_in == "kaon0SInelastic")
  { return_process_id = 31;  }

  else if (Process_name_in == "lambdaInelastic")
  { return_process_id = 32;  }

  else if (Process_name_in == "anti-lambdaInelastic")
  { return_process_id = 33;  }

  else if (Process_name_in == "sigma-Inelastic")
  { return_process_id = 34;  }

  else if (Process_name_in == "anti_sigma-Inelastic")
  { return_process_id = 35;  }

  else if (Process_name_in == "sigma+Inelastic")
  { return_process_id = 36;  }

  else if (Process_name_in == "anti_sigma+Inelastic")
  { return_process_id = 37;  }

  else if (Process_name_in == "xi-Inelastic")
  { return_process_id = 38;  }

  else if (Process_name_in == "anti_xi-Inelastic")
  { return_process_id = 39;  }

  else if (Process_name_in == "xi0Inelastic")
  { return_process_id = 40;  }

  else if (Process_name_in == "anti_xi0Inelastic")
  { return_process_id = 41;  }

  else if (Process_name_in == "omega-Inelastic")
  { return_process_id = 42;  }

  else if (Process_name_in == "anti_omega-Inelastic")
  { return_process_id = 43;  }

  else if (Process_name_in == "anti_protonInelastic")
  { return_process_id = 44;  }

  else if (Process_name_in == "anti_neutronInelastic")
  { return_process_id = 45;  }

  else if (Process_name_in == "anti_deuteronInelastic")
  { return_process_id = 46;  }

  else if (Process_name_in == "anti_tritonInelastic")
  { return_process_id = 47;  }

  else if (Process_name_in == "anti_He3Inelastic")
  { return_process_id = 48;  }

  else if (Process_name_in == "anti_alphaInelastic")
  { return_process_id = 49;  }

  else if (Process_name_in == "hFritiofCaptureAtRest")
  { return_process_id = 50;  }

  else if (Process_name_in == "hBertiniCaptureAtRest")
  { return_process_id = 51;  }

  else if (Process_name_in == "muMinusCaptureAtRest")
  { return_process_id = 52;  }

  else if (Process_name_in == "dInelastic")
  { return_process_id = 53;  }

  else if (Process_name_in == "tInelastic")
  { return_process_id = 54;  }

  else if (Process_name_in == "He3Inelastic")
  { return_process_id = 55;  }

  else if (Process_name_in == "alphaInelastic")
  { return_process_id = 56;  }

  else if (Process_name_in == "ionInelastic")
  { return_process_id = 57;  }

  else if (Process_name_in == "nKiller")
  { return_process_id = 58;  }

  else if (Process_name_in == "ePairProd")
  { return_process_id = 59;  }

  else if (Process_name_in == "CoupleTrans")
  { return_process_id = 60; }

  else if (Process_name_in == "Nan")
  { return_process_id = 111; }
  
  else 
  {
    return_process_id = 999;
    G4cout<<"unknown physics process : "<<Process_name_in<<G4endl;
  }



  return return_process_id;
}