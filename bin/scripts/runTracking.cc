{
  TProof::Open("workers=35");
  gProof->AddIncludePath("/home/garfield/buildARDA/bin/macro/");
  gProof->Load("/home/garfield/buildARDA/bin/macro/AnalysisSet.cc+");
  gProof->Load("/home/garfield/buildARDA/bin/macro/DetectorSet.cc+");
  gProof->Load("/home/garfield/buildARDA/bin/macro/DataSet.cc+");
  gProof->Load("/home/garfield/buildARDA/bin/macro/TrackVertexFitterSelectorMod.C+");

  // v takes all values between 8 and 30
  for (double v = 8.; v < 50.5; v += 1.) {
    int vint = v;
    TrackVertexFitterSelectorMod sel;
    sel.ReadFile(Form("outvel/Conv_320_%d.root", vint));
  }
}
