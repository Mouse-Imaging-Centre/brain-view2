#include "mniVertstatsFile.h"
#include <pcre++.h>
#include <algorithm>

using namespace pcrepp;
using namespace std;

extern "C" {
#include "time_stamp.h"
}

/*!
 * An empty constructor that does much of nothing. If one uses this
 * constructor, one has to call loadFile manually afterwards, or
 * construct the vertstats file from scratch */
mniVertstatsFile::mniVertstatsFile() {
  // do nothing - the real work is in loadFile

  this->initialiseVariables();

}

/*!
 * The main constructor which immediately reads in a vertstats file, and is therefore the constructor that is normally used.
 *
 * \param filename The file to read in.  \param readData Whether to
 * read the data part of the file. If false, then it reads only the
 * header and dataheader - this can be useful for speeding up the
 * extraction of header information.
 */
mniVertstatsFile::mniVertstatsFile(char *filename, bool readData) {

  this->initialiseVariables();
  this->loadFile(filename, readData);

}

/*!
 * Overloaded constructor taking a string as its input
 *
 * \param filename The file to readin.
 * \param readData Whether to read the data part of the file.
 */
mniVertstatsFile::mniVertstatsFile(string filename, bool readData) {

  this->initialiseVariables();
  this->loadFile((char *)filename.c_str(), readData);
}

mniVertstatsFileType mniVertstatsFile::determineFileType(char *filename) {

  mniVertstatsFileType thisFile;
  ifstream statsFile(filename);
  if (! statsFile) {
    cerr << "ERROR: could not read file " << filename << endl;
    exit(1);
  }
  string line;
  getline(statsFile, line);
  if (line.find("<header>") != string::npos) {
    thisFile = NEWSTYLE;
  }
  else {
    thisFile = OLDSTYLE;
  }
  statsFile.close();
  return thisFile;
}

void mniVertstatsFile::initialiseVariables() {

  this->dataheader = new vector<string> ();
  this->data = new vertexMatrix();
  this->numRows = 0;
  this->numColumns = 0;
  this->headerTree = new tree<mniVertstatsHeaderEntry>;

}

void mniVertstatsFile::destroyVariables() {
  delete this->dataheader;
  delete this->data;
  this->numRows = 0;
  this->numColumns = 0;
  delete this->headerTree;
}

/*! deletes all variables then reinitialises them so that a new file
  can be loaded. */
void mniVertstatsFile::close() {
  this->destroyVariables();
  this->initialiseVariables();
}
    

/*! Loads a file. See the constructor documentation for the details
  about what the arguments mean. This is also the place where the
  decision is made about whether one is dealing with an old style file
  (simple columns of numbers) or a vertstats file in all its glory. */
void mniVertstatsFile::loadFile(char *filename, bool readData) {

  mniVertstatsFileType ft = this->determineFileType(filename);

  switch(ft) {
  case NEWSTYLE:
    this->loadNewStyleFile(filename, readData);
    break;
  case OLDSTYLE:
    this->loadOldStyleFile(filename, readData);
    break;
  default:
    cerr << "ERROR: there should not be a default file type!" << endl;
    exit(1);
  }

}

