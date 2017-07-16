#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH1F.h"
#include "THStack.h"
#include "TString.h"
#include "TLegend.h"
#include "TLegendEntry.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>

typedef std::pair<TH1*, TH1*> Hist;
typedef std::vector<Hist> Hists;

// =============
// global option
// =============

// ==================
// global th1 handler
// ==================
std::vector<TH1*> new_hists;

// ============================
// global TPad property handler
// ============================
std::map<TPad*, bool> map_pad_drawn;

//_________________________________________________________________________________________________
//Parse Parameters from options input string
inline std::vector <TString> GetParms( std::string blah )
{
    int a = -1;
    int length = blah.length();
    std::vector <TString> options;
    
    while ( a < length )
    {
        int tempn = a;
        a = blah.find( "--", tempn + 1 );
        
        if ( a <= tempn )
            break;
            
        int b = blah.find( "--", a + 3 ) - 1;
        unsigned int myLength = b - a - 2;
        std::string mySubstring;
        
        if ( a + 2 + myLength > blah.length() )
            mySubstring = blah.substr( a + 2 );
        else
            mySubstring = blah.substr( a + 2, b - a - 2 );
            
        options.push_back( TString( mySubstring ) );
    }
    
    return options;
}

//_________________________________________________________________________________________________
std::vector<double> getBinInfo( std::vector<TH1*> hists )
{
    std::vector<double> bininfo;
    bininfo.push_back( hists[0]->GetNbinsX() );
    bininfo.push_back( hists[0]->GetXaxis()->GetXmin() );
    bininfo.push_back( hists[0]->GetXaxis()->GetXmax() );
    return bininfo;
}

//_________________________________________________________________________________________________
TH1* histWithFullError( TH1* nominal, TH1* error )
{
    TH1D* nominal_with_full_error = (TH1D*) nominal->Clone(nominal->GetName());
    if ( !nominal_with_full_error->GetSumw2N() )
        nominal_with_full_error->Sumw2();
    nominal_with_full_error->SetDirectory( 0 );
    
    for ( unsigned int ibin = 0; ibin <= nominal->GetNbinsX() + 1; ibin++ )
    {
        double content = nominal->GetBinContent( ibin );
        double nominal_hist_error = nominal->GetBinError( ibin );
        double additional_error = error ? error->GetBinContent( ibin ) : 0;
        double all_error = error ? sqrt( pow( nominal_hist_error, 2 ) + pow( additional_error, 2 ) )
                                 : nominal_hist_error;
        nominal_with_full_error->SetBinContent( ibin, content );
        nominal_with_full_error->SetBinError( ibin, all_error );
    }
    
    return nominal_with_full_error;
}

//_________________________________________________________________________________________________
TH1* histWithFullError( Hist& hist_pair )
{
    return histWithFullError( hist_pair.first, hist_pair.second );
}

//_________________________________________________________________________________________________
std::vector<TH1*> histsWithFullError( Hists& hist_pairs )
{
    std::vector<TH1*> hists;
    for ( auto& hist_pair : hist_pairs )
    {
        hists.push_back( histWithFullError( hist_pair ) );
    }
    return hists;
}

//_________________________________________________________________________________________________
TH1* getSumHists( std::vector<TH1*> hists )
{
    TH1D* sum_hist = (TH1D*) hists[0]->Clone();
    if ( !sum_hist->GetSumw2N() )
        sum_hist->Sumw2();
    sum_hist->SetDirectory( 0 );
    sum_hist->Reset();

    for ( auto& hist : hists )
        sum_hist->Add( hist );

    return sum_hist;
}

//_________________________________________________________________________________________________
TH1* getTotalBkgHists( std::vector<TH1*> hists )
{
    TH1* sum_hist = getSumHists( hists );
    if ( !gROOT->GetColor( 9999 ) )
        new TColor( 9999, 0.1, 0.2, 0.3, "", 0. ); // alpha = 0.5
    sum_hist->SetMarkerStyle( 1 );
    sum_hist->SetMarkerSize( 0 );
    sum_hist->SetMarkerColor( 9999 );
    sum_hist->SetLineColor( 1 );
    sum_hist->SetLineColor( 1 );
    sum_hist->SetFillColor( 1 );
    sum_hist->SetFillStyle( 3245 );

    return sum_hist;
}

//_________________________________________________________________________________________________

//_________________________________________________________________________________________________
THStack* getStack( std::vector<TH1*> hists )
{
    THStack *stack = new THStack();
    stack->SetName( "stack" );
    stack->SetTitle( "" );
    stack->SetMaximum( 86510.64 );

    for ( auto& hist : hists )
        stack->Add( hist, "" );

    return stack;
}

