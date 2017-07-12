//  .
// ..: P. Chang, philip@physics.ucsd.edu

// N.B.: I kept a very compact style in listing the functions in order to easily move around with {,} in vim.
//       Therefore, if one wants to add new feature please make the function extremely short as possible.
//       If the function is too long, maybe it doesn't belong here!

#include "tasutil.h"

using namespace TasUtil;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Particle class implementation
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//__________________________________________________________________________________________________
TasUtil::Particle::Particle()
{
}

//__________________________________________________________________________________________________
TasUtil::Particle::~Particle()
{
}

//__________________________________________________________________________________________________
bool TasUtil::Particle::varExists(TString name)
{
    if (std::find(var_Nam.begin(), var_Nam.end(), name) != var_Nam.end())
        return true;
    return false;
}

//__________________________________________________________________________________________________
void TasUtil::Particle::createFltVar(TString name, TString title)
{
    if (varExists(name)) printAlreadyExistsError(name, __FUNCTION__, kFLT);
    var_Nam.push_back(name);
    var_Ttl[name] = title;
    var_Typ[name] = kFLT;
    var_Flt[name] = -999;
}

//__________________________________________________________________________________________________
void TasUtil::Particle::createIntVar(TString name, TString title)
{
    if (varExists(name)) printAlreadyExistsError(name, __FUNCTION__, kINT);
    var_Nam.push_back(name);
    var_Ttl[name] = title;
    var_Typ[name] = kINT;
    var_Int[name] = -999;
}

//__________________________________________________________________________________________________
void TasUtil::Particle::createTLVVar(TString name, TString title)
{
    if (varExists(name)) printAlreadyExistsError(name, __FUNCTION__, kTLV);
    var_Nam.push_back(name);
    var_Ttl[name] = title;
    var_Typ[name] = kTLV;
    var_TLV[name].SetXYZT(0, 0, 0, 0);
}

//__________________________________________________________________________________________________
void TasUtil::Particle::createStrVar(TString name, TString title)
{
    if (varExists(name)) printAlreadyExistsError(name, __FUNCTION__, kSTR);
    var_Nam.push_back(name);
    var_Ttl[name] = title;
    var_Typ[name] = kSTR;
    var_Str[name] = "";
}

//__________________________________________________________________________________________________
void TasUtil::Particle::printError(TString name, TString msg, TString action, TasUtil::Particle::VarType type)
{
    std::cerr << "ERROR - " << msg.Data() << ", ";
    std::cerr << "name = " << name.Data() << ", ";
    std::cerr << "what was tried = " << action.Data() << ", ";
    switch (type)
    {
        case kINT: std::cerr << "type = int"            << " "; break;
        case kFLT: std::cerr << "type = float"          << " "; break;
        case kTLV: std::cerr << "type = TLorentzVector" << " "; break;
        case kSTR: std::cerr << "type = TString"        << " "; break;
        default: break;
    }
    std::cerr << std::endl;
}

//__________________________________________________________________________________________________
void TasUtil::Particle::printAlreadyExistsError(TString name, TString action, TasUtil::Particle::VarType type)
{
    printError(name, "already exists!", action, type);
}

//__________________________________________________________________________________________________
void TasUtil::Particle::printOutOfRangeError(TString name, TString action, TasUtil::Particle::VarType type)
{
    printError(name, "out-of-range error", action, type);
}

//__________________________________________________________________________________________________
void TasUtil::Particle::setFltVar(TString name, float var)
{
    try { var_Flt.at(name) = var; }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kFLT); exit(1); }
}

//__________________________________________________________________________________________________
void TasUtil::Particle::setIntVar(TString name, int var)
{
    try { var_Int.at(name) = var; }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kINT); exit(1); }
}

//__________________________________________________________________________________________________
void TasUtil::Particle::setStrVar(TString name, TString var)
{
    try { var_Str.at(name) = var; }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kSTR); exit(1); }
}