void mniVertstatsFile::loadOldStyleFile(char *filename, bool readData) {

  string line;

  // open the file 
  ifstream statsFile(filename);
  if (! statsFile) {
    cerr << "ERROR: could not read file " << filename << endl;
    exit(1);
  }

  // determine how many columns this file contains
  getline(statsFile, line);
  
  // split the line on white space(s), tab(s), EOL.

  int firstpos = 0, nextpos = 0;
  this->numColumns = 0;

  /// Added by Sahar to skip the spaces and tabs at the beginning of the line. July 11, 2014
  firstpos = line.find_first_not_of(" \t\n",0);
  
  while (nextpos != string::npos) {
    nextpos = line.find_first_of(" \t\n",firstpos);
    firstpos = nextpos+1;
    this->numColumns++;
  }

/// This was added to oobicpl-0-4-5 but is clearly wrong and is an infinite loop
//   while( nextpos = line.find_first_not_of(" \t\n",firstpos) != string::npos ) {
//     this->numColumns++;
//     firstpos = line.find_first_of(" \t\n",nextpos);
//   }
/// This is also an infinite loop
//   while( nextpos = line.find_first_of(" \t\n",firstpos) != string::npos ) {
//     firstpos = nextpos+1;
//     this->numColumns++;
//   }

  // generate some header names, e.g. Column1, Column2, etc.
  for (int i=0; i < this->numColumns; i++) {
    char cName[15];
    sprintf(cName, "%s%i", "Column", i);
    this->dataheader->push_back(cName);
  }

  if (readData) {
    // initialise the data member
    this->data->resize( this->numColumns );
    
    // reset the file position
    statsFile.seekg(0);
    
    // get the data out of the file
    while (! statsFile.eof() ) {
      for (int i=0; i < this->numColumns; i++) {
        double currentVal;
        statsFile >> currentVal;
        (*this->data)[i].push_back((float)currentVal);
      }
      this->numRows++;
    }

    /* the above file reading will always read the last number
       twice, since the inner loop does not check whether eof has
       been reached. So pop the last element */
    for (int i=0; i < this->numColumns; i++) {
      (*this->data)[i].pop_back();
    }
  }
}



void mniVertstatsFile::loadNewStyleFile(char *filename, bool readData) {

  // initialise variables
  bool endNow = false;

  // open the file
  ifstream statsFile(filename);
  if (! statsFile) {
    cerr << "ERROR: could not read file " << filename << endl;
    exit(1);
  }

  // state bits
  enum state { HEADER, DATA, NONE, DATAHEADER };

  state filestate = NONE;

  // loop over file
  while (! statsFile.eof() && ! endNow) {
    string line;
    int firstpos, lastpos;

    // holds the current tag name and value for the header parsing
    string tagName;
    string tagValue;
    
    // a switch to decide whether header processing is finished
    bool closeHeader = false;
    bool currentTagClosed = true;

    // the regex matches for opening and closing tags
    Pcre openTag("<([a-z]+)>", "i");
    Pcre closeTag("</([a-z]+)>", "i");
    
    tree<mniVertstatsHeaderEntry>::iterator top, current;


    //    cout << filestate << endl;
    // check for which part of the file is currently being read
    switch (filestate) {
    case HEADER:
      //cout << "In header" << endl;
      // do nothing for now other than setting statebit back to none

      /* initialise the header. The top of the tree will be called header,
         the top level nodes will be constructed as childs thereof, i.e:
         header
           |
         node1 -- node2
      */
      top = headerTree->begin();
      current = headerTree->insert(top, mniVertstatsHeaderEntry("header",""));

      while (closeHeader == false) {
        getline(statsFile, line);
        if (openTag.search(line) == true) {
          // found an opening tag
          if (openTag.matches() != 1) {
            cerr << "ERROR: illegal tag on line: " << line << endl;
            exit(1);
          }
          if (! currentTagClosed) {
            /* the file format insists that all information to go into
               the body of a header entry be finished before any
               subheaders appear. Therefore this tag is checked so
               that the value part of the header can be written before
               the child node is begun. */
            current = headerTree->replace
              (current, mniVertstatsHeaderEntry(tagName, tagValue));
          }
          tagName = openTag.get_match(0);
          // insert the tag with an empty value field into the tree.
          current = headerTree->append_child
            (current, mniVertstatsHeaderEntry(tagName, ""));
          currentTagClosed = false;
            
        }
    
    
        else if (closeTag.search(line) == true) {
          if (closeTag.get_match(0) == "header") {
            // end header processing, move on to dataheader
            closeHeader = true;
            filestate = DATAHEADER;
          }
          else if (closeTag.get_match(0) == tagName) {
            // the last open tag is being closed, i.e. no child nodes
            current = headerTree->replace
              (current, mniVertstatsHeaderEntry(tagName, tagValue));
            tagValue.erase();
            current = headerTree->parent(current);
            currentTagClosed = true;
          }
          else {
            // a different tag is being closed. There were child nodes.
            current = std::find(headerTree->begin(), headerTree->end(), closeTag.get_match(0));
            current = headerTree->parent(current);
            
          }
        }
        else {
          tagValue.append(line);
          tagValue.append("\n");
        }
      }
      break;
    case DATA:
      //cout << "in data" << endl;
      //cout << line << endl;
      if (readData) {
        while (! statsFile.eof() ) {
          for (int i=0; i < this->numColumns; i++) {
            float currentVal;
            statsFile >> currentVal;
            (*this->data)[i].push_back(currentVal);
          }
          this->numRows++;
        }
        
        /* the above file reading will always read the last number
           twice, since the inner loop does not check whether eof has
           been reached. So pop the last element */
        for (int i=0; i < this->numColumns; i++) {
          (*this->data)[i].pop_back();
        }
      }
      else {
        endNow = true;
      }
      filestate = NONE;
      break;
    case DATAHEADER:
      // a single line which describes the columns that follow
      getline(statsFile, line);
      
      // split the line on white space
      firstpos = 0;
      lastpos = line.length();
      while (lastpos != string::npos) {
        lastpos = line.find(" ", firstpos);
        this->dataheader->push_back(line.substr(firstpos, lastpos - firstpos));
        firstpos = lastpos +1;
      }
      this->numColumns = this->dataheader->size();
      // initialise the data member
      this->data->resize( this->numColumns );
      filestate = DATA;
      break;
    case NONE:
      // no state bit currently set. This means that the next readable 
      // element should set the state flag
      //      cout << "in none" << endl;
      getline(statsFile, line);

      if (line.find("<header>") != string::npos)
        filestate = HEADER;
          
      break;
    }
  }
}

