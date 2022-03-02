#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TFile.h>
#include "cFittedEvent.h"
#include "cPhysicalHit.h"
#include <TH2.h>
#include <TNtuple.h>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

void makeDeltaEvsE(TTree *trackTree, std::string saveDir){
  TTreeReader rdr(trackTree);
  TTreeReaderValue<cFittedEvent<cPhysicalHit>> event(rdr, "Full_event");
  TTreeReaderValue<double> Oscar_Energy_Max(rdr, "Oscar_Max_Energy");


  TFile saveFile(saveDir.c_str(), "recreate");
  saveFile.cd();
  TNtuple *nt = new TNtuple("deltaEvsENT", "", "vx:vy:vz:dEdx:dEdxNoLen:dEdxNoLenCut:beamAngle:eoscar:oscarAngle:evtnum");

  while (rdr.Next()) {
    // Look for a good track
    auto &tracks = event->getLines();

    bool toomanylines = false;
    auto bestTrack = tracks.end();
    
    // Take only events were a vertex was identified
    if (event->getVertex().empty()) continue;

    for (auto i = tracks.begin(); i != tracks.end(); ++i) {
      // Check if there are points outside the wanted region
      bool isptout = false;
      auto tpoi = i->getPoints();
      for (auto &p: tpoi) {
        if (p[1] <= 39) {
          isptout = true;
          break;
        }
      }

      if (!isptout) {
        if (bestTrack == tracks.end()) {
          bestTrack = i;
        }
        else {
          toomanylines = true;
        }
      }
    }

    if (toomanylines || (bestTrack == tracks.end())) continue;

    auto anchit = event->getAncillaryHit();

    double maxval = anchit.front().getEnergy();
    for (auto &h: anchit) {
      if (!isfinite(h.getEnergy())) continue;
      if (h.getGlobalChannelId() == 21231) continue;
      if (h.getEnergy() > maxval) {
        maxval = h.getEnergy();
      }
    }

    maxval = *Oscar_Energy_Max;
    TVector3 n = bestTrack->getDirection();
    TVector3 b = bestTrack->getBasepoint();

    auto poi = bestTrack->getPoints();
    TVector3 pt(poi.front()[0], poi.front()[1], poi.front()[2]);
    double maxt = (pt - b) * n;
    double mint = maxt;
    double entot = 0.;
    double pts   = 0.;
    std::vector<double> edep;
    for (auto &p: poi) {
      pt.SetXYZ(p[0], p[1], p[2]);
      double t = (pt - b) * n;
      if (t > maxt) maxt = t;
      if (t < mint) mint = t;
      entot += p.GetEnergy();
      pts++;
      edep.push_back(p.GetEnergy());
    }

    // Truncate high energy loss (delta rays). Truncated mean method
    std::sort(edep.begin(), edep.end());
    double edepTrunc = 0.;
    for (int i = 0; i < edep.size() * 0.7; i++) {
      edepTrunc += edep[i];
    }

    // Correction to take into accoun the track is not parallel to
    // the padplane tesselation
    double beamAngle = TVector3(1., 0., 0.).Angle(n);
    if (beamAngle > M_PI/2.) beamAngle = M_PI - beamAngle;

    // Wrap around
    double beamAngle2 = (beamAngle > M_PI/4.) ? M_PI/2. - beamAngle : beamAngle;
    pts /= abs(cos(beamAngle2));

    // Find vertex propagating line to the center of the padplane
    double center = 32.;
    TVector3 planeDir(0., 1., 0);
    double planeOffset = -center;

    double tintersection = -1. * (b * planeDir + planeOffset) / (n * planeDir);
    TVector3 vertex = b + n * tintersection;

    TVector3 oscarNormal(0., 1., 0.);
    
    TVector3 actual_vertex = event->getVertex().front();

    nt->Fill(actual_vertex.x(), actual_vertex.y(), actual_vertex.z(), entot/TMath::Abs(maxt-mint), entot/pts, edepTrunc/pts, beamAngle, maxval, oscarNormal.Angle(n), event->getEventNumber());
  }

  nt->Write();
}
