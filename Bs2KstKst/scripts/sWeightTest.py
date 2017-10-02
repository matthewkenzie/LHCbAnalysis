import ROOT as r

r.gROOT.ProcessLine(".L ../build/libLHCbAnalysisComponents.dylib")

tf = r.TFile.Open('root/MassFitResultWSWeights.root')

w = tf.Get('w')
w.Print()

altf = r.TFile.Open('root/AnalysisOutWSWeights.root')
tree = altf.Get('AnalysisTree')

canv = r.TCanvas("c","c",1200,1200)
canv.Divide(2,2)

data_to_cat_map = { 'HadronTOS2011' : '2011HadronTOS' ,
                    'GlobalTIS2011' : '2011GlobalTIS' ,
                    'HadronTOS2012' : '2012HadronTOS' ,
                    'GlobalTIS2012' : '2012GlobalTIS' }

for c in range(w.cat("DataCat").numTypes()):
    w.cat("DataCat").setIndex(c)
    canv.cd(c+1)
    data_proj = w.data("Data%s_sw"%data_to_cat_map[w.cat("DataCat").getLabel()])
    data = w.data("Data%sSWeights"%data_to_cat_map[w.cat("DataCat").getLabel()])
    data.Print("v")
    data_proj.Print("v")

    plot = w.var('B_s0_DTF_B_s0_M').frame()
    data.plotOn(plot, r.RooFit.LineColor(r.kRed),r.RooFit.MarkerColor(r.kRed))
    data_proj.plotOn(plot)

    h = r.TH1F("h%d"%c,"",100,5000,5800)
    tree.Draw( "B_s0_DTF_B_s0_M >> h%d"%c, "sweight*( pass_all && category==%d && itype>0)"%c, "goff" )
    h.SetLineColor(r.kBlue)
    h.SetMarkerColor(r.kBlue)
    h.SetLineWidth(2)
    h.SetMarkerStyle(r.kFullCircle)

    plot.addObject( h, "LEPsame" )
    plot.Draw()
    canv.Update()
    canv.Modified()


