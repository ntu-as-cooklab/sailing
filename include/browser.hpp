#ifdef _WIN32
#include <windows.h>
#include <ShellApi.h>
#endif

#ifdef linux
#include <unistd.h>
#endif

#include <string>

void launchURL(const std::string &url)
{
	#ifdef _WIN32
	ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	#endif

	#ifdef linux
  	system("xdg-open " + url);
	#endif
}
