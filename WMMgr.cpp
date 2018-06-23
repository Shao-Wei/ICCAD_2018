#include "WMMgr.h"
/*
void WMMgr::CutAll()
{
    size_t cnt = 0;
    while(cnt < _subseq.size()){
        if(CutSeq(_subseq[cnt])) { _subseq.erase(_subseq.begin() + cnt); }
        else { cnt++; continue; }
    }
}

void WMMgr::LinkSeq()
{
    for(size_t i=0; i<_subseq.size(); i++){
        size_t cnt = 0;
        string intmp;
        string outtmp;
        state* previous_s;
        transition* previous_t = _subseq[i]->_head;
        while(cnt < _subseq[i]->_seq.length()){
            intmp = intmp.assign(_subseq[i]->_seq, cnt, inlength);
            cnt += inlength;
            outtmp = outtmp.assign(_subseq[i]->_seq, cnt, outlength);
            cnt += outlength;
            
            state* s = new state();
            _states.push_back(s);
            transition* t = new transition(s, 0, intmp, outtmp);
            _transitions.push_back(t);
            previous_t->_end = s;
            s->addFanin(previous_t);
            s->addFanout(t);
            previous_s = s;
            previous_t = t;
        }
        _subseq[i]->_tail->_start = previous_s;
        previous_s->_output[0] = _subseq[i]->_tail;
    }
}

bool WMMgr::CutSeq(subseq* s)
{
	size_t cnt = 0;
	string intmp;
	string outtmp;
	string in_prev;
	string out_prev;
	vector<path*> paths;
	path* longest_path;
	int longest_length = 2;
	while(cnt<s->_seq.size()){
		intmp = intmp.assign(s->_seq, cnt, inlength);
        cnt += inlength;
        outtmp = outtmp.assign(s->_seq, cnt, outlength);
        cnt += outlength;
		if(paths.size()!=0){
			for(size_t i=0;i<paths.size();i++){
				for(size_t j=0;j<paths[i]->_states.back()->_output.size();j++){
					if(intmp == paths[i]->_states.back()->_output[j]->_input&&outtmp == paths[i]->_states.back()->_output[j]->_output){
						for(size_t k=0;k<paths[i]->_states.back()->_input.size();k++){
							if(in_prev == paths[i]->_states.back()->_input[k]->_input&&out_prev == paths[i]->_states.back()->_input[k]->_output){
								paths[i]->_states.push_back(paths[i]->_states.back()->_output[j]->_end);
								paths[i]->length = paths[i]->_states.size();
								paths[i]->_tail = paths[i]->_states.back()->_output[j];
								break;
							}
						}
						break;
					}
				}
				if(paths[i]->length<2){
					paths.erase(paths.begin()+i);
					i--;
				}
			}
		}
		for(size_t j=0;j<_states.size();j++){
			for(size_t k=0;k<_states[j]->_output.size();k++){
				if(intmp == _states[j]->_output[k]->_input&&outtmp == _states[j]->_output[k]->_output){
					path* tmp_p;
					tmp_p->_states.push_back(_states[j]);	
					tmp_p->length = tmp_p->_states.size();
					tmp_p->_tail = _states[j]->_output[k];
					tmp_p->cut = cnt - inlength - outlength;
					paths.push_back(tmp_p);
				}
			}				
		}
		in_prev = intmp;
		out_prev = outtmp;
	}
	if(paths.size()==0)
		return false;
	else{
		for(size_t i=0;i<paths.size();i++){
			if(paths[i]->length >= longest_length && paths[i]->_states.back()->_output.size() < (2^inlength)){
				longest_path = paths[i];
				longest_length = paths[i]->length;
			}
		}
		subseq* _front;
		subseq* _back;
		if(cut!=0){
			_front->_seq = s->_seq.substr(0,longest_path->cut);
			_front->_head = s->_head;
			_back->_seq = s->_seq.substr(longest_path->cut+longest_path->length*(inlength+outlength));
			_back->_head = longest_path->_tail;
			_back->_tail= s->_tail;
			_subseq.push_back(_front);
			_subseq.push_back(_back);
		}
		return true;
	}
}
*/

