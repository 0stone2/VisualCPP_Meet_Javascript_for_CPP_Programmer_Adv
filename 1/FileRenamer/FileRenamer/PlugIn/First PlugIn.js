/*
JavaScript (First PlugIn.js)코드입니다.
*/

function GetInfo () {
	var szName = "첫번째 플러그인";
	var szDesc = "나의 첫번째 플러그인입니다";
	var szVersion = "1.0.0";
	var szDate = "2016.04.22";
	var szAuthor = "박창진";
	var szEmail = "crazevil@gmail.com";

	return [szName, szDesc, szVersion, szDate, szAuthor, szEmail];
}

function Rename (szSrcFile) {
	nIndex = szSrcFile.lastIndexOf("\\");
	szFolder = szSrcFile.slice(0, nIndex)
	szFile = szSrcFile.slice(szFolder.length + 1)


	nIndex = szFile.lastIndexOf(".");
	if (nIndex < 0)
	{
		szExt = "";
	}
	else {
		szExt = szFile.slice(nIndex);
	}
	
	DbgString("Ext nIndex : " + nIndex.toString());


	nNumber = szFile.match(/\d+/g)[0]


	DbgString(szSrcFile);
	DbgString("Folder : " + szFolder)
	DbgString("Ext : " + szExt);

	DbgString("Number : " + nNumber.toString());
	
	DbgString(szFolder + "\\" + nNumber.toString() + szExt);

	return szFolder + "\\" + nNumber.toString() + szExt;
}

