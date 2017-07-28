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
#include <cstdlib>

typedef std::pair<TH1*, TH1*> Hist;
typedef std::vector<Hist> Hists;

// =============
// global option
// =============
std::vector<TString> options_array;
std::map<TString, TString> options;

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
TString getDefaultOpt(TString key, TString default_val)
{
    return options[key].IsNull() ? default_val : options[key];
}

//_________________________________________________________________________________________________
TString getOpt( TString key )
{

    // General rule of thumb on the option names
    // 0. If the option does not relate anything to ROOT, anything's fair game.
    //    But please be mindful of options name being not to obscure.
    // 1. Drop "Set" and see whether that is enough.
    // 2. If another object is called before calling "Set", place a lower case indicator in front.
    // 3. If following 1. and 2. there is a duplicate option name, (e.g. Ratio pane has same Maximum and Minimum)
    //    then, place a "category" name of user's choosing (e.g. "ratio", "error", etc.)
    //    and place a "_" delimiter.

    TString gFont = "42";
    TString gFontSize = "0.063";
    TString gLabelOffset = "0.0225";
    TString gTickLength = "-0.02";
    TString gNdiv = "505";

    if      ( key.EqualTo( "xTitle"            )  ) return getDefaultOpt( key, "XVar"             ) ;
    else if ( key.EqualTo( "xTickLength"       )  ) return getDefaultOpt( key, gTickLength        ) ;
    else if ( key.EqualTo( "xTitleOffset"      )  ) return getDefaultOpt( key, "1.2"              ) ;
    else if ( key.EqualTo( "xLabelOffset"      )  ) return getDefaultOpt( key, gLabelOffset       ) ;
    else if ( key.EqualTo( "xTitleSize"        )  ) return getDefaultOpt( key, gFontSize          ) ;
    else if ( key.EqualTo( "xLabelSize"        )  ) return getDefaultOpt( key, gFontSize          ) ;
    else if ( key.EqualTo( "xTitleFont"        )  ) return getDefaultOpt( key, gFont              ) ;
    else if ( key.EqualTo( "xLabelFont"        )  ) return getDefaultOpt( key, gFont              ) ;
    else if ( key.EqualTo( "xNdivisions"       )  ) return getDefaultOpt( key, gNdiv              ) ;
    else if ( key.EqualTo( "xNbin"            )  ) return getDefaultOpt( key, ""                 ) ;

    else if ( key.EqualTo( "yTitle"            )  ) return getDefaultOpt( key, "YVar"             ) ;
    else if ( key.EqualTo( "yTickLength"       )  ) return getDefaultOpt( key, gTickLength        ) ;
    else if ( key.EqualTo( "yTitleOffset"      )  ) return getDefaultOpt( key, "2.1"              ) ;
    else if ( key.EqualTo( "yLabelOffset"      )  ) return getDefaultOpt( key, gLabelOffset       ) ;
    else if ( key.EqualTo( "yTitleSize"        )  ) return getDefaultOpt( key, gFontSize          ) ;
    else if ( key.EqualTo( "yLabelSize"        )  ) return getDefaultOpt( key, gFontSize          ) ;
    else if ( key.EqualTo( "yTitleFont"        )  ) return getDefaultOpt( key, gFont              ) ;
    else if ( key.EqualTo( "yLabelFont"        )  ) return getDefaultOpt( key, gFont              ) ;
    else if ( key.EqualTo( "yNdivisions"       )  ) return getDefaultOpt( key, gNdiv              ) ;

    else if ( key.EqualTo( "MaximumMultiplier" )  ) return getDefaultOpt( key, "1.2"              ) ;

    else if ( key.EqualTo( "Minimum"           )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "Maximum"           )  ) return getDefaultOpt( key, ""                 ) ;

    else if ( key.EqualTo( "error_FillColor"   )  ) return getDefaultOpt( key, "1"                ) ;
    else if ( key.EqualTo( "error_FillStyle"   )  ) return getDefaultOpt( key, "3245"             ) ;

    else if ( key.EqualTo( "ratio_xTitle"      )  ) return getDefaultOpt( key, getOpt( "xTitle" ) ) ;
    else if ( key.EqualTo( "ratio_yTitle"      )  ) return getDefaultOpt( key, getOpt( "reverseRatio" ).IsNull() ? "Data / MC" : "MC / Data" ) ;
    else if ( key.EqualTo( "ratio_Minimum"     )  ) return getDefaultOpt( key, "0.7"              ) ;
    else if ( key.EqualTo( "ratio_Maximum"     )  ) return getDefaultOpt( key, "1.3"              ) ;
    else if ( key.EqualTo( "ratio_DrawOpt"     )  ) return getDefaultOpt( key, "ex0p"             ) ;

    else if ( key.EqualTo( "data_DrawOpt"      )  ) return getDefaultOpt( key, "ex0p"             ) ;

    else if ( key.EqualTo( "stack_DrawOpt"     )  ) return getDefaultOpt( key, "hist"             ) ;

    else if ( key.EqualTo( "legend_bkgDrawOpt" )  ) return getDefaultOpt( key, "f"                ) ;

    else if ( key.EqualTo( "plotOutputName"    )  ) return getDefaultOpt( key, "test"             ) ;
    else if ( key.EqualTo( "autoStack"         )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "showOverflow"      )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "showUnderflow"     )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "printRatio"        )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "printTotalBkg"     )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "printData"         )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "sumDataHists"      )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "reverseRatio"      )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "ratioPaneAtBottom" )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "divideByBinWidth"  )  ) return getDefaultOpt( key, ""                 ) ;
    else if ( key.EqualTo( "scaleByLumi"       )  ) return getDefaultOpt( key, ""                 ) ;

    else
    {
        std::cout << "[plotmaker::] Unrecognized option! option = " << key << std::endl;
        exit(-1);
    }
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
void showOverflow( TH1* hist )
{
    double overflow_content = hist->GetBinContent( hist->GetNbinsX() + 1 );
    double overflow_error   = hist->GetBinError  ( hist->GetNbinsX() + 1 );
    double lastbin_content = hist->GetBinContent( hist->GetNbinsX()  );
    double lastbin_error   = hist->GetBinError  ( hist->GetNbinsX()  );
    hist->SetBinContent( hist->GetNbinsX(), overflow_content + lastbin_content );
    hist->SetBinError  ( hist->GetNbinsX(), sqrt( pow( overflow_error, 2 ) + pow( lastbin_error, 2 ) ) );
}

