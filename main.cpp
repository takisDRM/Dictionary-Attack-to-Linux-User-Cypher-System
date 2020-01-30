#define _XOPEN_SOURCE
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <crypt.h>

using namespace std;

class Encryption
{
    ifstream passwdFile, shadowFile, dictionary;
	ofstream passwdFile_out, shadowFile_out;
    string pathOfPasswdFile, pathOfShadowFile, pathOfDictionary;
    int flagOpenFiles;
    string *arrayOfSalt;
    string *nickname;
    string *password;
	string *wholeThePasswdFile;
	string *wholeTheShadowFile;
	int sizeArrayOfSalt;
    int sizePassword;
    int sizeNickname;
	int sizeWholeThePasswdFile;
	int sizeWholeTheShadowFile;
    void openFiles();

    public:
    Encryption();
    ~Encryption();
    int getSizeArrayOfSalt();
    string getPassword();
    string getNickname();
    void setPassword(string _password);
    void setNickname(string _nickname);
    int checkPassAndNickProperness(string chkString);
	int getThesisOfNickname(string chkString);
    void displayArrayOfSalt();
    void displayPassAndNick();
    void addStringInFieldArraySalt(string insString);
    string saltStringGen();
    int checkIfSaltExists(string salt);
    int getFlagOpenFiles();
    void copyFilesToFields();
	void getNickAndPassAndSaltFromFiles();
	int saveFilesPasswdShadow();
	string getThesisSaltUsingThesis(int thesisOf_Salt);
	string getPasswordUsingThesis(int thesisOf_Password);
	void displayNickname();
	string getNicknameUsingThesis(int thesisOf_Nickname);
	string searchPassInTheDictionary(string _password, int _nicknameThesis);
};

int Encryption::saveFilesPasswdShadow()
{
	int flag= 0;
	
	delete [] wholeThePasswdFile;
	sizeWholeThePasswdFile= 0;
	
	wholeThePasswdFile= new string[sizeNickname];
	sizeWholeThePasswdFile= sizeNickname;
	
	string tmp= "";
	for(int i=0; i<sizeWholeThePasswdFile; ++i)
	{
		tmp= nickname[i]+":x:1515:user:/data/network/online:/bin/bash";
		wholeThePasswdFile[i]= tmp;
	}
	
	tmp= "";
	delete [] wholeTheShadowFile;
	sizeWholeTheShadowFile= 0;
	wholeTheShadowFile= new string[sizePassword];
	sizeWholeTheShadowFile= sizePassword;
	for(int i=0; i<sizeWholeTheShadowFile; ++i)
	{
		tmp= nickname[i]+":$1$"+arrayOfSalt[i]+"$"+password[i]+"::10113:0:99999:7:::";
		wholeTheShadowFile[i]= tmp;
	}
	
	
	cout<<"File: \"passwd.my\""<<endl;
	cout<<"----------------"<<endl;
	for(int i=0; i<sizeWholeThePasswdFile; ++i)
	{
		cout<<wholeThePasswdFile[i]<<endl;
	}
	
	cout<<"\nFile: \"shadow.my\""<<endl;
	cout<<"----------------"<<endl;
	for(int i=0; i<sizeWholeTheShadowFile; ++i)
	{
		cout<<wholeTheShadowFile[i]<<endl;
	}
	
	
	passwdFile.close();
	shadowFile.close();
	
	passwdFile_out.open(pathOfPasswdFile.c_str());
    shadowFile_out.open(pathOfShadowFile.c_str());
	
	if(passwdFile_out.is_open() && shadowFile_out.is_open())
	{
		for(int i=0; i<sizeWholeThePasswdFile; ++i)
		{
			passwdFile_out<<wholeThePasswdFile[i]<<"\n";
			shadowFile_out<<wholeTheShadowFile[i]<<"\n";
		}
		flag= 1;
	}	
	
	passwdFile_out.close();
	shadowFile_out.close();
	
	return flag;
}

