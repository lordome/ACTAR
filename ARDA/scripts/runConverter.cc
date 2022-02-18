{
  // v takes all values between 8 and 30
  for (double v = 8.; v < 50.5; v += 1.) {
    int vint = v;
    Converter("out/ACTAR_320.root", Form("outvel/Conv_320_%d.root", vint), "demo", v, "/home/garfield/buildARDA/bin/macro/datafile/LookupTable_forPad.dat", "/home/garfield/buildARDA/bin/macro/datafile/LookupTable_forMaya.dat", "/home/garfield/buildARDA/bin/macro/datafile/LookupTable_forDSSD.dat", "/home/garfield/buildARDA/bin/macro/datafile/EneCalTable_forPad_Dummy.dat", "/home/garfield/buildARDA/bin/macro/datafile/EneCalTable_forMaya_Dummy.dat", "/home/garfield/buildARDA/bin/macro/datafile/EneCalTable_forDSSD_Dummy.dat");
  }
}
