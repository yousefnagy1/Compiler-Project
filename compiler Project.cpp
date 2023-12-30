#include<bits/stdc++.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

using namespace std;

int isKeyword(char buffer[]){
    char keywords[32][10] =
    {"auto","break","case","char","const","continue","default",
                            "do","double","else","enum","extern","float","for","goto",
                            "if","int","long","register","return","short","signed",
                            "sizeof","static","struct","switch","typedef","union",
                            "unsigned","void","volatile","while"};
    int i, flag = 0;

    for(i = 0; i < 32; ++i){
        if(strcmp(keywords[i], buffer) == 0){
            flag = 1;
            break;
        }
    }

    return flag;
}

//code parser 



enum token{
	cin_sy , cout_sy, cin_op_sy, cout_op_sy, lparn_sy, rparn_sy, plus_sy, mult_sy, id , int_sy, real_sy, 
	error_sy, end_source_sy	
};

class parser{
	
	private:
		ifstream f;
		token curr_token;
		token check_reserved (string s){
			if (s=="cin") return cin_sy;
			else if (s=="cout") return cout_sy;
			else return id;
		}

		token get_token(){
			
			char ch;
			string s;
			int state = 0;
			
			
			while(state>=0 && state <=11 && !f.eof()){
				
				switch(state){
					case 0:
						f.get(ch);
						if (f.eof()) return end_source_sy;
						else if (isspace(ch)) {s=ch; state = 0;}
						else if (isdigit(ch)) {s=ch; state = 1;}
						else if (ch=='.') {s=ch; state = 2;}
						
						else if (isalpha(ch)) {s=ch; state = 4;}
						else if (ch=='<') state = 5;
						else if (ch=='>') state = 6;
						else if (ch=='+') state = 7;
						else if (ch=='*') state = 8;
						else if (ch=='(') state = 9;
						else if (ch==')') state = 10;
						else state = 11;
						break;
					
					case 1:
						f.get(ch);	
						if (isdigit(ch)) {s+=ch; state=1;}
						else if (ch=='.') state=3;
						else {f.putback(ch); return int_sy;}
						break;
					
					case 2:
						f.get(ch);
						if (isdigit(ch)) {s+=ch; state=2;}
						else
						{
							f.putback(ch);
							return real_sy;
						}
						break;
					
					case 3:
						f.get(ch);
						if (isdigit(ch)) {s+=ch; state=3;}
						else
						{
							f.putback(ch);
							return real_sy;
						}
						break;
					
					case 4:
						f.get(ch);
						if(isalnum(ch)) {s+=ch; state=4;}
						else
						{
							f.putback(ch);
							return check_reserved(s);
						}	
						break;
					
					case 5:	
						f.get(ch);
						if(ch=='<') return cout_op_sy;
						else return error_sy;
						break;
					
					case 6:
						f.get(ch);
						if(ch=='>') return cin_op_sy;
						else return error_sy;
						break;
					
					case 7:
						return plus_sy; break;
					
					case 8:
						return mult_sy; break;
					
					case 9:
						return lparn_sy; break;
						
					case 10:
						return rparn_sy; break;
						
					case 11:
						return error_sy; break;
												
					
				}
			}
			return end_source_sy;
		}

	string name(token t){
		
		switch(t){
			
			case cin_sy: return "cin token";break;
			case cout_sy: return "cout token";break;
			case cin_op_sy: return ">> token";break;
			case cout_op_sy: return "<< token";break;
			case id: return "identifier";break;
			case int_sy: return "int token";break;
			case real_sy: return "real token";break;
			case plus_sy: return "+ token";break;
			case mult_sy: return "* token";break;
			case lparn_sy: return "( token";break;
			case rparn_sy: return ") token";break;
			case error_sy: return "error token";break;
			case end_source_sy: return "End of file";break;	
			
		}
		
		return "error token";
	}		
		
	public:	
		
		
		parser(string filename){
			f.open(filename.c_str());
		}
		
		~parser(){
			f.close();
		}
		void match(token terminal){
			
			if (curr_token == terminal)
				cout<<name(curr_token)<<" is matched"<<endl;
			
			else
				syntax_error(curr_token);
			curr_token = get_token();
		}
		
		void syntax_error(token terminal){
			cout<<name(terminal)<<" is not Expected\n";
		}
		
