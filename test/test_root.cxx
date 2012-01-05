#include <iostream>
#include <TTree.h>
#include <TH1.h>
#include <TCanvas.h>

int main(int argc, char **argv) {
    std::cout << "Test of inclusion of root into build process!" << std::endl
              << "A pdf file, hist.pdf, should be created in cd." << std::endl;

    // test the inclusion of root to the build process.
    TH1F* hist = new TH1F("hist", "histogram", 10, 0, 10);
    TCanvas* canv = new TCanvas;
    hist->Draw();
    canv->SaveAs("hist.pdf");


    return 0;
}
