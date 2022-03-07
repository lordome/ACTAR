#ifndef cDrawEvents_h
#define cDrawEvents_h

#include <cmath>
#include <list>
#include "TVector3.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TSystem.h"

template <class T>
class cDrawEvents
{
public:
    cDrawEvents(){};
    cDrawEvents(int &, int &, int &, float &, float &, float &);
    cDrawEvents(int *, int *, int *, float *, float *, float *);
    virtual ~cDrawEvents();

    void setEvent(T &);
    void setEvent(T *);

    void drawEvent2D(const bool &);
    void drawEvent3D(const bool &);
    void drawComponents2D(const bool &, double, double, double, double);
    void drawColors2D(const bool &, double, double, double, double);
    void drawTracks3D(const bool &, double, double, double, double);
    void drawAll3D(const bool &);
    void drawAll2D(const bool &);

    void drawVertex(const bool &);

    void deAllocate();

private:
    T *event;

    TCanvas *canvas2d = NULL;
    TCanvas *canvas3d = NULL;
    TCanvas *canvasColor2D = NULL;
    TCanvas *canvasDivided2D = NULL;
    TCanvas *canvasDivided3D = NULL;
    TCanvas *canvasAll2D = NULL;
    TCanvas *canvasAll3D = NULL;
    TCanvas *canDrawComp = NULL;
    TCanvas *canVertex = NULL;

    TH2F *h2D = NULL;
    TH3D *h3D = NULL;
    std::vector<TH2F *> vec2D[3] = {
        {new TH2F()}, {new TH2F()}, {new TH2F()}};

    std::vector<TH3F *> vec3D = {
        NULL,
    };
    std::vector<TPolyLine3D *> vecLines = {
        NULL,
    };

    TH2F *hXY_comp = NULL;
    TH2F *hXZ_comp = NULL;
    TH2F *hYZ_comp = NULL;

    TH2F *hXY_vert = NULL;
    TH2F *hXZ_vert = NULL;
    TH2F *hYZ_vert = NULL;

    TH2F *hAll2D = NULL;
    TH3F *hAll3D = NULL;

    int binX = 128;
    int binY = 128;
    int binZ = 100;

    float maxX = 128.;
    float maxY = 128.;
    float maxZ = 500.;
};

template <class T>
cDrawEvents<T>::cDrawEvents(int &sbinX, int &sbinY, int &sbinZ,
                            float &smaxX, float &smaxY, float &smaxZ)
{
    binX = sbinX;
    binY = sbinY;
    binZ = sbinZ;
    maxX = smaxX;
    maxY = smaxY;
    maxZ = smaxZ;
};

template <class T>
cDrawEvents<T>::cDrawEvents(int *sbinX, int *sbinY, int *sbinZ,
                            float *smaxX, float *smaxY, float *smaxZ)
{
    binX = sbinX;
    binY = sbinY;
    binZ = sbinZ;
    maxX = smaxX;
    maxY = smaxY;
    maxZ = smaxZ;
};

template <class T>
cDrawEvents<T>::~cDrawEvents()
{

    if (canvas2d)
        delete canvas2d;
    if (canvas3d)
        delete canvas3d;
    if (canvasColor2D)
        delete canvasColor2D;
    if (canvasDivided2D)
        delete canvasDivided2D;
    if (canvasDivided3D)
        delete canvasDivided3D;
    if (canvasAll2D)
        delete canvasAll2D;
    if (canvasAll3D)
        delete canvasAll3D;
    if (canVertex)
        delete canVertex;

    if (h2D)
        delete h2D;
    if (h3D)
        delete h3D;
    if (hXY_comp)
        delete hXY_comp;
    if (hXZ_comp)
        delete hXZ_comp;
    if (hYZ_comp)
        delete hYZ_comp;
    if (canDrawComp)
        delete canDrawComp;

    if (hXY_vert)
        delete hXY_vert;
    if (hXZ_vert)
        delete hXZ_vert;
    if (hYZ_vert)
        delete hYZ_vert;

    for (auto &it_vec : vec2D)
    {
        for (auto it_hist : it_vec)
        {
            delete it_hist;
        }
        it_vec.clear();
    }

    for (auto &p : vec3D)
    {
        delete p;
    }
    vec3D.clear();

    for (auto &p : vecLines)
    {
        delete p;
    };
    vecLines.clear();

    if (hAll2D)
    {
        delete hAll2D;
    };

    if (hAll3D)
    {
        delete hAll3D;
    }
}