//_________________________________________________________________________________________________
void stylizeAxes( TH1* h, TPad* pad )
{
    double pad_h = pad->GetAbsHNDC();
    double pad_w = pad->GetAbsWNDC();

    double WidthPixel =  pad->GetWw();

    h->SetMinimum( 0 );
    h->SetMaximum( 90836.17 );
    h->SetDirectory( 0 );
    h->SetStats( 0 );
    h->SetFillColor( -1 );
    h->GetXaxis ( )->SetNdivisions  ( 505 );
    h->GetXaxis ( )->SetLabelFont   ( 42 );
    h->GetXaxis ( )->SetLabelOffset ( 0.0225 );
    h->GetXaxis ( )->SetLabelSize   ( 0.063 );
    h->GetXaxis ( )->SetTitleSize   ( 0.063 );
    h->GetXaxis ( )->SetTitle       ( "XVar" );
    h->GetXaxis ( )->SetTickLength  ( -0.02 );
    h->GetXaxis ( )->SetTitleOffset ( 1.2 );
    h->GetXaxis ( )->SetTitleFont   ( 42 );
    h->GetYaxis ( )->SetNdivisions  ( 505 );
    h->GetYaxis ( )->SetLabelFont   ( 42 );
    h->GetYaxis ( )->SetLabelOffset ( 0.0225 );
    h->GetYaxis ( )->SetLabelSize   ( 0.063 );
    h->GetYaxis ( )->SetTitleSize   ( 0.063 );
    h->GetYaxis ( )->SetTitle       ( "YVar" );
    h->GetYaxis ( )->SetTickLength  ( -0.02 );
    h->GetYaxis ( )->SetTitleOffset ( 2.1 );
    h->GetYaxis ( )->SetTitleFont   ( 42 );
    h->GetZaxis ( )->SetLabelFont   ( 42 );
    h->GetZaxis ( )->SetLabelSize   ( 0.035 );
    h->GetZaxis ( )->SetTitleSize   ( 0.035 );
    h->GetZaxis ( )->SetTitleFont   ( 42 );
}

//_________________________________________________________________________________________________
void stylizeRatioAxes( TH1* h, TPad* pad )
{
    double pad_h = pad->GetAbsHNDC();
    double pad_w = pad->GetAbsWNDC();

    double WidthPixel =  pad->GetWw();

    float padding_scale_factor = 0.7 / 0.3;

    h->SetTitle("");
    h->SetMinimum( 0.3 );
    h->SetMaximum( 1.7 );
    h->SetEntries( 17204.55 );
    h->SetMarkerStyle( 19 );
    h->GetXaxis()->SetNdivisions( 505 );
    h->GetXaxis()->SetLabelFont( 42 );
    h->GetXaxis()->SetLabelOffset( 0.0225 * padding_scale_factor );
    h->GetXaxis()->SetLabelSize( 0.063 * padding_scale_factor );
    h->GetXaxis()->SetTitleSize( 0.063 * padding_scale_factor );
    h->GetXaxis()->SetTickLength( -0.02 * padding_scale_factor );
    h->GetXaxis()->SetTitleOffset( 1.2 );
    h->GetXaxis()->SetTitleFont( 42 );
    h->GetXaxis()->SetTitle( "Ratio" );
    h->GetYaxis()->SetNdivisions( 505 );
    h->GetYaxis()->SetLabelFont( 42 );
    h->GetYaxis()->SetLabelOffset( 0.0225 );
    h->GetYaxis()->SetLabelSize( 0.063 * padding_scale_factor );
    h->GetYaxis()->SetTitleSize( 0.063 * padding_scale_factor );
    h->GetYaxis()->SetTickLength( -0.02 * ( 10. / 6. ) );
    h->GetYaxis()->SetTitleOffset( 2.1 / padding_scale_factor );
    h->GetYaxis()->SetTitleFont( 42 );
    h->GetYaxis()->SetTitle( "Ratio" );
    h->GetZaxis()->SetLabelFont( 42 );
    h->GetZaxis()->SetLabelSize( 0.035 );
    h->GetZaxis()->SetTitleSize( 0.035 );
    h->GetZaxis()->SetTitleFont( 42 );

}

//_________________________________________________________________________________________________
void draw( TH1* h, TString option, TPad* pad )
{
    pad->cd();

    if ( !map_pad_drawn[pad] )
    {
        h->Draw( option.Data() );
        stylizeAxes( h, pad );
        map_pad_drawn[pad] = true;
    }
    else
    {
        h->Draw( (option + "same").Data() );
    }
}

