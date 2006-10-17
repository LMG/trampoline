//---------------------------------------------------------------------------*
//                                                                           *
//                               goil Project                                *
//                   Generated by GALGAS Project Creation                    *
//                       june 1st, 2006, at 15h40'43"                        *
//                                                                           *
//---------------------------------------------------------------------------*

#include "goil_semantics.h"
#include "utilities/MF_MemoryControl.h"

//---------------------------------------------------------------------------*

const char *generatedBy = 
"/*\n"
" * Header file for %s\n"
" *\n"
" * Generated by goil\n"
" */\n\n";


void
routine_generateHeader(
    C_Lexique &inLex,
    GGS_lstring inVersion,
    GGS_lstring inDesc
    COMMA_LOCATION_ARGS)
{
    if (inLex.currentFileErrorsCount() == 0) {
        C_String outFile =
            inLex.sourceFileName().stringByDeletingPathExtension() + ".h" ;
        C_String multiInclude =
            inLex.sourceFileName().lastPathComponent().stringByDeletingPathExtension().stringWithUpperCase();
//        printf("%s\n",outFile.cString());
        /*  Open the Header File    */
        FILE *hf = fopen(outFile.cString(),"w");
        if (hf) {
            fprintf(hf,"%s",generatedBy);
            fprintf(hf,"#ifndef __%s_H__\n",multiInclude.cString());
            fprintf(hf,"#define __%s_H__\n\n",multiInclude.cString());
            fprintf(hf,"/*  %s  */\n",inDesc.cString());
            fprintf(hf,"#define OIL_VERSION \"%s\"\n",inVersion.cString());
            
            fprintf(hf,"\n#endif\n");
            fclose(hf);
        }
    }
}

//---------------------------------------------------------------------------*