		void sample_parser(){
			curr_token = get_token();
			program();
			match(end_source_sy);
			
		}
		
		
		void program(){
			if(curr_token == cin_sy){
				match(cin_sy);
				match(cin_op_sy);
				expr_seq();
			}
			else if(curr_token == cout_sy){
				match(cout_sy);
				match(cout_op_sy);
				expr_seq();
			}
			else
				syntax_error(curr_token);
		}
		
		
		void expr_seq(){
			expr();
			while(curr_token == id || curr_token == int_sy || curr_token == real_sy || curr_token == lparn_sy || curr_token == rparn_sy ){
				expr();
			}
		}
		
		
		void expr(){
			
			switch(curr_token){
				
				case id: match(id); break;
				case int_sy : match(int_sy);break;
				case real_sy : match(real_sy);break;
				
				case lparn_sy:
					match (lparn_sy);
					if (curr_token == plus_sy){
						match (plus_sy);
						expr_seq();
						match(rparn_sy);
					}
					else if (curr_token == mult_sy){
						match (mult_sy);
						expr_seq();
						match(rparn_sy);
					}
					else{
						syntax_error(curr_token);
					}
					break;
				
			}
			
			
		}
		
	
};

int main(){
    char ch, buffer[15],b[30], logical_op[] = "><",math_op[]="+-*/=",numer[]=".0123456789",other[]=",;\(){}[]'':";
    ifstream fin("project.txt");
    int mark[1000]={0};
    int i,j=0,kc=0,ic=0,lc=0,mc=0,nc=0,oc=0,aaa=0;
    vector < string > k;
    vector<char >id;
    vector<char>lo;
    vector<char>ma;
    vector<string>nu;
     vector<char>ot;
    if(!fin.is_open()){
        cout<<"error while opening the file\n";
        exit(0);
    }

    while(!fin.eof()){
           ch = fin.get();
          for(i = 0; i < 12; ++i){
               if(ch == other[i]){
                    int aa=ch;
                if(mark[aa]!=1){
                    ot.push_back(ch);
                    mark[aa]=1;
                    ++oc;
                }
               }
        }

        for(i = 0; i < 5; ++i){
               if(ch == math_op[i]){
                    int aa=ch;
                if(mark[aa]!=1){
                    ma.push_back(ch);
                    mark[aa]=1;
                    ++mc;
                }
               }
        }
        for(i = 0; i < 2; ++i){
               if(ch == logical_op[i]){
                    int aa=ch;
                if(mark[aa]!=1){
                    lo.push_back(ch);
                    mark[aa]=1;
                    ++lc;
                }
               }

        }
        if(ch=='0' || ch=='1' || ch=='2' || ch=='3' || ch=='4' || ch=='5' || ch=='6' || ch=='7' || ch=='8' || ch=='9' || ch=='.' ||ch == ' ' || ch == '\n' || ch == ';'){

            if(ch=='0' || ch=='1' || ch=='2' || ch=='3' || ch=='4' || ch=='5' || ch=='6' || ch=='7' || ch=='8' || ch=='9' || ch=='.')b[aaa++]=ch;
            if((ch == ' ' || ch == '\n' || ch == ';') && (aaa != 0)){
                   b[aaa] = '\0';
                   aaa = 0;
                   char arr[30];
                    strcpy(arr,b);
                        nu.push_back(arr);
                    ++nc;

                }
        }


           if(isalnum(ch)){
               buffer[j++] = ch;
           }
           else if((ch == ' ' || ch == '\n') && (j != 0)){
                   buffer[j] = '\0';
                   j = 0;

                   if(isKeyword(buffer) == 1){

                       k.push_back(buffer);
                       ++kc;
                   }
                   else{



                    if(buffer[0]>=97 && buffer[0]<=122) {
                        if(mark[buffer[0]-'a']!=1){
                        id.push_back(buffer[0]);
                        ++ic;
                        mark[buffer[0]-'a']=1;
                       }

                    }

                   }

           }

    }

    fin.close();
    printf("Keywords: ");
     for(int f=0;f<kc;++f){
            if(f==kc-1){
                cout<<k[f]<<"\n";
            }
            else {
                cout<<k[f]<<", ";
            }
    }
    printf("Identifiers: ");
     for(int f=0;f<ic;++f){
        if(f==ic-1){
                cout<<id[f]<<"\n";
            }
            else {
                cout<<id[f]<<", ";
            }
    }
    printf("Math Operators: ");
    for(int f=0;f<mc;++f){
            if(f==mc-1){
                cout<<ma[f]<<"\n";
            }
            else {
                cout<<ma[f]<<", ";
            }
    }
    printf("Logical Operators: ");
    for(int f=0;f<lc;++f){
            if(f==lc-1){
                cout<<lo[f]<<"\n";
            }
            else {
                cout<<lo[f]<<", ";
            }

    }
    printf("Numerical Values: ");
    for(int f=0;f<nc;++f){
            if(f==nc-1){
                cout<<nu[f]<<"\n";
            }
            else {
                cout<<nu[f]<<", ";
            }

    }
    printf("Others: ");
    for(int f=0;f<oc;++f){
            if(f==oc-1){
                cout<<ot[f]<<"\n";
            }
            else {
                cout<<ot[f]<<" ";
            }

    }
    cout<<endl;
    cout<<endl;
    cout<<"parser"<<endl;
    cout<<endl;
    
    string name="project.txt";
    parser p(name);
    p.sample_parser();
    p.~parser();

    return 0;
}