template <class T>
void cDrawEvents<T>::setEvent(T &evt)
{
    event = &evt;
};

template <class T>
void cDrawEvents<T>::setEvent(T *evt)
{
    event = evt;
}

template <class T>
void cDrawEvents<T>::drawEvent2D(const bool &wait)
{

    delete canvas2d;
    canvas2d = new TCanvas("Canvas2D", "Canvas2D", 800, 600);
    canvas2d->cd();

    delete h2D;
    h2D = new TH2F("Hist2DXYCLASSE", "", binX, 0, maxX, binY, 0, maxY);
    for (auto &it_lines : event->getLines())
    {
        for (auto &it_hits : it_lines.getPoints())
        {
            for (int j = 0; j < it_hits.getEnergy(); j++)
            {
                h2D->Fill(it_hits.getX(), it_hits.getY());
            }
        }
    }
    h2D->Draw("colz");
    gSystem->ProcessEvents();
    if (wait)
    {
        canvas2d->WaitPrimitive();
    }
    return;
};

template <class T>
void cDrawEvents<T>::drawEvent3D(const bool &wait)
{
    delete canvas3d;
    canvas3d = new TCanvas("Canvas3D", "Canvas3D", 800, 600);
    canvas3d->cd();
    delete h3D;
    h3D = new TH3D("Hist3D", "", binX, 0, maxX, binY, 0, maxY, binZ, 0, maxZ);
    for (auto &it_lines : event->getLines())
    {
        for (auto &it_hits : it_lines.getPoints())
        {
            for (int j = 0; j < it_hits.getEnergy(); j++)
            {
                h3D->Fill(it_hits.getX(), it_hits.getY(), it_hits.getZ());
            }
        }
    }
    h3D->Draw("LEGO");
    gSystem->ProcessEvents();

    if (wait)
    {
        canvas3d->WaitPrimitive();
    }
    return;
};

template <class T>
void cDrawEvents<T>::drawColors2D(const bool &wait, double x1, double y1, double x_w, double y_w)
{

    for (auto &it_vec : vec2D)
    {
        for (auto it_hist : it_vec)
        {
            if (it_hist)
                delete it_hist;
        }
        it_vec.clear();
    }

    if (canvasColor2D)
    {
        delete canvasColor2D;
    }

    canvasColor2D = new TCanvas("canvasColor2D", "canvasColor2D", x1, y1, x_w, y_w);
    canvasColor2D->Divide(3);
    canvasColor2D->cd();

    string titles[3];
    string titlesRoots[3] = {"vHisto2DXY",
                             "vHisto2DXZ",
                             "vHisto2DYZ"};
    int it_count = 0;
    for (auto &it_lines : event->getLines())
    {

        for (int it = 0; it < 3; it++)
        {
            titles[it] = titlesRoots[it] + to_string(it_count);
        }

        vec2D[0].emplace_back(new TH2F(titles[0].c_str(), titles[0].c_str(), binX, 0, maxX, binY, 0, maxY));
        vec2D[1].emplace_back(new TH2F(titles[1].c_str(), titles[1].c_str(), binX, 0, maxX, binZ, 0, maxZ));
        vec2D[2].emplace_back(new TH2F(titles[2].c_str(), titles[2].c_str(), binY, 0, maxY, binZ, 0, maxZ));

        for (auto &it_hits : it_lines.getPoints())
        {
            vec2D[0][it_count]->Fill(it_hits.getX(), it_hits.getY());
            vec2D[1][it_count]->Fill(it_hits.getX(), it_hits.getZ());
            vec2D[2][it_count]->Fill(it_hits.getY(), it_hits.getZ());
        }

        for (unsigned int itera = 0; itera < 3; itera++)
        {
            canvasColor2D->cd(itera + 1);
            vec2D[itera][it_count]->SetFillColor(it_count + 1);
            vec2D[itera][it_count]->Draw("BOX SAME");
        }
        it_count += 1;
    }

    if (wait)
    {
        canvasColor2D->WaitPrimitive();
    }
    return;
};

