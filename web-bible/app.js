(function() {

var _    = require('underscore');
var fs   = require('fs');
var path = require('path');


var theApp = {
   startTime: null,
   endTime: null,
};

function extend(child, parent) {
   var fnObj = function() {};
   fnObj.prototype = parent.prototype;
   child.prototype = new fnObj();
   child.prototype.constructor = child;
   child.uber = parent.prototype;
}


function extend2(Child, Parent) {
   var p = Parent.prototype;
   var c = Child.prototype;
   for (var i in p) {
      c[i] = p[i];
   }
   c.uber = p;
}


function extendCopy(p) {
   var c = {};
   for (var i in p) {
      c[i] = p[i];
   }
   c.uber = p;
   return c;
}

/*
function deepCopy(p, c) {
   var c = c || {};
   for (var i in p) {
      if (typeof p[i] === 'object') {
         c[i] = (p[i].constructor === Array) ? [] : {}; [] : {};
         deepCopy(p[i], c[i]);
      } else {
         c[i] = p[i]; c[i] = p[i];
      }
   }
   return c;
}*/


function object(o) {
   function F() {}
   F.prototype = o;
   return new F();
}


function objectEx(o) {
   var n;
   function F() {}
   F.prototype = o;
   n = new F();
   n.uber = o;
   return n;
}


// Borrow a constructor and copy the prototype
function child() {
   parent.apply(this, arguments);
}
//extend2(child, parent);

// ---------------------------------------------------------------
// Collect all words for fast searching
function Dictionary() {

   // case sensitive and insensitive words
   this.ciWords = {};
   this.csWords = {};

   this.addWord = function(word, ref) {
      // ignore empty words
      if (word.length === 0) {
         //console.log("empty word with reference", ref);
         return;
      }

      var valRef = this.csWords[word];
      if ( _.isUndefined( valRef ) ) {
         this.csWords[word] = {count: 1, refer: ref};
      }
      else {
         ++this.csWords[word].count;
      }

      // case insensitive words
      var ciWord = word.toLowerCase();
      valRef = this.ciWords[ciWord];
      if ( _.isUndefined( valRef ) ) {
         this.ciWords[ciWord] = {count: 1, refer: ref};
      }
      else {
         ++this.ciWords[ciWord].count;
      }
   };

   this.getNumCSWords = function() {
      return Object.keys(this.csWords).length;
   };

   this.getNumCIWords = function() {
      return Object.keys(this.ciWords).length;
   };

   this.showStatistics = function() {
      console.log("Case   sensitive words count: ", this.getNumCSWords());
      console.log("Case insensitive words count: ", this.getNumCIWords());

      // for (a in this.ciWords) {
      //    console.log(a);
      // }
      //console.log(Object.keys(this.ciWords)[0]);
   };
}

// ---------------------------------------------------------------
// Represents a single verse in a chapter
function Verse(chapter, number, text, newParagraph) {
   this.parent       = chapter;
   this.number       = number;
   this.text         = text;
   this.newParagraph = newParagraph;
}

Verse.prototype = {
   id: function() {
      if (this.parent !== null) {
         return this.parent.id() + ':' + this.number;
      }
      return 'null:' + this.number;
   }
};


// ---------------------------------------------------------------
// Verse container, holds one or more verses
function VerseGroup() {
   this.chap   = null;
   this.start  = 0;
   this.end    = 0;
}

VerseGroup.prototype = {
   numVerses: function() {
      return this.end - this.start;
   },

   // parse and construct verses described by str
   // expected format is
   // BOOK CHAP:VERSE
   // BOOK CHAP:VERSE1-VERSEN (describes range)
   // BOOK CHAP:VERSE1,VERSE[I],... (several verses)
   // Book Chapter:VerseStart-VerseEnd
   parse: function(bible, str) {

   },

   // combine specified groups into one
   join: function(group1, group2) {

   }
};


// ---------------------------------------------------------------
// Represents a chapter in the book
function Chapter(book, number) {
   this.parent = book;
   this.number = number;
   this.verses = [];

   // the pair <verse index, heading>, where heading should be displayed
   // just about the verse with the "verse index"
   this.heading = {};
}

Chapter.prototype = {
   id: function() {
      //assert(this.parent);
      return this.parent.id + ' ' + this.number;
   },

   numVerses: function() {
      return _.size(this.verses);
   },

   // Return the verse with specified number
   getVerse: function(number) {
      if (number < 1 || number > this.numVerses())
         throw "Invalid verse number: " + number.toString();
      return this.verses[number - 1];
   }
};


// ---------------------------------------------------------------
// Represents a single book in a bible
function Book() {
   // reference to parent object (Bible)
   this.parent    = null;
   this.id        = '';
   this.abbr      = '';
   this.name      = '';
   this.desc      = '';
   this.chapters  = [];
}

Book.prototype = {
   id: function() {
      return this.id;
   },

   numChapters: function() {
      return _.size(this.chapters);
   },

   // Returns the chapter with given number (not index)
   // null if the chapter with specified number does not exists
   getChapter: function(number) {
      if (number < 1 || number > this.numChapters()) {
         return null;
      }
      return this.chapters[number - 1];
   }
};



// Represents a Bible with specified name and language
function Bible(name, abbr) {
   this.name  = name;
   this.abbr  = abbr;

   this.privateBooks = {};
   this.dict  = null;

   // this.books: function() {
   //    var privateBooks = {};
   //    var booksOrder   = {};
   // };
}

Bible.prototype = {
   numBooks: function() {
      return Object.keys(this.privateBooks).length;
   },

   addBook: function(book) {
      this.privateBooks[book.id] = book;
   },

   getBook: function(id) {
      return this.privateBooks[id];
   },

   ref: function(query) {
      var array = /(.+?)\s+(\d+):?(\d+)?/gm.exec(query);
      if (array.length < 2)
         return;

      var bookName = array[1].toLowerCase();

      var bookCandidates = [];
      _.each(this.privateBooks, function(book) {
         if ( book.abbr.toLowerCase().search(bookName) >= 0 )
            bookCandidates.push(book);
      });

      for (var i = 0; i < bookCandidates.length; ++i) {
         console.log(bookCandidates[i].abbr);
      }
      //var result = array[1].toLowerCase();
   },

   search: function(query, opts) {

   }
};




// ---------------------------------------------------------------
function ViewOptions() {
   this.viewMode      = null;
   this.font          = null;
   this.paragraphMode = true;

   this.init = function(vm, font, pm) {
      this.viewMode      = vm;
      this.font          = font;
      this.paragraphMode = pm;
   };
}

var viewOptions = new ViewOptions();

// ---------------------------------------------------------------
function VerseView() {
   this.display = function(verse) {
      return verse.number + '. ' + verse.text;

      // if (viewOptions.paragraphMode === false) {
      //    return verse.number + '. ' + verse.text;
      // }
   };
}


// ---------------------------------------------------------------
function ChapterView(verseView) {
   this.display = function(chapter) {

      var vv = this.verseView;
      var result = '';

      _.each(chapter.verses, function(v) {
         if (result.length !== 0)
            result += '\n';
         result += chapter.id();
         result += ':';
         result += vv.display(v);
      });

      return result;
   };
   this.verseView = verseView;
}

// ---------------------------------------------------------------
function BookView(chapterView) {
   this.chapterView = chapterView;

   this.display = function(book) {

      var chapView = this.chapterView;
      _.each(book.chapters, function(c) {
         console.log(chapView.display(c));
      });

      console.log('\n\n');
   };
}

// ---------------------------------------------------------------
function BibleView(bookView) {
   this.bookView = bookView;

   this.display = function(bible) {
      var bookView = this.bookView;
      _.each(bible.privateBooks, function(book) {
         bookView.display(book);
      });
   };
}



// ---------------------------------------------------------------
function loadBook(bible, filePath) {

   var book  = new Book();
   book.parent = bible;

   var str = fs.readFileSync(filePath, 'utf8');
   var tagsExpr = /(\\[pvc])\s+?(\d+)?/gm;


   var header       = '';
   var myArray      = '';
   var newParagraph = 1;
   var chapter      = null;

   var verseNumber  = 0;
   var verseStart   = 0;
   var output       = '';


   function extractVerse(offset, size) {
      if (verseNumber !== 0) {
         var text   = str.substr(offset, size);
         var markup = text.trim();

         // remove unnecessary staff from the file
         // !original one! var deleteExpr = /(\\zw|\\zx)[\s\S]*?(\1\*)/gm;
         var deleteExpr = /(\\\w+)[\s\S]*?(\1\*)/gm;
         var textOnly = markup.replace(deleteExpr, '');
         textOnly = textOnly.replace(/\n/gm, ' ');
         textOnly = textOnly.replace(/\s{2,}/gm, ' ');

         // get rid of any non character
         textOnly = textOnly.replace(/[,\.:;\"\?\(\)\!]/gm, '');

         var verse = new Verse(chapter, verseNumber, textOnly, newParagraph);
         chapter.verses.push(verse);

         var wordsArray = textOnly.split(' ');
         wordsArray.forEach( function(word) {
            bible.dict.addWord(word);
         });

         // text = text.replace(/\\add\s/g, '[');
         // text = text.replace(/\\add\*/g, ']');
         // output += (parseInt(verseNumber) + ' ' + text + '\n');
         //
         //
         // text = text.replace(/\\add\s/g, '[');
         // text = text.replace(/\\add\*/g, ']');
         // output += (parseInt(verseNumber) + ' ' + text + '\n');
         verseNumber = 0;
      }
   }

   function extractBookId(header) {
      var array = /\\id\s(\w+)/gm.exec(header);
      if (array.length < 2) {
         throw 'mandatory field \\id is missing';
      }
      return array[1];
   }


   function extractDescription(header) {
      var array = /\\toc1\s+(.*)/gm.exec(header);
      if (array === null || array.length < 2) {
         //return '';
         throw 'mandatory field \\toc1 is missing';
      }
      return array[1];
   }


   function extractName(header) {
      var array = /\\toc2\s+(.*)/gm.exec(header);
      if (array === null || array.length < 2) {
         //return '';
         throw 'mandatory field \\toc2 is missing';
      }
      return array[1];
   }


   function extractAbbreviation(header) {
      var array = /\\toc3\s+(.*)/gm.exec(header);
      if (array === null || array.length < 2) {
         //return '';
         throw 'mandatory field \\toc3 is missing';
      }
      return array[1];
   }

   try {
      while ((myArray = tagsExpr.exec(str)) !== null) {

         // keep header for further processing
         if (header.length === 0) {
            header = str.substr(0, myArray.index);
            //break;
         }

         if ( myArray[1] === '\\c' ) {
            var number = myArray[2];

            chapter = new Chapter(book, number);
            newParagraph = 0;
            book.chapters.push(chapter);
         }
         else if ( myArray[1] === '\\v' ) {
            extractVerse(verseStart, myArray.index - verseStart);

            newParagraph = 0;
            verseStart  = myArray.index + myArray[0].length;
            verseNumber = myArray[2];
         }
         else if ( myArray[1] === '\\p' ) {
            extractVerse(verseStart, myArray.index - verseStart);
            newParagraph = 1;
         }
      }

      extractVerse(verseStart, str.length - verseStart);

      book.id   = extractBookId(header);
      book.desc = extractDescription(header);
      book.name = extractName(header);
      book.abbr = extractAbbreviation(header);

      bible.addBook(book);
   }
   catch (err) {
      console.error("Failed to load: %s, Exception: %s",
                    path.basename(filePath), err);
   }

   return book;
}


// ---------------------------------------------------------------
function loadBible(dataRoot, lang, version, callback) {

   var dataDir    = dataRoot;

   if (lang.length > 0)
      dataDir += (lang + '/');
   if (version.length > 0)
      dataDir += version + '/';

   // enumerate files in a given directory
   fs.readdir(dataDir, function(err, files) {
      var bible    = new Bible();
      bible.dict   = new Dictionary();

      if (err) {
         callback(err, bible);
         return;
      }

      files.forEach( function(p) {
         if ( path.extname(p) === '.usfm' ) {
            //console.log("Processing file: ", p);
            var book = loadBook(bible, dataDir + p);
         }
      });

      callback(err, bible);
   });
}


function measureTime(startTime) {
   var diff = process.hrtime(startTime);
   if (diff[0] > 0)
      console.log('Benchmark took %d seconds, %d nanosecdons', diff[0], diff[1] );
   else
      console.log('Benchmark took %d nanosecdons', diff[1]);

   console.log("Initialization completed.");
}


// ---------------------------------------------------------------
function onBibleLoaded(err, bible) {
   console.log("-> onBibleLoaded");
   if (err) {
      console.log('ERROR: ', err);
      measureTime(theApp.startTime);
      return;
   }

   var verseView        = new VerseView();
   var chapterView      = new ChapterView(verseView);
   var bookView         = new BookView(chapterView);
   var bibleView        = new BibleView(bookView);

   viewOptions.init('Web page', 'Trebuchet MS', true);

   //bibleView.display(bible);
   bible.dict.showStatistics();

   // check that the verse reference works correctly
   var result = bible.ref('Gen 1:1');

   // check that the search works properly
   result = bible.search('in', {wholeWord: true, caseSensitive: true});

   console.log("Number of books: ", bible.numBooks());
   measureTime(theApp.startTime);

   console.log("<- onBibleLoaded");
}


// ---------------------------------------------------------------
function scriptEntry() {
   console.log("Initializing...");

   /// start time measurement
   theApp.startTime = process.hrtime();

   var dataRoot = 'c:/Users/Hayk/Dropbox (Personal)/Private/projects/bible project/data/real/';
   loadBible(dataRoot, '', '', onBibleLoaded);

   // var dataRoot = './content/test/';
   // loadBible(dataRoot, 'eng', 'kjv', onBibleLoaded);

   // var obj = {};
   // obj['z'] = 1;
   // obj['a'] = 47;
   // obj['g'] = 33;
   // obj['c'] = -1;

   //console.log(bible);

   // var vvv = Object.keys(obj)
   //    .map(function (k) { return [k, obj[k]]; })
   //    .sort(function (a, b) {
   //       return a[1] - b[1]
   //    });

   // .forEach(function (d) {
   //    vvv.push(d[0]);
   //    console.log(d[1]);
   // });
}

scriptEntry();

}());
