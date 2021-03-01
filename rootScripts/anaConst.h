#ifndef ANACONST_H
#define ANACONST_H

#include <map>

const double hplanck = 4.135667696e-15; // eV*s
const double clight = 299792458; //m/s
const double pi = acos(-1);
const double m2tobarn = 1e-28;

const double eRadius = 2.817940285e-15;  // classical electron radius (m)
const double eMass = 0.510998902e+6;   // electron mass (eV)

double gLambda = 532e-9; //m
double eEnergy = 18e9; //eV

double gEnergy = hplanck * clight / gLambda; //eV
double Gamma = eEnergy / eMass;
double a = 1 / ( 1 + 4*Gamma*gEnergy/eMass);//
double gMaxEnergy = 4*a*gEnergy*std::pow(Gamma,2)/1e9;//GeV

void recalcMaxEnergy(){
  Gamma = eEnergy / eMass;
  a = 1 / ( 1 + 4*Gamma*gEnergy/eMass);//
  gMaxEnergy = 4*a*gEnergy*std::pow(Gamma,2)/1e9;//GeV
}


const int nXsec=2;
const string xSecNm[nXsec]={"uX","pX"};
const string xSecTi[nXsec]={"#sigma_{unpol}","#sigma_{pol}"};
TH1D *hXsec[nXsec], *iXsec[nXsec];

const int nDet = 8;
const double zPos[nDet]={0.05,5,6.5,16,16.7,25,27,27.7};//m

const int nWght = 6;//[counts,upolXsec, asym, E, asym*E]
const string wghtTit[nWght]={"counts","unpolXsec","polXsec","energy*unpolXsec","energy*polXsec","polXsec*sqrt(unpolXsec)"};
const int n2Dbins=800;

const int nPart=2;
const string partT[nPart]={"gamma","electron"};
const string partN[nPart]={"g","e"};

double partXBinLimit[nPart][2]={{-48.5,-47.5},{-10,12}};
double partYBinLimit[nPart][2]={{-1,1},{-0.07,0.07}};
TH2D *xy[nPart][nDet][nWght];
const int nSeg=9;
const double seg[nSeg]={500,400,300,200,100,50,10,5,1};
const double segL[nSeg]={0.10,0.08,0.09,0.08,0.08,0.08,0.08,0.08,0.08};
TH1D *yElec[nWght][nSeg];

std::map<int,int> pid;

#endif