Encryption::Encryption()
{
    pathOfPasswdFile= "./passwd.my";
    pathOfShadowFile= "./shadow.my";
	pathOfDictionary= "./dictionary";
	
    password= NULL;
    nickname= NULL;
	arrayOfSalt= NULL;
	wholeThePasswdFile= NULL;
	wholeTheShadowFile= NULL;
    sizeNickname= 0;
    sizePassword= 0;
	sizeArrayOfSalt= 0;
	sizeWholeThePasswdFile= 0;
	sizeWholeTheShadowFile= 0;
    flagOpenFiles= 0;

    passwdFile.open(pathOfPasswdFile.c_str());
    shadowFile.open(pathOfShadowFile.c_str());
	dictionary.open(pathOfDictionary.c_str());

    if((passwdFile.is_open()) && (shadowFile.is_open()) && (dictionary.is_open()))
        flagOpenFiles= 1;
    else
    {
        pathOfPasswdFile= "";
        pathOfShadowFile= "";
		pathOfDictionary= "";
    }
}

Encryption::~Encryption()
{
    delete [] arrayOfSalt;
    delete [] nickname;
    delete [] password;
	delete [] wholeThePasswdFile;
	delete [] wholeTheShadowFile;

    arrayOfSalt= NULL;
    nickname= NULL;
    password= NULL;
	wholeThePasswdFile= NULL;
	wholeTheShadowFile= NULL;
}

void Encryption::addStringInFieldArraySalt(string insString)
{
    if(arrayOfSalt == NULL)
    {
        ++sizeArrayOfSalt;
        arrayOfSalt= new string[sizeArrayOfSalt];
        arrayOfSalt[sizeArrayOfSalt - 1]= insString;
    }
    else
    {
        string *tmpArrayOfSalt= arrayOfSalt;

        ++sizeArrayOfSalt;
        arrayOfSalt= new string[sizeArrayOfSalt];
        for(int i=0; i<sizeArrayOfSalt-1; ++i)
            arrayOfSalt[i] = tmpArrayOfSalt[i];
        
        arrayOfSalt[sizeArrayOfSalt-1]= insString;

        delete [] tmpArrayOfSalt;
    }
}

string Encryption::saltStringGen()
{
    string salt= "";
    int randomNum= 0;
    char stringsOfSalt[]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                           'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                           '1','2','3','4','5','6','7','8','9','.','/'};
    int sizeStringsOfSalt= sizeof(stringsOfSalt);

    srand(time(NULL));
    for(int i=0; i<8; ++i)
    {
        randomNum= rand()%sizeStringsOfSalt;
        salt= salt+ stringsOfSalt[randomNum];
    }
    //cout<<salt<<endl;
    return salt;
}

int Encryption::checkIfSaltExists(string salt)
{
    int flag= 0;

    if(arrayOfSalt != NULL)
    {
        for(int i=0; i<sizeArrayOfSalt; ++i)
            if(salt == arrayOfSalt[i])
            {
                flag= 1;
                break;
            }
    }
    return flag;
}

int Encryption::getSizeArrayOfSalt()
{
    return sizeArrayOfSalt;
}

void Encryption::displayArrayOfSalt()
{
    cout<<"sizeArrayOfSalt= "<<sizeArrayOfSalt<<endl;
    for(int i=0; i<sizeArrayOfSalt; ++i)
        cout<<i<<". "<<arrayOfSalt[i]<<endl;
}

void Encryption::setPassword(string _password)
{
    if(password == NULL)
    {
        ++sizePassword;
        password= new string[sizePassword];
        password[sizePassword - 1]= _password;
    }
    else
    {
        string *tmpPassword= password;

        ++sizePassword;
        password= new string[sizePassword];
        for(int i=0; i<sizePassword-1; ++i)
            password[i] = tmpPassword[i];

        password[sizePassword-1]= _password;

        delete [] tmpPassword;
    }
}

