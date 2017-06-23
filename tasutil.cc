//  .
// ..: P. Chang, philip@physics.ucsd.edu

// N.B.: I kept a very compact style in listing the functions in order to easily move around with {,} in vim.
//       Therefore, if one wants to add new feature please make the function extremely short as possible.
//       If the function is too long, maybe it doesn't belong here!

#include "tasutil.h"

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
void TasUtil::exit(int q)
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
//eof
