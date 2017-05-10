// an attempt to serialize

template <typename T>
void Compare(T x, T y){
  if( x != y ){
    std::cerr << x << " and " << y << " does not match \n";
  }
}

void readPrimaryFile(int nEvents=10) {
  auto extkin = new o2::eventgen::FromTreeKinematicsGenerator("Kinematics.root");

  // read back from the file via our StackGenerator
  //auto stackgen = new o2::eventgen::PersistentStackGenerator("events.root");

  // make a different stack
  o2::Data::Stack * myStack2 = new o2::Data::Stack();
  // fill stack with a new event generator
  FairPrimaryGenerator * gen2 = new FairPrimaryGenerator();
  FairMCEventHeader *eventheader = new FairMCEventHeader();
  gen2->AddGenerator(extkin);
  gen2->SetEvent(eventheader);

  gen2->GenerateEvent(myStack2);

  auto n1 = myStack2->GetListOfParticles()->GetEntries();
  std::cout << "outputstack has " << myStack2->GetListOfParticles()->GetEntries() << " particles \n";
}
