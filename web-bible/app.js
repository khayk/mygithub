(function() {

var _    = require('underscore');
var fs   = require('fs');
var path = require('path');


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
// Represents a single verse in a chapter
function Verse(chapter, number, text, newParagraph) {
   this.parent       = chapter;
   this.number       = number;
   this.text         = text;
   this.newParagraph = newParagraph;
};

Verse.prototype = {
   id: function() {
      if (this.parent != null) {
         return this.parent.id() + ':' + this.number;
      }
      return 'null:' + this.number;
   }
};

// ---------------------------------------------------------------
// Represents a chapter in the book
function Chapter(book, number) {
   this.parent = book;
   this.number = number;
   this.verses = [];

   /// the pair <verse index, heading>, where heading should be displayed
   /// just about the verse with the "verse index"
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
}


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
}



// Represents a Bible with specified name and language
function Bible(name, abbr) {
   this.name  = name;
   this.abbr  = abbr;

   this.privateBooks = {};
   // this.books: function() {
   //    var privateBooks = {};
   //    var booksOrder   = {};
   // };
}

Bible.prototype = {

   addBook: function(book) {
      this.privateBooks[book.id] = book;
   },

   getBook: function(id) {
      return this.privateBooks[id];
   }
}




// ---------------------------------------------------------------
function ViewOptions() {
   this.viewMode      = null;
   this.font          = null;
   this.paragraphMode = true;

   this.init = function(vm, font, pm) {
      this.viewMode      = vm;
      this.font          = font;
      this.paragraphMode = pm;
   }
}

var viewOptions = new ViewOptions();

// ---------------------------------------------------------------
function VerseView() {
   this.display = function(verse) {
      return verse.number + '. ' + verse.text;

      // if (viewOptions.paragraphMode === false) {
      //    return verse.number + '. ' + verse.text;
      // }
   }
}


// ---------------------------------------------------------------
function ChapterView(verseView) {
   this.display = function(chapter) {

      var vv = this.verseView;
      var result = '';

      _.each(chapter.verses, function(v) {
         if (result.length != 0)
            result += '\n';
         result += chapter.id();
         result += ':';
         result += vv.display(v);
      });

      return result;
   }
   this.verseView = verseView;
}

// ---------------------------------------------------------------
function BookView(chapterView) {
   this.chapterView = chapterView;

   this.display = function(book) {

      var chapView = this.chapterView;
      _.each(book.chapters, function(c) {
         console.log(chapView.display(c));
      })

      console.log('\n\n');
   }
}

// ---------------------------------------------------------------
function BibleView(bookView) {
   this.bookView = bookView;

   this.display = function(bible) {
      var bookView = this.bookView;
      _.each(bible.privateBooks, function(book) {
         bookView.display(book);
      });
   }
}

/*
// the json file path, that describes the content to be loaded
function loadBible(descFile) {
   //console.log(file + ' is loading...');

   var bible = new Bible();
   bible.name = 'King James Version';
   bible.abbr = 'KJV';

   var bookCfg = [
      {id:'GEN',  name:'Genesis', abbr:'Gen',  numChaps:1, numVerses:1},
      {id:'NUM',  name:'Numbers', abbr:'Num',  numChaps:2, numVerses:2},
      {id:'EXOD', name:'Exodus',  abbr:'Exod', numChaps:3, numVerses:3}
   ];

   for (var i = 0; i < _.size(bookCfg); i += 1) {

      var book  = new Book();
      book.parent = bible;
      book.id   = bookCfg[i].id;
      book.name = bookCfg[i].name;
      book.abbr = bookCfg[i].abbr;

      bible.addBook(book);

      for (var j = 1; j <= bookCfg[i].numChaps; j += 1) {
         var chapter = new Chapter(book, j);

         for (var k = 1; k <= bookCfg[i].numVerses; k += 1) {
            var verse = new Verse(chapter, k, 'verse number ' + k.toString(), false);
            chapter.verses.push(verse);
         }

         book.chapters.push(chapter);
      }
   };

   return bible;
}*/

// ---------------------------------------------------------------
function onBibleLoaded(bible) {

   var verseView        = new VerseView();
   var chapterView      = new ChapterView(verseView);
   var bookView         = new BookView(chapterView);
   var bibleView        = new BibleView(bookView);

   viewOptions.init('Web page', 'Arial', true);

   console.log("onBibleLoaded");
   bibleView.display(bible);
   console.log("onBibleLoaded End");
}


// ---------------------------------------------------------------
function loadBook(filePath) {
   var book  = new Book();

   var str = fs.readFileSync(filePath, 'utf8');
   var tagsExpr = /(\\[pvc])\s+?(\d+)?/gm;


   var header       = '';
   var myArray      = '';
   var newParagraph = 1;
   var chapter      = null;

   var verseNumber  = 0;
   var verseStart   = 0;
   var output = '';


   function extractVerse(offset, size) {
      if (verseNumber !== 0) {
         var text   = str.substr(offset, size);
         text = text.replace(/\n/gm, ' ');

         var verse = new Verse(chapter, verseNumber, text, newParagraph);
         chapter.verses.push(verse);
         
         // text = text.replace(/\\add\s/g, '[');
         // text = text.replace(/\\add\*/g, ']');
         // output += (parseInt(verseNumber) + ' ' + text + '\n');
         verseNumber = 0;
      }
   }


   while ((myArray = tagsExpr.exec(str)) !== null) {
      
      /// keep header for further processing
      if (header.length === 0) {
         header = str.substr(0, myArray.index);
         //console.log(header);
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


   book.id   = 1;//bookCfg[i].id;
   book.name = "121212";//bookCfg[i].name;
   book.abbr = '111';//bookCfg[i].abbr;

   return book;
}


// ---------------------------------------------------------------
function loadBible(lang, version) {

   var dataRoot = './content/test/';
   var dataDir  = dataRoot + lang + '/' + version + '/';

      /// enumerate files in a given directory
   fs.readdir(dataDir, function(err, files) {
      var bible    = new Bible();

      if (err) {
         console.log("ERROR: ", err);
         return;
      }

      files.forEach( function(p) {
         if ( path.extname(p) === '.usfm' ) {
            console.log("Processing file: ", p);
            var book = loadBook(dataDir + p);
            book.parent = bible;
            bible.addBook(book);

            //console.log(book.chapters[0].toString());
            //console.log(book.toString());
         }
      });

      onBibleLoaded(bible);
   });
}


// ---------------------------------------------------------------
function scriptEntry() {
   var bible = loadBible('eng', 'kjv');
}

scriptEntry();

}());
