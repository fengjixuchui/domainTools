#include "LdapApi.h"

HANDLE CreateFileApi(LPCWSTR fileName)
{
	HANDLE hFile;		// ���
	hFile = CreateFile(fileName,                // name of the write
		GENERIC_WRITE,          // open for writing
		FILE_SHARE_READ,                      // do not share
		NULL,                   // default security
		OPEN_ALWAYS,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template
	return hFile;
}

// �ַ����ָ�
std::vector<std::wstring> splitString2(std::wstring strSrc, std::wstring pattern)
{
	std::vector<std::wstring> resultstr;

	// ������ַ�����󣬿��Խ�ȡ���һ������
	std::wstring strcom = strSrc.append(pattern);
	// wprintf(L"%s\n", strcom);
	auto pos = strSrc.find(pattern);
	auto len = strcom.size();

	//
	while (pos != std::wstring::npos)
	{
		std::wstring coStr = strcom.substr(0, pos);
		// wprintf(L"%s ", coStr.c_str());
		resultstr.push_back(coStr);

		strcom = strcom.substr(pos + pattern.size(), len);
		pos = strcom.find(pattern);
	}

	return resultstr;
}


int wmain(int argc, wchar_t* argv[])
{

	setlocale(LC_ALL, "");							// ��������
	if (argc != 4) {
		wprintf(L"Usage: %s <DC> <domainname\\username> <password>\n", argv[0]);
		wprintf(L"       %s ����� ����\\���û� ���û�����\n", argv[0]);
		wprintf(L"       %s hack.local hack\\username password\n", argv[0]);
		exit(1);
	}
	LPWSTR lpDCName = argv[1];							// hack.local
	LPWSTR lpDomainUserName = argv[2];					// hack\iis_user
	LPWSTR lpDomainUserPassword = argv[3];
	HANDLE hDelegFile = CreateFileApi(L"Deleg.txt");				// ί��©��

	std::vector<std::wstring> aaa;					// ���봴��vector�����ı���ȥ����splitString���ص�ֵ����Ȼȡ�������ݻ�����
	aaa = splitString2(lpDomainUserName, L"\\");
	LPCWSTR lpDomainName = aaa[0].c_str();			// ȡ������  eg:hack
	LPCWSTR lpUserName = aaa[1].c_str();			// ���û���	eg:iis_user

	wprintf(L"DCName: %s\nUserName: %s\nPassword: %s\n", lpDCName, lpUserName, lpDomainUserPassword);
	// ���ί��©����������Դ��Լ��ί�ɣ�
	wprintf(L"------------------------------------check delegationVul...------------------------------------\n");
	LdapApi theLdapApi(lpDCName, (PWCHAR)lpUserName, (PWCHAR)lpDomainUserPassword, hDelegFile);
	int iConnRet = theLdapApi.connect();
	if (iConnRet != 1) {
		exit(0);
	}
	theLdapApi.RBCD();
	theLdapApi.CD();
	theLdapApi.ud();
}