//__________________________________________________________________________________________________
void TasUtil::Particle::setTLVVar(TString name, TLorentzVector var)
{
    try { var_TLV.at(name) = var; }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kTLV); exit(1); }
}

//__________________________________________________________________________________________________
const float& TasUtil::Particle::getFltVar(TString name) const
{
    try { return var_Flt.at(name); }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kFLT); exit(2); }
    return var_Flt.at(name);
}

//__________________________________________________________________________________________________
const int& TasUtil::Particle::getIntVar(TString name) const
{
    try { return var_Int.at(name); }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kINT); exit(2); }
    return var_Int.at(name);
}

//__________________________________________________________________________________________________
const TLorentzVector& TasUtil::Particle::getTLVVar(TString name) const
{
    try { return var_TLV.at(name); }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kTLV); exit(2); }
    return var_TLV.at(name);
}

//__________________________________________________________________________________________________
const TString& TasUtil::Particle::getStrVar(TString name) const
{
    try { return var_Str.at(name); }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kSTR); exit(2); }
    return var_Str.at(name);
}

//__________________________________________________________________________________________________
TasUtil::Particle::VarType TasUtil::Particle::getType(TString name)
{
    try { return var_Typ.at(name); }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__); exit(4); }
    return var_Typ.at(name);
}

//__________________________________________________________________________________________________
void TasUtil::Particle::print()
{
    for (auto& name : var_Nam)
    {
        switch (getType(name))
        {
            case kFLT: printFltVar(name); break;
            case kINT: printIntVar(name); break;
            case kTLV: printTLVVar(name); break;
            case kSTR: printStrVar(name); break;
            case kNON: printOutOfRangeError(name, __FUNCTION__); exit(5); break;
        }
    }
}

//__________________________________________________________________________________________________
void TasUtil::Particle::printFltVar(TString name)
{
    try { std::cout << name.Data() << " kFLT " << " " << var_Flt.at(name) << std::endl; }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kFLT); exit(3); }
}

//__________________________________________________________________________________________________
void TasUtil::Particle::printIntVar(TString name)
{
    try { std::cout << name.Data() << " kINT " << " " << var_Int.at(name) << std::endl; }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kINT); exit(3); }
}

//__________________________________________________________________________________________________
void TasUtil::Particle::printTLVVar(TString name)
{
    try {
        std::cout << name.Data() << " kTLV ";
        std::cout << var_TLV.at(name).Pt () << " ";
        std::cout << var_TLV.at(name).Eta() << " ";
        std::cout << var_TLV.at(name).Phi() << " ";
        std::cout << var_TLV.at(name).M  () << " ";
        std::cout << std::endl;
    }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kTLV); exit(3); }
}