//_________________________________________________________________________________________________
void draw( THStack* h, TString option, TPad* pad )
{
    pad->cd();

    if ( !map_pad_drawn[pad] )
    {
        h->Draw( option.Data() );
        stylizeAxes( h->GetHistogram(), pad );
        map_pad_drawn[pad] = true;
    }
    else
    {
        h->Draw( (option + "same").Data() );
    }
}

//_________________________________________________________________________________________________
void drawData( TH1* h, TString option, TPad* pad )
{
    h->SetMarkerStyle(19);
    h->SetMarkerSize(1);
    h->SetLineColor(1);

    pad->cd();

    if ( !map_pad_drawn[pad] )
    {
        h->Draw( option.Data() );
        stylizeAxes( h, pad );
        map_pad_drawn[pad] = true;
    }
    else
    {
        h->Draw( (option + "same").Data() );
    }
}

//_________________________________________________________________________________________________
void drawRatio( TH1* h, TString option, TPad* pad )
{
    h->SetMarkerStyle(19);
    h->SetMarkerSize(1);
    h->SetLineColor(1);

    pad->cd();

    if ( !map_pad_drawn[pad] )
    {
        h->Draw( option.Data() );
        stylizeRatioAxes( h, pad );
        map_pad_drawn[pad] = true;
    }
    else
    {
        h->Draw( (option + "same").Data() );
    }
}

//_________________________________________________________________________________________________
void addToLegend( TH1* h, TLegend* legend, const char* option )
{
    TString name = h->GetName();
    legend->AddEntry( h, name.Data(), option );
}

//_________________________________________________________________________________________________
std::vector<TH1*> getRatioHists( std::vector<TH1*> data_hists, std::vector<TH1*> bkg_hists )
{
    std::vector<TH1*> ratio_hists;
    
    for ( unsigned int ihist = 0; ihist < data_hists.size(); ++ihist )
    {
        TH1D* ratio = ( TH1D* ) data_hists[ihist]->Clone( data_hists[ihist]->GetName() );
        
        if ( !ratio->GetSumw2N() )
            ratio->Sumw2();
            
        ratio->SetDirectory( 0 );
        
        ratio->Divide( data_hists[ihist], bkg_hists.size() == 1 ? bkg_hists[0] : bkg_hists[ihist] );
        ratio_hists.push_back( ratio );
    }
    
    return ratio_hists;
}

//_________________________________________________________________________________________________
std::vector<TH1*> getRatioHists( std::vector<TH1*> data_hists, TH1* bkg_hist )
{
    std::vector<TH1*> bkg_hists;
    bkg_hists.push_back( bkg_hist );

    return getRatioHists( data_hists, bkg_hists );
}

//_________________________________________________________________________________________________
void drawLegend( std::vector<TH1*> data_hists, std::vector<TH1*> bkg_hists, std::vector<TH1*> sig_hists, TPad* pad )
{
    TLegend* leg = new TLegend( 0, 0, 1, 0.65, NULL, "brNDC" );
    leg->SetBorderSize( 0 );
    leg->SetLineColor( 0 );
    leg->SetLineStyle( 1 );
    leg->SetLineWidth( 1 );
    leg->SetFillColor( 0 );
    leg->SetFillStyle( 0 );
    leg->SetMargin( 0.13 );
    leg->SetEntrySeparation( 0.065 );
    leg->SetTextSize( 0.062 * 6. / 4. );
    
    for ( auto& data_hist : data_hists )
        addToLegend( data_hist, leg, "ep" );
        
    for ( auto& bkg_hist : bkg_hists )
        addToLegend( bkg_hist, leg, "f" );
        
    for ( auto& sig_hist : sig_hists )
        addToLegend( sig_hist, leg, "l" );
        
    int entries = leg->GetNRows();
    leg->SetY1( 0.65 - leg->GetEntrySeparation() * entries );
    pad->cd();
    leg->Draw();
}

//_________________________________________________________________________________________________
void drawLogos( TPad* pad )
{
    pad->cd();
    TLatex* tex = new TLatex( 0.5, 0.93, "35.9 fb^{-1} (13 TeV)" );
    tex->SetNDC();
    tex->SetTextFont( 42 );
    tex->SetTextSize( 0.062 );
    tex->SetLineWidth( 2 );
    tex->Draw();
    pad->Modified();
}

