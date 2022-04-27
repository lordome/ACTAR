#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <TObject.h>

#include <Parameters.h>

using namespace std;

#ifndef MCHANNELINFO_H
#define MCHANNELINFO_H

class MChannelInfo
{
	public:
	short globalchannelid; 
	std::vector<unsigned short> peaknumber;
	std::vector<unsigned short> peakheight;
	std::vector<unsigned short> peaktime;

	MChannelInfo();
	~MChannelInfo();
	
	ClassDef(MChannelInfo,1) // Channel data class
};


#endif

#ifndef MDATASTRUCTURE_H
#define MDATASTRUCTURE_H


class MDataStructure
{
	private:
	
	public:
	short globalasadnumber;
	int event_number;
	long int time_stamp;
	std::vector<MChannelInfo> Channels;
	MDataStructure();
	~MDataStructure();
	
	ClassDef(MDataStructure,1) // AsAd data class
};

#endif


#ifndef MEVENTOLD_H
#define MEVENTOLD_H


class MEventOld
{
	public:
	int event_number;
	std::vector<MDataStructure> Fragments;
	MEventOld();
	~MEventOld();
	
	ClassDef(MEventOld,1) // Event data class
};

#endif
