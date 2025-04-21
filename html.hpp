#include <iostream>

using namespace std;

class web
{
    private:
        bool starts_with(string line, string find)
        {
            return line.compare(0, find.length(), find) == 0;
        }
    public:
        string parse(string raw)
        {
            string compiled;
            if (starts_with(raw, "; "))
            {
                raw.replace(0, 2, "");
                compiled = "<h1>" + raw + "</h1>";
            }
            if (starts_with(raw, ": "))
            {
                raw.replace(0, 2, "");
                compiled = "<h2>" + raw + "</h2>";
            }
            if (starts_with(raw, "* "))
            {
                raw.replace(0, 2, "");
                compiled = "<a>" + raw + "</a>";
                if (raw.find_last_of(" ") != string::npos)
                {
                    try
                    {
                        string link = raw.substr(0, raw.length() - raw.find_last_of(" "));
                        raw.replace(raw.find(link), link.length(), "");
                        compiled = "<a href=\"" + link + "\">" + raw + "</a>";
                    }
                    catch (exception error)
                    {
                        return compiled;
                    }
                }
            }
            return compiled;
        }
};