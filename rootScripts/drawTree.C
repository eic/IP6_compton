#include "anaConst.h"

void drawTree(string fnm="../build/compton.root"){
  TFile *fin=TFile::Open(fnm.c_str(),"READ");
  TTree *t=(TTree*)fin->Get("t");

  //gStyle->SetOptStat("oue");

  eEnergy = 5e9;
  recalcMaxEnergy();

  TH2F *xy[2][2];
  TCanvas *c1[2][2];
  const int pid[2]={22,11};
  const string xSec[2]={"uXsec","pXsec"};
  const double yRg[2][2]={{-100,100},{-0.07,0.07}};
  const double xRg[2][2]={{100,300},{42,69}};

  for(int i=0;i<2;i++)
    for(int j=0;j<2;j++){
      c1[i][j]=new TCanvas(Form("c%d",i*2+j),Form("xy%d%s",pid[i],xSec[j].c_str()));
      xy[i][j]=new TH2F(Form("xy%d%s",pid[i],xSec[j].c_str()),
			Form("xy%d%s",pid[i],xSec[j].c_str()),
			200,xRg[i][0],xRg[i][1],
			200,yRg[i][0],yRg[i][1]);
      t->Project(Form("xy%d%s",pid[i],xSec[j].c_str()),"bQD9_hits.y:bQD9_hits.x",
		 Form("%s*(bQD9_hits.pID==%d)",xSec[j].c_str(),pid[i]));
    }

  for(int i=0;i<2;i++)
    xy[i][1]->Divide(xy[i][0]);

  for(int i=0;i<2;i++)
    for(int j=0;j<2;j++){
      c1[i][j]->cd();
      xy[i][j]->Draw("colz");
    }


  TH1F *rho[2];
  TCanvas *c2[2];
  for(int i=0;i<2;i++){
    rho[i]=new TH1F(Form("rho%s",xSec[i].c_str()),Form("rho%s",xSec[i].c_str()),200,0,1);
    c2[i]=new TCanvas(Form("c%s",xSec[i].c_str()),Form("c%s",xSec[i].c_str()));
    t->Project(Form("rho%s",xSec[i].c_str()),Form("bQD9_hits.ke/%f/1000",gMaxEnergy),
	       Form("%s*(bQD9_hits.pID==22)",xSec[i].c_str()));
    rho[i]->DrawCopy();
  }

  rho[1]->Divide(rho[0]);
  c2[1]->cd();
  rho[1]->DrawCopy();

  //fin->Close();
}