//_________________________________________________________________________________________________
void showUnderflow( TH1* hist )
{
    double underflow_content = hist->GetBinContent( 0 );
    double underflow_error   = hist->GetBinError  ( 0 );
    double firstbin_content = hist->GetBinContent( 1  );
    double firstbin_error   = hist->GetBinError  ( 1  );
    hist->SetBinContent( hist->GetNbinsX(), underflow_content + firstbin_content );
    hist->SetBinError  ( hist->GetNbinsX(), sqrt( pow( underflow_error, 2 ) + pow( firstbin_error, 2 ) ) );
}

//_________________________________________________________________________________________________
void correctOverUnderflow( TH1* hist )
{
    if ( !getOpt( "showOverflow" ).IsNull() ) showOverflow( hist );
    if ( !getOpt( "showUnderflow" ).IsNull() ) showUnderflow( hist );
}

//_________________________________________________________________________________________________
void rebin( TH1* hist )
{
    if ( !getOpt( "xNbin" ).IsNull() )
    {
        int target_nbin = getOpt( "xNbin" ).Atoi();
        int current_nbin = hist->GetNbinsX();
        if ( current_nbin % target_nbin != 0 )
        {
            std::cout << "[plotmaker::] The target bin you asked target_nbin=";
            std::cout << target_nbin << " ";
            std::cout << "is not possible for a bin with current_nbin=";
            std::cout << current_nbin << std::endl;
            exit(-1);
        }
        int rebin_factor = current_nbin / target_nbin;
        hist->Rebin( rebin_factor );
    }
}

//_________________________________________________________________________________________________
void divideByBinWidth( TH1* hist )
{
    if ( !getOpt( "divideByBinWidth" ).IsNull() )
    {
        for ( unsigned int ibin = 1; ibin <= hist->GetNbinsX(); ibin++ )
        {
            hist->SetBinContent( ibin, hist->GetBinContent( ibin ) / hist->GetBinWidth( ibin ) );
            hist->SetBinError( ibin, hist->GetBinError( ibin ) / hist->GetBinWidth( ibin ) );
        }
    }
}