template <class T>
void cDrawEvents<T>::drawTracks3D(const bool &wait, double x1, double y1, double x_w, double y_w)
{

    for (auto &p : vec3D)
    {
        if (p)
            delete p;
    }
    vec3D.clear();

    for (auto &p : vecLines)
    {
        if (p)
            delete p;
    }
    vecLines.clear();

    if (canvasDivided3D)
    {
        delete canvasDivided3D;
    }

    canvasDivided3D = new TCanvas("Canvas3DTracks", "Canvas3DTracks", x1, y1, x_w, y_w);
    canvasDivided3D->Divide(3, 3);
    canvasDivided3D->cd();

    vec3D.emplace_back(new TH3F("TotalEvent", "", binX, 0, maxX, binY, 0, maxY, binZ, 0, maxZ));
    for (auto &it_lines : event->getLines())
        for (auto &it_hits : it_lines.getPoints())
            vec3D[0]->Fill(it_hits.getX(), it_hits.getY(), it_hits.getZ());

    for (auto &it_unfitPoints : event->getUnfittedPoints())
        vec3D[0]->Fill(it_unfitPoints.getX(), it_unfitPoints.getY(), it_unfitPoints.getZ());

    canvasDivided3D->cd(1);
    vec3D[0]->Draw("lego");

    string title;
    int it_count = 0;

    for (auto &it_lines : event->getLines())
    {
        title = "vec3D" + to_string(it_count);
        vec3D.emplace_back(new TH3F(title.c_str(), title.c_str(), binX, 0, maxX, binY, 0, maxY, binZ, 0, maxZ));
        vec3D.at(it_count)->GetXaxis()->SetTitle(" X axis [mm/2] ");
        vec3D.at(it_count)->GetYaxis()->SetTitle(" Y axis [mm/2] ");
        vec3D.at(it_count)->GetZaxis()->SetTitle(" Z axis [a.u.] ");
        vec3D.at(it_count)->GetXaxis()->SetTitleOffset(2);
        vec3D.at(it_count)->GetYaxis()->SetTitleOffset(2);

        for (auto &it_hits : it_lines.getPoints())
            vec3D[it_count + 1]->Fill(it_hits.getX(), it_hits.getY(), it_hits.getZ());

        canvasDivided3D->cd(it_count + 2);
        vec3D[it_count + 1]->Draw("LEGO");

        if (it_lines.isFittable())
        { // now drawing line

            vecLines.emplace_back(new TPolyLine3D());
            //(TPolyLine3D *drawObj = new TPolyLine3D();
            TVector3 base1 = it_lines.getBasepoint() + it_lines.getDirection() * 1000;
            vecLines.back()->SetNextPoint(base1.x(), base1.y(), base1.z());
            TVector3 bn = it_lines.getBasepoint() - it_lines.getDirection() * 1000;
            vecLines.back()->SetNextPoint(bn.x(), bn.y(), bn.z());
            vecLines.back()->Draw("same");
        }
        it_count += 1;
    }

    vec3D.emplace_back(new TH3F("unfPoints", "unfPoints", binX, 0, maxX, binY, 0, maxY, binZ, 0, maxZ));
    for (auto &it_unfPts : event->getUnfittedPoints())
    {
        vec3D.back()->Fill(it_unfPts.getX(), it_unfPts.getY(), it_unfPts.getZ());
    }

    canvasDivided3D->cd(9);
    vec3D.back()->Draw("BOX");

    if (wait)
    {
        canvasDivided3D->WaitPrimitive();
    }
    return;
};

template <class T>
void cDrawEvents<T>::drawComponents2D(const bool &wait, double x1, double y1, double x_w, double y_w)
{

    if (hXY_comp)
        delete hXY_comp;
    if (hXZ_comp)
        delete hXZ_comp;
    if (hYZ_comp)
        delete hYZ_comp;
    if (canDrawComp)
        delete canDrawComp;
    canDrawComp = new TCanvas("components2D", "components2D", x1, y1, x_w, y_w);
    canDrawComp->Divide(3, 1);

    hXY_comp = new TH2F("histXY_comp", "histXZ_comp", binX, 0, maxX, binY, 0, maxY);
    hXZ_comp = new TH2F("histXZ_comp", "histXZ_comp", binX, 0, maxX, binZ, 0, maxZ);
    hYZ_comp = new TH2F("histYZ_comp", "histYZ_comp", binY, 0, maxY, binZ, 0, maxZ);

    for (auto &it_lines : event->getLines())
        for (auto &it_hits : it_lines.getPoints())
        {
            hXY_comp->Fill(it_hits.getX(), it_hits.getY());
            hXZ_comp->Fill(it_hits.getX(), it_hits.getZ());
            hYZ_comp->Fill(it_hits.getY(), it_hits.getZ());
        }

    for (auto &it_unfitPoints : event->getUnfittedPoints())
    {
        hXY_comp->Fill(it_unfitPoints.getX(), it_unfitPoints.getY());
        hXZ_comp->Fill(it_unfitPoints.getX(), it_unfitPoints.getZ());
        hYZ_comp->Fill(it_unfitPoints.getY(), it_unfitPoints.getZ());
    }

    canDrawComp->cd(1);
    hXY_comp->Draw("colz");
    canDrawComp->cd(2);
    hXZ_comp->Draw("colz");
    canDrawComp->cd(3);
    hYZ_comp->Draw("colz");

    if (wait)
    {
        canDrawComp->WaitPrimitive();
    }
    return;
}

