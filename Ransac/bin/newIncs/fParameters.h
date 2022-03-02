#ifndef fParameters_h
#define fParameters_h

class fParameters {

public:

  fParameters() {

  };
  virtual ~fParameters(){

  };


  //parameters for Clustering
   const double  threshold      =1500;   //minimum energy requested for a single cluster.

   const double  fenergy        =40000; //energy threshold for a track in order to be considered a beam track.
   const double  width          =4;     //maximum distance from model accepted in clustering
   const double  fwidth         =3.5;   //maximum distance from model accepted in clustering for beam tracks
   const double  loops          =10000;  //number of loops, i.e. number of random couples chosen.
   const double  trsize         =10;    //min number of pads required in order to consider a cluster a real track
   const double  besize         =60;    //min number of pads required in order to consider a cluster a real track FOR BEAM

  //Parameters for Tracking(Minuit)
   const int     numx           =128;   //Pad Dimension
   const int     numy           =64;
   const double  numz           =100; //numz depends on time and drift speed. (100: default value)

  //Parameters for Vertex
   const double  SqVertex_Width =55;    //maximum distance accepted between two different cluster


private:

};

/*
//parameters for Clustering Working just fine. Modifying them in order to find light particles.
 const double  threshold      =3500;   //minimum energy requested for a single cluster.
 const double  threshold      =3500;   //minimum energy requested for a single cluster.

 const double  fenergy        =40000; //energy threshold for a track in order to be considered a beam track.
 const double  width          =4;     //maximum distance from model accepted in clustering
 const double  fwidth         =3.5;   //maximum distance from model accepted in clustering for beam tracks
 const double  loops          =10000;  //number of loops, i.e. number of random couples chosen.
 const double  trsize         =15;    //min number of pads required in order to consider a cluster a real track
 const double  besize         =60;    //min number of pads required in order to consider a cluster a real track FOR BEAM

//Parameters for Tracking(Minuit)
 const int     numx           =128;   //Pad Dimension
 const int     numy           =64;
 const double  numz           =100; //numz depends on time and drift speed. (100: default value)

//Parameters for Vertex
 const double  SqVertex_Width =55;    //maximum distance accepted between two different cluster
*/

#endif
