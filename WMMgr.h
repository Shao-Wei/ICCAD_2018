#ifndef WMMGR_H
#define WMMGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

class state;      
class transition;
class path;
class subseq;

class WMMgr
{
public:
WMMgr(){}
~WMMgr(){}

bool ReadFile(char* inputfile, char* input_w1, char* input_w2, char* input_w3);
void CutAll(){}          //TODO
bool CutSeq(subseq* s){ return true; } //TODO 
void LinkSeq(){}         //TODO
void WriteFile(char* optfile_name);        //TODO
/*-----------------------------------------------------------------------------
ReadFile(): ReadFile() reads the input .KISS and three watermark sequences.
Then constructs internal graph, and store watermark sequences to _watermark. 
ReadFile() outputs "False" if the input .KISS represents a completely 
specified automata; "True" if not.

CutSeq()  : CutSeq() finds the longest path which starts at arbitrary state
and ends at a uncomplete state. The path matches part of the watermark sequence,
the unmatched part of the sequence is then stored to be processed later. Found 
paths are stored to _path for further linking; the remain sequences are appended 
to _subseq.

LinkSeq() : LinkSeq() takes all the remaining sequences in the storage and 
add new states and transitions to link the watermark sequence all together.
This function shouldn't be called unless CutSeq() fails to find new paths.

WriteFile(): WriteFile writes the result automata in the format of .KISS. Dummy 
transitions are ignored(if either _start or _end is 0).
-----------------------------------------------------------------------------*/
private:
string hex2bin(char c);
vector<string> str_tok(const string& s, const string& seperator);
vector<state*> _states;
vector<transition*> _transitions; 
// Every transition should be stored here for proper memory management.
// However, direct use is not recommanded. Do so via state traversal.
state* _initial;
int _inlength;
int _outlength;
int _term;
subseq* _w1; // _subseq[0], preserving _head info
subseq* _w2; // _subseq[1], preserving _head info
subseq* _w3; // _subseq[2], preserving _head info

// Stored when ReadFile(). Dummy transition are attached to the head and
// tail(both _start & _end are 0).
vector<subseq*> _subseq;
vector<path*> _path;



};

class state
{
public:
state(){nameset=false;}
~state(){}
void addFanin (transition* t) { _input.push_back(t); }
void addFanout(transition* t) { _output.push_back(t); }
void setname(string statename){ name='s'+statename; nameset=true; }
bool hasname(){return nameset; }
string getname(){return name;}


vector<transition*> _input;
vector<transition*> _output;
string name;
bool nameset;
};

class transition
{
public:
transition()
{ _start = 0; _end = 0; _input = ""; _output = ""; }
transition(state* s, state* e, string i, string o) 
{ _start = s; _end = e; _input = i; _output = o; }
~transition(){}

state* _start;
state* _end;
string _input;
string _output;
};

class path
{
public:
path(){}
~path(){}

vector<state*> _states;
transition* _tail;
int length;
int cut;
};

class subseq
{
public:
subseq(){}
~subseq(){}

void sethead(transition* h) { _head = h; }
void settail(transition* t) { _tail = t; }
void setseq(string s) { _seq = s; }
string _seq;
transition* _head;
transition* _tail;
};

#endif //WMMGR_H
