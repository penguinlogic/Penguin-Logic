#include "scanner.h"

/**************************************************************************************
Constructor opens the definition file or returns an error if it can't, creates a local pointer to the names module, initialises the line and position counters and reads the first line and character from the file.
*/

scanner::scanner (names * names_mod, const char *defname, bool & ok)
{
  inf.open (defname);
  if (!inf)
    {
      ok = false;
      cout << "Error: Unable to open file '" << defname << "'" << endl;
    }
  else
    {
      ok = true;
      cout << defname << " was opened successfully" << endl;
    }
  nmz = names_mod;
  current.linenum = 1;
  current.pos = 0;
  getline (inf, current.line);
  getch ();
}


/*************************************************************************************
getch function reads in the next character from the input file, keeping a record of the previous character in case of error and incrementing the line and character position as required. 
*/
void
scanner::getch ()
{
   if (!inf.eof ())
    {
      if (current.pos == current.line.size ())
	{
	  getline (inf, current.line);
	  current.pos = 0;
	  current.linenum++;

	}

      if (current.pos < current.line.size ())
	{
	  current.ch = current.line[current.pos];
	  current.pos++;
	}
    }
}

/***************************************************************************************
blank lines and whitespace are ignored by the scanner.
*/

void
scanner::skipspaces ()
{

  while (current.line.size () == 0 && !inf.eof ())
    {
      getline (inf, current.line);
      current.pos = 0;
      current.linenum++;
      getch ();
    }

  while (!inf.eof () && isspace (current.ch))
    getch ();
}

/***************************************************************************************
comments, either enclosed in forward slashes or starting with double forward slashes and at the end of a line, are ignored by the scanner.
*/


void
scanner::skipcomments ()
{
  if (current.ch == '/')
    {
      getch ();
      if (current.ch == '/')
	{
	  do
	    {
	      getch ();
	    }
	  while (current.pos != current.line.size () && !inf.eof ());
	}

      else
	{
	  do
	    {
	      getch ();
	    }
	  while (current.ch != '/' && !inf.eof ());
	}
      getch ();
      skipspaces ();
    }
}


/***************************************************************************************
the getnumber function reads consecutive digits from the input file by repeatedly calling getch and checking to see whether the character is a digit, and returns the result as an integer.
*/

void
scanner::getnumber (int &number)
{
  number = 0;
  string str;

  while (!inf.eof () && isdigit (current.ch))
    {
      str.push_back (current.ch);
      stringstream ss (str);
      ss >> number;
      getch ();
    }
}

// overloaded version builds a digit vector as well for the sig gen.
void
scanner::getnumber (int &number, vector <int> &wvform)
{
  number = 0;
  string str;

  while (!inf.eof () && isdigit (current.ch))
    {
      str.push_back (current.ch);
      stringstream ss (str);
      ss >> number;

	  int digit_tmp;
	  string str_digit;
	  str_digit.push_back (current.ch);
	  stringstream tt (str_digit);
	  tt >> digit_tmp;
	  wvform.push_back(digit_tmp); // builds a vector of digits for the siggen
	                               // needed because storing as an int loses any
								   // leading zeros

      getch ();
    }
}

/***************************************************************************************
the getname function reads consecutive alphanumeric characters from the input file by repeatedly calling getch and checking to see whether the character is alphanumeric, and returns the result as an namestring.
*/


void
scanner::getname (namestring & str)
{
  str.clear ();
  while (!inf.eof () && (isalnum (current.ch)))
    {
      str.push_back (current.ch);
      getch ();
    }

}

/***************************************************************************************
the getsymbol function sets the parameters of the symbol passed to it. It ignores spaces and comments and then calls getnumber, getname or directly sets the parameters depending on the current character.
*/

void
scanner::getsymbol (symbol & s)
{
  last = current;
  vector <int> wvform;
	
  s.set_parameters (notype, novalue, -1, -1);

  skipspaces ();
  skipcomments ();

  if (inf.eof ())		//End of file reached, endfile symbol returned
    {
      s.set_parameters (Charsym, endfile, -1, -1);
    }

  else if (isdigit (current.ch))
    {
      getnumber (num, wvform);
      s.set_parameters (Uint, novalue, num, -1, wvform);
    }

  else if (isalpha (current.ch))
    {
      getname (idstr);

      if (idstr == "DEVICES")
	s.set_parameters (Section, DEVICES, -1, -1);
      else if (idstr == "CONNECTIONS")
	s.set_parameters (Section, CONNECTIONS, -1, -1);
      else if (idstr == "MONITOR")
	s.set_parameters (Section, MONITOR, -1, -1);

      else if (idstr == "period")
	s.set_parameters (Devswitch, period, -1, -1);
      else if (idstr == "initialvalue")
	s.set_parameters (Devswitch, initialvalue, -1, -1);
      else if (idstr == "numinputs")
	s.set_parameters (Devswitch, numinputs, -1, -1);
	  else if (idstr == "waveform")
	s.set_parameters (Devswitch, waveform, -1, -1);


      else if (idstr == "CLOCK" || idstr == "SWITCH" || idstr == "AND"
	       || idstr == "NAND" || idstr == "OR" || idstr == "NOR"
	       || idstr == "DTYPE" || idstr == "XOR" || idstr == "SIGGEN")
	s.set_parameters (Devname, novalue, -1, nmz->lookup (idstr));

      else if (idstr == "DATA" || idstr == "SET" || idstr == "CLEAR"
	       || idstr == "CLK" || idstr == "I1" || idstr == "I2"
	       || idstr == "I3" || idstr == "I4" || idstr == "I5"
	       || idstr == "I6" || idstr == "I7" || idstr == "I8"
	       || idstr == "I9" || idstr == "I10" || idstr == "I11"
	       || idstr == "I12" || idstr == "I13" || idstr == "I14"
	       || idstr == "I15" || idstr == "I16")
	s.set_parameters (Inname, novalue, -1, nmz->lookup (idstr));

      else if (idstr == "Q" || idstr == "QBAR")
	s.set_parameters (Outname, novalue, -1, nmz->lookup (idstr));

      else
	s.set_parameters (Uname, novalue, -1, nmz->lookup (idstr));
    }


  else
    {
      switch (current.ch)
	{
	case '.':
	  s.set_parameters (Charsym, dot, -1, 0);
	  break;
	case '{':
	  s.set_parameters (Charsym, lbrak, -1, 0);
	  break;
	case '}':
	  s.set_parameters (Charsym, rbrak, -1, 0);
	  break;
	case '=':
	  s.set_parameters (Charsym, equals, -1, 0);
	  break;
	case ';':
	  s.set_parameters (Charsym, semicolon, -1, 0);
	  break;
	case '>':
	  s.set_parameters (Charsym, rarrow, -1, 0);
	  break;
	case '-':
	  s.set_parameters (Charsym, dash, -1, 0);
	  break;
	default:
	  s.set_parameters (Charsym, bad, -1, 0);
	  break;
	}
      getch ();
    }

}

/***************************************************************************************
the print_err function enables the position of an error and a corresponding error message to be displayed at the position of the previous character to have been read in, the one which will have caused an error to be recognised.
*/

void
scanner::print_err (const char *error)
{
  cout << "Error in line " << last.
    linenum << ", at character " << last.pos << ":" << endl;

  cout << last.line << endl;

  for (unsigned int i = 0; i < last.pos; i++)
    {
      if (last.line[i] == '\t')
	cout << '\t';
      else
	cout << " ";
    }				// accounts for tabs at in erroneous line

  cout << "^" << endl;
  cout << error << endl;
}
