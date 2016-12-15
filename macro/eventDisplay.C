void eventDisplay()
{
  //-----User Settings:-----------------------------------------------
  TString  InputFile     ="AliceO2_TGeant3.mc_1_event.root";
  TString  ParFile       ="AliceO2_TGeant3.params_1.root";
  TString  OutFile	 ="tst.root";


  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  FairFileSource *fFileSource = new FairFileSource(InputFile.Data());
  fRun->SetSource(fFileSource);
  fRun->SetOutputFile(OutFile.Data());

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(ParFile.Data());
  rtdb->setFirstInput(parInput1);

  FairEventManager *fMan= new FairEventManager();

  //----------------------Traks and points -------------------------------------
  FairMCTracks    *Track     = new FairMCTracks("Monte-Carlo Tracks");
//  FairMCPointDraw *TorinoDetectorPoints = new FairMCPointDraw("FairTestDetectorPoint", kRed, kFullSquare);

  fMan->AddTask(Track);
//  fMan->AddTask(TorinoDetectorPoints);


  fMan->Init();

}