/* returns a vector containing the data column identified as an
 * argument.  
 * \param columnName A string identifying the column name
 * (as defined in the dataheader.
 */
vertexColumn mniVertstatsFile::getDataColumn(string columnName) {
  // stl::find does not work as it returns an iterator rather than
  // the position - so do it manually;
  int position = -1;

  // create the regular expression - make it match on whole word
  columnName.insert(columnName.begin(), '^');
  columnName.insert(columnName.end(), '$');

  // make sure that all periods are treated literally
  Pcre periodReplace("\\.", "g");
  columnName = periodReplace.replace(columnName, "\\.");
  //cout << "regex:" << columnName << endl;

  Pcre regex(columnName);

  for (int i=0; i < this->numColumns; i++) {
    //if ((*this->dataheader)[i].find(columnName) != string::npos)
    if (regex.search((*this->dataheader)[i]) == true) 
      position = i;
  }
  if (position == -1) {
    throw InvalidColumnError();
    vertexColumn empty;
    return empty;
  }
  else 
    return (*this->data)[position];
}

/*! overloaded function, in this case identifying the column to be returned by its integer index.
 *
 * \param index The index of the column that is desired.
 *
 * \note It is usually more desireable to identify the column by
 * name. This function here is useful if one wants to loop through all
 * of the columns.
 */
vertexColumn mniVertstatsFile::getDataColumn(int index) {
  if (index >= this->numColumns || index < 0) {
    throw InvalidColumnError();
    vertexColumn empty;
    return empty;
  }
  else
    return (*this->data)[index];
}

/*! adds a new column of data to the file with the name specified. If
 * the the size of this data column does not correspond to the other
 * data already in the file, it will die a horrible death.
 *
 * \param data The vector to add to the file.
 * \param columnName The name that this column will take.
 *
 * \bug currently exits if sizes of columns are not the same. Should
 * instead throw an error.
 */
void mniVertstatsFile::putDataColumn(vertexColumn data, string columnName) {
  this->data->push_back(data);
  numColumns++;
  dataheader->push_back(columnName);

  if (numRows == 0) {
    numRows = data.size();
  }
  else {
    if (numRows != data.size()) {
      // should throw an error instead, I suppose
      cerr << "ERROR: all columns of data must be of the same size." << endl;
      exit(1);
    }
  }
}
  
