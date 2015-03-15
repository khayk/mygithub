#include "StdAfx.h"
#include "ASCII_Chars_Unicode.h"
#include "../utils/Common.h"

#include <boost/lexical_cast.hpp>
#include <sstream>

struct MappingRaw {
    wchar_t        from;
    wchar_t        to;
    const char*    desc;
};

void fileWriter(MappingRaw* map, const int numRows, const string_t& fileName)
{
    std::set<wchar_t> uniqnessCheck;
    std::stringstream ss;

    for (int i = 0; i < numRows; ++i) {
        ss  << toUtf8(wstring_t(1, map[i].from))
            << "|" << toUtf8(wstring_t(1, map[i].to));
            //<< "|" << map[i].desc;
        ss << std::endl;

        if ( !uniqnessCheck.insert(map[i].from).second )
            throw std::logic_error("Character have different mapping: " 
            + boost::lexical_cast<string_t>((int)map[i].from) );
    }

    writeFileAsBinary(fileName, ss.str());
}

void AsciiToUniMapping::exportArmenianASCII_Unicode( const string_t& mapFile )
{
    MappingRaw map[] = {
        { 0x00B2 ,  0x0531, "Ա ARMENIAN CAPITAL LETTER AYB"                },
        { 0x00B4 ,  0x0532, "Բ ARMENIAN CAPITAL LETTER BEN"                },
        { 0x00B6 ,  0x0533, "Գ ARMENIAN CAPITAL LETTER GIM"                },
        { 0x00B8 ,  0x0534, "Դ ARMENIAN CAPITAL LETTER DA"                 },
        { 0x00BA ,  0x0535, "Ե ARMENIAN CAPITAL LETTER ECH"                },
        { 0x00BC ,  0x0536, "Զ ARMENIAN CAPITAL LETTER ZA"                 },
        { 0x00BE ,  0x0537, "Է ARMENIAN CAPITAL LETTER EH"                 },
        { 0x00C0 ,  0x0538, "Ը ARMENIAN CAPITAL LETTER ET"                 },
        { 0x00C2 ,  0x0539, "Թ ARMENIAN CAPITAL LETTER TO"                 },
        { 0x00C4 ,  0x053A, "Ժ ARMENIAN CAPITAL LETTER ZHE"                },
        { 0x00C6 ,  0x053B, "Ի ARMENIAN CAPITAL LETTER INI"                },
        { 0x00C8 ,  0x053C, "Լ ARMENIAN CAPITAL LETTER LIWN"               },
        { 0x00CA ,  0x053D, "Խ ARMENIAN CAPITAL LETTER XEH"                },
        { 0x00CC ,  0x053E, "Ծ ARMENIAN CAPITAL LETTER CA"                 },
        { 0x00CE ,  0x053F, "Կ ARMENIAN CAPITAL LETTER KEN"                },
        { 0x00D0 ,  0x0540, "Հ ARMENIAN CAPITAL LETTER HO"                 },
        { 0x00D2 ,  0x0541, "Ձ ARMENIAN CAPITAL LETTER JA"                 },
        { 0x00D4 ,  0x0542, "Ղ ARMENIAN CAPITAL LETTER GHAD"               },
        { 0x00D6 ,  0x0543, "Ճ ARMENIAN CAPITAL LETTER CHEH"               },
        { 0x00D8 ,  0x0544, "Մ ARMENIAN CAPITAL LETTER MEN"                },
        { 0x00DA ,  0x0545, "Յ ARMENIAN CAPITAL LETTER YI"                 },
        { 0x00DC ,  0x0546, "Ն ARMENIAN CAPITAL LETTER NOW"                },
        { 0x00DE ,  0x0547, "Շ ARMENIAN CAPITAL LETTER SHA"                },
        { 0x00E0 ,  0x0548, "Ո ARMENIAN CAPITAL LETTER VO"                 },
        { 0x00E2 ,  0x0549, "Չ ARMENIAN CAPITAL LETTER CHA"                },
        { 0x00E4 ,  0x054A, "Պ ARMENIAN CAPITAL LETTER PEH"                },
        { 0x00E6 ,  0x054B, "Ջ ARMENIAN CAPITAL LETTER JHEH"               },
        { 0x00E8 ,  0x054C, "Ռ ARMENIAN CAPITAL LETTER RA"                 },
        { 0x00EA ,  0x054D, "Ս ARMENIAN CAPITAL LETTER SEH"                },
        { 0x00EC ,  0x054E, "Վ ARMENIAN CAPITAL LETTER VEW"                },
        { 0x00EE ,  0x054F, "Տ ARMENIAN CAPITAL LETTER TIWN"               },
        { 0x00F0 ,  0x0550, "Ր ARMENIAN CAPITAL LETTER REH"                },
        { 0x00F2 ,  0x0551, "Ց ARMENIAN CAPITAL LETTER CO"                 },
        { 0x00F4 ,  0x0552, "Ւ ARMENIAN CAPITAL LETTER YIWN"               },
        { 0x00F6 ,  0x0553, "Փ ARMENIAN CAPITAL LETTER PIWR"               },
        { 0x00F8 ,  0x0554, "Ք ARMENIAN CAPITAL LETTER KEH"                },
        { 0x00FA ,  0x0555, "Օ ARMENIAN CAPITAL LETTER OH"                 },
        { 0x00FC ,  0x0556, "Ֆ ARMENIAN CAPITAL LETTER FEH"                },

        { 0x00A0 ,  0x00A0, "  NO-BREAK SPACE"                             },
        { 0x00A1 ,  0x00A9, "© COPYRIGHT SIGN"                             },
        { 0x00A2 ,  0x00A7, "§ SECTION SIGN"                               },
        { 0x00A4 ,  0x0029, ") RIGHT PARENTHESIS"                          },
        { 0x00A5 ,  0x0028, "( LEFT PARENTHESIS"                           },
        { 0x00A6 ,  0x00BB, "» RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK" },
        { 0x00A7 ,  0x00AB, "« LEFT-POINTING DOUBLE ANGLE QUOTATION MARK"  },
        { 0x00A9 ,  0x002E, ". FULL STOP"                                  },
        { 0x00AB ,  0x002C, ", COMMA"                                      },
        { 0x00AC ,  0x002D, "- HYPHEN-MINUS"                               },
        { 0x00AD ,  0x058A, "֊ ARMENIAN HYPHEN"                            },
        { 0x00AE ,  0x2026, "… HORIZONTAL ELLIPSIS"                        },
        { 0x00FE ,  0x055A, "՚ ARMENIAN APOSTROPHE"                         },
        { 0x00FF ,  0x055A, "՚ ARMENIAN APOSTROPHE"                         },
        { 0x00B0 ,  0x055B, "՛ ARMENIAN EMPHASIS MARK"                     },
        { 0x00AF ,  0x055C, "՜ ARMENIAN EXCLAMATION MARK"                  },
        { 0x00AA ,  0x055D, "՝ ARMENIAN COMMA"                             },
        { 0x00B1 ,  0x055E, "՞ ARMENIAN QUESTION MARK"                     },

        { 0x00B3 ,  0x0561, "ա ARMENIAN SMALL LETTER AYB"                  },
        { 0x00B5 ,  0x0562, "բ ARMENIAN SMALL LETTER BEN"                  },
        { 0x00B7 ,  0x0563, "գ ARMENIAN SMALL LETTER GIM"                  },
        { 0x00B9 ,  0x0564, "դ ARMENIAN SMALL LETTER DA"                   },
        { 0x00BB ,  0x0565, "ե ARMENIAN SMALL LETTER ECH"                  },
        { 0x00BD ,  0x0566, "զ ARMENIAN SMALL LETTER ZA"                   },
        { 0x00BF ,  0x0567, "է ARMENIAN SMALL LETTER EH"                   },
        { 0x00C1 ,  0x0568, "ը ARMENIAN SMALL LETTER ET"                   },
        { 0x00C3 ,  0x0569, "թ ARMENIAN SMALL LETTER TO"                   },
        { 0x00C5 ,  0x056A, "ժ ARMENIAN SMALL LETTER ZHE"                  },
        { 0x00C7 ,  0x056B, "ի ARMENIAN SMALL LETTER INI"                  },
        { 0x00C9 ,  0x056C, "լ ARMENIAN SMALL LETTER LIWN"                 },
        { 0x00CB ,  0x056D, "խ ARMENIAN SMALL LETTER XEH"                  },
        { 0x00CD ,  0x056E, "ծ ARMENIAN SMALL LETTER CA"                   },
        { 0x00CF ,  0x056F, "կ ARMENIAN SMALL LETTER KEN"                  },
        { 0x00D1 ,  0x0570, "հ ARMENIAN SMALL LETTER HO"                   },
        { 0x00D3 ,  0x0571, "ձ ARMENIAN SMALL LETTER JA"                   },
        { 0x00D5 ,  0x0572, "ղ ARMENIAN SMALL LETTER GHAD"                 },
        { 0x00D7 ,  0x0573, "ճ ARMENIAN SMALL LETTER CHEH"                 },
        { 0x00D9 ,  0x0574, "մ ARMENIAN SMALL LETTER MEN"                  },
        { 0x00DB ,  0x0575, "յ ARMENIAN SMALL LETTER YI"                   },
        { 0x00DD ,  0x0576, "ն ARMENIAN SMALL LETTER NOW"                  },
        { 0x00DF ,  0x0577, "շ ARMENIAN SMALL LETTER SHA"                  },
        { 0x00E1 ,  0x0578, "ո ARMENIAN SMALL LETTER VO"                   },
        { 0x00E3 ,  0x0579, "չ ARMENIAN SMALL LETTER CHA"                  },
        { 0x00E5 ,  0x057A, "պ ARMENIAN SMALL LETTER PEH"                  },
        { 0x00E7 ,  0x057B, "ջ ARMENIAN SMALL LETTER JHEH"                 },
        { 0x00E9 ,  0x057C, "ռ ARMENIAN SMALL LETTER RA"                   },
        { 0x00EB ,  0x057D, "ս ARMENIAN SMALL LETTER SEH"                  },
        { 0x00ED ,  0x057E, "վ ARMENIAN SMALL LETTER VEW"                  },
        { 0x00EF ,  0x057F, "տ ARMENIAN SMALL LETTER TIWN"                 },
        { 0x00F1 ,  0x0580, "ր ARMENIAN SMALL LETTER REH"                  },
        { 0x00F3 ,  0x0581, "ց ARMENIAN SMALL LETTER CO"                   },
        { 0x00F5 ,  0x0582, "ւ ARMENIAN SMALL LETTER YIWN"                 },
        { 0x00F7 ,  0x0583, "փ ARMENIAN SMALL LETTER PIWR"                 },
        { 0x00F9 ,  0x0584, "ք ARMENIAN SMALL LETTER KEH"                  },
        { 0x00FB ,  0x0585, "օ ARMENIAN SMALL LETTER OH"                   },
        { 0x00FD ,  0x0586, "ֆ ARMENIAN SMALL LETTER FEH"                  },
        { 0x00A8 ,  0x0587, "և ARMENIAN SMALL LIGATURE ECH YIWN"           },
        { 0x00A3 ,  0x0589, "։ ARMENIAN FULL STOP"                         },

        { 0x02CA ,  0x055B, "՛ ARMENIAN EMPHASIS MARK"                     },
        { 0x0301 ,  0x0301, "՛ ARMENIAN EMPHASIS MARK"                     },
        { 0x0374 ,  0x0374, "՛ ARMENIAN EMPHASIS MARK"                     },
        { 0x2020 ,  0x2020, "† CROSS"                                      },

        { 0x03BC ,  0x0562, "բ ARMENIAN SMALL LETTER BEN"                  }

        // { 0x0000 ,  0x058A, L"֊ ARMENIAN HYPHEN"                            },
        // { 0x0000 ,  0x058F, L"֏ ARMENIAN DRAM SIGN"                         },
        // { 0x0000 ,  0x0559, L"ՙ ARMENIAN MODIFIER LETTER LEFT HALF RING"    },
        // 
        // { 0x0000 ,  0x055F, L"՟ ARMENIAN ABBREVIATION MARK"                 },
    };
    
    fileWriter(map, sizeof (map) / sizeof(MappingRaw), mapFile);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void AsciiToUniMapping::exportRussianASCII_Unicode( const string_t& mapFile )
{
    MappingRaw map[] = {
        
        { 0x00C0   , 0x0410,  "А CYRILLIC CAPITAL LETTER A"             },
        { 0x00C1   , 0x0411,  "Б CYRILLIC CAPITAL LETTER BE"            },
        { 0x00C2   , 0x0412,  "В CYRILLIC CAPITAL LETTER VE"            },
        { 0x00C3   , 0x0413,  "Г CYRILLIC CAPITAL LETTER GHE"           },
        { 0x00C4   , 0x0414,  "Д CYRILLIC CAPITAL LETTER DE"            },
        { 0x00C5   , 0x0415,  "Е CYRILLIC CAPITAL LETTER IE"            },
        { 0x00C6   , 0x0416,  "Ж CYRILLIC CAPITAL LETTER ZHE"           },
        { 0x00C7   , 0x0417,  "З CYRILLIC CAPITAL LETTER ZE"            },
        { 0x00C8   , 0x0418,  "И CYRILLIC CAPITAL LETTER I"             },
        { 0x00C9   , 0x0419,  "Й CYRILLIC CAPITAL LETTER SHORT I"       },
        { 0x00CA   , 0x041A,  "К CYRILLIC CAPITAL LETTER KA"            },
        { 0x00CB   , 0x041B,  "Л CYRILLIC CAPITAL LETTER EL"            },
        { 0x00CC   , 0x041C,  "М CYRILLIC CAPITAL LETTER EM"            },
        { 0x00CD   , 0x041D,  "Н CYRILLIC CAPITAL LETTER EN"            },
        { 0x00CE   , 0x041E,  "О CYRILLIC CAPITAL LETTER O"             },
        { 0x00CF   , 0x041F,  "П CYRILLIC CAPITAL LETTER PE"            },
        { 0x00D0   , 0x0420,  "Р CYRILLIC CAPITAL LETTER ER"            },
        { 0x00D1   , 0x0421,  "С CYRILLIC CAPITAL LETTER ES"            },
        { 0x00D2   , 0x0422,  "Т CYRILLIC CAPITAL LETTER TE"            },
        { 0x00D3   , 0x0423,  "У CYRILLIC CAPITAL LETTER U"             },
        { 0x00D4   , 0x0424,  "Ф CYRILLIC CAPITAL LETTER EF"            },
        { 0x00D5   , 0x0425,  "Х CYRILLIC CAPITAL LETTER HA"            },
        { 0x00D6   , 0x0426,  "Ц CYRILLIC CAPITAL LETTER TSE"           },
        { 0x00D7   , 0x0427,  "Ч CYRILLIC CAPITAL LETTER CHE"           },
        { 0x00D8   , 0x0428,  "Ш CYRILLIC CAPITAL LETTER SHA"           },
        { 0x00D9   , 0x0429,  "Щ CYRILLIC CAPITAL LETTER SHCHA"         },
        { 0x00DA   , 0x042A,  "Ъ CYRILLIC CAPITAL LETTER HARD SIGN"     },
        { 0x00DB   , 0x042B,  "Ы CYRILLIC CAPITAL LETTER YERU"          },
        { 0x00DC   , 0x042C,  "Ь CYRILLIC CAPITAL LETTER SOFT SIGN"     },
        { 0x00DD   , 0x042D,  "Э CYRILLIC CAPITAL LETTER E"             },
        { 0x00DE   , 0x042E,  "Ю CYRILLIC CAPITAL LETTER YU"            },
        { 0x00DF   , 0x042F,  "Я CYRILLIC CAPITAL LETTER YA"            },

        { 0x00E0   , 0x0430,  "а CYRILLIC SMALL LETTER A"               },
        { 0x00E1   , 0x0431,  "б CYRILLIC SMALL LETTER BE"              },
        { 0x00E2   , 0x0432,  "в CYRILLIC SMALL LETTER VE"              },
        { 0x00E3   , 0x0433,  "г CYRILLIC SMALL LETTER GHE"             },
        { 0x00E4   , 0x0434,  "д CYRILLIC SMALL LETTER DE"              },
        { 0x00E5   , 0x0435,  "е CYRILLIC SMALL LETTER IE"              },
        { 0x00E6   , 0x0436,  "ж CYRILLIC SMALL LETTER ZHE"             },
        { 0x00E7   , 0x0437,  "з CYRILLIC SMALL LETTER ZE"              },
        { 0x00E8   , 0x0438,  "и CYRILLIC SMALL LETTER I"               },
        { 0x00E9   , 0x0439,  "й CYRILLIC SMALL LETTER SHORT I"         },
        { 0x00EA   , 0x043A,  "к CYRILLIC SMALL LETTER KA"              },
        { 0x00EB   , 0x043B,  "л CYRILLIC SMALL LETTER EL"              },
        { 0x00EC   , 0x043C,  "м CYRILLIC SMALL LETTER EM"              },
        { 0x00ED   , 0x043D,  "н CYRILLIC SMALL LETTER EN"              },
        { 0x00EE   , 0x043E,  "о CYRILLIC SMALL LETTER O"               },
        { 0x00EF   , 0x043F,  "п CYRILLIC SMALL LETTER PE"              },
        { 0x00F0   , 0x0440,  "р CYRILLIC SMALL LETTER ER"              },
        { 0x00F1   , 0x0441,  "с CYRILLIC SMALL LETTER ES"              },
        { 0x00F2   , 0x0442,  "т CYRILLIC SMALL LETTER TE"              },
        { 0x00F3   , 0x0443,  "у CYRILLIC SMALL LETTER U"               },
        { 0x00F4   , 0x0444,  "ф CYRILLIC SMALL LETTER EF"              },
        { 0x00F5   , 0x0445,  "х CYRILLIC SMALL LETTER HA"              },
        { 0x00F6   , 0x0446,  "ц CYRILLIC SMALL LETTER TSE"             },
        { 0x00F7   , 0x0447,  "ч CYRILLIC SMALL LETTER CHE"             },
        { 0x00F8   , 0x0448,  "ш CYRILLIC SMALL LETTER SHA"             },
        { 0x00F9   , 0x0449,  "щ CYRILLIC SMALL LETTER SHCHA"           },
        { 0x00FA   , 0x044A,  "ъ CYRILLIC SMALL LETTER HARD SIGN"       },
        { 0x00FB   , 0x044B,  "ы CYRILLIC SMALL LETTER YERU"            },
        { 0x00FC   , 0x044C,  "ь CYRILLIC SMALL LETTER SOFT SIGN"       },
        { 0x00FD   , 0x044D,  "э CYRILLIC SMALL LETTER E"               },
        { 0x00FE   , 0x044E,  "ю CYRILLIC SMALL LETTER YU"              },
        { 0x00FF   , 0x044F,  "я CYRILLIC SMALL LETTER YA"              },

        { 0x00A1   , 0x0401,  "Ё CYRILLIC CAPITAL LETTER IO"            },
        { 0x00BF   , 0x0451,  "ё CYRILLIC SMALL LETTER IO"              },

        { 0x00A8   , 0x2116,  "№ NUMERO SIGN"                           }        
    };

    fileWriter(map, sizeof (map) / sizeof(MappingRaw), mapFile);
}

void AsciiToUniMapping::createArafi_UnicodeMapping( const string_t& mapFile )
{
    MappingRaw map[] = {
        { 0x47  ,  0x0531, "Ա ARMENIAN CAPITAL LETTER AYB"                },
        { 0x45  ,  0x0532, "Բ ARMENIAN CAPITAL LETTER BEN"                },
        { 0x44  ,  0x0533, "Գ ARMENIAN CAPITAL LETTER GIM"                },
        { 0x58  ,  0x0534, "Դ ARMENIAN CAPITAL LETTER DA"                 },
        { 0x46  ,  0x0535, "Ե ARMENIAN CAPITAL LETTER ECH"                },
        { 0x42  ,  0x0536, "Զ ARMENIAN CAPITAL LETTER ZA"                 },
        { 0x2B  ,  0x0537, "Է ARMENIAN CAPITAL LETTER EH"                 },
        { 0x4F  ,  0x0538, "Ը ARMENIAN CAPITAL LETTER ET"                 },
        { 0x50  ,  0x0539, "Թ ARMENIAN CAPITAL LETTER TO"                 },
        { 0x5A  ,  0x053A, "Ժ ARMENIAN CAPITAL LETTER ZHE"                },
        { 0x4A  ,  0x053B, "Ի ARMENIAN CAPITAL LETTER INI"                },
        { 0x4E  ,  0x053C, "Լ ARMENIAN CAPITAL LETTER LIWN"               },
        { 0x23  ,  0x053D, "Խ ARMENIAN CAPITAL LETTER XEH"                },
        { 0x29  ,  0x053E, "Ծ ARMENIAN CAPITAL LETTER CA"                 },
        { 0x49  ,  0x053F, "Կ ARMENIAN CAPITAL LETTER KEN"                },
        { 0x4C  ,  0x0540, "Հ ARMENIAN CAPITAL LETTER HO"                 },
        { 0x5E  ,  0x0541, "Ձ ARMENIAN CAPITAL LETTER JA"                 },
        { 0x2A  ,  0x0542, "Ղ ARMENIAN CAPITAL LETTER GHAD"               },
        { 0x51  ,  0x0543, "Ճ ARMENIAN CAPITAL LETTER CHEH"               },
        { 0x54  ,  0x0544, "Մ ARMENIAN CAPITAL LETTER MEN"                },
        { 0x26  ,  0x0545, "Յ ARMENIAN CAPITAL LETTER YI"                 },
        { 0x48  ,  0x0546, "Ն ARMENIAN CAPITAL LETTER NOW"                },
        { 0x24  ,  0x0547, "Շ ARMENIAN CAPITAL LETTER SHA"                },
        { 0x59  ,  0x0548, "Ո ARMENIAN CAPITAL LETTER VO"                 },
        { 0x43  ,  0x0549, "Չ ARMENIAN CAPITAL LETTER CHA"                },
        { 0x3A  ,  0x054A, "Պ ARMENIAN CAPITAL LETTER PEH"                },
        { 0x41  ,  0x054B, "Ջ ARMENIAN CAPITAL LETTER JHEH"               },
        { 0x25  ,  0x054C, "Ռ ARMENIAN CAPITAL LETTER RA"                 },
        { 0x52  ,  0x054D, "Ս ARMENIAN CAPITAL LETTER SEH"                },
        { 0x53  ,  0x054E, "Վ ARMENIAN CAPITAL LETTER VEW"                },
        { 0x4B  ,  0x054F, "Տ ARMENIAN CAPITAL LETTER TIWN"               },
        { 0x28  ,  0x0550, "Ր ARMENIAN CAPITAL LETTER REH"                },
        { 0x56  ,  0x0551, "Ց ARMENIAN CAPITAL LETTER CO"                 },
        { 0x55  ,  0x0552, "Ւ ARMENIAN CAPITAL LETTER YIWN"               },
        { 0x57  ,  0x0553, "Փ ARMENIAN CAPITAL LETTER PIWR"               },
        { 0x40  ,  0x0554, "Ք ARMENIAN CAPITAL LETTER KEH"                },
        { 0x21  ,  0x0555, "Օ ARMENIAN CAPITAL LETTER OH"                 },
        { 0x7E  ,  0x0556, "Ֆ ARMENIAN CAPITAL LETTER FEH"                },

        // { 0x   ,  0x00A0, "  NO-BREAK SPACE"                             },
        // { 0x   ,  0x00A7, "§ SECTION SIGN"                               },
        { 0x7D  ,  0x0029, ") RIGHT PARENTHESIS"                          },
        { 0x22  ,  0x0028, "( LEFT PARENTHESIS"                           },
        { 0xF7  ,  0x00BB, "» RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK" },
        { 0x2265,  0x00AB, "« LEFT-POINTING DOUBLE ANGLE QUOTATION MARK"  },
        { 0x3E  ,  0x002E, ". FULL STOP"                                  },
        { 0x2C  ,  0x002C, ", COMMA"                                      },
        { 0x2D  ,  0x002D, "- HYPHEN-MINUS"                               },
        { 0xB5  ,  0x2026, "… HORIZONTAL ELLIPSIS"                        },
        { 0x5F  ,  0x005F, "_ LOW LINE"                        },
        { 0x27  ,  0x055A, "՚ ARMENIAN APOSTROPHE"                         },
        { 0x5D  ,  0x055B, "՛ ARMENIAN EMPHASIS MARK"                     },
        { 0x3F  ,  0x055C, "՜ ARMENIAN EXCLAMATION MARK"                  },
        { 0x6D  ,  0x055D, "՝ ARMENIAN COMMA"                             },
        { 0x2E  ,  0x055E, "՞ ARMENIAN QUESTION MARK"                     },

        { 0x67  ,  0x0561, "ա ARMENIAN SMALL LETTER AYB"                  },
        { 0x65  ,  0x0562, "բ ARMENIAN SMALL LETTER BEN"                  },
        { 0x64  ,  0x0563, "գ ARMENIAN SMALL LETTER GIM"                  },
        { 0x78  ,  0x0564, "դ ARMENIAN SMALL LETTER DA"                   },
        { 0x66  ,  0x0565, "ե ARMENIAN SMALL LETTER ECH"                  },
        { 0x62  ,  0x0566, "զ ARMENIAN SMALL LETTER ZA"                   },
        { 0x3D  ,  0x0567, "է ARMENIAN SMALL LETTER EH"                   },
        { 0x6F  ,  0x0568, "ը ARMENIAN SMALL LETTER ET"                   },
        { 0x70  ,  0x0569, "թ ARMENIAN SMALL LETTER TO"                   },
        { 0x7A  ,  0x056A, "ժ ARMENIAN SMALL LETTER ZHE"                  },
        { 0x6A  ,  0x056B, "ի ARMENIAN SMALL LETTER INI"                  },
        { 0x6E  ,  0x056C, "լ ARMENIAN SMALL LETTER LIWN"                 },
        { 0x33  ,  0x056D, "խ ARMENIAN SMALL LETTER XEH"                  },
        { 0x30  ,  0x056E, "ծ ARMENIAN SMALL LETTER CA"                   },
        { 0x69  ,  0x056F, "կ ARMENIAN SMALL LETTER KEN"                  },
        { 0x6C  ,  0x0570, "հ ARMENIAN SMALL LETTER HO"                   },
        { 0x36  ,  0x0571, "ձ ARMENIAN SMALL LETTER JA"                   },
        { 0x38  ,  0x0572, "ղ ARMENIAN SMALL LETTER GHAD"                 },
        { 0x71  ,  0x0573, "ճ ARMENIAN SMALL LETTER CHEH"                 },
        { 0x74  ,  0x0574, "մ ARMENIAN SMALL LETTER MEN"                  },
        { 0x37  ,  0x0575, "յ ARMENIAN SMALL LETTER YI"                   },
        { 0x68  ,  0x0576, "ն ARMENIAN SMALL LETTER NOW"                  },
        { 0x34  ,  0x0577, "շ ARMENIAN SMALL LETTER SHA"                  },
        { 0x79  ,  0x0578, "ո ARMENIAN SMALL LETTER VO"                   },
        { 0x63  ,  0x0579, "չ ARMENIAN SMALL LETTER CHA"                  },
        { 0x3B  ,  0x057A, "պ ARMENIAN SMALL LETTER PEH"                  },
        { 0x61  ,  0x057B, "ջ ARMENIAN SMALL LETTER JHEH"                 },
        { 0x35  ,  0x057C, "ռ ARMENIAN SMALL LETTER RA"                   },
        { 0x72  ,  0x057D, "ս ARMENIAN SMALL LETTER SEH"                  },
        { 0x73  ,  0x057E, "վ ARMENIAN SMALL LETTER VEW"                  },
        { 0x6B  ,  0x057F, "տ ARMENIAN SMALL LETTER TIWN"                 },
        { 0x39  ,  0x0580, "ր ARMENIAN SMALL LETTER REH"                  },
        { 0x76  ,  0x0581, "ց ARMENIAN SMALL LETTER CO"                   },
        { 0x75  ,  0x0582, "ւ ARMENIAN SMALL LETTER YIWN"                 },
        { 0x77  ,  0x0583, "փ ARMENIAN SMALL LETTER PIWR"                 },
        { 0x32  ,  0x0584, "ք ARMENIAN SMALL LETTER KEH"                  },
        { 0x31  ,  0x0585, "օ ARMENIAN SMALL LETTER OH"                   },
        { 0x60  ,  0x0586, "ֆ ARMENIAN SMALL LETTER FEH"                  },
        { 0x5B  ,  0x0587, "և ARMENIAN SMALL LIGATURE ECH YIWN"           },


        { 0x2F  ,  0x0589, "։ ARMENIAN FULL STOP"                         },
        { 0xD8  ,  0x7B,   "{" },
        { 0x220F,  0x7D,   "}" },
        { 0xDA  ,  0x7C,   "|" },
        { 0xE5  ,  0x2A,   "*" },
        { 0xE6  ,  0x2F,   "/" },
        { 0xBA  ,  0x30,   "0" },
        { 0xA1  ,  0x31,   "1" },
        { 0x2122,  0x32,   "2" },
        { 0xA3  ,  0x33,   "3" },
        { 0xA2  ,  0x34,   "4" },
        { 0x221E,  0x35,   "5" },
        { 0xA7  ,  0x36,   "6" },
        { 0xB6  ,  0x37,   "7" },
        { 0x2022,  0x38,   "8" },
        { 0xAA  ,  0x39,   "9" },
        { 0x3C  ,  0x2D,   "-" }
//         { 0x   ,  0x, "" },
//         { 0x   ,  0x, "" },
        // { 0x    ,  0x, "" },
        // { 0x   ,  0x, "" },
        // { 0x   ,  0x, "" },
        


        // { 0x   ,  0x055B, "՛ ARMENIAN EMPHASIS MARK"                     },
        // { 0x   ,  0x0301, "՛ ARMENIAN EMPHASIS MARK"                     },
        // { 0x   ,  0x0374, "՛ ARMENIAN EMPHASIS MARK"                     },
        // { 0x   ,  0x0562, "բ ARMENIAN SMALL LETTER BEN"                  }
    };

    fileWriter(map, sizeof (map) / sizeof(MappingRaw), mapFile);
}

