template<class T>
cFittedEvent<T>::cFittedEvent() {}

template<class T>
cFittedEvent<T>::~cFittedEvent() {}

template<class T>
void cFittedEvent<T>::setEventNumber(int v) {
  eventnum = v;
}

template<class T>
int cFittedEvent<T>::getEventNumber() const {
  return eventnum;
}

template<class T>
void cFittedEvent<T>::setRunNumber(int v) {
  runnum = v;
}

template<class T>
int cFittedEvent<T>::getRunNumber() const {
  return runnum;
}

template<class T>
void cFittedEvent<T>::setTimestamp(unsigned long int v) {
  timestamp = v;
}

template<class T>
unsigned long int cFittedEvent<T>::getTimestamp() const {
  return timestamp;
}

template<class T>
std::list<cFittedLine<T>>& cFittedEvent<T>::getLines() {
  return lines;
}

template<class T>
const std::list<cFittedLine<T>>& cFittedEvent<T>::getLines() const {
  return lines;
}

template<class T>
void  cFittedEvent<T>::setLines(std::list<cFittedLine<T>>& v) {
  lines=v;
}
template<class T>
std::list<T>& cFittedEvent<T>::getUnfittedPoints() {
  return unfittedPoints;
}

template<class T>
const std::list<T>& cFittedEvent<T>::getUnfittedPoints() const {
  return unfittedPoints;
}

template<class T>
void cFittedEvent<T>::setUnfittedPoints(std::list<T>& v){
  unfittedPoints=v;
}

template<class T>
std::list<T>& cFittedEvent<T>::getAncillaryHit() {
  return ancillaryHit;
}

template<class T>
const std::list<T>& cFittedEvent<T>::getAncillaryHit() const {
  return ancillaryHit;
}

template<class T>
void cFittedEvent<T>::setAncillaryHit(std::list<T>& v){
  ancillaryHit=v;
}

template<class T>
void cFittedEvent<T>::setVertex(std::list<cVertex<T>>& v){
  vertex=v;
}

template<class T>
std::list<cVertex<T>>& cFittedEvent<T>::getVertex(){
  return vertex;
}

templateClassImp(cFittedEvent);