//_________________________________________________________________________________________________
void plotmaker(
    std::string options_string,
    Hists datas_pair_in,
    Hists bkgs_pair_in,
    Hists sigs_pair_in = Hists()
)
{
    // ~-~-~-~-~-~-~-~
    // gStyle settings
    // ~-~-~-~-~-~-~-~
    gStyle->SetOptStat( 0 );

    // ~-~-~-~-~-~-~
    // Parse options
    // ~-~-~-~-~-~-~
    std::vector<TString> Options = GetParms( options_string );
    
    // ~-~-~-~-~-~-~-~
    // The main canvas
    // ~-~-~-~-~-~-~-~
    // For some reason, even though I ask 800x824, it outputs 796x796 pdf or png files.
    // In x-direction 4 pixels are added
    // In y-direction 28 pixels are added
    TCanvas* canvas = new TCanvas( "canvas", "canvas", 0, 0, 1004, 828 );
    canvas->SetHighLightColor( 2 );
    canvas->Range( 0, 0, 1, 1 );
    canvas->SetFillColor( -1 );
    canvas->SetBorderMode( 0 );
    canvas->SetBorderSize( 0 );
    canvas->SetFrameBorderMode( 0 );
    canvas->SetFrameBorderSize( 0 );
    
    // ~-~-~-~-~-~-~
    // The main TPad
    // ~-~-~-~-~-~-~
    // The bottom main pain with 70% height in y-axis
    TPad* pad0 = new TPad( "pad0", "pad0", 0, 0, 1 - 0.4, 0.7 );
    pad0->Draw();
    pad0->cd();
    pad0->Range( -80, -25953.19, 320, 103812.8 );
    pad0->SetFillColor( -1 );
    pad0->SetFillStyle( 4000 );
    pad0->SetBorderMode( 0 );
    pad0->SetBorderSize( 2 );
    pad0->SetLeftMargin( 150. / 600. );
    pad0->SetRightMargin( 50. / 600. );
    pad0->SetBottomMargin( 0.2 );
    pad0->SetTopMargin( 0.2 / 0.7  - 0.2 );
    pad0->SetFrameBorderMode( 0 );
    pad0->SetFrameBorderSize( 0 );
    pad0->SetFrameBorderMode( 0 );
    pad0->SetFrameBorderSize( 0 );
    
    // ~-~-~-~-~-~-~-~
    // The Legend TPad
    // ~-~-~-~-~-~-~-~
    // TPad for the TLegend
    canvas->cd();
    TPad* pad1 = new TPad( "pad1", "pad1", 1 - 0.40, 0, 1, 1 );
    pad1->Draw();
    
    // ~-~-~-~-~-~-~-~
    // The Ratio TPad
    // ~-~-~-~-~-~-~-~
    canvas->cd();
    TPad *pad2 = new TPad( "pad2", "pad2", 0, 0.7, 0.6, 1 );
    pad2->Draw();
    pad2->cd();
    pad2->Range( -80, -0.82, 320, 1.98 );
    pad2->SetFillColor( -1 );
    pad2->SetFillStyle( 4000 );
    pad2->SetBorderMode( 0 );
    pad2->SetBorderSize( 2 );
    pad2->SetGridy();
    pad2->SetLeftMargin( 150. / 600. );
    pad2->SetRightMargin( 50. / 600. );
    pad2->SetBottomMargin( 0.4 );
    pad2->SetFrameBorderMode( 0 );
    pad2->SetFrameBorderSize( 0 );
    pad2->SetFrameBorderMode( 0 );
    pad2->SetFrameBorderSize( 0 );

    // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    // Sanity check that I even have histograms to work with
    // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    if ( datas_pair_in.size() + bkgs_pair_in.size() + sigs_pair_in.size() == 0 )
    {
        std::cout << "[plotmaker::] No histograms were provided!" << std::endl;
        std::cout << "[plotmaker::] What the hell do you want me to do with no TH1's?" << std::endl;
        return;
    }
    
    // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    // Turn the pair into a more workable "list of histograms"
    // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    std::vector<TH1*> data_hists = histsWithFullError( datas_pair_in );
    std::vector<TH1*> bkg_hists = histsWithFullError( bkgs_pair_in );
    std::vector<TH1*> sig_hists = histsWithFullError( sigs_pair_in );
    
    // ~-~-~-~-~-~-~-~
    // Draw background
    // ~-~-~-~-~-~-~-~
    if ( bkg_hists.size() )
    {
        // ~-~-~-~-~-~-~-~-~-
        // Main bkg stack plot
        // ~-~-~-~-~-~-~-~-~-
        THStack* stack = getStack( bkg_hists );
        draw( stack, "hist", pad0 );
        
        // ~-~-~-~-~-~-~-~-~-
        // Total bkg histogram
        // ~-~-~-~-~-~-~-~-~-
        TH1* total = getTotalBkgHists( bkg_hists );
        draw( total, "hist p", pad0 );
        draw( total, "e2psame ", pad0 );
    }
    
    // ~-~-~-~-~
    // Draw data
    // ~-~-~-~-~
    if ( data_hists.size() )
    {
        for ( auto& data_hist : data_hists )
            drawData( data_hist, "ex0p", pad0 );
    }
    
    // ~-~-~-~-~-~
    // Draw legend
    // ~-~-~-~-~-~
    drawLegend( data_hists, bkg_hists, sig_hists, pad1 );
    
    // ~-~-~-~-~-~-~-~-~-~-~-
    // Draw CMS/Lumi/COM logo
    // ~-~-~-~-~-~-~-~-~-~-~-
    drawLogos( pad0 );
    
    // ~-~-~-~-~-~-~-~
    // Draw Ratio plot
    // ~-~-~-~-~-~-~-~
    std::vector<TH1*> ratio_hists = getRatioHists( data_hists, getTotalBkgHists( bkg_hists ) );

    for (auto& ratio_hist : ratio_hists )
        drawRatio( ratio_hist, "ex0p", pad2 );

    // ~-~-~-~-~-
    // Save plots
    // ~-~-~-~-~-
    canvas->SaveAs( "test.png" );
    canvas->SaveAs( "test.pdf" );
    pad0->cd();
    pad0->SaveAs( "test_bare.pdf" );
}

