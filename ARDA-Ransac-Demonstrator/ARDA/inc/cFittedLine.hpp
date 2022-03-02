templateClassImp(cFittedLine);

template<class T>
cFittedLine<T>::cFittedLine(TVector3 n, TVector3 b) {
  direction = n;
  basepoint = b;
}

template<class T>
cFittedLine<T>::cFittedLine() {}

template<class T>
cFittedLine<T>::~cFittedLine() {}

template<class T>
TVector3 cFittedLine<T>::getDirection() const {
  return direction;
}

template<class T>
TVector3 cFittedLine<T>::getBasepoint() const {
  return basepoint;
}

template<class T>
void cFittedLine<T>::setDirection(const TVector3& v) {
  direction = v;
}

template<class T>
void cFittedLine<T>::setBasepoint(const TVector3& v) {
  basepoint = v;
}

template<class T>
void cFittedLine<T>::setPoints(const std::list<T> ps) {
  points = ps;
}

template<class T>
std::list<T>& cFittedLine<T>::getPoints() {
  return points;
}

template<class T>
bool cFittedLine<T>::isFittable(){
  return fittable;
}

template<class T>
void cFittedLine<T>::setGone(bool x){
  gone=x;
}

template<class T>
bool cFittedLine<T>::isGone(){
  return gone;
}

template<class T>
void cFittedLine<T>::setFittable(bool x){
  fittable=x;
}

/*
template<class T>
void cFittedLine<T>::Draw(Option_t* option) {
  if (drawObj) delete drawObj;
  drawObj = new TPolyLine3D;


  TVector3 base1= basepoint - direction*100;
  drawObj->SetNextPoint(base1.x(), base1.y(), base1.z());

  TVector3 bn = basepoint + direction*100;

  drawObj->SetNextPoint(bn.x(), bn.y(), bn.z());

  drawObj->Draw(option);
}
*/