template <class T>
void cDrawEvents<T>::drawVertex(const bool &wait)
{
    if (hXY_vert)
        delete hXY_vert;
    if (hXZ_vert)
        delete hXZ_vert;
    if (hYZ_vert)
        delete hYZ_vert;
    if (canVertex)
        delete canVertex;
    canVertex = new TCanvas("canVertex", "canVertex");
    canVertex->Divide(3, 1);

    hXY_vert = new TH2F("histXY_vert", "histXZ_vert", binX, 0, maxX, binY, 0, maxY);
    hXZ_vert = new TH2F("histXZ_vert", "histXZ_vert", binX, 0, maxX, binZ, 0, maxZ);
    hYZ_vert = new TH2F("histYZ_vert", "histYZ_vert", binY, 0, maxY, binZ, 0, maxZ);

    for (auto &it_vert : event->getVertex())
    {
        for (auto &it_lines : it_vert.getTracks())
        {
            for (auto &it_hits : it_lines.getPoints())
            {
                hXY_vert->Fill(it_hits.getX(), it_hits.getY());
                hXZ_vert->Fill(it_hits.getX(), it_hits.getZ());
                hYZ_vert->Fill(it_hits.getY(), it_hits.getZ());
            }
        }
    }
        // for (auto &it_unfitPoints : event->getUnfittedPoints())
        // {
        //     hXY_vert->Fill(it_unfitPoints.getX(), it_unfitPoints.getY());
        //     hXZ_vert->Fill(it_unfitPoints.getX(), it_unfitPoints.getZ());
        //     hYZ_vert->Fill(it_unfitPoints.getY(), it_unfitPoints.getZ());
        // }

        canVertex->cd(1);
        hXY_vert->Draw("colz");
        canVertex->cd(2);
        hXZ_vert->Draw("colz");
        canVertex->cd(3);
        hYZ_vert->Draw("colz");

        if (wait)
        {
            canVertex->WaitPrimitive();
        }
        return;
    }

    template <class T>
    void cDrawEvents<T>::drawAll3D(const bool &wait)
    {

        if (hAll3D)
        {
            delete hAll3D;
        }
        if (canvasAll3D)
        {
            delete canvasAll3D;
        }

        canvasAll3D = new TCanvas("canvasAll3D", "canvasAll3D", 800, 600);
        canvasAll3D->cd();

        hAll3D = new TH3F("AllHits3D", "", binX, 0, maxX, binY, 0, maxY, binZ, 0, maxZ);
        for (auto &it_lines : event->getLines())
            for (auto &it_hits : it_lines.getPoints())
                hAll3D->Fill(it_hits.getX(), it_hits.getY(), it_hits.getZ());

        for (auto &it_unfitPoints : event->getUnfittedPoints())
            hAll3D->Fill(it_unfitPoints.getX(), it_unfitPoints.getY(), it_unfitPoints.getZ());

        hAll3D->Draw("lego");

        if (wait)
        {
            canvasDivided3D->WaitPrimitive();
        }
        return;
    }

    // template<class T>
    // TVector3 cDrawEvents<T>::getBasepoint(const std::vector<double>& par) const {
    //   TVector3 xtil(-sin(par[0]), cos(par[0]) * cos(par[1]), cos(par[0]) * sin(par[1]));
    //   TVector3 ytil(0, -sin(par[1]), cos(par[1]));

    //   return xtil*par[2] + ytil*par[3];
    // }

#endif
