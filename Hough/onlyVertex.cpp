
// end tracking
// begin vertex.

arrayV2 model;
vector<int> maybetracks; //index of tracks that may be fitted. It gets cancelled during multiple iterations.
vector<int> besttracks;  //index of the tracks used for the event reconstruction
besttracks.clear();

//in this for loop different cluster are comapred:
//if a cluster has a pad which (squared) distance from the model of another track is smaller than
//SqVertex_Width, then this two tracks are considered from the same event.
//The simplest approach consists in basically considering the case in which we have the maximum number of tracks
//considered as belonging to the same event. More advanced techniques check whether both tracks are bel
for (unsigned int a = 0; a < vectracks.size(); a++)
{
    if (vectracks[a].isFittable() == false)
    {
        continue;
    }

    maybetracks.clear();
    cFittedLine<int> aTrack = vectracks[a];
    maybetracks.push_back(a);

    model[0] = aTrack.getBasepoint();
    model[1] = aTrack.getDirection();

    for (unsigned int i = 0; i < vectracks.size(); i++)
    {

        if (i == a || vectracks[i].isFittable() == false)
        {
            continue;
        }
        for (auto &h : vectracks[i].getPoints())
        {
            arrayD4 point = {hits[h].getX(), hits[h].getY(), hits[h].getZ(), hits[h].getEnergy()};
            if (GetError(model, point) < SqVertex_Width)
            {
                maybetracks.push_back(i);
                break; //a single point is enough.
            }
        } // end for(auto &h: vectracks[i].getPoints())

        if (maybetracks.size() > besttracks.size() && maybetracks.size() > 1)
        {
            besttracks = maybetracks;
        }

    } // end for (int i=0; i<vectracks.size() ; i++)
} // end for (int a=0; a<vectracks.size() ; a++)

// Vertex
vertex_asTVector = TVector3(0, 0, 0);
vertex_asTVector = MinuitForVertex(*tracker, besttracks);
cVertex<cPhysicalHit> vertex(vertex_asTVector.x(), vertex_asTVector.y(), vertex_asTVector.z());
for (auto &i : besttracks)
{
    auto actual_track = vectracks[i];
    cFittedLine<cPhysicalHit> track_withHits(actual_track.getDirection(), actual_track.getBasepoint());
    for (auto &j : actual_track.getPoints())
    {
        //getting the physical hits from vectracks, the vector containing all tracks.
        // "hits" is where hits are actually stored. vectracks only contains indices.
        track_withHits.getPoints().push_back(hits[j]);
    }
    vertex.getTracks().push_back(track_withHits);
    //tracker_withHits->getLines().push_back(besttrack_withHits);
}

tracker_withHits->getVertex().push_back(vertex);

Double_t GetError(arrayV2 model, arrayD4 p)
{
    TVector3 a = {p[0] - model[0][0], p[1] - model[0][1], p[2] - model[0][2]};
    TVector3 H = model[0] + a.Dot(model[1]) * model[1];
    TVector3 d = {p[0] - H[0], p[1] - H[1], (p[2] - H[2]) * 10}; //it was necessary to put this factor 10 due to measurement units (as I have set the precalibrator).

    return d.Mag2();
}

//begin Vertex

void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
    // Since arguments are defined by the MINUIT algorith, nobody can change them.
    arrayD4 p = {par[0], par[1], par[2], 1.};
    arrayV2 model;
    Double_t err = 0;
    Double_t chisq = 0;

    for (auto &i : eTrack.getLines())
    {
        model[0] = i.getBasepoint();
        model[1] = i.getDirection();
        err = GetError(model, p); // si può pesare questo test con il chiquadro di ogni traccia: più una traccia è precisa, minore sarà il suo contributo all'errore finale. /GlobalTrack.GetChi2(i);
        chisq += err;
    }

    f = chisq;
}

TVector3 MinuitForVertex(cFittedEvent<int> &tracker, vector<int> besttracks)
{
    // Initialization
    eTrack.getLines().clear();
    for (auto &i : besttracks)
    {
        list<cFittedLine<int>>::iterator iterTrack = tracker.getLines().begin();
        advance(iterTrack, i);
        if (!iterTrack->isFittable())
        {
            continue;
        }
        eTrack.getLines().push_back(*iterTrack);
    }

    // Valiables
    Double_t Vertex[3] = {0};
    Double_t Vertex_err[3] = {0};
    Double_t delta = 100.;

    // Minuit
    TMinuit *gMinuit = new TMinuit(3);
    gMinuit->SetPrintLevel(-1); //-1:quiet, 0:normal, 1:verbose
    gMinuit->SetErrorDef(1.);

    // Set Paramters
    Double_t maxX = numx;
    Double_t maxY = numy;
    Double_t maxZ = numz;
    gMinuit->DefineParameter(0, "Vx", 0, 0.01, -delta, maxX + delta);
    gMinuit->DefineParameter(1, "Vy", maxY / 2., 0.01, -delta, maxY + delta);
    gMinuit->DefineParameter(2, "Vz", maxZ / 2., 0.01, -delta, maxZ + delta);

    // Calculation MINUIT using Migrad algorithm
    gMinuit->SetFCN(FunctionForVertex);
    gMinuit->Migrad();

    // Get Results
    gMinuit->GetParameter(0, Vertex[0], Vertex_err[0]);
    gMinuit->GetParameter(1, Vertex[1], Vertex_err[1]);
    gMinuit->GetParameter(2, Vertex[2], Vertex_err[2]);

    TVector3 ver(Vertex[0], Vertex[1], Vertex[2]);

    delete gMinuit;

    return ver;
}