void Encryption::setNickname(string _nickname)
{
    if(nickname == NULL)
    {
        ++sizeNickname;
        nickname= new string[sizeNickname];
        nickname[sizeNickname - 1]= _nickname;
    }
    else
    {
        string *tmpNickname= nickname;

        ++sizeNickname;
        nickname= new string[sizeNickname];
        for(int i=0; i<sizeNickname-1; ++i)
            nickname[i] = tmpNickname[i];

        nickname[sizeNickname-1]= _nickname;

        delete [] tmpNickname;
    }
	/*****test****/
	cout<<_nickname<<endl;
	cout<<sizeNickname<<endl;
	cout<<nickname[sizeNickname-1]<<endl;
}

int Encryption::getFlagOpenFiles()
{
    return flagOpenFiles;
}

int Encryption::checkPassAndNickProperness(string chkString)
{
    int flag= 0;
    int lenChkString= chkString.length();
    cout<<endl;

    if((lenChkString>=6) && (lenChkString<=8))  //Apo 6-8 xaraktires kai na arxizei apo kefalaio h' pezo latiniko xaraktira.
        if ((chkString[0]>='A' && chkString[0]<='Z') || (chkString[0]>='a' && chkString[0]<='z'))
        {
            int i= 0;
            flag= 1;
            while(chkString[i]!='\0')
            {
                if((chkString[i]>=0 && chkString[i]<=32) || (chkString[i]>=127))
                {
                    //cout<<"Find wrong: "<<chkString[i]<<endl;
                    flag= 0;
                    //break;
                }
                else
                    //cout<<"****** OK "<<chkString[i]<<endl;
                ++i;
            }
        }
    return flag;
}

void Encryption::copyFilesToFields()
{
    int flag= 0, cnt= 0;
	string tmp= "";
	
	passwdFile.seekg(0, std::ios::beg);
	
	while(getline(passwdFile, tmp))
		++cnt;
	
	sizeWholeThePasswdFile= cnt;
	wholeThePasswdFile= new string[cnt];
	passwdFile.clear();
	passwdFile.seekg(0, std::ios::beg);
	
	for(int i=0; i<sizeWholeThePasswdFile; ++i)
	{
		getline(passwdFile, tmp);
		wholeThePasswdFile[i]= tmp;
		//cout<<wholeThePasswdFile[i]<<endl;
	}
	
	tmp= "";
	cnt= 0;
	
	while(getline(shadowFile, tmp))
		++cnt;
	
	sizeWholeTheShadowFile= cnt;
	wholeTheShadowFile= new string[cnt];
	shadowFile.clear();
	shadowFile.seekg(0, ios::beg);
	
	for(int i=0; i<sizeWholeTheShadowFile; ++i)
	{
		getline(shadowFile, tmp);
		wholeTheShadowFile[i]= tmp;
		//cout<<wholeTheShadowFile[i]<<endl;
	}
}

void Encryption::displayPassAndNick()
{
	if(passwdFile.is_open()  &&  shadowFile.is_open())
	{
		cout<<"The file \"passwd.my\" has "<<sizeWholeThePasswdFile<<" line(s)."<<endl;
		cout<<"-----------------------------------"<<endl;
		for(int i=0; i<sizeWholeThePasswdFile; ++i)
			cout<<wholeThePasswdFile[i]<<endl;

		cout<<endl;
		
		cout<<"The file \"shadow.my\" has "<<sizeWholeThePasswdFile<<" line(s)."<<endl;
		cout<<"-----------------------------------"<<endl;
		for(int i=0; i<sizeWholeTheShadowFile; ++i)
			cout<<wholeTheShadowFile[i]<<endl;

		cout<<endl;
	}
	
    if(nickname!=NULL  &&  arrayOfSalt!=NULL  &&  password!=NULL)
    {
		cout<<"Nickname\tSalt\t\tPassword"<<endl;
		cout<<"--------\t----\t\t--------"<<endl;
        for(int i=0; i<sizeNickname; ++i)
            cout<<i<<". "<<nickname[i]<<'\t'<<arrayOfSalt[i]<<'\t'<<password[i]<<endl;
    }
}

