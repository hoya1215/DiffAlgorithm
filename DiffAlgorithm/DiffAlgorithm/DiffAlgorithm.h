#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <unordered_set>

#ifdef DIFFALGORITHM_EXPORTS
#define DIFFALGORITHM_DECLSPEC __declspec(dllexport)
#else
#define DIFFALGORITHM_DECLSPEC __declspec(dllimport)
#endif

extern "C"
{
    using namespace std;

    enum LineDiffType : int
    {
        Diff = 0,
        Same,
        Added,
        Deleted,
        Unchanged,
        Removed,
        Changed,
        Highlight
    };

#pragma pack(push, 4)

    struct LineInfo_S
    {
    public:
        LineDiffType Type;
        LineDiffType Data;
        LPSTR Text;
        //LineDiffType Type;
        //LineDiffType Data;

    };
#pragma pack(pop)

    struct FolderNode_S
    {
        LPSTR Name;
        LPSTR Path;
        bool IsDirectory;
        LineDiffType Type;
        FolderNode_S** Child;
        int ChildCount;
        int Index;
    };

    struct LineOutput
    {
        LineInfo_S* originList;
        int originCount;
        LineInfo_S* targetList;
        int targetCount;
    };

    DIFFALGORITHM_DECLSPEC void CompareLinesByLCS(LineInfo_S** originOutput, LineInfo_S** targetOutput, const char** origin, const char** target,const int originSize, const int targetSize, int* count);
    DIFFALGORITHM_DECLSPEC bool CompareFolder(FolderNode_S* originOutput, FolderNode_S* targetOutput, const char* originInput, const char* targetInput);
    
    // callback
    typedef void(__stdcall* ProgressCallback)(int percent);
    ProgressCallback g_progressCallback = nullptr;
    DIFFALGORITHM_DECLSPEC void __stdcall SetProgressCallback(ProgressCallback cb);

    DIFFALGORITHM_DECLSPEC void FreeMemory(LineInfo_S* lines);

    bool CompareFile(string path1, string path2);
    bool CreateFolder(FolderNode_S* origin, FolderNode_S* target);
    string GetFileName(string path)
    {
        return filesystem::path(path).filename().string();
    }

    void CopyStringToChar(char* c, const string& src, size_t maxLen)
    {
        strncpy_s(c, maxLen, src.c_str(), _TRUNCATE);
    }

}