void mniVertstatsFile::putHeader(mniVertstatsHeaderEntry header,
                                 string headerParent) {
  tree<mniVertstatsHeaderEntry>::iterator it, top;
  Pcre newlineCheck("\n$");

  // ensure that there is a newline at end of header
  if (newlineCheck.search(header.value) == false) {
    cout << "no newline" << endl;
    header.value.append("\n");
  }

  it = find(headerTree->begin(), headerTree->end(), headerParent);
  // special case: if headerParent is "header" and it can't be found, 
  // create it at the top level
  if (it == headerTree->end() && headerParent == "header") {
    top = headerTree->begin();
    it = headerTree->insert(top, mniVertstatsHeaderEntry("header",""));
  }
  // next case: headerParent != header, but can't be found. So we create
  // the parent at depth==2, then add the new header as a child
  else if (it == headerTree->end()) {
    top = find(headerTree->begin(), headerTree->end(), "header");
    it = headerTree->append_child(top, 
                                  mniVertstatsHeaderEntry(headerParent, ""));
  }
  headerTree->append_child(it, header);
}

void mniVertstatsFile::putHeader(tree<mniVertstatsHeaderEntry> headerTree) {
  delete this->headerTree;
  this->headerTree = new tree<mniVertstatsHeaderEntry>(headerTree);
}


void mniVertstatsFile::printHeaderStructure() {
  tree<mniVertstatsHeaderEntry>::iterator it;
  it = headerTree->begin();
  while ( it != headerTree->end()) {
    for (int i=0; i < headerTree->depth(it); i++)
      cout << "  ";
    cout << (*it).key << endl;
    ++it;
  }
}
/*! returns the header value identified by the string argument.
 *
 * \param headerKey A string identifying the header. This string can
 * take one of two forms: a simple string, in which case the header
 * tree is searched and the first match is returned, or a string with
 * different keys separated by colons, i.e. patient:name, in which
 * case the search will descend through the hierarchy specified by the
 * colons. This is the only way to get the correct header if there are
 * mulitple headers of the same name in different places in the
 * hierarchy.
 *
 * \note This function should - and will eventually - throw an error
 * if an unknown header is searched for. Right now it returns an empty
 * string and prints a warning.
 */

string mniVertstatsFile::getHeaderValue(string headerKey) {
  tree<mniVertstatsHeaderEntry>::iterator it;
  try {
    it = getHeaderIterator(headerKey);
  }
  catch(const HeaderNotFoundError e) {
    return "";
  }
  return (*it).value;
}

/*! returns the header iterator identified by the string argument.
 *
 * \param headerKey A string identifying the header. This string can
 * take one of two forms: a simple string, in which case the header
 * tree is searched and the first match is returned, or a string with
 * different keys separated by colons, i.e. patient:name, in which
 * case the search will descend through the hierarchy specified by the
 * colons. This is the only way to get the correct header if there are
 * mulitple headers of the same name in different places in the
 * hierarchy.
 *
 * \note This function should - and will eventually - throw an error
 * if an unknown header is searched for. Right now it returns an empty
 * string and prints a warning.
 */
tree<mniVertstatsHeaderEntry>::iterator
mniVertstatsFile::getHeaderIterator(string headerKey) {
  tree<mniVertstatsHeaderEntry>::iterator it;
  tree<mniVertstatsHeaderEntry>::sibling_iterator sibbegin, sibend;
  vector<string> components;

  Pcre colonSearch(":", "g");
  
  if (colonSearch.search(headerKey) == true) {
    components = colonSearch.split(headerKey);
    it = headerTree->begin();
    for (int i=0; i < components.size(); i++) {
      sibbegin = headerTree->begin(it);
      sibend = headerTree->end(it);
      it = find(sibbegin, sibend, components[i]);
    }
  }
  else {
    it = find(headerTree->begin(), headerTree->end(), headerKey);
  }
  if (it == headerTree->end()) {
    //    cerr << "WARNING: key " << headerKey << " not found." << endl;
    throw HeaderNotFoundError();
    //    return headerTree->end();
  }
  else
    return it;
}  

