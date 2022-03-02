#ifdef __CLING__

#pragma link C++ class std::list<TVector3>+;
#pragma link C++ class cPhysicalHit+;
#pragma link C++ class cPhysicalEvent+;
#pragma link C++ class cFittedEvent<cPhysicalHit>+;
#pragma link C++ class std::list<cPhysicalHit>+;
#pragma link C++ class cFittedLine<int>+;
#pragma link C++ class cFittedLine<cPhysicalHit>+;
#pragma link C++ class std::list<cFittedLine<cPhysicalHit>+>;
#pragma link C++ class vector<double>+;
#pragma link C++ class std::list<int>+;
#pragma link C++ class cVertex<int>+;
#pragma link C++ class cFittedEvent<int>;
#pragma link C++ class vector<cHit>+;
#pragma link C++ class map<Int_t, cLookupTable::chanData>+;
#pragma link C++ class vector<cRawSignal>+;
#pragma link C++ class std::list<cVertex<int>>+;
#pragma link C++ class cVertex<cPhysicalHit>+;
#pragma link C++ class std::list<TVector3>+;
#pragma link C++ class std::list<cFittedLine<int>>+;
#pragma link C++ class std::list<cFittedLine<cPhysicalHit>+>;
#pragma link C++ class cVertexFinder<cPhysicalHit>+;
#pragma link C++ class std::map<double, std::list<cFittedLine<cPhysicalHit>>::iterator>+;
#pragma link C++ class cLineMerger<cPhysicalHit>+;
#pragma link C++ class cProximityClusterizer<cPhysicalHit>+;
#endif

/*
#ifdef __CLING__

#pragma link C++ class std::list<TVector3>+;
#pragma link C++ class std::list<cHit>+;
#pragma link C++ class std::list<cHitWrapper>+;
#pragma link C++ class cFittedLine<cHitWrapper>+;
#pragma link C++ class std::list<cFittedLine<cHitWrapper>+>;
#pragma link C++ class cFittedEvent<cHitWrapper>+;
#pragma link C++ class cVertexFinder<cHitWrapper>+;

#endif
*/