//__________________________________________________________________________________________________
void TasUtil::Particle::printStrVar(TString name)
{
    try { std::cout << name.Data() << " kSTR " << " " << var_Str.at(name) << std::endl; }
    catch (const std::out_of_range& oor) { printOutOfRangeError(name, __FUNCTION__, kSTR); exit(3); }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Auto histogram maker
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//__________________________________________________________________________________________________
TasUtil::AutoHist::AutoHist()
{
    resolution = 1000;
}

//__________________________________________________________________________________________________
TasUtil::AutoHist::~AutoHist()
{
    if (histdb.size() > 0)
        save("autohist_output.root");
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::save(TString ofilename)
{
    TFile* ofile = new TFile(ofilename, "recreate");
    ofile->cd();
    for (auto& pair_tstr_th1 : histdb)
    {
        TString name = pair_tstr_th1.first;
        TH1* hist = pair_tstr_th1.second;
        if (hist)
        {
            hist->Write();
            delete hist;
        }
    }
    histdb.clear();
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::fill(double xval, TString name, double wgt)
{
    TH1* hist = 0;
    std::pair<std::map<TString,TH1*>::iterator,bool> ret;
    ret = histdb.insert(std::pair<TString, TH1*>(name, hist));
    if (ret.second == false)
    {
        fill(xval, (*ret.first).second, wgt);
    }
    else
    {
        hist = createHist(xval, name, wgt);
        histdb[name] = hist;
    }
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::fill(double xval, TString name, double wgt, int nbin, double min, double max)
{
    TH1* hist = 0;
    std::pair<std::map<TString,TH1*>::iterator,bool> ret;
    ret = histdb.insert(std::pair<TString, TH1*>(name, hist));
    if (ret.second == false)
    {
        fill(xval, (*ret.first).second, wgt, true);
    }
    else
    {
        hist = createFixedBinHist(xval, name, wgt, nbin, min, max);
        histdb[name] = hist;
    }
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::fill(double xval, TString name, double wgt, int nbin, double* bins)
{
    TH1* hist = 0;
    std::pair<std::map<TString,TH1*>::iterator,bool> ret;
    ret = histdb.insert(std::pair<TString, TH1*>(name, hist));
    if (ret.second == false)
    {
        fill(xval, (*ret.first).second, wgt, true);
    }
    else
    {
        hist = createFixedBinHist(xval, name, wgt, nbin, bins);
        histdb[name] = hist;
    }
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::fill(
        double xval, double yval, TString name, double wgt,
        int nbx, double xm, double xM,
        int nby, double ym, double yM
        )
{
    TH1* hist = 0;
    std::pair<std::map<TString,TH1*>::iterator,bool> ret;
    ret = histdb.insert(std::pair<TString, TH1*>(name, hist));
    if (ret.second == false)
    {
        ((TH2D*) (*ret.first).second)->Fill(xval, yval, wgt);
    }
    else
    {
        hist = createFixedBinHist(xval, yval, name, wgt, nbx, xm, xM, nby, ym, yM);
        histdb[name] = hist;
    }
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::fill(
        double xval, double yval, TString name, double wgt,
        int nbx, double* bx, 
        int nby, double* by)
{
    TH1* hist = 0;
    std::pair<std::map<TString,TH1*>::iterator,bool> ret;
    ret = histdb.insert(std::pair<TString, TH1*>(name, hist));
    if (ret.second == false)
    {
        ((TH2D*) (*ret.first).second)->Fill(xval, yval, wgt);
    }
    else
    {
        hist = createFixedBinHist(xval, yval, name, wgt, nbx, bx, nby, by);
        histdb[name] = hist;
    }
}

//__________________________________________________________________________________________________
int TasUtil::AutoHist::getRes(double range)
{
    if (range > 1000) return 10;
    else if (range > 500) return 100;
    else if (range > 250) return 1000;
    else if (range > 1) return 1000;
    else return 10000;
}

//__________________________________________________________________________________________________
int TasUtil::AutoHist::getRes(TH1* h)
{
    double max = h->GetXaxis()->GetXmax();
    double min = h->GetXaxis()->GetXmin();
    int nbin = h->GetNbinsX();
    return nbin / (max - min);
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::createHist(double xval, TString name, double wgt, bool alreadyneg, int forceres)
{
    // Create a histogram with the xval as the characteristic value.
    // It takes xval, then multiply by 2 find the closest integer,
    // and give histogram of nbin where each intger has 1000 bins.
    // If it extends to negative value, blow it up
    int bound = 2 * max(((int) fabs(xval)), 1);
    bool extendneg = (alreadyneg || xval < 0) ? true : false;
    double min = extendneg ? -bound : 0;
    double max = bound;
    int res = forceres > 0 ? forceres : getRes(max - min);
    int nbin = (max - min) * res;
    TH1D* h = new TH1D(name, name, nbin, min, max);
    h->SetDirectory(0);
    h->Sumw2();
    h->Fill(xval, wgt);
    return h;
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::createFixedBinHist(double xval, TString name, double wgt, int n, double m, double M)
{
    TH1D* h = new TH1D(name, name, n, m, M);
    h->SetDirectory(0);
    h->Sumw2();
    h->Fill(xval, wgt);
    return h;
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::createFixedBinHist(double xval, TString name, double wgt, int n, double* bin)
{
    TH1D* h = new TH1D(name, name, n, bin);
    h->SetDirectory(0);
    h->Sumw2();
    h->Fill(xval, wgt);
    return h;
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::createFixedBinHist(
        double xval, double yval, TString name, double wgt,
        int xn, double xm, double xM,
        int yn, double ym, double yM)
{
    TH2D* h = new TH2D(name, name, xn, xm, xM, yn, ym, yM);
    h->SetDirectory(0);
    h->Sumw2();
    h->Fill(xval, yval, wgt);
    return h;
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::createFixedBinHist(
        double xval, double yval, TString name, double wgt,
        int xn, double* xb,
        int yn, double* yb)
{
    TH2D* h = new TH2D(name, name, xn, xb, yn, yb);
    h->SetDirectory(0);
    h->Sumw2();
    h->Fill(xval, yval, wgt);
    return h;
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::fill(double xval, TH1*& h, double wgt, bool norebinning)
{
    if ( (xval >= h->GetXaxis()->GetXmax() || xval <= h->GetXaxis()->GetXmin()) && !norebinning )
    {
        TH1D* hist = (TH1D*) createHist(xval, h->GetName(), wgt, h->GetXaxis()->GetXmin() < 0);
        transfer(hist, h);
        delete h;
        h = hist;
    }
    else
    {
        h->Fill(xval, wgt);
    }
}

//__________________________________________________________________________________________________
void TasUtil::AutoHist::transfer(TH1* tohist, TH1* fromhist)
{
    for (int ibin = 0; ibin <= fromhist->GetNbinsX(); ++ibin)
    {
        double wgt = fromhist->GetBinContent(ibin);
        double err = fromhist->GetBinError(ibin);
        double val = fromhist->GetBinCenter(ibin);
        if (fabs(wgt))
        {
            int ibin = tohist->FindBin(val);
            double curerror = tohist->GetBinError(ibin);
            tohist->Fill(val, wgt);
            tohist->SetBinError(ibin, sqrt(curerror*curerror + err*err));
        }
    }
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::get(TString name)
{
    try
    {
        return histdb.at(name);
    }
    catch (const std::out_of_range& oor)
    {
        TasUtil::print("Warning! No histogram named " + name + " found!");
        return 0;
    }
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::crop(TH1* orighist, int nbin, double min, double max)
{
    // Checks whether this is uniform binning
    // https://root-forum.cern.ch/t/taxis-getxbins/19598/2
    if ((*(orighist->GetXaxis()->GetXbins())).GetSize() == 0)
    {
        int    orignbin = orighist->GetNbinsX();
        double origmin = orighist->GetXaxis()->GetXmin();
        double origmax = orighist->GetXaxis()->GetXmax();
        double origbinsize = (origmax - origmin) / orignbin;
        if (std::remainder(fabs(origmin - max), origbinsize) < 1e-9 && std::remainder(fabs(origmin - min), origbinsize) < 1e-9 )
        {
            TH1D* rtnhist = new TH1D(orighist->GetName(), orighist->GetName(), nbin, min, max);
            rtnhist->Sumw2();
            rtnhist->SetDirectory(0);
            transfer(rtnhist, orighist);
            return rtnhist;
        }
        // Not good, I can't crop it.
        else
        {
            TasUtil::error("You are trying to crop a histogram to a binning where you can't!", __FUNCTION__);
        }
    }
    return 0;
}

//__________________________________________________________________________________________________
TH1* TasUtil::AutoHist::hadd(TH1* hist1, TH1* hist2)
{
    double min1 = hist1->GetXaxis()->GetXmin();
    double max1 = hist1->GetXaxis()->GetXmax();
    double min2 = hist2->GetXaxis()->GetXmin();
    double max2 = hist2->GetXaxis()->GetXmax();
    if (min1 <= min2 && max1 >= max2)
    {
        transfer(hist1, hist2);
        return hist1;
    }
    else if (min2 <= min1 && max2 >= max1)
    {
        transfer(hist2, hist1);
        return hist2;
    }
    else if (max1 >= max2 && min1 >= min2)
    {
        TH1* newhist = createHist(max1, hist1->GetName(), 0, true, getRes(hist1));
        transfer(newhist, hist1);
        transfer(newhist, hist2);
        return newhist;
    }
    else if (max2 >= max1 && min2 >= min1)
    {
        TH1* newhist = createHist(max2, hist2->GetName(), 0, true, getRes(hist2));
        transfer(newhist, hist1);
        transfer(newhist, hist2);
        return newhist;
    }
    else
    {
        TasUtil::error("it should never reach here! send email to philip@physics.ucsd.edu", __FUNCTION__);
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Data extractor
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
TasUtil::Particles TasUtil::get(
        std::function<unsigned int()> size,
        std::function<bool(int)> pass,
        std::function<TasUtil::Particle(int)> extractor)
{
    // The return object
    Particles particles;
    for (unsigned int idx_in_vec_ttree = 0; idx_in_vec_ttree < size(); ++idx_in_vec_ttree)
    {
        if (pass(idx_in_vec_ttree))
        {
            particles.push_back(extractor(idx_in_vec_ttree));
        }
    }
    return particles;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Math functions
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//_________________________________________________________________________________________________
TLorentzVector TasUtil::Math::TLV(TasUtil::Math::LorentzVector p)
{
    TLorentzVector tlv;
    tlv.SetPxPyPzE(p.x(), p.y(), p.z(), p.e());
    return tlv;
}

//_________________________________________________________________________________________________
TasUtil::Math::LorentzVector TasUtil::Math::LV(TLorentzVector p)
{
    TasUtil::Math::LorentzVector lv;
    lv.SetPxPyPzE(p.X(), p.Y(), p.X(), p.E());
    return lv;
}

//_________________________________________________________________________________________________
TLorentzVector TasUtil::Math::TLVXYZE(float x, float y, float z, float e)
{
    TLorentzVector tlv;
    tlv.SetPxPyPzE(x, y, z, e);
    return tlv;
}

//_________________________________________________________________________________________________
TasUtil::Math::LorentzVector TasUtil::Math::LVXYZE (float x, float y, float z, float e)
{
    TasUtil::Math::LorentzVector lv;
    lv.SetPxPyPzE(x, y, z, e);
    return lv;
}

//_________________________________________________________________________________________________
TLorentzVector TasUtil::Math::TLVPtEtaPhiM(float x, float y, float z, float e)
{
    TLorentzVector tlv;
    tlv.SetPtEtaPhiM(x, y, z, e);
    return tlv;
}

//_________________________________________________________________________________________________
TLorentzVector TasUtil::Math::TLVPtEtaPhiE(float x, float y, float z, float e)
{
    TLorentzVector tlv;
    tlv.SetPtEtaPhiE(x, y, z, e);
    return tlv;
}

//_________________________________________________________________________________________________
float TasUtil::Math::MT(TLorentzVector p4, TLorentzVector metp4)
{
    /// Compute MT
    float mt = sqrt(2 * p4.Pt() * metp4.Pt() * (1 - cos(metp4.DeltaPhi(p4))));
    return mt;
}

//_________________________________________________________________________________________________
TLorentzVector TasUtil::Math::TLVMET(float met, float met_phi)
{
    TLorentzVector tlv;
    tlv.SetPtEtaPhiE(met, 0, met_phi, met);
    return tlv;
}

//_________________________________________________________________________________________________
TasUtil::Math::LorentzVector  TasUtil::Math::METLV(float met, float met_phi)
{
    // For some odd reason LorentzVector of float that SNT uses only work with SetPxPyPzE
    TLorentzVector tlv = TLVMET(met, met_phi);
    LorentzVector lv;
    lv.SetPxPyPzE(lv.x(), lv.y(), lv.z(), lv.e());
    return lv;
}

float TasUtil::Math::DEta(TLorentzVector a, TLorentzVector b) { return fabs(a.Eta() - b.Eta()); }
float TasUtil::Math::DPhi(TLorentzVector a, TLorentzVector b) { return fabs(a.DeltaPhi(b));     }
float TasUtil::Math::DR  (TLorentzVector a, TLorentzVector b) { return fabs(a.DeltaR(b));       }
float TasUtil::Math::DPt (TLorentzVector a, TLorentzVector b) { return fabs(a.Pt()-b.Pt());     }
float TasUtil::Math::Mass(TLorentzVector a, TLorentzVector b) { return (a + b).M();             }
float TasUtil::Math::Pt  (TLorentzVector a, TLorentzVector b) { return (a + b).Pt();            }
float TasUtil::Math::MT  (TLorentzVector p4  , float met, float met_phi) { return MT(p4, TLVMET(met, met_phi)); }

float TasUtil::Math::DEta(TasUtil::Math::LorentzVector a, float                        b) { return DEta(TLV(a), TLVPtEtaPhiE(1,b,0,1)); }
float TasUtil::Math::DPhi(TasUtil::Math::LorentzVector a, float                        b) { return DPhi(TLV(a), TLVPtEtaPhiE(1,0,b,1)); }
float TasUtil::Math::DEta(TasUtil::Math::LorentzVector a, TasUtil::Math::LorentzVector b) { return DEta(TLV(a), TLV(b)               ); }
float TasUtil::Math::DPhi(TasUtil::Math::LorentzVector a, TasUtil::Math::LorentzVector b) { return DPhi(TLV(a), TLV(b)               ); }
float TasUtil::Math::DR  (TasUtil::Math::LorentzVector a, TasUtil::Math::LorentzVector b) { return DR  (TLV(a), TLV(b)               ); }
float TasUtil::Math::DPt (TasUtil::Math::LorentzVector a, TasUtil::Math::LorentzVector b) { return DPt (TLV(a), TLV(b)               ); }
float TasUtil::Math::Mass(TasUtil::Math::LorentzVector a, TasUtil::Math::LorentzVector b) { return Mass(TLV(a), TLV(b)               ); }
float TasUtil::Math::Pt  (TasUtil::Math::LorentzVector a, TasUtil::Math::LorentzVector b) { return Pt  (TLV(a), TLV(b)               ); }
float TasUtil::Math::MT  (TasUtil::Math::LorentzVector a, TasUtil::Math::LorentzVector b) { return MT  (TLV(a), TLV(b)               ); }
float TasUtil::Math::MT  (TasUtil::Math::LorentzVector a, float met, float met_phi) { return MT(TLV(a), TLVMET(met, met_phi)); }

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Printing utilities
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//_________________________________________________________________________________________________
void TasUtil::print(TString msg, const char* fname, int flush_before, int flush_after)
{
    /// printf replacement
    if (flush_before) printf("\n");
    if (strlen(fname) == 0) printf("TasUtil:: %s\n", msg.Data());
    else printf("TasUtil:: [in func %s()] %s\n", fname, msg.Data());
    if (flush_after) printf("\n");
    fflush(stdout);
}

//_________________________________________________________________________________________________
void TasUtil::warning(TString msg, const char* fname)
{
    /// warning message. Does not exit the program.
    print("WARNING - "+msg+".\n", fname);
}

//_________________________________________________________________________________________________
void TasUtil::error(TString msg, const char* fname, int is_error)
{
    /// Error & exit
    if (!is_error)
        return;
    //exit();
    print("ERROR - "+msg+", exiting.\n", fname);
    abort();
}

//_________________________________________________________________________________________________
void TasUtil::start(int q, int sleep_time)
{
    /// Fun start (from TM Hong's BaBar days)
    if (q)
        return;
    print(" _");
    print("/\\\\");
    print("\\ \\\\  \\__/ \\__/");
    print(" \\ \\\\ (oo) (oo)  Here we come!");
    print("  \\_\\\\/~~\\_/~~\\_");
    print(" _.-~===========~-._");
    print("(___________________)");
    print("      \\_______/");
    print();
    print("System info:");
    gSystem->Exec("hostname");
    gSystem->Exec("uname -a");
    gSystem->Exec("date");
    gSystem->Exec("whoami");
    gSystem->Exec("pwd");
    fflush(stdout);
    if (sleep_time>0)
        sleep(sleep_time);
}

//_________________________________________________________________________________________________
void TasUtil::announce(TString msg, Int_t q)
{
    /// Fun message presented by the moose
    if (q)
        return;
    // Random
    srand(time(NULL));      // Init rand seed
    Int_t   r = rand()%10+1;// Generate rand number
    Int_t   moose = r>4 ? 1 : 0;
    // Moose type
    TString eyes  = "open";
    if      (r==9) eyes = "closed";
    else if (r==8) eyes = "dead";
    else if (r==7) eyes = "small";
    else if (r==7) eyes = "sunny";
    else if (r==6) eyes = "calc";
    else if (r==4) eyes = "crazy";
    else if (r==3) eyes = "vampire";
    else if (r==2) eyes = "rich";
    else if (r==1) eyes = "sick";
    print();
    if      (msg.Length()>0) print("________________________________________");
    if      (msg.Length()>0) print(msg);
    if      (msg.Length()>0) print("--O-------------------------------------");
    if      (moose)          print("  O    \\_\\_    _/_/");
    if      (moose)          print("   O       \\__/");
    else                     print("   O       ^__^");
    if      (eyes=="open")   print("    o      (oo)\\_______");
    else if (eyes=="closed") print("    o      (==)\\_______");
    else if (eyes=="dead")   print("    o      (xx)\\_______");
    else if (eyes=="small")  print("    o      (..)\\_______");
    else if (eyes=="sunny")  print("    o      (66)\\_______");
    else if (eyes=="calc")   print("    o      (00)\\_______");
    else if (eyes=="crazy")  print("    o      (**)\\_______");
    else if (eyes=="vampire")print("    o      (@@)\\_______");
    else if (eyes=="rich")   print("    o      ($$)\\_______");
    else if (eyes=="sick")   print("    o      (++)\\_______");
    if (true)                print("     o     (__)\\       )\\/\\");
    if      (eyes=="dead")   print("            U  ||----w |");
    else if (eyes=="crazy")  print("            U  ||----w |");
    else if (eyes=="sick")   print("            U  ||----w |");
    else if (eyes=="vampire")print("            VV ||----w |");
    else                     print("               ||----w |");
    if (true)                print("               ||     ||");
    print();
    //sleep(0);
}

//_________________________________________________________________________________________________
void TasUtil::end(int q)
{
    /// Fun exit (from TM Hong's BaBar days)
    if (q)
        return;
    print();
    print("   \\__/    \\__/");
    print("   (oo)    (oo)");
    print("(\\//~~\\\\  //~~\\\\");
    print(" \\/\\__//  \\\\__//\\");
    print("   ||||    ||\\\\ Who cares!");
    print("__ |||| __ |||| ___");
    print("  (_)(_)  (_)(_)");
    print();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// TTree++ (TTreeX) class
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//_________________________________________________________________________________________________
TasUtil::TTreeX::TTreeX()
{
}

//_________________________________________________________________________________________________
TasUtil::TTreeX::~TTreeX()
{
}

//_________________________________________________________________________________________________
void* TasUtil::TTreeX::getValPtr(TString brname)
{
    TBranch* br = GetBranch(brname);
    unsigned int nleaves = br->GetListOfLeaves()->GetEntries();
    if (nleaves != 1)
        TasUtil::error("# of leaf for this branch="
                + brname + " is not equals to 1!", __FUNCTION__);
    if (!(((TLeaf*) br->GetListOfLeaves()->At(0))->GetValuePointer()))
        GetEntry(0);
    return ((TLeaf*) br->GetListOfLeaves()->At(0))->GetValuePointer();
}

//eof
