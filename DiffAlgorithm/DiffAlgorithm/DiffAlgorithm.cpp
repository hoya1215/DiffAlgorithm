#include "pch.h"
#include "DiffAlgorithm.h"

DIFFALGORITHM_DECLSPEC void CompareLinesByLCS(LineInfo_S** originOutput, LineInfo_S** targetOutput, const char** origin, const char** target,const int originSize,const int targetSize, int* count)
{
	queue<string> LCS;
	int startLine = 0;

	queue<string> originQ;
	queue<string> targetQ;

	vector<LineInfo_S> originLines;
	vector<LineInfo_S> targetLines;

	// UpperBound
	unordered_map<string, deque<int>> targetMap;
	vector<int> targetV(targetSize, -1);
	vector<int> dp(targetSize, 0);
	int test = 0;

	for (int i = 0; i < originSize; ++i)
	{
		originQ.push(origin[i]);
	}

	for (int i = 0; i < targetSize; ++i)
	{
		targetQ.push(target[i]);
		if (strcmp(target[i], "") == 0)
		{
			test++;
			continue;
		}
		targetMap[string(target[i])].push_back(i);
	}


	// UpperBound
	for (int i = 0; i < originSize; ++i)
	{
		if (strcmp(origin[i], "") == 0)
			continue;

		auto it = targetMap.find(origin[i]);

		if (it != targetMap.end() && !it->second.empty())
		{
			targetV[it->second.front()] = i;
			it->second.pop_front();
		}
	}

	int maxCount = 0;
	int maxIndex = targetV.size();
	map<int, int> UB;
	for (int i = targetV.size()- 1; i >= 0; --i)
	{
		int key = targetV[i];
		int index = targetV.size();
		//for (int j = i + 1; j < targetV.size(); ++j)
		//{
		//	if (key < targetV[j])
		//	{
		//		index = j;
		//		break;
		//	}
		//}
		
		UB[key] = i;
		auto it = UB.upper_bound(key);
		if (it != UB.end())
			index = it->second;
		
		//int index = upper_bound(targetV.begin() + i, targetV.end(), key) - targetV.begin();
		if (index < targetV.size())
		{
			dp[i] = dp[index] + 1;
			
		}
		else
			dp[i] = 1;

		if (maxCount <= dp[i])
		{
			maxCount = dp[i];
			maxIndex = i;
		}
	}

	//*count = maxCount;

	int prev = -1;
	for (int i = maxIndex; i < targetV.size(); ++i)
	{
		int next = targetV[i];
		if (prev < next)
		{
			LCS.push(origin[next]);
			prev = next;
		}
	}



	//*count = LCS.size();

	//for (int oLine = 0; oLine < originSize; ++oLine)
	//{
	//	for (int tLine = startLine; tLine < targetSize; ++tLine)
	//	{
	//		if (strcmp(origin[oLine] , target[tLine]) == 0)
	//		{
	//			if (strcmp(origin[oLine], "") == 0)
	//				continue;

	//			LCS.push(origin[oLine]);
	//			startLine = tLine + 1;
	//			break;
	//		}
	//	}
	//	
	//}

	// origin
	while (!LCS.empty())
	{
		//*count += 1;

		string originText = originQ.front();
		string targetText = targetQ.front();
		string LCSText = LCS.front();

		if (originText == targetText)
		{
			

			LineInfo_S originLine = { LineDiffType::Same, LineDiffType::Same };
			originLine.Text = new char[originText.size() + 1];
			strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

			LineInfo_S targetLine = { LineDiffType::Same, LineDiffType::Same };
			targetLine.Text = new char[targetText.size() + 1];
			strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

			originLines.push_back(originLine);
			targetLines.push_back(targetLine);

			originQ.pop();
			targetQ.pop();
			if (originText != "")
			{

				LCS.pop();
			}
		}
		else
		{
			if (originText == LCSText)
			{

				LineInfo_S originLine = { LineDiffType::Deleted, LineDiffType::Deleted };
				string s = "";
				originLine.Text = new char[s.size() + 1];
				strcpy_s(originLine.Text, s.size() + 1, s.c_str());

				LineInfo_S targetLine = { LineDiffType::Added, LineDiffType::Added };
				targetLine.Text = new char[targetText.size() + 1];
				strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

				originLines.push_back(originLine);
				targetLines.push_back(targetLine);

				targetQ.pop();
			}
			else if (targetText == LCSText)
			{

				LineInfo_S originLine = { LineDiffType::Added, LineDiffType::Added };
				originLine.Text = new char[originText.size() + 1];
				strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

				LineInfo_S targetLine = { LineDiffType::Deleted, LineDiffType::Deleted };
				string s = "";
				targetLine.Text = new char[s.size() + 1];
				strcpy_s(targetLine.Text, s.size() + 1, s.c_str());

				originLines.push_back(originLine);
				targetLines.push_back(targetLine);

				originQ.pop();
			}
			else
			{
				LineInfo_S originLine = { LineDiffType::Diff,LineDiffType::Diff };
				originLine.Text = new char[originText.size() + 1];
				strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

				LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
				targetLine.Text = new char[targetText.size() + 1];
				strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());


				originLines.push_back(originLine);
				targetLines.push_back(targetLine);

				originQ.pop();
				targetQ.pop();
			}
		}
	}

	while (!originQ.empty() && !targetQ.empty())
	{

		string originText = originQ.empty() ? "" : originQ.front();
		string targetText = targetQ.empty() ? "" : targetQ.front();

		if (!originQ.empty() && !targetQ.empty())
		{
			if (originText == targetText)
			{
				LineInfo_S originLine = { LineDiffType::Same, LineDiffType::Same };
				originLine.Text = new char[originText.size() + 1];
				strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

				LineInfo_S targetLine = { LineDiffType::Same, LineDiffType::Same };
				targetLine.Text = new char[targetText.size() + 1];
				strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

				originLines.push_back(originLine);
				targetLines.push_back(targetLine);

				originQ.pop();
				targetQ.pop();
			}
			else
			{
				LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
				originLine.Text = new char[originText.size() + 1];
				strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

				LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
				targetLine.Text = new char[targetText.size() + 1];
				strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

				originLines.push_back(originLine);
				targetLines.push_back(targetLine);

				originQ.pop();
				targetQ.pop();
			}
		}
		else
		{
			LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
			originLine.Text = new char[originText.size() + 1];
			strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

			LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
			targetLine.Text = new char[targetText.size() + 1];
			strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

			originLines.push_back(originLine);
			targetLines.push_back(targetLine);

			if (!originQ.empty())
				originQ.pop();
			if (!targetQ.empty())
				targetQ.pop();
		}

		//if (originText != "" && targetText != "")
		//{
		//	
		//	LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
		//	originLine.Text = new char[originText.size() + 1];
		//	strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

		//	LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
		//	targetLine.Text = new char[targetText.size() + 1];
		//	strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

		//	originLines.push_back(originLine);
		//	targetLines.push_back(targetLine);

		//	originQ.pop();
		//	targetQ.pop();
		//}
		//else
		//{
		//	// "" 이면 Added, Deleted 추가

		//	LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
		//	originLine.Text = new char[originText.size() + 1];
		//	strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

		//	LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
		//	targetLine.Text = new char[targetText.size() + 1];
		//	strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

		//	originLines.push_back(originLine);
		//	targetLines.push_back(targetLine);

		//	if (!originQ.empty())
		//		originQ.pop();
		//	if (!targetQ.empty())
		//		targetQ.pop();
		//}
	}


	//while (!LCS.empty())
	//{


	//	string originText = originQ.front();
	//	string targetText = targetQ.front();
	//	string LCSText = LCS.front();

	//	if (originText == targetText)
	//	{

	//			LineInfo_S originLine = { LineDiffType::Same, LineDiffType::Same };
	//			originLine.Text = new char[originText.size() + 1];
	//			strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

	//			LineInfo_S targetLine = { LineDiffType::Same, LineDiffType::Same };
	//			targetLine.Text = new char[targetText.size() + 1];
	//			strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

	//			originLines.push_back(originLine);
	//			targetLines.push_back(targetLine);

	//			originQ.pop();
	//			targetQ.pop();
	//			if (originText != "")
	//				LCS.pop();

	//	}
	//	else
	//	{
	//		if (originText == LCSText)
	//		{

	//			LineInfo_S originLine = { LineDiffType::Deleted, LineDiffType::Deleted };
	//			string s = "";
	//			originLine.Text = new char[s.size() + 1];
	//			strcpy_s(originLine.Text, s.size() + 1, s.c_str());

	//			LineInfo_S targetLine = { LineDiffType::Added, LineDiffType::Added };
	//			targetLine.Text = new char[targetText.size() + 1];
	//			strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

	//			originLines.push_back(originLine);
	//			targetLines.push_back(targetLine);

	//				targetQ.pop();
	//		}
	//		else if (targetText == LCSText)
	//		{

	//			LineInfo_S originLine = { LineDiffType::Added, LineDiffType::Added };
	//			originLine.Text = new char[originText.size() + 1];
	//			strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

	//			LineInfo_S targetLine = { LineDiffType::Deleted, LineDiffType::Deleted };
	//			string s = "";
	//			targetLine.Text = new char[s.size() + 1];
	//			strcpy_s(targetLine.Text, s.size() + 1, s.c_str());

	//			originLines.push_back(originLine);
	//			targetLines.push_back(targetLine);

	//				originQ.pop();
	//		}
	//		else
	//		{
	//			LineInfo_S originLine = { LineDiffType::Diff,LineDiffType::Diff };
	//			originLine.Text = new char[originText.size() + 1];
	//			strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

	//			LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
	//			targetLine.Text = new char[targetText.size() + 1];
	//			strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());


	//			originLines.push_back(originLine);
	//			targetLines.push_back(targetLine);

	//				originQ.pop();
	//				targetQ.pop();
	//		}
	//	}
	//}


	//while (!originQ.empty() || !targetQ.empty())
	//{
	//	*count = 1000;

	//	string originText = originQ.empty() ? "" : originQ.front();
	//	string targetText = targetQ.empty() ? "" : targetQ.front();

	//	if (!originQ.empty() && !targetQ.empty())
	//	{
	//		LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
	//		originLine.Text = new char[originText.size() + 1];
	//		strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

	//		LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
	//		targetLine.Text = new char[targetText.size() + 1];
	//		strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

	//		if (originText == "" && targetText == "")
	//		{
	//			originLine.Type = LineDiffType::Same;
	//			originLine.Data = LineDiffType::Same;
	//			targetLine.Type = LineDiffType::Same;
	//			targetLine.Data = LineDiffType::Same;
	//		}

	//		originLines.push_back(originLine);
	//		targetLines.push_back(targetLine);

	//		originQ.pop();
	//		targetQ.pop();
	//	}
	//	else
	//	{
	//		// "" 이면 Added, Deleted 추가

	//		LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
	//		originLine.Text = new char[originText.size() + 1];
	//		strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

	//		LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
	//		targetLine.Text = new char[targetText.size() + 1];
	//		strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

	//		originLines.push_back(originLine);
	//		targetLines.push_back(targetLine);

	//		if (!originQ.empty())
	//			originQ.pop();
	//		if (!targetQ.empty())
	//			targetQ.pop();
	//	}

	//	//if (originText != "" && targetText != "")
	//	//{

	//	//	LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
	//	//	originLine.Text = new char[originText.size() + 1];
	//	//	strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

	//	//	LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
	//	//	targetLine.Text = new char[targetText.size() + 1];
	//	//	strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

	//	//	originLines.push_back(originLine);
	//	//	targetLines.push_back(targetLine);

	//	//	originQ.pop();
	//	//	targetQ.pop();
	//	//}
	//	//else
	//	//{
	//	//	// "" 이면 Added, Deleted 추가

	//	//	LineInfo_S originLine = { LineDiffType::Diff, LineDiffType::Diff };
	//	//	originLine.Text = new char[originText.size() + 1];
	//	//	strcpy_s(originLine.Text, originText.size() + 1, originText.c_str());

	//	//	LineInfo_S targetLine = { LineDiffType::Diff, LineDiffType::Diff };
	//	//	targetLine.Text = new char[targetText.size() + 1];
	//	//	strcpy_s(targetLine.Text, targetText.size() + 1, targetText.c_str());

	//	//	originLines.push_back(originLine);
	//	//	targetLines.push_back(targetLine);

	//	//	if(!originQ.empty())
	//	//		originQ.pop();
	//	//	if(!targetQ.empty())
	//	//		targetQ.pop();
	//	//}
	//}

	// output
	*originOutput = new LineInfo_S[originLines.size()];
	*targetOutput = new LineInfo_S[targetLines.size()];

	for (int i = 0; i < originLines.size(); ++i)
	{

		(*originOutput)[i].Text = originLines[i].Text;
		(*originOutput)[i].Type = originLines[i].Type;
		(*originOutput)[i].Data = originLines[i].Data;
	}

	for (int i = 0; i < targetLines.size(); ++i)
	{
		(*targetOutput)[i].Text = targetLines[i].Text;
		(*targetOutput)[i].Type = targetLines[i].Type;
		(*targetOutput)[i].Data = targetLines[i].Data;
	}

	*count = originLines.size();
}



