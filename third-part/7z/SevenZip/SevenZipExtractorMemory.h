#pragma once
#include "SevenZipArchive.h"
#include "CompressionFormat.h"
#include "ProgressCallback.h"
#include "SevenZipPwd.h"
#include "FileStream.h"

namespace SevenZip
{

    class SevenZipExtractorMemory : public SevenZipArchive
    {
    public: 
        SevenZipExtractorMemory();
        virtual ~SevenZipExtractorMemory();

		virtual HRESULT ExtractArchive(CFileStream &fileStreams, ProgressCallback* callback, SevenZipPassword *pSevenZipPassword = NULL);
        const TString& GetErrorString(); 
    //private:
        // ¸Ä³Épublic
		HRESULT ExtractArchive(CFileStream &fileStreams,const CMyComPtr< IStream >& archiveStream,  ProgressCallback* callback, SevenZipPassword *pSevenZipPassword);
        
    private:
		 
        TString m_message;
    };
}