void Encryption::getNickAndPassAndSaltFromFiles()
{
	int charCount= 0, j=0;
	string salt="", nick="", pass="";
	
	for(int i=0; i<sizeWholeThePasswdFile; ++i)
		while(wholeThePasswdFile[i][j] != '\0')
		{
			if(wholeThePasswdFile[i][j] == ':')
			{
				setNickname(nick);
				//cout<<nick<<endl;
				nick= "";
				j= 0;
				break;
			}
			else
				nick= nick + wholeThePasswdFile[i][j];
			
			++j;
		}

	j= 0;
	charCount= 0;
	for(int i=0; i<sizeWholeTheShadowFile; ++i)
	{
		while(wholeTheShadowFile[i][j]!='\0')
		{
			if(wholeTheShadowFile[i][j]== '$')
			{
				++charCount;
				++j;
			}
			if(charCount == 2)
				salt= salt + wholeTheShadowFile[i][j];
				
			if(charCount == 3)
			{
				addStringInFieldArraySalt(salt);
				//cout<<salt<<endl;
				++charCount;
				salt= "";
			}

			if(charCount == 4  &&  wholeTheShadowFile[i][j]!= ':')
			{
				pass= pass + wholeTheShadowFile[i][j];
			}
			else if(charCount == 4  &&  wholeTheShadowFile[i][j]== ':')
			{
				setPassword(pass);
				charCount= 0;
				j= 0;
				pass= "";
				break;
			}
			++j;
		}
	}
}

int Encryption::getThesisOfNickname(string _chkString)
{
	int thesis= -1;
	
	if(nickname!=NULL)
		for(int i=0; i<sizeNickname; ++i)
			if(nickname[i] == _chkString)
			{
				thesis= i;
				break;
			}
	
	return thesis;
}

string Encryption::getThesisSaltUsingThesis(int thesisOf_Salt)
{
	string salt_ret= "";
	
	if(thesisOf_Salt<sizeArrayOfSalt)
		salt_ret= arrayOfSalt[thesisOf_Salt];
	
	return salt_ret;
}

string Encryption::getPasswordUsingThesis(int thesisOf_Password)
{
	string pass_ret= "";
	
	if(thesisOf_Password<sizePassword)
		pass_ret= password[thesisOf_Password];
	
	return pass_ret;
}

string Encryption::getNicknameUsingThesis(int thesisOf_Nickname)
{
	string nick_ret= "";
	
	if(thesisOf_Nickname < sizeNickname)
		nick_ret= nickname[thesisOf_Nickname];
	
	return nick_ret;
}
void Encryption::displayNickname()
{
	for(int i=0; i<sizeNickname; ++i)
		cout<<i<<". "<<nickname[i]<<endl;
}

string Encryption::searchPassInTheDictionary(string _salt, int _nicknameSelection)
{
	string finalEncrypted, password, dictWord;
	int found= 0;
	
	while(getline(dictionary, dictWord))
	{
		finalEncrypted= crypt(dictWord.c_str(), _salt.c_str());
		password= getPasswordUsingThesis(_nicknameSelection);
		if(finalEncrypted == password)
		{
			found= 1;
			break;
		}
	}
	
	dictionary.clear();
	dictionary.seekg(0, std::ios::beg);
	
	if(found== 0)
		dictWord= "";
		
	return dictWord;
}