DIFFALGORITHM_DECLSPEC bool CompareFolder(FolderNode_S* originOutput, FolderNode_S* targetOutput, const char* originInput, const char* targetInput)
{
	bool bReturn = true;

	bool bIsOriginFolder = filesystem::is_directory(originInput);
	bool bIsTargetFolder = filesystem::is_directory(targetInput);

	if (bIsOriginFolder && bIsTargetFolder)
	{
		// 파일 이름 비교 -> child 비교 -> child 
		string originFileName = filesystem::path(originInput).filename().string();
		string targetFileName = filesystem::path(targetInput).filename().string();

		if (originFileName != targetFileName)
		{
			originOutput->Type = LineDiffType::Diff;
			targetOutput->Type = LineDiffType::Diff;
		}

		// child 공통 수열 찾기, 공통 수열 가지고 비교 후 add delete 넣어주기
		// 폴더 파일 따로
		queue<FolderNode_S*> originFolderQ;
		queue<FolderNode_S*> targetFolderQ;
		queue<FolderNode_S*> originFileQ;
		queue<FolderNode_S*> targetFileQ;
		queue<string> CommonFolder;
		queue<string> CommonFile;

		unordered_map<string, FolderNode_S*> targetFolderMap;
		unordered_map<string, FolderNode_S*> targetFileMap;

		for (int i = 0; i < originOutput->ChildCount; ++i)
		{
			if (originOutput->Child[i]->IsDirectory)
				originFolderQ.push(originOutput->Child[i]);
			else
				originFileQ.push(originOutput->Child[i]);
		}
		for (int i = 0; i < targetOutput->ChildCount; ++i)
		{
			if (targetOutput->Child[i]->IsDirectory)
			{
				targetFolderQ.push(targetOutput->Child[i]);
				string folderName = GetFileName(targetOutput->Child[i]->Path);
				targetFolderMap[folderName] = targetOutput->Child[i];
			}
			else
			{
				targetFileQ.push(targetOutput->Child[i]);
				string fileName = GetFileName(targetOutput->Child[i]->Path);
				targetFileMap[fileName] = targetOutput->Child[i];
			}
		}
		

		// Folder Map
		for (int i = 0; i < originFolderQ.size(); ++i)
		{
			string originName = GetFileName(originOutput->Child[i]->Path);
			if (targetFolderMap.find(originName) != targetFolderMap.end())
			{
				CommonFolder.push(filesystem::path(originOutput->Child[i]->Path).filename().string());
				if (!CompareFolder(originOutput->Child[i], targetFolderMap[originName], originOutput->Child[i]->Path, targetFolderMap[originName]->Path))
				{
					originOutput->Child[i]->Type = LineDiffType::Diff;
					targetFolderMap[originName]->Type = LineDiffType::Diff;
					bReturn = false;
				}
			}
		}

		// File Map
		for (int i = originFolderQ.size(); i < originOutput->ChildCount; ++i)
		{
			string originName = GetFileName(originOutput->Child[i]->Path);
			if (targetFileMap.find(originName) != targetFileMap.end())
			{
				CommonFile.push(filesystem::path(originOutput->Child[i]->Path).filename().string());

				if (!CompareFolder(originOutput->Child[i], targetFileMap[originName], originOutput->Child[i]->Path, targetFileMap[originName]->Path))
				{
					originOutput->Child[i]->Type = LineDiffType::Diff;
					targetFileMap[originName]->Type = LineDiffType::Diff;
					bReturn = false;
				}
			}
		}

		// Folder
		//int Start = 0;
		//for (int i = 0; i < originFolderQ.size(); ++i)
		//{
		//	for (int j = Start; j < targetFolderQ.size(); ++j)
		//	{
		//		string originName = GetFileName(originOutput->Child[i]->Path);
		//		string targetName = GetFileName(targetOutput->Child[j]->Path);
		//		if (originName == targetName)
		//		{
		//			CommonFolder.push(filesystem::path(originOutput->Child[i]->Path).filename().string());
		//			Start = j + 1;

		//			if (!CompareFolder(originOutput->Child[i], targetOutput->Child[j], originOutput->Child[i]->Path, targetOutput->Child[j]->Path))
		//			{
		//				originOutput->Child[i]->Type = LineDiffType::Diff;
		//				targetOutput->Child[j]->Type = LineDiffType::Diff;
		//				bReturn = false;
		//			}

		//			break;
		//		}
		//	}
		//}

		// File
		//Start = targetFolderQ.size();
		//for (int i = originFolderQ.size(); i < originOutput->ChildCount; ++i)
		//{
		//	for (int j = Start; j < targetOutput->ChildCount; ++j)
		//	{
		//		string originName = GetFileName(originOutput->Child[i]->Path);
		//		string targetName = GetFileName(targetOutput->Child[j]->Path);
		//		if (originName == targetName)
		//		{
		//			CommonFile.push(filesystem::path(originOutput->Child[i]->Path).filename().string());
		//			Start = j + 1;

		//			if (!CompareFolder(originOutput->Child[i], targetOutput->Child[j], originOutput->Child[i]->Path, targetOutput->Child[j]->Path))
		//			{
		//				originOutput->Child[i]->Type = LineDiffType::Diff;
		//				targetOutput->Child[j]->Type = LineDiffType::Diff;
		//				bReturn = false;
		//			}

		//			break;
		//		}
		//	}
		//}


		if (originOutput->ChildCount != CommonFolder.size() + CommonFile.size())
		{
			originOutput->Type = LineDiffType::Diff;
			targetOutput->Type = LineDiffType::Diff;
			bReturn = false;
		}


		// Folder
		vector<FolderNode_S*> originFolderV;
		vector<FolderNode_S*> targetFolderV;
		while (!CommonFolder.empty())
		{
			auto origin = originFolderQ.front();
			auto target = targetFolderQ.front();
			auto common = CommonFolder.front();

			string originName = GetFileName(origin->Path);
			string targetName = GetFileName(target->Path);

			if ((originName == targetName) && originName == common)
			{
				originFolderV.push_back(origin);
				targetFolderV.push_back(target);

				originFolderQ.pop(); targetFolderQ.pop(); CommonFolder.pop();
			}
			else if ((originName == targetName) && originName != common)
			{
				originFolderV.push_back(origin);
				targetFolderV.push_back(target);

				originFolderQ.pop(); targetFolderQ.pop();
			}
			else
			{
				if (originName == common)
				{
					FolderNode_S* folder = new FolderNode_S();
					folder->Type = LineDiffType::Deleted;
					//folder->Path = targetOutput->Path;
					folder->Path = _strdup(targetInput);

					target->Type = LineDiffType::Added;

					CreateFolder(folder, origin);
					
					originFolderV.push_back(folder);
					targetFolderV.push_back(target);

					targetFolderQ.pop();
				}
				else if (targetName == common)
				{
					FolderNode_S* folder = new FolderNode_S();
					folder->Type = LineDiffType::Deleted;
					//folder->Path = originOutput->Path;
					folder->Path = _strdup(originInput);

					origin->Type = LineDiffType::Added;

					CreateFolder(folder, target);
					targetFolderV.push_back(folder);
					originFolderV.push_back(origin);

					originFolderQ.pop();
				}
				else
				{
					origin->Type = LineDiffType::Diff;
					target->Type = LineDiffType::Diff;
					originFolderV.push_back(origin);
					targetFolderV.push_back(target);

					originFolderQ.pop(); targetFolderQ.pop();
				}
			}
		}

		while (!originFolderQ.empty() || !targetFolderQ.empty())
		{
			FolderNode_S* origin; FolderNode_S* target;
			if (originFolderQ.empty())
			{
				origin = new FolderNode_S();
				//origin->Path = originOutput->Path;
				origin->Path = _strdup(originInput);

				target = targetFolderQ.front();
				targetFolderQ.pop();

				CreateFolder(origin, target);

				origin->Type = LineDiffType::Deleted;
				origin->IsDirectory = true;
				target->Type = LineDiffType::Added;
				target->IsDirectory = true;
			}
			else
			{
				target = new FolderNode_S();
				//target->Path = targetOutput->Path;
				target->Path = _strdup(targetInput);

				origin = originFolderQ.front();
				originFolderQ.pop();

				CreateFolder(target, origin);

				origin->Type = LineDiffType::Added;
				origin->IsDirectory = true;
				target->Type = LineDiffType::Deleted;
				target->IsDirectory = true;
			}

			//if (targetFolderQ.empty())
			//{
			//	target = new FolderNode_S();
			//	target->Path = targetOutput->Path;
			//}
			//else
			//{
			//	target = targetFolderQ.front();
			//	targetFolderQ.pop();
			//}



			originFolderV.push_back(origin); targetFolderV.push_back(target);

		}

		// File
		vector<FolderNode_S*> originFileV;
		vector<FolderNode_S*> targetFileV;
		while (!CommonFile.empty())
		{
			auto origin = originFileQ.front();
			auto target = targetFileQ.front();
			auto common = CommonFile.front();

			string originName = GetFileName(origin->Path);
			string targetName = GetFileName(target->Path);

			if ((originName == targetName) && originName == common)
			{
				originFileV.push_back(origin);
				targetFileV.push_back(target);

				originFileQ.pop(); targetFileQ.pop(); CommonFile.pop();
			}
			else if (originName != targetName)
			{
				if (originName == common)
				{
					FolderNode_S* folder = new FolderNode_S();
					folder->Type = LineDiffType::Deleted;
					//folder->Path = targetOutput->Path;
					folder->Path = _strdup(targetInput);
					target->Type = LineDiffType::Added;
					originFileV.push_back(folder);
					targetFileV.push_back(target);

					targetFileQ.pop();
				}
				else if (targetName == common)
				{
					FolderNode_S* folder = new FolderNode_S();
					folder->Type = LineDiffType::Deleted;
					//folder->Path = originOutput->Path;
					folder->Path = _strdup(originInput);

					origin->Type = LineDiffType::Added;
					targetFileV.push_back(folder);
					originFileV.push_back(origin);

					originFileQ.pop();
				}
				else
				{
					origin->Type = LineDiffType::Diff;
					target->Type = LineDiffType::Diff;
					originFileV.push_back(origin);
					targetFileV.push_back(target);

					originFileQ.pop(); targetFileQ.pop();
				}
			}
		}

		while (!originFileQ.empty() || !targetFileQ.empty())
		{
			FolderNode_S* origin; FolderNode_S* target;
			if (originFileQ.empty())
			{
				origin = new FolderNode_S();
				//origin->Path = originOutput->Path;
				origin->Path = _strdup(originInput);

				target = targetFileQ.front();
				targetFileQ.pop();

				// 나중에 added, deleted 처리
				origin->Type = LineDiffType::Deleted;
				target->Type = LineDiffType::Added;
				origin->IsDirectory = false;
				target->IsDirectory = false;
			}
			else
			{
				origin = originFileQ.front();
				originFileQ.pop();

				target = new FolderNode_S();
				//target->Path = targetOutput->Path;
				target->Path = _strdup(targetInput);


				// 나중에 added, deleted 처리
				origin->Type = LineDiffType::Added;
				target->Type = LineDiffType::Deleted;
				origin->IsDirectory = false;
				target->IsDirectory = false;
			}



			originFileV.push_back(origin); targetFileV.push_back(target);

		}

		int size = originFolderV.size() + originFileV.size();

		originOutput->Child = new FolderNode_S * [size];
		targetOutput->Child = new FolderNode_S * [size];
		originOutput->ChildCount = size;
		targetOutput->ChildCount = size;


		for (int i = 0; i < originFolderV.size(); ++i)
			originOutput->Child[i] = originFolderV[i];
		for (int i = originFolderV.size(); i < size; ++i)
			originOutput->Child[i] = originFileV[i - originFolderV.size()];
		for (int i = 0; i < targetFolderV.size(); ++i)
			targetOutput->Child[i] = targetFolderV[i];
		for (int i = targetFolderV.size(); i < size; ++i)
			targetOutput->Child[i] = targetFileV[i - targetFolderV.size()];

	}
	else if (!bIsOriginFolder && !bIsTargetFolder)
	{
		string originFileName = filesystem::path(originInput).filename().string();
		string targetFileName = filesystem::path(targetInput).filename().string();

		if (originFileName != targetFileName)
		{
			originOutput->Type = LineDiffType::Diff;
			targetOutput->Type = LineDiffType::Diff;

			bReturn = false;
		}
		else
		{
			//if (filesystem::file_size(originInput) != filesystem::file_size(targetInput))
			//{
			//	originOutput->Type = LineDiffType::Diff;
			//	targetOutput->Type = LineDiffType::Diff;
			//	bReturn =  false;
			//}

			ifstream originFile(originInput, std::ios::binary);
			ifstream targetFile(targetInput, std::ios::binary);
			string originContent((std::istreambuf_iterator<char>(originFile)), std::istreambuf_iterator<char>());

			string targetContent((std::istreambuf_iterator<char>(targetFile)), std::istreambuf_iterator<char>());

			if (originContent != targetContent)
			{
				originOutput->Type = LineDiffType::Diff;
				targetOutput->Type = LineDiffType::Diff;
				bReturn = false;
			}

		}
	}
	else
	{
		originOutput->Type = LineDiffType::Diff;
		targetOutput->Type = LineDiffType::Diff;
		bReturn = false;
	}


	if (!bReturn)
	{
		originOutput->Type = LineDiffType::Diff;
		targetOutput->Type = LineDiffType::Diff;
	}

	return bReturn;
}