//_________________________________________________________________________________________________
TH1* histWithFullError( TH1* nominal, TH1* error )
{
    TH1D* nominal_with_full_error = ( TH1D* ) nominal->Clone( nominal->GetName() );
    nominal_with_full_error->SetTitle("");
    
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
    
    correctOverUnderflow( nominal_with_full_error );
    rebin( nominal_with_full_error );
    divideByBinWidth( nominal_with_full_error );
    
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
    if ( hists.size() == 0)
    {
        std::cout << "[plotmaker::] You asked for a total bkg hist when there are no bkg hists provided." << std::endl;
        exit(-1);
    }
    TH1* sum_hist = getSumHists( hists );
    if ( !gROOT->GetColor( 9999 ) )
        new TColor( 9999, 0.1, 0.2, 0.3, "", 0. ); // alpha = 0.5
    sum_hist->SetMarkerStyle( 1 );
    sum_hist->SetMarkerSize( 0 );
    sum_hist->SetMarkerColor( 9999 );
    sum_hist->SetLineColor( 1 );
    sum_hist->SetLineColor( 1 );
    sum_hist->SetFillColor( getOpt( "error_FillColor" ).Atoi() );
    sum_hist->SetFillStyle( getOpt( "error_FillStyle" ).Atoi() );

    if ( !getOpt( "printTotalBkg" ).IsNull() )
    {
        std::cout << "[plotmaker::] Printing total background histogram." << std::endl;
        sum_hist->Print("all");
    }

    return sum_hist;
}

//_________________________________________________________________________________________________
THStack* getStack( std::vector<TH1*> hists )
{
    THStack *stack = new THStack();
    stack->SetName( "stack" );
    stack->SetTitle( "" );

    for ( auto& hist : hists )
        stack->Add( hist, "" );

    //
    // Stupid ROOT THStack not reflecting maximum and minimum of pointer TH1* from "GetHistogram()" 
    // So I have to add this here instead of "stylizeAxes"
    //
    if ( !getOpt( "MaximumMultiplier" ).IsNull() )
        stack->SetMaximum( stack->GetMaximum() * getOpt( "MaximumMultiplier" ).Atof() );
    if ( !getOpt( "Minimum" ).IsNull() ) stack->SetMinimum( getOpt( "Minimum" ).Atof() );
    if ( !getOpt( "Maximum" ).IsNull() ) stack->SetMaximum( getOpt( "Maximum" ).Atof() );

    return stack;
}

//_________________________________________________________________________________________________
void stylizeAxes( TH1* h, TPad* pad )
{
    double pad_h = pad->GetAbsHNDC();
    double pad_w = pad->GetAbsWNDC();

    double WidthPixel =  pad->GetWw();

    if ( !getOpt( "MaximumMultiplier" ).IsNull() )
        h->SetMaximum( h->GetMaximum() * getOpt( "MaximumMultiplier" ).Atof() );
    if ( !getOpt( "Minimum" ).IsNull() ) h->SetMinimum( getOpt( "Minimum" ).Atof() );
    if ( !getOpt( "Maximum" ).IsNull() ) h->SetMaximum( getOpt( "Maximum" ).Atof() );
    h->SetDirectory( 0 );
    h->SetStats( 0 );
    h->SetFillColor( -1 );
    h->GetXaxis ( ) ->SetNdivisions  ( getOpt( "xNdivisions"  ) .Atoi( )   ) ;
    h->GetXaxis ( ) ->SetLabelFont   ( getOpt( "xLabelFont"   ) .Atoi( )   ) ;
    h->GetXaxis ( ) ->SetLabelOffset ( getOpt( "xLabelOffset" ) .Atof( )   ) ;
    h->GetXaxis ( ) ->SetLabelSize   ( getOpt( "xLabelSize"   ) .Atof( )   ) ;
    h->GetXaxis ( ) ->SetTitleSize   ( getOpt( "xTitleSize"   ) .Atof( )   ) ;
    h->GetXaxis ( ) ->SetTitle       ( getOpt( "xTitle"       )            ) ;
    h->GetXaxis ( ) ->SetTickLength  ( getOpt( "xTickLength"  ) .Atof( )   ) ;
    h->GetXaxis ( ) ->SetTitleOffset ( getOpt( "xTitleOffset" ) .Atof( )   ) ;
    h->GetXaxis ( ) ->SetTitleFont   ( getOpt( "xLabelFont"   ) .Atoi( )   ) ;
    h->GetYaxis ( ) ->SetNdivisions  ( getOpt( "yNdivisions"  ) .Atoi( )   ) ;
    h->GetYaxis ( ) ->SetLabelFont   ( getOpt( "yLabelFont"   ) .Atoi( )   ) ;
    h->GetYaxis ( ) ->SetLabelOffset ( getOpt( "yLabelOffset" ) .Atof( )   ) ;
    h->GetYaxis ( ) ->SetLabelSize   ( getOpt( "yLabelSize"   ) .Atof( )   ) ;
    h->GetYaxis ( ) ->SetTitleSize   ( getOpt( "yTitleSize"   ) .Atof( )   ) ;
    h->GetYaxis ( ) ->SetTitle       ( getOpt( "yTitle"       )            ) ;
    h->GetYaxis ( ) ->SetTickLength  ( getOpt( "yTickLength"  ) .Atof( )   ) ;
    h->GetYaxis ( ) ->SetTitleOffset ( getOpt( "yTitleOffset" ) .Atof( )   ) ;
    h->GetYaxis ( ) ->SetTitleFont   ( getOpt( "xLabelFont"   ) .Atoi( )   ) ;
//    // TODO: Come back below when we need to implement it
//    h->GetZaxis ( )->SetLabelFont   ( getOpt( "zLabelFont" ).Atoi() );
//    h->GetZaxis ( )->SetLabelSize   ( 0.035 );
//    h->GetZaxis ( )->SetTitleSize   ( 0.035 );
//    h->GetZaxis ( )->SetTitleFont   ( getOpt( "zTitleFont" ).Atoi() );
}

