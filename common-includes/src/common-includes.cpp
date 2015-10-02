// common-includes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <utils/Common.h>
#include <utils/FileFinder.h>

#include <Poco/Exception.h>
#include <Poco/FileStream.h>
#include <Poco/String.h>

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc != 2) {
        std::cout << "Specify input directory: executable inputdir" << std::endl;
        return 0;
    }

    try {
        FileFinder ff(true, toUtf8(argv[1]), "h;cpp");
        tStringSet files = ff.getFiles();

        std::map<string_t, int> occurencies;

        for (auto it = files.begin(); it != files.end(); ++it) {
            Poco::FileInputStream fis(ff.getRootPath() + *it);

            while (fis) {
                string_t l;
                getline(fis, l);

                string_t::size_type p = l.find("#include ");
                if ( p != string_t::npos ) {
                    if ( !Poco::trim(l.substr(0, p)).empty() )
                        continue;

                    string_t::size_type xpos = l.find_last_of("\\/");
                    string_t::size_type epos = l.find_last_of("\">");

                    if ( xpos == string_t::npos )
                        xpos = l.find_first_of("\"<");
                    if ( xpos != string_t::npos ) {
                        string_t fn = l.substr(xpos + 1, epos - xpos - 1);
                        ++occurencies[fn];
                    }
                }
            }
        }

        std::multimap<int, string_t> omm;

        for (auto cit = occurencies.rbegin(); cit != occurencies.rend(); ++cit) {
            omm.insert( std::make_pair(cit->second, cit->first) );
        }

        for (auto cit = omm.rbegin(); cit != omm.rend(); ++cit) {
            std::cout << std::setw(5) << cit->first << " - " << cit->second << std::endl;
        }
    }
    catch (const Poco::Exception& pe) {
        std::cout << pe.displayText() << std::endl;
    }
    catch ( ... ) {
        std::cout << "Unknown exception." << std::endl;
    }

	return 0;
}