int main()
{
    Encryption test;
    char sel= 'j';
    string str, password, nickname, finalEncrypted, password2, dictWord;
    int flag_exists_salt, flag_close, thesis, flag_login, nicknameSelection= -50, found;

    if(test.getFlagOpenFiles() == 1)
    {
        test.copyFilesToFields();
		test.getNickAndPassAndSaltFromFiles();	
        do
        {
            system("clear");
			cout<<"---  Files have open!!!  ---"<<endl;
            cout<<"*  (C)reate Account\n*  (S)ign in\n*  (V)iew ALL\n*  (D)ictionary Attack!!!\n*  (E)xit\n\nSelect: ";
            cin>>sel;

            switch(sel)
            {
                case 'c':
                case 'C':   str= "";
                            password= "";
                            nickname= "";
                            flag_exists_salt= 1;

                            cout<<"*  Please give the Nickname: ";
                            cin>>nickname;
                            cout<<"*  and Password: ";
                            cin>>password;
                            if( (test.checkPassAndNickProperness(nickname) == 1))
                            {
                                test.setNickname(nickname);
                                while(flag_exists_salt == 1)
                                {
                                    str= test.saltStringGen();
                                    flag_exists_salt= test.checkIfSaltExists(str);
                                    //cout<<str<<" "<<flag<<endl;
                                    if(flag_exists_salt != 1)
                                        test.addStringInFieldArraySalt(str);
                                }
								finalEncrypted= "";
								finalEncrypted= crypt(password.c_str(), str.c_str());
								test.setPassword(finalEncrypted);
								//cout<<"finalEncrypted= "<<finalEncrypted<<endl;
								flag_exists_salt= 1;
                            }
                            else
                                cout<<"!!!!! WRONG Nickname or Password !!!!!"<<endl;
                            break;
                case 's':
                case 'S':	password= "";
							password2= "";
                            nickname= "";
							str= "";
							finalEncrypted= "";
							thesis= -1;
							flag_login= 0;
							
							cout<<"*  Please give the Nickname: ";
                            cin>>nickname;
                            cout<<"*  and Password: ";
                            cin>>password;
							password2= password;
							
							thesis= test.getThesisOfNickname(nickname);
							if(thesis >= 0)
                            {
								str= test.getThesisSaltUsingThesis(thesis);
								//cout<<"salt= "<<str<<endl;
								
								if(str != "")
								{
									finalEncrypted= crypt(password2.c_str(), str.c_str());
									password= test.getPasswordUsingThesis(thesis);
									
									//cout<<"finalEncrypted= "<<finalEncrypted<<endl;
									//cout<<"password      = "<<password<<endl;
								}
								
								if((finalEncrypted == password) && str!="" )
									flag_login= 1;
							}
							
							if(flag_login == 1)
								cout<<"\n*  O.K. - Success!!!"<<endl;
							else
								cout<<"\n*  Wrong Nickname or Password!!!"<<endl;

							password= "";
							password2= "";
                            nickname= "";
							str= "";
							finalEncrypted= "";
							flag_login= 0;
							thesis= -1;
							
                            break;

                case 'v':
                case 'V':   cout<<endl;
							test.displayPassAndNick();
                            break;
							
				case 'd':
				case 'D':	nickname= "";
							password= "";
							finalEncrypted= "";
							str= "";
							dictWord= "";
							found= 0;
							nicknameSelection= -50;
							
							cout<<"\n\nChoose a User"<<endl;
							cout<<"----------------"<<endl;
							test.displayNickname();
							cout<<"\nSelection: ";
							cin>>nicknameSelection;
							
							nickname= test.getNicknameUsingThesis(nicknameSelection);
							if(nickname != "")
							{
								str= test.getThesisSaltUsingThesis(nicknameSelection);
								dictWord= test.searchPassInTheDictionary(str, nicknameSelection);
								if(dictWord != "")
									cout<<"*  The Account has broken!!! The password is: "<<dictWord<<endl;
								else
									cout<<"*  The password there isn't in the dictionary!!!"<<endl;
							}
							else
								cout<<"*  Wrong selection!!!"<<endl;

							nickname= "";
							password= "";
							finalEncrypted= "";
							str= "";
							dictWord= "";
							nicknameSelection= -50;
							found= 0;
							break;

                case 'e':
                case 'E':	flag_close= test.saveFilesPasswdShadow();
							cout<<"\n*  The file has saved successfully"<<endl;
							cout<<"*  Bye-Bye!!!"<<endl;
                            break;
            }

            cout<< "\nPress ENTER key to continue..."<<endl;
			cin.ignore();
			cin.get();
        } while(sel != 'e'  &&   sel != 'E');
    }
	else
	{
		cout<<"\n*  File(s) not found!!!"<<endl;
		cout<< "Press ENTER key to EXIT..."<<endl;
		cin.ignore();
		cin.get();
	}
}

//g++ asf_4.cpp -lcrypt -o output
