#include <iostream>
#include "TFile.h"
#include "TEnv.h"
#include "TTree.h"
#include "cPhysicalEvent.h"
#include "cUserInput.h"
#include "TTreeReader.h"
#include "cLookupTable.h"
#include "cRawEvent.h"
#include "TVector3.h"
#include <utility>
#include <map>
#include <string>

using namespace std;

struct calCoeff
{
  Double_t m;
  Double_t q;
};

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    cout << "Usage: precalibrator input1.root input2.root [...] output.root" << endl;
    return 0;
  }

  // Loading oscar calibration
  map<int, calCoeff> oscarCal;
  TEnv foscal("oscal.cal");
  for (int i = 21000; i < 21400; i++)
  {
    string mname = to_string(i) + "m";
    string qname = to_string(i) + "q";

    if (foscal.Defined(mname.data()) && foscal.Defined(qname.data()))
    {
      calCoeff val;
      val.m = foscal.GetValue(mname.data(), val.m);
      val.q = foscal.GetValue(qname.data(), val.q);

      oscarCal.emplace(i, val);
    }
  }

  // Loading pad calibration
  map<int, calCoeff> padCal;
  TEnv fpadcal("padcal.cal");
  for (int i = 0; i < 20000; i++)
  {
    string mname = to_string(i) + "m";
    string qname = to_string(i) + "q";

    if (fpadcal.Defined(mname.data()) && fpadcal.Defined(qname.data()))
    {
      calCoeff val;
      val.m = fpadcal.GetValue(mname.data(), val.m);
      val.q = fpadcal.GetValue(qname.data(), val.q);

      padCal.emplace(i, val);
    }
  }

  // Load conversion from ADC sample to distance
  TVector3 velocity;
  TEnv velDat("velocity.cal");
  velocity[0] = velDat.GetValue("x", velocity[0]);
  velocity[1] = velDat.GetValue("y", velocity[1]);
  velocity[2] = velDat.GetValue("z", velocity[2]);

  // Open output file and save the calibrations used
  TFile fout(argv[argc - 1], "CREATE");
  if (fout.IsZombie())
  {
    cout << "Output file creation failed" << endl;
    return 0;
  }
  fout.cd();
  foscal.Write("oscal");
  fpadcal.Write("padcal");
  velDat.Write("veldat");

  // Creating output tree
  TTree outTree("physicalEventTree", "physicalEventTree");
  cPhysicalEvent *phEvt = nullptr;
  outTree.Branch("event", &phEvt);
  outTree.SetDirectory(&fout);
  outTree.AutoSave();

  // Cycle on the input file
  for (int argi = 1; argi < argc - 1; argi++)
  {
    TFile fin(argv[argi]);
    if (fin.IsZombie())
    {
      cout << "Unable to open file " << argv[argi] << endl;
      continue;
    }

    TTree *inTree = (TTree *)fin.Get("rawEventTree");
    if (!inTree)
    {
      cout << "Unable to open TTree in file " << argv[argi] << endl;
    }

    // Retrieve lookup table
    cLookupTable *lt = (cLookupTable *)fin.Get("lookupTable");
    if (!lt)
    {
      cout << "Unable to open cLookupTable in file " << argv[argi] << endl;
    }

    // Now I cicle all the events in the input TTree
    TTreeReader rdr(inTree);
    TTreeReaderValue<cRawEvent> event(rdr, "event");

    Long64_t nent = rdr.GetEntries(false);

    cout << endl;
    while (rdr.Next())
    {
      if (rdr.GetCurrentEntry() % 10 == 0)
      {
        cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;
      }

      // WRITING SINGLE EVENT
      delete phEvt;
      phEvt = new cPhysicalEvent;

      phEvt->setEventNumber(event->getEventNumber());
      phEvt->setTimestamp(event->getTimestamp());

      //SET RUNNUMBER!!!!

      for (auto &hit : event->getHits())
      {
        // WRITING SINGLE HIT
        int gid = hit.getGlobalChannelId();
        // HIT ON PADPLANE
        if (padCal.count(gid))
        {
          cPhysicalHit phHit;
          phHit.setGlobalChannelId(gid);
          phHit[0] = lt->getTable()[gid].getCol() * velocity[0];
          phHit[1] = lt->getTable()[gid].getRow() * velocity[1];
          phHit[2] = hit.getPeakTime() * velocity[2];
          phHit.setEnergy(hit.getPeakHeight() * padCal[gid].m + padCal[gid].q);
          phHit.setTrackable(true);

          // Noisy channels not to use for tracking
          if (gid >= 100 && gid < 200)
            phHit.setTrackable(false);

          // The hits on the beam are noisier, so it is better to add a cut
          if (phHit[1] >= 26. && phHit[1] <= 36. && phHit.GetEnergy() <= 60.)
            phHit.setTrackable(false);

          phEvt->getHits().push_back(phHit);
        }

        // HIT ON OSCAR
        if (gid >= 21000 && gid < 21500)
        {
          if (oscarCal.count(gid) == 1)
          {
            cPhysicalHit phHit;
            phHit.setGlobalChannelId(gid);

            phHit.setEnergy(hit.getPeakHeight() * oscarCal[gid].m + oscarCal[gid].q);
            phHit.setTrackable(false);

            phEvt->getHits().push_back(phHit);
          }
        }
      }

      outTree.Fill();
    }

    fout.cd();
    outTree.Write("", TObject::kOverwrite);

    cout << endl;
  }

  return 0;
}
