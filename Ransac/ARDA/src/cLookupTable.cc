#include "cLookupTable.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

cLookupTable::chanData::chanData() {
  row   = -1;
  col   = -1;
  det   = -1;
  psaID = "";
}

cLookupTable::chanData::chanData(Int_t rowU, Int_t colU, Int_t detU, string psaIDU) {
  row   = rowU;
  col   = colU;
  det   = detU;
  psaID = psaIDU;
}

/**
\brief Load file constructor
This constructor loads the lookuptable from a correctly formatted file:
every line must end with a return character and its elements must be
whitespace separated. In order they must be:

channel aget asad cobo row col detector psaID

Every line starting with a # is ignored
*/
cLookupTable::cLookupTable(string filename) {
  ifstream fop(filename);

  if (!fop) {
    cout << "Error while trying to open lookup table file " << filename << endl;
    return;
  }

  string line;

  while(getline(fop, line)) {
    Int_t  channel  = -1,
           aget     = -1,
           asad     = -1,
           cobo     = -1,
           row      = -1,
           col      = -1,
           detector = -1;
    string psaID    = "";

    if (line.length() > 0) {
      if (line[0] != '#') {
        stringstream ss;
        ss.str(line);
        ss >> channel
           >> aget
           >> asad
           >> cobo
           >> row
           >> col
           >> detector
           >> psaID;

        table[getGlobalChannelId(channel, aget, asad, cobo)] = chanData(row, col, detector, psaID);
      }
    }
  }
}

cLookupTable::cLookupTable(string gecoFile, string psaFile) {
  ifstream fop(gecoFile);

  if (!fop) {
    cout << "Error while trying to open geco lookup table file " << gecoFile << endl;
    return;
  }

  string line;

  // First line is intestation, I don't need it
  getline(fop, line);

  while(getline(fop, line)) {
    Int_t  channel  = -1,
           aget     = -1,
           asad     = -1,
           cobo     = -1,
           row      = -1,
           col      = -1,
           dumb     = -1,
           detector = -1;
    string dumbstring;

    if (line.length() > 0) {
      if (line[0] != '#') {
        stringstream ss;
        ss.str(line);
        ss >> dumb
           >> cobo
           >> asad
           >> aget
           >> channel
           >> dumb
           >> row
           >> col
           >> detector
           >> dumbstring;

        // Initialize all channels as empty
        table[getGlobalChannelId(channel, aget, asad, cobo)] = chanData(row, col, detector, "none");
      }
    }
  }

  ifstream fop2(psaFile);

  if (!fop2) {
    cout << "Error while trying to open psa lookup table file " << psaFile << endl;
    return;
  }

  while(getline(fop2, line)) {
    Int_t  globChId  = -1;
    string psaID;

    if (line.length() > 0) {
      if (line[0] != '#') {
        stringstream ss;
        ss.str(line);
        ss >> globChId
           >> psaID;

        // Initialize all channels as empty
        table[globChId].setPsaID(psaID);
      }
    }
  }
}

void cLookupTable::loadPreprocessorTable(string fname) {
  ifstream fop(fname);

  if (!fop) {
    cout << "Error while trying to open preprocessor table file " << fname << endl;
    return;
  }

  string cand;

  while(!fop.eof()) {
    fop >> cand;
    prepNames.push_back(cand);
  }
}

const list<std::string>& cLookupTable::getPreprocessorRequested() {
  return prepNames;
}

/**
\brief Generates globalchannelid from channel, aget, asad, cobo
*/
Int_t cLookupTable::getGlobalChannelId(Int_t channel, Int_t aget, Int_t asad, Int_t cobo) {
  return channel + 100 * aget +  1000 * asad + 10000 * cobo;
}

/// Returns the cobo number from the global channel id
Int_t cLookupTable::getCoboFromGlobalChannelId(Int_t globid) {
  return globid / 10000 % 10;
}

/// Returns the asad number from the global channel id
Int_t cLookupTable::getAsadFromGlobalChannelId(Int_t globid) {
  return globid / 1000 % 10;
}

/// Returns the aget number from the global channel id
Int_t cLookupTable::getAgetFromGlobalChannelId(Int_t globid) {
  return globid / 100 % 10;
}

/// Returns the channel number from the global channel id
Int_t cLookupTable::getChanFromGlobalChannelId(Int_t globid) {
  return globid % 100;
}


/**
Returns the PSA id that corresponds to the given global channel id. If it
is not found it returns an empty string
*/
string cLookupTable::getPSAIdFromGlobalChannelId(Int_t globid) const {
  if (table.count(globid) == 0) {
    return "";
  }
  else {
    return table.at(globid).getPsaID();
  }
}

/**
Returns the PSA id that corresponds to the given cobo, asad, aget and channel numbers. If it
is not found it returns an empty string
*/
string cLookupTable::getPSAIdFromNumbers(Int_t channel, Int_t aget, Int_t asad, Int_t cobo) const {
  Int_t globid = getGlobalChannelId(channel, aget, asad, cobo);
  return getPSAIdFromGlobalChannelId(globid);
}

ClassImp(cLookupTable);
