#include <iostream>

bool allCharsUnique(const char* s) {
   bool cs[0xff] = {0};
   unsigned char ch;
   while ( (ch = static_cast<unsigned char>(*s)) != '\0' ) {
      if ( cs[ch] )
         return false;
      cs[ch] = true;
      ++s;
   }
   return true;
}

void test(const char* s) {
   if (allCharsUnique(s))
      std::cout << s << ": (YES)" << std::endl;
   else
      std::cout << s << ": (NO)" << std::endl;
}

int main() {
   test("1");
   test("22");
   test("bcd");
   test("something");
   test("hayk");
   test("ararat");
   test("?344");
   test("");
   test("abca");
   
   char p[] = "12345";
   p[0] = static_cast<char>(-3);
   p[1] = static_cast<char>(-3);
   test(p);

   return 0;
}