#ifndef __ZIPARCHIVE_H__
#define __ZIPARCHIVE_H__

#pragma once


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>


#include "SevenZip/FileStream.h"
#include "SevenZip/FileSys.h"
#include "SevenZip/ArchiveUpdateCallback.h"
#include "SevenZip/SevenZipExtractor.h"


class MemFileStreamMemory : public IStream {
 public:
  MemFileStreamMemory(const BlobBuffer& blob);
  ~MemFileStreamMemory();

  STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject);
  STDMETHOD_(ULONG, AddRef)();
  STDMETHOD_(ULONG, Release)();

  // ISequentialStream Interface
 public:
  STDMETHOD(Read)(void* pv, ULONG cb, ULONG* pcbRead);
  STDMETHOD(Write)(void const* pv, ULONG cb, ULONG* pcbWritten);

  // IStream Interface
 public:
  STDMETHOD(SetSize)(ULARGE_INTEGER);
  STDMETHOD(CopyTo)(IStream*, ULARGE_INTEGER, ULARGE_INTEGER*, ULARGE_INTEGER*);
  STDMETHOD(Commit)(DWORD);
  STDMETHOD(Revert)(void);
  STDMETHOD(LockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
  STDMETHOD(UnlockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
  STDMETHOD(Clone)(IStream**);
  STDMETHOD(Seek)
  (LARGE_INTEGER liDistanceToMove,
   DWORD dwOrigin,
   ULARGE_INTEGER* lpNewFilePointer);
  STDMETHOD(Stat)(STATSTG* pStatstg, DWORD grfStatFlag);

 private:
  LONG _refcount;
  BlobBuffer _blob;
  unsigned long _blob_read_pos;
};  



typedef struct ZIP_FIND_DATA
{
	TCHAR		szFileName[MAX_PATH];
	TCHAR		szComment[64];
	DWORD		nFileSizeCompressed;
	DWORD		nFileSizeUncompressed;
	FILETIME	ftCreationDate;
	DWORD		dwFileAttributes;
	int			nIndex;
} ZIP_FIND_DATA, *LPZIP_FIND_DATA;

class CZipFile;
class CZipArchive;

//	ZIP file wrapper from zip archive
class CZipFile
{
	friend CZipArchive;
protected:
	DWORD	m_dwPos;
public:
	CZipFile(DWORD dwSize=0);
	~CZipFile();
	BOOL Read(void* pBuffer, DWORD dwSize, LPDWORD pdwRead = NULL);
	BOOL Close();
	BOOL IsOpen() const ;
	BYTE* GetData() ;
	DWORD GetSize() const;
	DWORD GetPosition() const;
	DWORD Seek(DWORD dwOffset, UINT nFrom);

	BOOL Attach(LPBYTE pData, DWORD dwSize);
	void Detach();
	BlobBuffer &getBlob();
protected: 
	BlobBuffer m_blob;
};

//	ZIP Archive class, load files from a zip archive
class CZipArchive
{
protected: 
	CZipFile		m_fileRes;	
	char			m_szPassword[64];
public:
	CZipArchive();
	~CZipArchive();

	BOOL Open(LPCTSTR pszFileName, LPCSTR pszPassword);
	BOOL Open(HMODULE hModule, LPCTSTR pszName, LPCTSTR pszPassword, LPCTSTR pszType = _T("ZIP"));
    BOOL Open(LPBYTE pszData, DWORD dwDataSize, LPCTSTR pszPassword);

	void Close();
	BOOL IsOpen() const;
	
	BOOL SetPassword(LPCSTR pstrPassword);
	 
	BOOL GetFile(LPCTSTR pszFileName, CZipFile& file); 
	DWORD GetFileSize(LPCTSTR pszFileName);
	 
protected:
	BOOL OpenZip();
	void CloseFile();

	DWORD ReadFile(void* pBuffer, DWORD dwBytes);
private:
	CFileStream m_fileStreams;
};

//  
class CMemZipArchive {
 public:
  CMemZipArchive();

  bool Open(LPBYTE pszData, DWORD dwDataSize);

  bool ExtractArchive(LPCTSTR path);

  BOOL SetPassword(LPCTSTR pstrPassword);

  void SetProgressCallback(SevenZip::ProgressCallback* pProgressCallback);

 protected:
  TCHAR m_szPassword[64];
  SevenZip::SevenZipExtractor m_decompress;
  SevenZip::ProgressCallback* m_pProgressCallback;

 private:
  CZipFile m_fileRes;
  CFileStream m_fileStreams;
};

#endif	//	__ZIP7ARCHIVE_H__