/*! Searches to see if the header already exists. If yes, appends the
 *  value to that header, if not, creates that header at depth == 1
 *
 * \param header The name of the header
 * \param value The value to add/create
 *
 * \bug Does not correctly handle deep headers. It should work if that
 * header is actually found, but not if it has to be created.
 */
void mniVertstatsFile::addToHeader(string header, string value) {
  tree<mniVertstatsHeaderEntry>::iterator it;
  try {
    it = getHeaderIterator(header);
    // there indeed was a "header" header already.
    string oldValue = (*it).value;
    oldValue.append("\n");
    oldValue.append(value);
    headerTree->replace(it, mniVertstatsHeaderEntry(header, value));
  }
  catch(const HeaderNotFoundError e) {
    // see if any header at all exists
    try {
      it = getHeaderIterator("header");
      // "header" not found, but top level header exists, so create the 
      // "header" header below it.
      it = headerTree->append_child(it, mniVertstatsHeaderEntry(header, 
                                                                value));
    }
    catch(const HeaderNotFoundError e) {
      // the top header does not exist, so create it
      it = headerTree->begin();
      it = headerTree->insert(it, mniVertstatsHeaderEntry("header",""));
      
      // and now create a child called as desired
      it = headerTree->append_child(it, mniVertstatsHeaderEntry(header,
                                                                value));
    }
  }
}

void mniVertstatsFile::addToHistory(string moreHistory) {
  addToHeader("history", moreHistory);
}

void mniVertstatsFile::addToHistory(int argc, char *argv[]) {
  // to be used for the history string
  string argString = time_stamp(argc, argv);
  addToHeader("history", argString);
}


/*! A function that writes out a header entry pointed to by the
 *  iterator. The main reason for its existence is that it can call
 *  itself in order to traverse and faithfully reproduce the header
 *  hierarchy. 
 * 
 * \param it The iterator pointing to the node to be written out. Will
 * also write out all the nodes children.
 * \param outFile an ofstream object to which the header will be written
*/
void mniVertstatsFile::writeHeaderEntry(tree<mniVertstatsHeaderEntry>::iterator it, ofstream &outFile) {

  tree<mniVertstatsHeaderEntry>::sibling_iterator sibit;
  // write out the node header tag
  outFile << "<" << (*it).key << ">" << endl;
  // write out the node value
  outFile << (*it).value;

  // check for children, and call itself if node has children
  if (headerTree->number_of_children(it) > 0) {
    for (sibit = headerTree->begin(it); sibit != headerTree->end(it); ++sibit){
      writeHeaderEntry(sibit, outFile);
    }
  }
  
  // write end tag
  outFile << "</" << (*it).key << ">" << endl;
}

/*! The data is written to the filename specified.
 * \param filename The filename to write to.
 * \bug Does not check whether the file exists, but automatically clobbers.
 */ 
bool mniVertstatsFile::writeFile(string filename, bool oldStyle) {
  ofstream outFile(filename.c_str());

  if (outFile.bad())
    return false;

  if (oldStyle == false) {

    tree<mniVertstatsHeaderEntry>::iterator it;
    //  tree<mniVertstatsHeaderEntry>::sibling_iterator sibit;
    
    // write the header
    it = headerTree->begin();
    writeHeaderEntry(it, outFile);
    
    cout << "wrote header entry" << endl;
    cout << "numColumns: " << numColumns << endl;
    
    // write the data header - except the last one
    for (int i=0; i < this->numColumns-1; i++) {
      outFile << (*dataheader)[i] << " ";
    }
    // write out last header - this is to eliminate extra spaces at end
    outFile << (*dataheader)[this->numColumns-1];
    
    outFile << endl;
  }
  // now write the data.
  for (int i=0; i < this->numRows; i++) {
    for (int j=0; j < this->numColumns-1; j++) {
      outFile << (*this->data)[j][i] << " ";
    }
    outFile << (*this->data)[this->numColumns-1][i];
    outFile << endl;
  }

  return true;
}  

