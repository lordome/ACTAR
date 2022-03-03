#include "MTrack.h"
#include <TH1.h>
#include <TH2.h>
#include <TSpectrum.h>
#include <TF1.h>
#include <TPolyLine.h>
#include <TMarker.h>

void Calibrate(TH2I*);
void CleanPad(float[NPADY][NPADX],float[NPADY][NPADX], float);
bool GetVertexFromRMS(TH2F*,int&,int&);
bool GetVerticalConsistensy(MTrack*,MTrack*);
float AverageEloss(MTrack*,float[NPADY][NPADX]);
float FitMat(std::vector <int>,std::vector <int>,std::vector <float>,std::vector <int>,float &,float &);
void FitMat(float[NPADY][NPADX],int,int,int,int,float,float&,float&);
float FitMat3D(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, float, MTrack*);
void MakeConfigFileThr(char*,char*,float[NB_COBO*NB_ASAD*NB_AGET*NB_CHANNEL],float);
float Profile2D(float [128][128],float, float, TH1D*, int, float&, float&, float&, float&);
float Profile3D(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, MTrack*, TH1D*, TH1D*, double, float&, bool&, TMarker*[5] );
float Profile3D_v2(std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, MTrack*, TH2D*, TH2D*, TH1D*, TH1D*, double, float&, bool&, TMarker*[5] );


