#include "StdAfx.h"
#include "ASCII_Chars_Unicode.h"
#include "../utils/Common.h"

#include <sstream>

void AsciiToUniMapping::exportArmenianASCII_Unicode( const string_t& mapFile )
{
    wchar_t map[][2] = {

        { 0x00B2   ,  0x0531 }, // Ա ARMENIAN CAPITAL LETTER AYB
        { 0x00B4   ,  0x0532 }, // Բ ARMENIAN CAPITAL LETTER BEN
        { 0x00B6   ,  0x0533 }, // Գ ARMENIAN CAPITAL LETTER GIM
        { 0x00B8   ,  0x0534 }, // Դ ARMENIAN CAPITAL LETTER DA
        { 0x00BA   ,  0x0535 }, // Ե ARMENIAN CAPITAL LETTER ECH
        { 0x00BC   ,  0x0536 }, // Զ ARMENIAN CAPITAL LETTER ZA
        { 0x00BE   ,  0x0537 }, // Է ARMENIAN CAPITAL LETTER EH
        { 0x00C0   ,  0x0538 }, // Ը ARMENIAN CAPITAL LETTER ET
        { 0x00C2   ,  0x0539 }, // Թ ARMENIAN CAPITAL LETTER TO
        { 0x00C4   ,  0x053A }, // Ժ ARMENIAN CAPITAL LETTER ZHE
        { 0x00C6   ,  0x053B }, // Ի ARMENIAN CAPITAL LETTER INI
        { 0x00C8   ,  0x053C }, // Լ ARMENIAN CAPITAL LETTER LIWN
        { 0x00CA   ,  0x053D }, // Խ ARMENIAN CAPITAL LETTER XEH
        { 0x00CC   ,  0x053E }, // Ծ ARMENIAN CAPITAL LETTER CA
        { 0x00CE   ,  0x053F }, // Կ ARMENIAN CAPITAL LETTER KEN
        { 0x00D0   ,  0x0540 }, // Հ ARMENIAN CAPITAL LETTER HO
        { 0x00D2   ,  0x0541 }, // Ձ ARMENIAN CAPITAL LETTER JA
        { 0x00D4   ,  0x0542 }, // Ղ ARMENIAN CAPITAL LETTER GHAD
        { 0x00D6   ,  0x0543 }, // Ճ ARMENIAN CAPITAL LETTER CHEH
        { 0x00D8   ,  0x0544 }, // Մ ARMENIAN CAPITAL LETTER MEN
        { 0x00DA   ,  0x0545 }, // Յ ARMENIAN CAPITAL LETTER YI
        { 0x00DC   ,  0x0546 }, // Ն ARMENIAN CAPITAL LETTER NOW
        { 0x00DE   ,  0x0547 }, // Շ ARMENIAN CAPITAL LETTER SHA
        { 0x00E0   ,  0x0548 }, // Ո ARMENIAN CAPITAL LETTER VO
        { 0x00E2   ,  0x0549 }, // Չ ARMENIAN CAPITAL LETTER CHA
        { 0x00E4   ,  0x054A }, // Պ ARMENIAN CAPITAL LETTER PEH
        { 0x00E6   ,  0x054B }, // Ջ ARMENIAN CAPITAL LETTER JHEH
        { 0x00E8   ,  0x054C }, // Ռ ARMENIAN CAPITAL LETTER RA
        { 0x00EA   ,  0x054D }, // Ս ARMENIAN CAPITAL LETTER SEH
        { 0x00EC   ,  0x054E }, // Վ ARMENIAN CAPITAL LETTER VEW
        { 0x00EE   ,  0x054F }, // Տ ARMENIAN CAPITAL LETTER TIWN
        { 0x00F0   ,  0x0550 }, // Ր ARMENIAN CAPITAL LETTER REH
        { 0x00F2   ,  0x0551 }, // Ց ARMENIAN CAPITAL LETTER CO
        { 0x00F4   ,  0x0552 }, // Ւ ARMENIAN CAPITAL LETTER YIWN
        { 0x00F6   ,  0x0553 }, // Փ ARMENIAN CAPITAL LETTER PIWR
        { 0x00F8   ,  0x0554 }, // Ք ARMENIAN CAPITAL LETTER KEH
        { 0x00FA   ,  0x0555 }, // Օ ARMENIAN CAPITAL LETTER OH
        { 0x00FC   ,  0x0556 }, // Ֆ ARMENIAN CAPITAL LETTER FEH


        { 0x00    ,  0x0559 }, // ՙ ARMENIAN MODIFIER LETTER LEFT HALF RING
        { 0x00    ,  0x055A }, // ՚ ARMENIAN APOSTROPHE
        { 0x00    ,  0x055B }, // ՛ ARMENIAN EMPHASIS MARK
        { 0x00    ,  0x055C }, // ՜ ARMENIAN EXCLAMATION MARK
        { 0x00    ,  0x055D }, // ՝ ARMENIAN COMMA
        { 0x00    ,  0x055E }, // ՞ ARMENIAN QUESTION MARK
        { 0x00    ,  0x055F }, // ՟ ARMENIAN ABBREVIATION MARK


        { 0x00B3   ,  0x0561 }, // ա ARMENIAN SMALL LETTER AYB
        { 0x00B5   ,  0x0562 }, // բ ARMENIAN SMALL LETTER BEN
        { 0x00B7   ,  0x0563 }, // գ ARMENIAN SMALL LETTER GIM
        { 0x00B9   ,  0x0564 }, // դ ARMENIAN SMALL LETTER DA
        { 0x00BB   ,  0x0565 }, // ե ARMENIAN SMALL LETTER ECH
        { 0x00BD   ,  0x0566 }, // զ ARMENIAN SMALL LETTER ZA
        { 0x00BF   ,  0x0567 }, // է ARMENIAN SMALL LETTER EH
        { 0x00C1   ,  0x0568 }, // ը ARMENIAN SMALL LETTER ET
        { 0x00C3   ,  0x0569 }, // թ ARMENIAN SMALL LETTER TO
        { 0x00C5   ,  0x056A }, // ժ ARMENIAN SMALL LETTER ZHE
        { 0x00C7   ,  0x056B }, // ի ARMENIAN SMALL LETTER INI
        { 0x00C9   ,  0x056C }, // լ ARMENIAN SMALL LETTER LIWN
        { 0x00CB   ,  0x056D }, // խ ARMENIAN SMALL LETTER XEH
        { 0x00CD   ,  0x056E }, // ծ ARMENIAN SMALL LETTER CA
        { 0x00CF   ,  0x056F }, // կ ARMENIAN SMALL LETTER KEN
        { 0x00D1   ,  0x0570 }, // հ ARMENIAN SMALL LETTER HO
        { 0x00D3   ,  0x0571 }, // ձ ARMENIAN SMALL LETTER JA
        { 0x00D5   ,  0x0572 }, // ղ ARMENIAN SMALL LETTER GHAD
        { 0x00D7   ,  0x0573 }, // ճ ARMENIAN SMALL LETTER CHEH
        { 0x00D9   ,  0x0574 }, // մ ARMENIAN SMALL LETTER MEN
        { 0x00DB   ,  0x0575 }, // յ ARMENIAN SMALL LETTER YI
        { 0x00DD   ,  0x0576 }, // ն ARMENIAN SMALL LETTER NOW
        { 0x00DF   ,  0x0577 }, // շ ARMENIAN SMALL LETTER SHA
        { 0x00E1   ,  0x0578 }, // ո ARMENIAN SMALL LETTER VO
        { 0x00E3   ,  0x0579 }, // չ ARMENIAN SMALL LETTER CHA
        { 0x00E5   ,  0x057A }, // պ ARMENIAN SMALL LETTER PEH
        { 0x00E7   ,  0x057B }, // ջ ARMENIAN SMALL LETTER JHEH
        { 0x00E9   ,  0x057C }, // ռ ARMENIAN SMALL LETTER RA
        { 0x00EB   ,  0x057D }, // ս ARMENIAN SMALL LETTER SEH
        { 0x00ED   ,  0x057E }, // վ ARMENIAN SMALL LETTER VEW
        { 0x00EF   ,  0x057F }, // տ ARMENIAN SMALL LETTER TIWN
        { 0x00F1   ,  0x0580 }, // ր ARMENIAN SMALL LETTER REH
        { 0x00F3   ,  0x0581 }, // ց ARMENIAN SMALL LETTER CO
        { 0x00F5   ,  0x0582 }, // ւ ARMENIAN SMALL LETTER YIWN
        { 0x00F7   ,  0x0583 }, // փ ARMENIAN SMALL LETTER PIWR
        { 0x00F9   ,  0x0584 }, // ք ARMENIAN SMALL LETTER KEH
        { 0x00FB   ,  0x0585 }, // օ ARMENIAN SMALL LETTER OH
        { 0x00FD   ,  0x0586 }, // ֆ ARMENIAN SMALL LETTER FEH
        { 0x00A8   ,  0x0587 }, // և ARMENIAN SMALL LIGATURE ECH YIWN


        { 0x00   , 0x0589 }, // ։ ARMENIAN FULL STOP
        { 0x00   , 0x058A }, // ֊ ARMENIAN HYPHEN
        { 0x00   , 0x058F }, // ֏ ARMENIAN DRAM SIGN

    };

    int rows = sizeof (map) / sizeof(sizeof(map[0]));
    std::stringstream ss;
    ss.write((const char*)&map[0][0], 2*sizeof(wchar_t));
    ss << std::endl;

    writeFileAsBinary(mapFile, ss.str());
}