DIFFALGORITHM_DECLSPEC void FreeMemory(LineInfo_S* lines)
{
	delete[] lines;
}

bool CompareFile(string path1, string path2)
{
	// 폴더인지 파일인지 -> 이름 같은지 -> 내용 같은지
	if (filesystem::is_directory(path1) != filesystem::is_directory(path2))
	{
		return false;
	}

	if (filesystem::path(path1).filename().string() != filesystem::path(path2).filename().string())
	{
		return false;
	}

	if (filesystem::is_directory(path1))
	{
		// 여기서 폴더 비교, 재귀 타이밍 정하기
	}
	else
	{
		ifstream originFile(path1, std::ios::binary);
		ifstream targetFile(path2, std::ios::binary);
		string originContent((std::istreambuf_iterator<char>(originFile)), std::istreambuf_iterator<char>());

		string targetContent((std::istreambuf_iterator<char>(targetFile)), std::istreambuf_iterator<char>());

		if (originContent != targetContent)
			return false;
	}


	return true;
}

bool CreateFolder(FolderNode_S* origin, FolderNode_S* target)
{
	origin->Name = const_cast<LPSTR>("");
	origin->IsDirectory = target->IsDirectory;
	origin->Type = LineDiffType::Added;
	target->Type = LineDiffType::Deleted;
	origin->ChildCount = target->ChildCount;
	origin->Child = new FolderNode_S * [target->ChildCount];
	for (int i = 0; i < target->ChildCount; ++i)
	{
		FolderNode_S* node = new FolderNode_S();
		origin->Child[i] = node;
		CreateFolder(origin->Child[i], target->Child[i]);
	}

	return true;
}