bool WMMgr::ReadFile(char* inputfile, char* input_w1, char* input_w2, char* input_w3)
{
    string line; // for getline (tmp string)
    string del = " ";

	
    //-- initial .i .o .s .p .r -----------------------------------
    size_t blank;
    string input_num, output_num, state_num, reset_state;
	int    in_num,    out_num,    statenum,  term,     r_state;

	fstream readin;
	readin.open(inputfile,ios::in);
    
	getline(readin, line);
	blank = line.find(" ");
	input_num = line.substr(blank+1);
	in_num = atoi(input_num.c_str());
	_inlength = in_num;

	getline(readin,line);
	blank = line.find(" ");
	output_num = line.substr(blank+1);
	out_num = atoi(output_num.c_str());
	_outlength = out_num;

	getline(readin,line);
	blank = line.find(" ");
	state_num = line.substr(blank+1);
	statenum = atoi(state_num.c_str());

	getline(readin,line); //skip .t, term is calculated later
    term = 0;

	getline(readin,line);
	blank = line.find(" ");
	reset_state = line.substr(blank+2);
	r_state = atoi(reset_state.c_str());

    //-- store watermark 128'b ------------------------------------
    int pad0=0;

    fstream fs_w1;
    string  binary1; 
    fs_w1.open(input_w1, ios::in);
    getline(fs_w1, line);
    for(size_t i=0; i<32; i++){
        binary1 = binary1 + hex2bin(line[i]);
    }
    pad0=((in_num+out_num)-binary1.size()%(in_num+out_num));
    for(int i=0;i<pad0;i++){
        binary1=binary1+'0';
    }
    transition* t1 = new transition(0, 0, "", "");
    transition* t2 = new transition(0, 0, "", "");
    _w1 = new subseq();
    _w1->setseq(binary1);
    _w1->sethead(t1);
    _w1->settail(t2);
    _subseq.push_back(_w1);
    _transitions.push_back(t1);
    _transitions.push_back(t2);
    fs_w1.close();

    fstream fs_w2;
    string  binary2; 
    fs_w2.open(input_w2, ios::in);
    getline(fs_w2, line);
    for(size_t i=0; i<32; i++){
        binary2 = binary2 + hex2bin(line[i]);
    }
    pad0=((in_num+out_num)-binary2.size()%(in_num+out_num));
    for(int i=0;i<pad0;i++){
        binary2=binary2+'0';
    }
    transition* t3 = new transition(0, 0, "", "");
    transition* t4 = new transition(0, 0, "", "");
    _w2 = new subseq();
    _w2->setseq(binary2);
    _w2->sethead(t3);
    _w2->settail(t4);
    _subseq.push_back(_w2);
    _transitions.push_back(t3);
    _transitions.push_back(t4);
    fs_w2.close();

    fstream fs_w3;
    string  binary3; 
    fs_w3.open(input_w3, ios::in);
    getline(fs_w3, line);
    for(size_t i=0; i<32; i++){
        binary3 = binary3 + hex2bin(line[i]);
    }
    pad0=((in_num+out_num)-binary3.size()%(in_num+out_num));
    for(int i=0;i<pad0;i++){
        binary3=binary3+'0';
    }
    transition* t5 = new transition(0, 0, "", "");
    transition* t6 = new transition(0, 0, "", "");
    _w3 = new subseq();
    _w3->setseq(binary3);
    _w3->sethead(t5);
    _w3->settail(t6);
    _subseq.push_back(_w3);
    _transitions.push_back(t5);
    _transitions.push_back(t6);
    fs_w3.close();
    //-- states & transitions -------------------------------------
    for(int i=0; i<statenum; i++){
        state* s = new state();
        _states.push_back(s);
    }
	while(getline(readin,line)){
		int curr, child;
        vector<string> tok;
        vector<string> inout_set;
        state* currstate;
        state* childstate;
		if(line ==".e") break;         // .e break
    
        tok = str_tok(line, del);
        curr  = atoi(tok[1].substr(1).c_str());
        child = atoi(tok[2].substr(1).c_str());
        currstate = _states[curr];
        childstate = _states[child];
        if(!currstate->hasname()){currstate->setname(tok[1].substr(1).c_str());}
        if(!childstate->hasname()){childstate->setname(tok[2].substr(1).c_str());}
        // extend if - exists
        size_t cnt = 0;
        bool   no_dc = true;
        string inout = tok[0] + tok[3];
        inout_set.push_back(inout);
        while(cnt < inout_set.size()){ 
            cout<<inout_set[cnt]<<endl; 
            no_dc = true; 
            for(int i=0; i<_inlength + _outlength; i++){
                if(inout_set[cnt][i] == '-'){
                    no_dc = false;
                    cout<<cnt<<" "<<i<<endl;
                    string add1 = inout_set[cnt].substr(0, i) + '1' + inout_set[cnt].substr(i+1);
                    string add2 = inout_set[cnt].substr(0, i) + '0' + inout_set[cnt].substr(i+1);
                    cout<<add1<<" "<<add2<<endl;
                    inout_set.push_back(add1);
                    inout_set.push_back(add2);
                    break;
                }
            }
            if(no_dc) { cnt++; }
            else { inout_set.erase(inout_set.begin()+cnt); }
        }
        for(size_t i=0; i<inout_set.size(); i++){
            term++;
            string sub_in  = inout_set[i].substr(0, _inlength);
            string sub_out = inout_set[i].substr(_inlength);
            transition* t = new transition(currstate, childstate, sub_in, sub_out);
            _transitions.push_back(t);
            currstate->addFanout(t);
		    childstate->addFanin(t);
        }

    }
    _initial=_states[r_state];
    cout << "term" << term << endl;
    _term=term;
	if(term < statenum*(2^(in_num+out_num))) return true;
	else return false;
}

