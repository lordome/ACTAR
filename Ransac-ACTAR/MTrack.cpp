///////////////////////////////////////
//                                   //
// T. Roger - GANIL 2015             //
// email: roger@ganil.fr             //
//                                   //
// MTrack class file:                //
//     - Track  graphical properties //
//                                   //
///////////////////////////////////////


#include "MTrack.h"


using namespace std;

MTrack::MTrack()
{
	L2DXY=new TLine();
	L2DXZ=new TLine();
	L2DYZ=new TLine();
	L3D=new TLine();
}

MTrack::MTrack(char* path)
{
	L2DXY=new TLine();
	L2DXZ=new TLine();
	L2DYZ=new TLine();
	L3D=new TLine();
	
	FILE* f=fopen(path,"r");
	if(f!=NULL)
	{
		int i=0;
		while(fscanf(f,"%f %f",&ElossTable[i][0],&ElossTable[i][1])!=EOF)
		{
			i++;
			if(i>63) break;
		}
		fclose(f);
	}
	else cout << "Eloss File " << path << " doesn't exist... Skipping Eloss Table." << endl;
}


MTrack::~MTrack()
{
	delete L2DXY;
	delete L2DXZ;
	delete L2DYZ;
	delete L3D;
// 	for(int i=0;i<NPADX;i++)
// 		delete ElossTable[i];
// 	delete [] ElossTable;
}




void MTrack::ResetLines()
{
	L2DXY->SetX1(-1);
	L2DXY->SetY1(-1);
	L2DXY->SetX2(-1);
	L2DXY->SetY2(-1);
	
	L2DXZ->SetX1(-1);
	L2DXZ->SetY1(-1);
	L2DXZ->SetX2(-1);
	L2DXZ->SetY2(-1);

	L2DYZ->SetX1(-1);
	L2DYZ->SetY1(-1);
	L2DYZ->SetX2(-1);
	L2DYZ->SetY2(-1);

	L3D->SetX1(-1);
	L3D->SetY1(-1);
	L3D->SetX2(-1);
	L3D->SetY2(-1);
}

void MTrack::UpdateTLines()
{
	this->L2DXY->SetX1(this->Xm);
	this->L2DXY->SetY1(this->Ym);
	this->L2DXY->SetX2(this->Xh);
	this->L2DXY->SetY2(this->Yh);
	
	this->L2DXZ->SetX1(this->Xm);
	this->L2DXZ->SetY1(this->Zm);
	this->L2DXZ->SetX2(this->Xh);
	this->L2DXZ->SetY2(this->Zh);
	
	this->L2DYZ->SetX1(this->Ym);
	this->L2DYZ->SetY1(this->Zm);
	this->L2DYZ->SetX2(this->Yh);
	this->L2DYZ->SetY2(this->Zh);

}