//_________________________________________________________________________________________________
void plotmaker(
    std::string options_string,
    TH1* data_in,
    Hists bkgs_pair_in,
    Hists sigs_pair_in = Hists()
)
{
    Hists datas_pair_in;
    datas_pair_in.push_back( std::pair<TH1*, TH1*>( data_in, 0 ) );
    
    plotmaker( options_string, datas_pair_in, bkgs_pair_in, sigs_pair_in );
}

//_________________________________________________________________________________________________
void plotmaker(
    std::string options_string,
    TH1* data_in,
    std::vector<TH1*> bkg_hists,
    std::vector<TH1*> sig_hists = std::vector<TH1*>()
)
{

    Hists datas_pair_in;
    datas_pair_in.push_back( std::pair<TH1*, TH1*>( data_in, 0 ) );
    
    Hists bkgs_pair_in;
    
    for ( auto& bkg_hist : bkg_hists )
        bkgs_pair_in.push_back( std::pair<TH1*, TH1*>( bkg_hist, 0 ) );
        
    Hists sigs_pair_in;
    
    for ( auto& sig_hist : sig_hists )
        sigs_pair_in.push_back( std::pair<TH1*, TH1*>( sig_hist, 0 ) );
        
    plotmaker( options_string, datas_pair_in, bkgs_pair_in, sigs_pair_in );
}

////_________________________________________________________________________________________________
//// Get binning information from histogram
//std::vector<double> getBinInfo( Hists& datas_pair_in, Hists& bkgs_pair_in, Hists& sigs_pair_in )
//{
//    std::vector<double> bininfo;
//    
//    if ( datas_pair_in.size() != 0 )
//    {
//        bininfo.push_back( datas_pair_in[0].first->GetNbinsX() );
//        bininfo.push_back( datas_pair_in[0].first->GetXaxis()->GetXmin() );
//        bininfo.push_back( datas_pair_in[0].first->GetXaxis()->GetXmax() );
//        return bininfo;
//    }
//    else if ( bkgs_pair_in.size() != 0 )
//    {
//        bininfo.push_back( datas_pair_in[0].first->GetNbinsX() );
//        bininfo.push_back( datas_pair_in[0].first->GetXaxis()->GetXmin() );
//        bininfo.push_back( datas_pair_in[0].first->GetXaxis()->GetXmax() );
//        return bininfo;
//    }
//    else if ( sigs_pair_in.size() != 0 )
//    {
//        bininfo.push_back( datas_pair_in[0].first->GetNbinsX() );
//        bininfo.push_back( datas_pair_in[0].first->GetXaxis()->GetXmin() );
//        bininfo.push_back( datas_pair_in[0].first->GetXaxis()->GetXmax() );
//        return bininfo;
//    }
//    else
//    {
//        std::cout << "[plotmaker::] Failed retrieving histogram binning information!" << std::endl;
//        std::cout << "[plotmaker::] No histograms were found at all! WTH?" << std::endl;
//        return bininfo;
//    }
//}