string WMMgr::hex2bin(char c)
{
    string result;    
    switch(c)
    {
        case'0': result = "0000"; break;
        case'1': result = "0001"; break;
        case'2': result = "0010"; break;
        case'3': result = "0011"; break;
        case'4': result = "0100"; break;
        case'5': result = "0101"; break;
        case'6': result = "0110"; break;
        case'7': result = "0111"; break;
        case'8': result = "1000"; break;
        case'9': result = "1001"; break;
        case'a': result = "1010"; break;
        case'b': result = "1011"; break;
        case'c': result = "1100"; break;
        case'd': result = "1101"; break;
        case'e': result = "1110"; break;
        case'f': result = "1111"; break;
    }
    return result;
}

void WMMgr::WriteFile(char* optfile_name)
{
    fstream os;
    os.open(optfile_name,ios::out);
    os<<".i "<<_inlength<<endl<<".o "<<_outlength<<endl<<".s "<<_states.size()<<endl<<".p "<<_term<<endl<<".r "<<_initial->getname()<<endl;
    for(unsigned i =0 ;i<_states.size();i++){
        for(unsigned j =0;j<_states[i]->_output.size();j++){
                os<<_states[i]->_output[j]->_input<<" "<<_states[i]->getname()<<" "<<_states[i]->_output[j]->_end->getname()<<" "<<_states[i]->_output[j]->_output<<endl;
        }
    }
    os<<".e\n";
    /*cout << _states[0]->_input[3]->_input << " , " << _states[0]->_input[3]->_output << endl;
    cout << _states[0]->_input[2]->_input << " , " << _states[0]->_input[2]->_output << endl;
    cout << _states[0]->_input[1]->_input << " , " << _states[0]->_input[1]->_output << endl;*/
}

vector<string> WMMgr::str_tok(const string& s, const string& seperator)
{
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;
  
    while(i != s.size()){
    // find first letter
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
            if(s[i] == seperator[x]){
                ++i;
                flag = 0;
                break;
            }
        } 
    // find next seperator
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){
            for(string_size x = 0; x < seperator.size(); ++x)
            if(s[j] == seperator[x]){ flag = 1; break; }
            if(flag == 0) ++j;
        }
        if(i != j){
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}
