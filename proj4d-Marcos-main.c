#include "parseredd.h"
#include "ada2asaedd.h"
#include "asa2npedd.h"
#include "np2gciedd.h"
#include "gci2gcoedd.h"

int main(int argc, char *argv[]){
    parser(argv[1]);
    ada2asa();
    asa2np();
    np2gci();
    gci2gco("proj4d-Marcos-GCI.txt");
}
