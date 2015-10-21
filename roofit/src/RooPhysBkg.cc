/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 * @(#)root/roofit:$Id: RooPhysBkg.cxx 24286 2008-06-16 15:47:04Z wouter $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//
// BEGIN_HTML
// RooArgusBg is a RooAbsPdf implementation describing the ARGUS background shape
// END_HTML
//

#include "RooFit.h"

#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooPhysBkg.h"
#include "RooRealVar.h"
#include "RooRealConstant.h"
#include "RooMath.h"
#include "TMath.h"

ClassImp(RooPhysBkg)


//_____________________________________________________________________________
RooPhysBkg::RooPhysBkg(const char *name, const char *title,
		       RooAbsReal& _m, RooAbsReal& _m0, RooAbsReal& _c, RooAbsReal& _s) :
  RooAbsPdf(name, title),
  m("m","Mass",this,_m),
  m0("m0","Resonance mass",this,_m0),
  c("c","Slope parameter",this,_c),
  s("s","Sigma",this,_s)
{
}



//_____________________________________________________________________________
RooPhysBkg::RooPhysBkg(const RooPhysBkg& other, const char* name) :
  RooAbsPdf(other,name),
  m("m",this,other.m),
  m0("m0",this,other.m0),
  c("c",this,other.c),
  s("s",this,other.s)

{
}



//_____________________________________________________________________________
Double_t RooPhysBkg::evaluate() const {
  Double_t ssq=s*s;
  Double_t sfth=ssq*ssq;
  Double_t csq=c*c;
  Double_t m0sq=m0*m0;
  Double_t xsq=m0sq;
  Double_t msq=m*m;

  Double_t exp_arg_3 = m0* (c + m/ssq) - (xsq + msq)/(2.*ssq);//Adim
  Double_t term_3 = -m0sq + csq*sfth + xsq + m0*m + msq + ssq*(2 + c*m0 + 2*c*m);//Mass^2
  Double_t term_4 = (csq*sfth + xsq + 2*c*ssq*m + msq)/(2.*ssq) - (xsq + msq)/(2.*ssq); //Adim
  Double_t term_5 = (-m0sq + csq*sfth + msq + ssq*(3 + 2*c*m)); //Mass^2

  Double_t part_1 = 2*exp(exp_arg_3) *s* ( term_3 );//Mass^3
  Double_t part_2 = exp(term_4)*sqrt(2*M_PI)*(c*ssq + m)*term_5;//Mass^3

  Double_t up = 0.5*s * ( part_1 + part_2*
			  TMath::Erf((c*ssq - m0 + m)/(sqrt(2)*s)));

  // because I don't like compile warnings
  up *= 1.;

  Double_t rat1 = (msq)/(2.*ssq); //Adim
  Double_t exp_arg_1 = -m0sq + csq*sfth + msq + ssq*(2 + 2*c*m); //Mass^2
  Double_t exp_arg_2 = csq*sfth + 2*c*ssq*m + msq; //Mass^2
  Double_t rat2 = (exp_arg_2)/(2.*ssq); //Adim

  Double_t term_1 = exp(-rat1) *s * 2 *s* (exp_arg_1); //Mass^4
  Double_t prod_1 = (c*ssq + m); //Mass
  Double_t prod_2 = (-m0sq + csq*sfth + msq + ssq*(3 + 2*c*m)); //Mass^2
  Double_t prod_3 = prod_1*prod_2;
  Double_t term_2 = exp(rat2 - rat1) * sqrt(2*M_PI)* prod_3; //Mass^3

  Double_t down = 0.5*s * (term_1 +
			   term_2 *
			   TMath::Erf((c*ssq + m)/(sqrt(2)*s)));

  // because I don't like compile warnings
  down *= 1.;
  //  return (up-down)<=0?0:(m-m0)>15*s?0:(up-down);
  //  return (up-down)<=0?0:(up-down);


  double new_exp1 = pow((m0 - m + c*(-1)*ssq),2)/(2*ssq);
  double new_erf1 = exp(new_exp1)*sqrt(2*M_PI)*(m - c*(-1)*ssq)*
    TMath::Erf(((m0 - m + c*(-1)*ssq))/(sqrt(2)*s));
  double new_exp2 = -(m0*(m0 - 2*m + 2*c*(-1)*ssq))/(2*ssq) - msq/(2*ssq);
  double new_erf2 = sqrt(2*M_PI)*(m - c*(-1)*ssq)*
    TMath::Erf(((-m + c*(-1)*ssq))/(sqrt(2)*s));
  double new_exp3 = pow((m - c*(-1)*ssq),2)/(2*ssq) - msq/(2*ssq);

  //The 2*s term can be ~ to 0 (multiplied by negative expo)
  //  double new_sum1 = 2*s + (-2*s + new_erf1)/exp(new_exp2) - exp(new_exp3)*new_erf2;

  double new_sum1 = (-2*s + new_erf1)*exp(new_exp2) - exp(new_exp3)*new_erf2;

  //  cout<<new_exp1<<" "<<new_erf1<<" "<<new_exp2<<" "<<new_erf2<<" "<<new_exp3<<" "<<new_sum1<<" "<<2*s<<" "<<(-2*s+new_erf1)/exp(new_exp2)<<" "<<exp(new_exp3)*new_erf2<<endl;

  double new_up = new_sum1;

  double new_erf3 = TMath::Erf(((-m + c*(-1)*ssq))/(sqrt(2)*s));
  double new_exp4 = exp((msq + csq*sfth)/(2*ssq) - (m*(m + 2*c*(-1)*ssq))/(2*ssq));
  double new_sum2 =  new_exp4 *
    sqrt(2*M_PI)*(-m + c*(-1)*ssq)* new_erf3 * (3*ssq + pow((m - c*(-1)*ssq),2)) ;

  double new_exp5 = exp( (m0sq + msq + 2*c*(-1)*m0*ssq + csq*sfth)/(2*ssq) - (m0*(m0 + 2*c*(-1)*ssq))/(2*ssq) - (m*(m + 2*c*(-1)*ssq))/(2*ssq));
  double new_erf4 = TMath::Erf(((m0 - m + c*(-1)*ssq))/(sqrt(2)*s));
  double new_sum3 = new_exp5*sqrt(2*M_PI)*(-m + c*(-1)*ssq)*new_erf4*(3*ssq + pow((m - c*(-1)*ssq),2));

  double new_sum4 = 2*exp(c*(-1)*m - (m*(m + 2*c*(-1)*ssq))/(2*ssq))*s* (2*ssq + pow((m - c*(-1)*ssq),2));
  double new_sum6 = 2*exp(m*(c*(-1) + m0/ssq) - (m0*(m0 + 2*c*(-1)*ssq))/(2*ssq) - (m*(m + 2*c*(-1)*ssq))/(2*ssq))*s*
    (2*ssq + (m0sq + m0*(m - c*(-1)*ssq) + pow((m - c*(-1)*ssq),2)));
  double new_nume1 = (new_sum3 + new_sum6);

  double new_sum5 = ( new_sum4 + new_sum2 - new_nume1 );
  double new_down = (1/m0sq)*new_sum5;

  double val = (1/(2*sqrt(2*M_PI)))* ( new_up - new_down);
  if(TMath::IsNaN(val)) val = 0;

  return val<=0?0:val;


}



//_____________________________________________________________________________
//Int_t RooPhysBkg::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
//{
//
//  if (matchArgs(allVars,analVars,m)) return 1;

//  return 0;

//}



//_____________________________________________________________________________
//Double_t RooPhysBkg::analyticalIntegral(Int_t code, const char* rangeName) const
//{
//  assert(code==1);
//
//  return 1;
//}


