#ifndef MEVENTREDUCED_H
#define MEVENTREDUCED_H

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <TObject.h>

using namespace std;

class MEventReduced : public TObject
{

	class ReducedData : public TObject
	{
	public:
		ReducedData() { hasSaturation = false; };
		virtual ~ReducedData(){};

		unsigned short globalchannelid;
		bool hasSaturation;

		std::vector<float> peakheight;
		std::vector<float> peaktime;

		ClassDef(ReducedData, 2);
	};

public:
	MEventReduced(){};
	virtual ~MEventReduced(){};
	unsigned long int event;
	unsigned long int timestamp;
	std::vector<ReducedData> CoboAsad;

	ClassDef(MEventReduced, 1);
};

#endif
