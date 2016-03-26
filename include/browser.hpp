#include <string>
#include <os.h>

void launchURL(const std::string &url)
{
	#ifdef _WIN32
	ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	#endif

	#ifdef linux
  	system("xdg-open " + url);
	#endif
}