//_________________________________________________________________________________________________
void stylizeRatioAxes( TH1* h, TPad* pad )
{
    double pad_h = pad->GetAbsHNDC();
    double pad_w = pad->GetAbsWNDC();

    double WidthPixel =  pad->GetWw();

    float sf = 0.7 / 0.3;

    h->SetTitle("");
    h->SetMinimum( getOpt( "ratio_Minimum" ).Atof() );
    h->SetMaximum( getOpt( "ratio_Maximum" ).Atof() );
    h->SetMarkerStyle( 19 );
    h->GetXaxis( ) ->SetNdivisions( getOpt( "xNdivisions" ).Atoi( ) );
    h->GetXaxis( ) ->SetLabelFont( getOpt( "xLabelFont" ) .Atoi( ) ) ;
    h->GetXaxis( ) ->SetLabelOffset( getOpt( "xLabelOffset" ).Atof() * sf ) ;
    h->GetXaxis( ) ->SetLabelSize( getOpt( "xLabelSize" ).Atof() * sf ) ;
    h->GetXaxis( ) ->SetTitleSize( getOpt( "xLabelSize" ).Atof() * sf ) ;
    h->GetXaxis( ) ->SetTickLength( getOpt( "xTickLength" ).Atof() * sf ) ;
    h->GetXaxis( ) ->SetTitleOffset( getOpt( "xTitleOffset" ).Atof() ) ;
    h->GetXaxis( ) ->SetTitleFont( getOpt( "xLabelFont" ).Atoi( ) );
    h->GetXaxis( ) ->SetTitle( getOpt( "ratio_xTitle" ) );
    h->GetYaxis( ) ->SetNdivisions( getOpt( "yNdivisions" ).Atoi( ) );
    h->GetYaxis( ) ->SetLabelFont( getOpt( "yLabelFont" ).Atoi( ) );
    h->GetYaxis( ) ->SetLabelOffset( getOpt( "yLabelOffset" ).Atof( ) );
    h->GetYaxis( ) ->SetLabelSize( getOpt( "yLabelSize" ).Atof() * sf ) ;
    h->GetYaxis( ) ->SetTitleSize( getOpt( "yTitleSize" ).Atof() * sf ) ;
    h->GetYaxis( ) ->SetTickLength( getOpt( "yTickLength" ).Atof() * ( 10. / 6. ) ) ;
    h->GetYaxis( ) ->SetTitleOffset( getOpt( "yTitleOffset" ).Atof() / sf ) ;
    h->GetYaxis( ) ->SetTitleFont( getOpt( "yTitleFont" ).Atoi() );
    h->GetYaxis( ) ->SetTitle( getOpt( "ratio_yTitle" ) );
//    // TODO: When we need to we'll implement this
//    h->GetZaxis( ) ->SetLabelFont( 42 ) ;
//    h->GetZaxis( ) ->SetLabelSize( 0.035 ) ;
//    h->GetZaxis( ) ->SetTitleSize( 0.035 ) ;
//    h->GetZaxis( ) ->SetTitleFont( 42 ) ;

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
//    h->SetLineColor(1);

    if ( !getOpt( "printData" ).IsNull() )
        h->Print( "all" );

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
//    h->SetLineColor(1);

    pad->cd();

    if ( !getOpt( "printRatio" ).IsNull() )
        h->Print("all");

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
    if ( !name.Contains( "skip" ) )
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
        
        if ( !getOpt( "reverseRatio" ).IsNull() )
            ratio->Divide( bkg_hists.size() == 1 ? bkg_hists[0] : bkg_hists[ihist], data_hists[ihist] );
        else
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
        
    std::reverse( std::begin( bkg_hists ), std::end( bkg_hists ) );
    for ( auto& bkg_hist : bkg_hists )
        addToLegend( bkg_hist, leg, getOpt( "legend_bkgDrawOpt" ) );
        
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
void parseOptions( std::string options_string )
{
    options_array.clear();
    options.clear();
    options_array = GetParms( options_string );
    for ( auto& option : options_array )
    {
        TObjArray* oa = option.Tokenize(" ");
        if ( oa->GetEntries() == 1 )
        {
            TString key = ((TObjString*) oa->At(0))->GetString();
            key.ReplaceAll("\n","");
//            std::cout << ":" << key << ":" << "true" << ":" << std::endl; // for debugging
            options[key] = "true";
        }
        else
        {
            TString so = option.Strip(TString::kBoth);
            Ssiz_t delim_pos = so.First(" ");
            TString key = so(0, delim_pos);
            TString val = so(delim_pos+1, so.Length());
            key.ReplaceAll("\n","");
            val.ReplaceAll("\n","");
//            std::cout << ":" << key << ":" << val << ":" << std::endl; // for debugging
            options[key] = val;
        }
    }

}

//_________________________________________________________________________________________________
void replaceWithSummedHist( std::vector<TH1*>& hists )
{
    TH1* summed_hist = getSumHists( hists );
    hists.clear();
    hists.push_back( summed_hist );
}

//_________________________________________________________________________________________________
void clearGlobalSettings()
{
    options_array.clear();
    options.clear();
    map_pad_drawn.clear();
}

//_________________________________________________________________________________________________
std::vector<TH1*> plotmaker(
    std::string options_string,
    Hists datas_pair_in,
    Hists bkgs_pair_in,
    Hists sigs_pair_in = Hists()
)
{
    // ~-~-~-~-~-~-~-~-~-~-~
    // Clear global settings
    // ~-~-~-~-~-~-~-~-~-~-~
    clearGlobalSettings();

    // ~-~-~-~-~-~-~-~
    // gStyle settings
    // ~-~-~-~-~-~-~-~
    gStyle->SetOptStat( 0 );

    // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    // Parse options in to a global variable
    // ~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    parseOptions( options_string );
    
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
    TPad* pad0 = 0;
    if ( !getOpt( "ratioPaneAtBottom" ).IsNull() )
        pad0 = new TPad( "pad0", "pad0", 0, 0.3, 1 - 0.4, 1.0 );
    else
        pad0 = new TPad( "pad0", "pad0", 0, 0, 1 - 0.4, 0.7 );
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
    TPad* pad2 = 0;
    if ( !getOpt( "ratioPaneAtBottom" ).IsNull() )
        pad2 = new TPad( "pad2", "pad2", 0, 0.125, 0.6, 0.425 );
    else
        pad2 = new TPad( "pad2", "pad2", 0, 0.7, 0.6, 1 );
    pad2->Draw();
    pad2->cd();
    pad2->Range( -80, -0.82, 320, 1.98 );
    pad2->SetFillColor( 0 );
//    pad2->SetFillStyle( 4000 );
    pad2->SetBorderMode( 0 );
    pad2->SetBorderSize( 2 );
    pad2->SetGridy();
    pad2->SetLeftMargin( 150. / 600. );
    pad2->SetRightMargin( 50. / 600. );
    pad2->SetBottomMargin( 0.4 );
    pad2->SetTopMargin( 0.01 );
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
        return std::vector<TH1*>();
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

        // ~-~-~-~-~-~-~-~-
        // auto stack order
        // ~-~-~-~-~-~-~-~-
        if ( !getOpt( "autoStack" ).IsNull() )
        {
            struct {
                bool operator() (TH1* a, TH1* b) const
                {
                    return a->Integral() < b->Integral();
                }
            } sortByIntegral;
            std::sort( bkg_hists.begin(), bkg_hists.end(), sortByIntegral );
        }

        // ~-~-~-~-~-~-~-~-~-
        // Main bkg stack plot
        // ~-~-~-~-~-~-~-~-~-
        THStack* stack = getStack( bkg_hists );
        draw( stack, getOpt( "stack_DrawOpt" ), pad0 );
        
        // ~-~-~-~-~-~-~-~-~-
        // Total bkg histogram
        // ~-~-~-~-~-~-~-~-~-
        TH1* total = getTotalBkgHists( bkg_hists );
        draw( total, "hist p", pad0 );
        draw( total, "e2p ", pad0 );
    }
    
    // ~-~-~-~-~
    // Draw data
    // ~-~-~-~-~
    if ( data_hists.size() )
    {
        if ( !getOpt( "sumDataHists" ).IsNull() )
            replaceWithSummedHist( data_hists );

        for ( auto& data_hist : data_hists )
            drawData( data_hist, getOpt( "data_DrawOpt" ), pad0 );
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
    std::vector<TH1*> ratio_hists;
    if ( data_hists.size() != 0 && bkg_hists.size() != 0 )
    {
        ratio_hists = getRatioHists( data_hists, getTotalBkgHists( bkg_hists ) );

        for (auto& ratio_hist : ratio_hists )
            drawRatio( ratio_hist, getOpt( "ratio_DrawOpt" ), pad2 );
    }

    // ~-~-~-~-~-
    // Save plots
    // ~-~-~-~-~-
    canvas->SaveAs( getOpt( "plotOutputName" ) + ".png" );
//    canvas->SaveAs( getOpt( "plotOutputName" ) + ".pdf" );
    pad0->SetLogy();
    canvas->SaveAs( getOpt( "plotOutputName" ) + "_logy.png" );
//    canvas->SaveAs( getOpt( "plotOutputName" ) + "_logy.pdf" );
    pad0->cd();
    pad0->SetLogy(0);
//    pad0->SaveAs( getOpt( "plotOutputName" ) + "_noratio.pdf" );
    pad0->SetLogy();
//    pad0->SaveAs( getOpt( "plotOutputName" ) + "_noratio_logy.pdf" );

    delete canvas;

    return ratio_hists;
}

//_________________________________________________________________________________________________
std::vector<TH1*> plotmaker(
    std::string options_string,
    TH1* data_in,
    Hists bkgs_pair_in,
    Hists sigs_pair_in = Hists()
)
{
    Hists datas_pair_in;
    datas_pair_in.push_back( std::pair<TH1*, TH1*>( data_in, 0 ) );
    
    return plotmaker( options_string, datas_pair_in, bkgs_pair_in, sigs_pair_in );
}

//_________________________________________________________________________________________________
std::vector<TH1*> plotmaker(
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
        
    return plotmaker( options_string, datas_pair_in, bkgs_pair_in, sigs_pair_in );
}

//_________________________________________________________________________________________________
std::vector<TH1*> plotmaker(
    std::string options_string,
    TH1* data_in,
    TH1* bkg_in,
    std::vector<TH1*> sig_hists = std::vector<TH1*>()
)
{

    Hists datas_pair_in;
    datas_pair_in.push_back( std::pair<TH1*, TH1*>( data_in, 0 ) );
    
    Hists bkgs_pair_in;
    bkgs_pair_in.push_back( std::pair<TH1*, TH1*>( bkg_in, 0 ) );
    
    Hists sigs_pair_in;
    
    for ( auto& sig_hist : sig_hists )
        sigs_pair_in.push_back( std::pair<TH1*, TH1*>( sig_hist, 0 ) );
        
    return plotmaker( options_string, datas_pair_in, bkgs_pair_in, sigs_pair_in );
}

//_________________________________________________________________________________________________
std::vector<TH1*> plotmaker(
    std::string options_string,
    std::vector<TH1*> data_hists,
    std::vector<TH1*> bkg_hists,
    std::vector<TH1*> sig_hists = std::vector<TH1*>()
)
{

    Hists datas_pair_in;
    
    for ( auto& data_hist : data_hists )
        datas_pair_in.push_back( std::pair<TH1*, TH1*>( data_hist, 0 ) );
        
    Hists bkgs_pair_in;
    
    for ( auto& bkg_hist : bkg_hists )
        bkgs_pair_in.push_back( std::pair<TH1*, TH1*>( bkg_hist, 0 ) );
        
    Hists sigs_pair_in;
    
    for ( auto& sig_hist : sig_hists )
        sigs_pair_in.push_back( std::pair<TH1*, TH1*>( sig_hist, 0 ) );
        
    return plotmaker( options_string, datas_pair_in, bkgs_pair_in, sigs_pair_in );
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
