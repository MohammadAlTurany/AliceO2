// an attempt to serialize

template <typename T>
void Compare(T x, T y){
  if( x != y ){
    std::cerr << x << " and " << y << " does not match \n";
  }
}

void generatePrimaryFile(int nEvents=10) {
  // Create PrimaryGenerator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairBoxGenerator* boxGen = new FairBoxGenerator(211, 2000); /*protons*/

  boxGen->SetEtaRange(-0.9,0.9);
  boxGen->SetPRange(0.1, 5);
  boxGen->SetPhiRange(0., 360.);
  boxGen->SetDebug(kTRUE);
  primGen->AddGenerator(boxGen);

  TClonesArray *list = new TClonesArray("TParticle");
  // put this into a file
  TFile outfile("events.root", "RECREATE");
  TTree newTree("EventTree", "The tree with generated events");
  TBranch *eventbranch = newTree.Branch("Events", &list);
  if (eventbranch == nullptr) {
    std::cout << "Could not create branch\n";
  }
  
  o2::Data::Stack * myStack = new o2::Data::Stack();
  // needed by the primary generator
  FairMCEventHeader *eventheader = new FairMCEventHeader();
  primGen->SetEvent(eventheader);
  for (int i=0; i<nEvents; ++i) {
    primGen->GenerateEvent(myStack);
    list->Clear();
    list->AbsorbObjects(myStack->GetListOfParticles());
    eventbranch->Fill();
    myStack->Reset();
    eventheader->Reset();
  }
  outfile.Write();
  outfile.Close();  

  // read back from the file via our StackGenerator
  auto stackgen = new o2::eventgen::PersistentStackGenerator("events.root");

  // make a different stack
  o2::Data::Stack * myStack2 = new o2::Data::Stack();
  // fill stack with a new event generator
  FairPrimaryGenerator * gen2 = new FairPrimaryGenerator();
  gen2->AddGenerator(stackgen);
  eventheader->Reset();
  gen2->SetEvent(eventheader);
  gen2->GenerateEvent(myStack2);

  auto n1 = myStack2->GetListOfParticles()->GetEntries();
  auto n2 = list->GetEntries();

  // comparision in case of 1 event
  if (nEvents == 1){

  std::cout << "outputstack has " << myStack2->GetListOfParticles()->GetEntries() << " particles \n";
  std::cout << "inputstack has " << list->GetEntries() << " particles \n";

  if (! (n1 == n2)) {
    std::cout << "error in size\n";
    return;
  }
  
  // compare in and out particles
  for (int i = 0; i<n1; ++i) {
    auto p1 = static_cast<TParticle*>(myStack2->GetListOfParticles()->At(i));
    auto p2 = static_cast<TParticle*>(list->At(i));

    Compare(p1->GetPdgCode(), p2->GetPdgCode());
    Compare(p1->Px(), p2->Px());
    Compare(p1->Py(), p2->Py());
    Compare(p1->Pz(), p2->Pz());
    Compare(p1->Vx(), p2->Vx());
    Compare(p1->Vy(), p2->Vy());
    Compare(p1->Vz(), p2->Vz());
    Compare(p1->Energy(), p2->Energy());
    Compare(p1->T(), p2->T());
    Compare(p1->GetWeight(), p2->GetWeight());
  }
  }
  
  for ( int i=0; i<nEvents+1; ++i ) {
    gen2->GenerateEvent(myStack2);
    myStack2->Reset();
  }
}
