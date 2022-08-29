# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <cmath>
# include <cstdlib>
# include <iomanip>
# include <stack>
# include <queue>
# include <sstream>

using namespace std;

int gTestNum = 0;

bool IsInt( float num ) {
  return floor( num ) == num;
} // IsInt()

bool IsInt( string str ) {
  return str.find_first_not_of( "0123456789" ) == string::npos;
} // IsInt()

/*
bool IsChar( string str ) {
  if ( str.length() == 1 ) {
    if ( IsLetter( str[0] ) ) {
      return true;
    } // if
  } // if

  return false;
} // IsChar()
*/
bool IsWhitespace( char ch ) {
  if ( ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' ) {
    return true;
  } // if
  else {
    return false;
  } // else
} // IsWhitespace()

bool IsLetter( char ch ) {
  if ( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) ) {
    return true;
  } // if

  return false;
} // IsLetter()

bool IsDigit( char ch ) {
  if ( ch >= '0' && ch <= '9' ) {
    return true;
  } // if

  return false;
} // IsDigit()

void ReadLine() {
  char ch = '\0';
  while ( cin.get( ch ) && ch != '\n' ) {
    
  } // while
} // ReadLine()

string FloatToStr( float num ) {
  stringstream ss;
  ss << fixed << setprecision(3) << num;
  string str = ss.str();
  return str;
} // FloatToStr() 

string IntToStr( int num ) {
  
  stringstream ss;
  ss << num;
  string str = ss.str();
  return str;

} // IntToStr()

enum TokenType {
  ID, CONSTANT, UNDEFINED_TYPE, 
  GE, LE,             // >= <=
  EQ, NEQ,            // == !=   
  INT, FLOAT, CHAR,   
  STRING, BOOL, VOID,
  IF, ELSE, WHILE,
  DO, RETURN,
  AND, OR,            // && ||
  PE, ME,             // += -=
  TE, DE,             // *= /=
  RE,                 // %=
  PP, MM,             // ++ --
  RS, LS              // >> <<
};

enum ErrorType {
  NO_ERROR, LEXICAL_ERROR, SYNTACTICAL_ERROR,
  SEMANTIC_ERROR, QUIT_ERROR
}; 

class ErrorMsg {
public:
  int mLine;
  ErrorType mType;
  string mToken;

  ErrorMsg() {
    mLine = 0;
    mType = NO_ERROR;
    mToken = "";
  } // ErrorMsg()

  ErrorMsg( int line, ErrorType type, string token ) {
    mLine = line;
    mType = type;
    mToken = token;
  } // ErrorMsg()

  ErrorMsg( int line, ErrorType type, char token ) {
    mLine = line;
    mType = type;
    mToken = mToken + token;
  } // ErrorMsg()

  void Set( int line, ErrorType type, string token ) {
    mLine = line;
    mType = type;
    mToken = token;
  } // Set()

  void Set( int line, ErrorType type, char token ) {
    mLine = line;
    mType = type;
    mToken = mToken + token;
  } // Set()

}; // ErrorMsg

class Token {
public:
  string mValue;
  TokenType mType;

  Token() {
    Init();
  } // Token()

  void Init() {
    mValue = "";
    mType = UNDEFINED_TYPE;
  } // Init()

  bool IsEmpty() {
    if ( mValue.compare( "" ) == 0 ) {
      return true;
    } // if
    else {
      return false;
    } // else
  } // IsEmpty()
}; // class Token

class TokenScanner {
  bool PeekChar( char& ch ) {
    if ( mNextChar == '\0' && ! ( cin.get( mNextChar ) ) ) {
      return false;
    } // if
    else {
      ch = mNextChar;
      return true;
    } // else
  } // PeekChar()

  bool GetNonWhitespaceChar( char& ch ) {
    bool getCharSuccess = GetChar( ch );
    while ( getCharSuccess && IsWhitespace( ch ) ) {
      getCharSuccess = GetChar( ch );
    } // while

    if ( getCharSuccess && ch == '/' ) {
      // reminder: divide symbol( "/" ) will be in this section
      char tmpChar = '\0';
      if ( PeekChar( tmpChar ) && tmpChar == '/' ) {
        ReadLine();
        return GetNonWhitespaceChar( ch );
      } // if
    } // if

    return getCharSuccess;
  } // GetNonWhitespaceChar()

  void GetRestOfID( string& str ) {
    // start with letter
    char tmpChar = '\0';
    while ( PeekChar( tmpChar ) &&
            ( IsLetter( tmpChar ) || IsDigit( tmpChar ) || tmpChar == '_' ) ) {
      GetChar( tmpChar );
      str += tmpChar;
    } // while
  } // GetRestOfID()

  bool GetRestOfNum( string& str ) {
    // start with digit or '.' or '-'
    char tmpChar = '\0';
    bool dotOnce = false;
    bool gotDigit = false;
    if ( str[0] == '.' ) {
      dotOnce = true;
    } // if

    while ( PeekChar( tmpChar )
            && ( IsDigit( tmpChar ) || ( !dotOnce && tmpChar == '.' ) ) ) {
      GetChar( tmpChar );
      str += tmpChar;
      if ( tmpChar == '.' ) {
        dotOnce = true;
      } // if

      if ( IsDigit( tmpChar ) ) {
        gotDigit = true;
      } // if
    } // while

    return gotDigit;
  } // GetRestOfNum()

  void GetRestOfSpecial( string& str ) {
    // special symbol
    char tmpChar = '\0';
    if ( str[0] == '!' || str[0] == '*' 
         || str[0] == '/' || str[0] == '%' ) {
      if ( PeekChar( tmpChar ) && tmpChar == '=' ) {
        GetChar( tmpChar );
        str += tmpChar;
      } // if
    } // if
    else if ( str[0] == '&' || str[0] == '|' ) {
      if ( PeekChar( tmpChar ) && tmpChar == str[0] ) {
        GetChar( tmpChar );
        str += tmpChar;
      } // if
    } // else if
    else if ( str[0] == '>' || str[0] == '<' || str[0] == '='
              || str[0] == '+' || str[0] == '-' ) {
      if ( PeekChar( tmpChar ) && ( tmpChar == str[0] || tmpChar == '=' ) ) {
        GetChar( tmpChar );
        str += tmpChar;
      } // if
    } // else if
  } // GetRestOfSpecial()

  void DetermineTokenType( Token& token ) {
    if ( token.mValue == "true" || token.mValue == "false" ) {
      token.mType = CONSTANT;
    } // if
    else if ( token.mValue == "int" ) {
      token.mType = INT;
    } // else if
    else if ( token.mValue == "float" ) {
      token.mType = FLOAT;
    } // else if
    else if ( token.mValue == "char" ) {
      token.mType = CHAR;
    } // else if
    else if ( token.mValue == "string" ) {
      token.mType = STRING;
    } // else if
    else if ( token.mValue == "bool" ) {
      token.mType = BOOL;
    } // else if
    else if ( token.mValue == "void" ) {
      token.mType = VOID;
    } // else if
    else if ( token.mValue == "if" ) {
      token.mType = IF;
    } // else if
    else if ( token.mValue == "else" ) {
      token.mType = ELSE;
    } // else if
    else if ( token.mValue == "while" ) {
      token.mType = WHILE;
    } // else if
    else if ( token.mValue == "do" ) {
      token.mType = DO;
    } // else if
    else if ( token.mValue == "return" ) {
      token.mType = RETURN;
    } // else if
    else if ( token.mValue == "==" ) {
      token.mType = EQ;
    } // else if
    else if ( token.mValue == "!=" ) {
      token.mType = NEQ;
    } // else if
    else if ( token.mValue == ">=" ) {
      token.mType = GE;
    } // else if
    else if ( token.mValue == "<=" ) {
      token.mType = LE;
    } // else if
    else if ( token.mValue == "&&" ) {
      token.mType = AND;
    } // else if
    else if ( token.mValue == "||" ) {
      token.mType = OR;
    } // else if
    else if ( token.mValue == "+=" ) {
      token.mType = PE;
    } // else if
    else if ( token.mValue == "-=" ) {
      token.mType = ME;
    } // else if
    else if ( token.mValue == "*=" ) {
      token.mType = TE;
    } // else if
    else if ( token.mValue == "/=" ) {
      token.mType = DE;
    } // else if
    else if ( token.mValue == "%=" ) {
      token.mType = RE;
    } // else if
    else if ( token.mValue == "++" ) {
      token.mType = PP;
    } // else if
    else if ( token.mValue == "--" ) {
      token.mType = MM;
    } // else if
    else if ( token.mValue == ">>" ) {
      token.mType = RS;
    } // else if
    else if ( token.mValue == "<<" ) {
      token.mType = LS;
    } // else if
  } // DetermineTokenType()

public:
  int mLine;
  int mColumn;
  char mNextChar;
  Token mNextToken;
  bool mIsNewLine;

  TokenScanner() {
    Init();
  } // TokenScanner()

  void Init() {
    mLine = 0;
    mColumn = 0;
    mIsNewLine = true;
    mNextChar = '\0';
    mNextToken.Init();
  } // Init()

  bool GetChar( char& ch ) {
    if ( mNextChar != '\0' ) {
      ch = mNextChar;
      mNextChar = '\0';
    } // if nextChar != '\0'
    else if ( cin.get( ch ) ) {
      ;
    } // if cin >> ch
    else {
      return false;
    } // else

    // check if valid char is encountered
    // only counting line number when true
    // if ( !mFirstValidChFound ) {
    //   if ( IsWhitespace( ch ) ) {
    //     return true;
    //   } // if
    //   else {
    //     mFirstValidChFound = true;
    //   } // else
    // } // if

    if ( mIsNewLine ) {
      mLine++;
      mColumn = 0;
      mIsNewLine = false;
    } // if

    if ( ch == '\n' ) {
      mIsNewLine = true;
    } // if

    mColumn++;
    return true;
  } // GetChar()

  bool ReadLine() {
    char tmpChar = '\0';
    bool getCharSuccess = false;
    getCharSuccess = GetChar( tmpChar );
    while ( getCharSuccess && tmpChar != '\n' ) {
      getCharSuccess = GetChar( tmpChar );
    } // while

    return getCharSuccess;
  } // ReadLine()

  void CleanInputAfterCMD() {
    char ch = '\0' ;
    ch = cin.peek();
    while ( ch == ' ' || ch == '\t' ) {
      GetChar( ch );
      ch = cin.peek();
    } // while

    if ( ch == '\n' ) {
      GetChar( ch );
    } // if
    else if ( ch == '/' ) {
      GetChar( ch );
      ch = cin.peek();
      if ( ch == '/' ) {
        ReadLine();
      } // if
      else {
        cin.putback( '/' );
      } // else
    } // if
  } // CleanInputAfterCMD()

  bool GetToken( Token& token ) {
    if ( !mNextToken.IsEmpty() ) {
      token = mNextToken;
      mNextToken.Init();
      

      return true;
    } // if

    char ch = '\0';
    string tokenValue = "";
    bool getTokenSuces = false;

    if ( GetNonWhitespaceChar( ch ) ) {
      tokenValue += ch;
      char tmpChar2 = '\0';

      if ( IsLetter( ch ) ) {
        GetRestOfID( tokenValue );
        token.mType = ID;
        getTokenSuces = true;
      } // if
      else if ( IsDigit( ch ) 
                || ( ch == '.' && PeekChar( tmpChar2 ) && IsDigit( tmpChar2 ) ) ) {
        GetRestOfNum( tokenValue );
        token.mType = CONSTANT;
        getTokenSuces = true;
      } // else if
      else if ( ch == '\'' ) {
        PeekChar( ch );
        // ''
        if ( ch == '\'' ) {
          ErrorMsg errorMsg( mLine, LEXICAL_ERROR, '\'' );
          throw errorMsg;
        } // if

        
        if ( ch == '\\' ) {
          GetChar( ch );
          tokenValue += ch;

          PeekChar( ch );
        } // if

        if ( ch == '\n' || ch == '\r' ) {
          ErrorMsg errorMsg( mLine, LEXICAL_ERROR, '\'' );
          throw errorMsg;
        } // if
        
        
        GetChar( ch );
        tokenValue += ch;

        PeekChar( ch );
        if ( ch != '\'' ) {
          ErrorMsg errorMsg( mLine, LEXICAL_ERROR, '\'' );
          throw errorMsg;
        } // if

        GetChar( ch );
        tokenValue += ch;

        token.mType = CONSTANT;
        getTokenSuces = true;
      } // else if
      else if ( ch == '\"' ) {
        PeekChar( ch );
        while ( ch != '\"' ) {
          
          if ( ch == '\n' || ch == '\r' ) {
            ErrorMsg errorMsg( mLine, LEXICAL_ERROR, '\"' );
            throw errorMsg;
          } // if

          GetChar( ch );
          tokenValue += ch;

          PeekChar( ch );
        } // while

        GetChar( ch );
        tokenValue += ch;
        token.mType = CONSTANT;
        getTokenSuces = true;
      } // else if
      else if ( ch == '(' || ch == ')' 
                || ch == '[' || ch == ']'
                || ch == '{' || ch == '}'
                || ch == '+' || ch == '-'
                || ch == '*' || ch == '/'
                || ch == '%' || ch == '^'
                || ch == '>' || ch == '<'
                || ch == '&' || ch == '|'
                || ch == '=' || ch == '!'
                || ch == ';' || ch == ':'
                || ch == '?' || ch == ',' ) {
        GetRestOfSpecial( tokenValue );
        token.mType = UNDEFINED_TYPE;
        getTokenSuces = true;
      } // else if

      token.mValue = tokenValue;
    } // if
    
    if ( !getTokenSuces ) {
      ErrorMsg errorMsg( mLine, LEXICAL_ERROR, ch );
      throw errorMsg;
    } // if
    else {
      DetermineTokenType( token );
    } // else

    return getTokenSuces;
  } // GetToken()

  bool PeekToken( Token& token ) {
    bool peakSuces = false;
    if ( !mNextToken.IsEmpty() ) {
      token = mNextToken;
      peakSuces = true;
    } // if
    else {
      peakSuces = GetToken( token );
      if ( peakSuces ) {
        mNextToken = token;
      } // if
    } // else

    return peakSuces;
  } // PeekToken()

}; // TokenScanner

enum DataType {
  VOID_TYPE, INT_TYPE, FLOAT_TYPE, 
  BOOL_TYPE, STRING_TYPE, CHAR_TYPE
};

class Data {
public:
  // type: int, float, bool, string, char, void
  DataType mType;

  // value
  string mVal; // string, char, int, float, bool
  vector<string> mBody; // function body or variable '[]' part
  
  // is variable or function
  bool mIsVar, mIsFunc;

  Data() {
    Init();
  } // Data()

  ~Data() {
  } // ~Data()

  void Init() {
    mType = VOID_TYPE;
    mIsVar = false;
    mIsFunc = false;
    mVal = "";
  } // Init()

  bool IsEmpty() {
    if ( mType == VOID_TYPE && mIsVar == false && mIsFunc == false
         && mVal == "" ) {
      return true;
    } // if 

    return false;
  } // IsEmpty()

  void SetByConstantToken( string token ) {
    if( token[0] == '\"' ) {
      mType = STRING_TYPE;
      mVal = token.substr( 1, token.size() - 2 );
    } // if
    else if ( token[0] == '\'' ) {
      mType = CHAR_TYPE;
      mVal = token.substr( 1, token.size() - 2 );
    } // if
    else if ( token == "true" || token == "false" ) {
      mType = BOOL_TYPE;
      mVal = token;
    } // if
    else if ( isdigit( token[0] ) || token[0] == '.' || token[0] == '-' ) {
      if ( token.find( '.' ) != string::npos) {
        mType = FLOAT_TYPE;
        float tmp = atof( token.c_str() );
        mVal = FloatToStr( tmp );
      } // if
      else {
        mType = INT_TYPE;
        mVal = token;
      } // else
    } // if
  } // SetByConstant()

  Data Plus( Data op2 ) {
    // acceptable type: int, float, string(with anything)
    Data returnValue;
    if ( mType == STRING_TYPE || op2.mType == STRING_TYPE ) {
      returnValue.mType = STRING_TYPE;
      // float accuracy display problem
      returnValue.mVal = mVal + op2.mVal;
    } // if
    else if ( mType == INT_TYPE || mType == FLOAT_TYPE ) {
      float value1 = 0.0, value2 = 0.0;
    
      value1 = atof( mVal.c_str() );
      value2 = atof( op2.mVal.c_str() );
      value1 = value1 + value2;

      if ( IsInt( value1 ) ) {
        returnValue.mType = INT_TYPE;
        returnValue.mVal = IntToStr( (int)value1 ); 
      } // if
      else {
        returnValue.mType = FLOAT_TYPE;
        returnValue.mVal = FloatToStr( value1 );
      } // else
    } // if

    return returnValue;
  } // Plus()

  Data Minus( Data op2 ) {
    // acceptable type: int, float
    Data returnValue;
    if ( mType == INT_TYPE || mType == FLOAT_TYPE ) {
      float value1 = 0.0, value2 = 0.0;
    
      value1 = atof( mVal.c_str() );
      value2 = atof( op2.mVal.c_str() );
      value1 = value1 - value2;

      if ( IsInt( value1 ) ) {
        returnValue.mType = INT_TYPE;
        returnValue.mVal = IntToStr( (int)value1 ); 
      } // if
      else {
        returnValue.mType = FLOAT_TYPE;
        returnValue.mVal = FloatToStr( value1 );
      } // else
    } // if

    return returnValue;
  } // Minus()

  Data Mul( Data op2 ) {
    // multiply this instance with the argument( op2 )
    Data returnValue;
    float value1 = 0.0, value2 = 0.0;
    
    value1 = atof( mVal.c_str() );
    value2 = atof( op2.mVal.c_str() );
    value1 = value1 * value2;

    if ( mType == FLOAT_TYPE || op2.mType == FLOAT_TYPE ) {
      returnValue.mType = FLOAT_TYPE;
      returnValue.mVal = FloatToStr( value1 );
    } // if
    else {
      returnValue.mType = INT_TYPE;
      returnValue.mVal = IntToStr( (int)value1 ); 
    } // else

    // returnValue.mVal = mVal * value.mVal;
    return returnValue;
  } // Mul()

  Data Div( Data op2 ) {
    // divide this instance with the argument( op2 )
    Data returnValue;
    float value1 = 0.0, value2 = 0.0;
    
    value1 = atof( mVal.c_str() );
    value2 = atof( op2.mVal.c_str() );
    value1 = value1 / value2;

    if ( IsInt( value1 ) ) {
      returnValue.mType = INT_TYPE;
      returnValue.mVal = IntToStr( (int)value1 ); 
    } // if
    else {
      returnValue.mType = FLOAT_TYPE;
      returnValue.mVal = FloatToStr( value1 );
    } // else

    return returnValue;
  } // Div()

  Data Mod( Data op2 ) {
    // mod this instance with the argument( op2 )
    Data returnValue;
    int value1 = 0, value2 = 0;
    
    value1 = atoi( mVal.c_str() );
    value2 = atoi( op2.mVal.c_str() );
    value1 = value1 % value2;
  
    returnValue.mType = INT_TYPE;
    returnValue.mVal = IntToStr( (int)value1 ); 

    return returnValue;
  } // Div()

}; // Data

class CallStack {
public:
  stack< map<string, Data> >* mCallStack;
  
  CallStack() {
    mCallStack = new stack< map<string, Data> >();
    map<string, Data> m;
    Data d;
    m["Done"] = d;
    m["cin"] = d;
    m["cout"] = d;
    m["ListAllVariables"] = d;
    m["ListAllFunctions"] = d;
    m["ListVariable"] = d;
    m["ListFunction"] = d;
    // cout << "stack size: " << mCallStack.size();
    mCallStack->push( m );
  } // CallStack()

  ~CallStack() {
    delete mCallStack;
  } // ~CallStack()

  bool IsDefined( string token ) {
    if ( !mCallStack->empty() ) {
      map<string, Data> m = mCallStack->top() ;
      mCallStack->pop();
      bool success = false;
      
      if ( m.find( token ) == m.end() ) {
        success = IsDefined( token );
        
      } // if
      else {
        success = true;
      } // else

      mCallStack->push( m );
      return success;
    } // if
    
    return false;
  } // IsDefined()

  void Get( string name, Data& data ) {
    if ( !mCallStack->empty() ) {
      map<string, Data> m = mCallStack->top() ;
      mCallStack->pop();
      if ( m.find( name ) != m.end() ) {
        data = m[name];
      } // if
      else {
        Get( name, data );
      } // else

      mCallStack->push( m );
    } // if
  } // Get()

  void Set( string name, Data& data ) {
    if ( !mCallStack->empty() ) {
      map<string, Data> m = mCallStack->top() ;
      mCallStack->pop();
      if ( m.find( name ) != m.end() ) {
        m[name].mVal = data.mVal;
      } // if
      else {
        Set( name, data );
      } // else

      mCallStack->push( m );
    } // if
  } // Set()

  void NewID( string token, vector<Token>* tokenStr ) {
    map<string, Data> m = mCallStack->top();
    mCallStack->pop();

    Data d;
    d.mIsVar = true;
    // d.mType = tokenStr->at( 0 ).mValue;
    if ( tokenStr->at( 0 ).mValue == "int" ) {
      d.mType = INT_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "float" ) {
      d.mType = FLOAT_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "bool" ) {
      d.mType = BOOL_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "string" ) {
      d.mType = STRING_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "char" ) {
      d.mType = CHAR_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "void" ) {
      d.mType = VOID_TYPE;
    } // if

    int i = 0;
    while ( i < tokenStr->size() ) {
      if ( tokenStr->at( i ).mValue == token ) {
        i++;
        while ( i < tokenStr->size() && tokenStr->at( i ).mValue != "," 
                && tokenStr->at( i ).mValue != ";" ) {
          d.mBody.push_back( tokenStr->at( i ).mValue );
          i++;
        } // while

        i = tokenStr->size();
      } // if

      i++;
    } // while

    m[token] = d;

    mCallStack->push( m );
  } // NewID()

  void NewFunc( string token ) {
    map<string, Data> m = mCallStack->top();
    mCallStack->pop();
    Data d;
    
    d.mIsFunc = true;

    m[token] = d;
    mCallStack->push( m );
  } // NewFunc()
  
  void NewFunc( string name, vector<Token>* tokenStr ) {
    map<string, Data> m = mCallStack->top();
    mCallStack->pop();
    Data d;
    
    if ( tokenStr->at( 0 ).mValue == "int" ) {
      d.mType = INT_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "float" ) {
      d.mType = FLOAT_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "bool" ) {
      d.mType = BOOL_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "string" ) {
      d.mType = STRING_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "char" ) {
      d.mType = CHAR_TYPE;
    } // if
    else if ( tokenStr->at( 0 ).mValue == "void" ) {
      d.mType = VOID_TYPE;
    } // if

    d.mIsFunc = true;
    // get function body
    for ( int i = 2 ; i < tokenStr->size() ; i++ ) {
      d.mBody.push_back( tokenStr->at( i ).mValue );
    } // for

    m[name] = d;
    mCallStack->push( m );
  } // NewFunc()

  void NewRecord() {
    map<string, Data> m;
    mCallStack->push( m );
  } // NewRecord()

  void PopRecord() {
    if ( !mCallStack->empty() ) {
      // map<string, Data> m = mCallStack.top();
      mCallStack->pop();
    } // if
  } // PopRecord()

  void PopExceptBase() {
    /*
    if ( gTestNum == 2 ) {
      cout << mCallStack->size();
    } // if
    */
    while ( mCallStack->size() != 1 ) {
      PopRecord();
    } // while
  } // PopExceptBase()

  void ListAllVariables() {
    if ( !mCallStack->empty() ) {
      map<string, Data> m = mCallStack->top() ;
      mCallStack->pop();
      for ( map<string, Data>::iterator it = m.begin() ; it != m.end() ; it++ ) {
        if ( it->second.mIsVar ) {
          cout << it->first << endl;
        } // if
      } // for

      ListAllVariables();
      mCallStack->push( m );
    } // if
  } // ListAllVariables()

  void ListAllFunctions() {
    if ( !mCallStack->empty() ) {
      map<string, Data> m = mCallStack->top() ;
      mCallStack->pop();
      for ( map<string, Data>::iterator it = m.begin() ; it != m.end() ; it++ ) {
        if ( it->second.mIsFunc ) {
          cout << it->first << "()\n";
        } // if
      } // for

      ListAllFunctions();
      mCallStack->push( m );
    } // if
  } // ListAllFunctions()

  void ListVariable( string name ) {
    if ( IsDefined( name ) ) {
      Data d;
      Get( name, d );
      cout << d.mType << " " << name;
      int indentNum = 0;
      for ( int i = 0 ; i < d.mBody.size() ; i++ ) {
        if ( d.mBody.at( i ) != "[" && d.mBody.at( i ) != "(" ) {
          cout << " ";
        } // if
        
        cout << d.mBody.at( i );
      } // for

      cout << " ;\n";
    } // if
  } // ListVariable()

  void ListFunction( string name ) {
    if ( IsDefined( name ) ) {
      Data d;
      Get( name, d );
      cout << d.mType << " " << name;
      int indentNum = 0;
      bool newLine = false;
      for ( int i = 0 ; i < d.mBody.size() ; i++ ) {
        if ( d.mBody.at( i ) == "}" ) {
          indentNum--;
        } // if

        if ( newLine ) {
          for ( int j = 0 ; j < indentNum ; j++ ) {
            cout << "  ";
          } // for

          newLine = false;
        } // if
        else if ( d.mBody.at( i ) != "[" && d.mBody.at( i ) != "(" 
                  && d.mBody.at( i ) != "++" && d.mBody.at( i ) != "--" ) {
          cout << " ";
        } // if
        
        cout << d.mBody.at( i );

        if ( d.mBody.at( i ) == "while" || d.mBody.at( i ) == "if" ) {
          cout << " ";
        } // if
        else if ( d.mBody.at( i ) == "{" ) {
          cout << endl;
          indentNum++;
          newLine = true;
        } // if
        else if ( d.mBody.at( i ) == ";" || d.mBody.at( i ) == "}" ) {
          cout << endl;
          newLine = true;
        } // if

      } // for
    } // if
  } // ListFunction()

}; // CallStack

CallStack* gCallStack;

class Evaler {
  // eval the token string
public:
  vector<Token> mTokenStr;
  int i;
  
  Evaler() {
    i = 0;
  } // Evaler

  void Statement() {
    i = 0;
    Data value;
    if ( Expression( value ) ) {
      
    } // if
    // else if if, while ...
  } // Statement()

  bool Expression( Data& value ) {
    // basic_expression { ',' basic_expression }
    if ( !Basic_expression( value ) ) {
      return false;
    } // if

    /*
    while ( mTokenStr[i].mValue == "," ) {
      i++;
      Basic_expression();

    } // while
    */
    return true;
  } // Expression()

  bool Basic_expression( Data& value ) {
    // value only out?
    string idStr = "", signStr = "";
    
    if ( mTokenStr[i].mType == ID ) {
      // Identifier rest_of_Identifier_started_basic_exp
      idStr = mTokenStr[i].mValue;
      i++;
      
      if ( mTokenStr[i].mValue == "[" ) {
        // [ '[' expression ']' ]
        i++;
        Data exprVal2;
        Expression( exprVal2 );

        // if ( token.mValue != "]" ) {
        i++;
      } // if

      if ( idStr == "cout" ) {
        while ( mTokenStr[i].mType == LS || mTokenStr[i].mType == RS ) {
          i++;
          Maybe_additive_exp( value ) ;
          while ( value.mVal.find( "\\n" ) != string::npos ) {
            value.mVal.replace( value.mVal.find( "\\n" ), 2, "\n" );
          } // while
          cout << value.mVal;
        } // while
        
        return true;
      } // if
      else if ( mTokenStr[i].mValue == "=" || mTokenStr[i].mType == TE || mTokenStr[i].mType == DE
          || mTokenStr[i].mType == RE || mTokenStr[i].mType == PE || mTokenStr[i].mType == ME ) {
        // assignment_operator
        Token assignOp = mTokenStr[i];
        i++;

        Basic_expression( value );
        
        Data idVal;
        gCallStack->Get( idStr, idVal );
        if ( assignOp.mType == TE ) {
          // *=
          value = idVal.Mul( value );
        } // if
        else if ( assignOp.mType == DE ) {
          // /=
          value = idVal.Div( value );
        } // if
        else if ( assignOp.mType == RE ) {
          // %=
          value = idVal.Mod( value );
        } // if
        else if ( assignOp.mType == PE ) {
          // +=
          value = idVal.Plus( value );
        } // if
        else if ( assignOp.mType == ME ) {
          // /=
          value = idVal.Minus( value );
        } // if

        gCallStack->Set( idStr, value );

        return true;
      } // if
      else {
        gCallStack->Get( idStr, value );
        Data tmpD;
        tmpD.SetByConstantToken( "1" );
        if ( mTokenStr[i].mType == PP ) {
          value = value.Plus( tmpD ); 
          gCallStack->Set( idStr, value );
          i++;
        } // if
        else if ( mTokenStr[i].mType == MM ) {
          value = value.Minus( tmpD );
          gCallStack->Set( idStr, value );
          i++;
        } // if

        Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( value );
      } // else

      return true;
    } // if ( mTokenStr[i].mType == ID )
    else if ( mTokenStr[i].mType == PP || mTokenStr[i].mType == MM ) {
      i++;
      if ( mTokenStr[i].mType == ID ) {
        idStr = mTokenStr[i].mValue;
        i++;

        gCallStack->Get( idStr, value );
        Data tmpD;
        tmpD.SetByConstantToken( "1" );
        if ( mTokenStr[i-2].mType == PP ) {
          value = value.Plus( tmpD );
          gCallStack->Set( idStr, value );
        } // if
        else {
          value = value.Minus( tmpD );
          gCallStack->Set( idStr, value );
        } // else
        
        // [ '[' expression ']' ] romce_and_romloe
        Data exprVal2;
        if ( mTokenStr[i].mValue == "[" ) {
          i++;

          Expression( exprVal2 );

          i++; // if ( token.mValue == "]" ) {

          return true;
        } // if

        Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( value ) ;
          
        return true;
      } // if


    } // if
    else if ( Sign( signStr ) ) {
      while ( Sign( signStr ) ) {

      } // while

      if ( Signed_unary_exp( value )  ) {

      } // if
      else if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( value ) ) {

      } // if

      if ( signStr == "!" ) {
        if ( value.mVal == "true" ) {
          value.mVal = "false";
        } // if
        else {
          value.mVal = "true";
        } // else
      } // if
      else if ( signStr == "-" ) {
        Data tmpD;
        tmpD.SetByConstantToken( "-1" );
        value = value.Mul( tmpD ); 
      } // if

      return true;
    } // if
    else if ( mTokenStr[i].mType == CONSTANT || mTokenStr[i].mValue == "(" ) {
      // ( Constant | '(' expression ')' ) romce_and_romloe
      if ( mTokenStr[i].mValue == "(" ) {
        i++;

        Expression( value );

        // if ( tokenStr[i].mValue == ")" ) {
        i++;

      } // if ( token.mValue == "(" )
      else {
        value.SetByConstantToken( mTokenStr[i].mValue );
        i++;
      } // else

      Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( value ) ;
      return true;
    } // else if

    return false;
  } // Basic_expression()

  bool Sign( string& signStr ) {
    if ( mTokenStr[i].mValue == "+" || mTokenStr[i].mValue == "-" || mTokenStr[i].mValue == "!" ) {
      if (  mTokenStr[i].mValue == "-" ) {
        if ( signStr == "-" ) {
          signStr = "+";
        } // if
        else {
          signStr = "-";
        } // else
      } // if
      else if ( signStr == "!" && mTokenStr[i].mValue == "!" ) {
        signStr = "";
      } // if
      else if ( signStr == "" ) {
        signStr = mTokenStr[i].mValue;
      } // else

      i++;
      return true;
    } // if
    
    return false;
  } // Sign()

  

  bool Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( Data& value ) {
    //  rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
    if ( !Rest_of_maybe_logical_OR_exp( value ) ) {
      return false;
    } // if

    Data bExprVal1, bExprVal2;
    if ( mTokenStr[i].mValue == "?" ) {
      i++;

      if ( Basic_expression( bExprVal1 ) ) {

        if ( mTokenStr[i].mValue == ":" ) {
          i++;

          if ( Basic_expression( bExprVal2 ) ) {
            return true;
          } // if
        } // if
      } // if
    } // if ( token.mValue == "?" )

    return true;
  } // Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp()

  bool Rest_of_maybe_logical_OR_exp( Data& value ) {
    // rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
    if ( !Rest_of_maybe_logical_AND_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == OR ) {
      i++;

      Maybe_logical_AND_exp( value2 );
        

    } // while

    return true;
  } // Rest_of_maybe_logical_OR_exp()

  bool Maybe_logical_AND_exp( Data& value ) {
    // maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    if ( !Maybe_bit_OR_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == AND ) {
      i++;
      
      Maybe_bit_OR_exp( value2 );
    } // while

    return true;
  } // Maybe_logical_AND_exp()

  bool Rest_of_maybe_logical_AND_exp( Data& value ) {
    if ( !Rest_of_maybe_bit_OR_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == AND ) {
      i++;

      Maybe_bit_OR_exp( value2 ) ;
    } // while

    return true;
  } // Rest_of_maybe_logical_AND_exp()

  bool Maybe_bit_OR_exp( Data& value ) {
    if ( !Maybe_bit_ex_OR_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mValue == "|" ) {
      i++;

      Maybe_bit_ex_OR_exp( value2 ) ;
    } // while

    return true;
  } // Maybe_bit_OR_exp()

  bool Rest_of_maybe_bit_OR_exp( Data& value ) {
    if ( !Rest_of_maybe_bit_ex_OR_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mValue == "|" ) {
      i++;

     Maybe_bit_ex_OR_exp( value2 );

    } // while

    return true;
  } // Rest_of_maybe_bit_OR_exp()

  bool Maybe_bit_ex_OR_exp( Data& value ) {
    if ( !Maybe_bit_AND_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mValue == "^" ) {
      i++;

      Maybe_bit_AND_exp( value2 );

    } // while

    return true;
  } // Maybe_bit_ex_OR_exp()

  bool Rest_of_maybe_bit_ex_OR_exp( Data& value ) {
    if ( !Rest_of_maybe_bit_AND_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mValue == "^" ) {
      i++;

      Maybe_bit_AND_exp( value2 );

    } // while

    return true;
  } // Rest_of_maybe_bit_ex_OR_exp()

  bool Maybe_bit_AND_exp( Data& value ) {
    if ( !Maybe_equality_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mValue == "&" ) {
      i++;

      Maybe_equality_exp( value2 ) ;

   
    } // while

    return true;
  } // Maybe_bit_AND_exp()

  bool Rest_of_maybe_bit_AND_exp( Data& value ) {
    if ( !Rest_of_maybe_equality_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mValue == "&" ) {
      i++;

      Maybe_equality_exp( value2 ) ;
  

    } // while

    return true;
  } // Rest_of_maybe_bit_AND_exp()

  bool Maybe_equality_exp( Data& value ) {
    if ( !Maybe_relational_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == EQ || mTokenStr[i].mType == NEQ ) {
      i++;
      Maybe_relational_exp( value2 ) ;

    
    } // while

    return true;
  } // Maybe_equality_exp()

  bool Rest_of_maybe_equality_exp( Data& value ) {
    if ( !Rest_of_maybe_relational_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == EQ || mTokenStr[i].mType == NEQ ) {
      i++;

      Maybe_relational_exp( value2 ) ;

   
    } // while

    return true;
  } // Rest_of_maybe_equality_exp()

  bool Maybe_relational_exp( Data& value ) {
    if ( !Maybe_shift_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == LE || mTokenStr[i].mType == GE
            || mTokenStr[i].mValue == "<" || mTokenStr[i].mValue == ">" ) {
      i++;

      Maybe_shift_exp( value2 ) ;

     
    } // while

    return true;
  } // Maybe_relational_exp()

  bool Rest_of_maybe_relational_exp( Data& value ) {
    if ( !Rest_of_maybe_shift_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == LE || mTokenStr[i].mType == GE
            || mTokenStr[i].mValue == "<" || mTokenStr[i].mValue == ">" ) {
      i++;

      Maybe_shift_exp( value2 ) ;

    
    } // while

    return true;
  } // Rest_of_maybe_relational_exp()

  bool Maybe_shift_exp( Data& value ) {
    if ( !Maybe_additive_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == LS || mTokenStr[i].mType == RS ) {
      if (  mTokenStr[i].mType == LS ) {
        // <<
        i++;
        Maybe_additive_exp( value2 );
        value.mVal = IntToStr( atoi( value.mVal.c_str() ) << atoi( value2.mVal.c_str() ) );
      } // if
      else {
        // >>
        i++;
        Maybe_additive_exp( value2 );
        value.mVal = IntToStr( atoi( value.mVal.c_str() ) >> atoi( value2.mVal.c_str() ) );
      } // else
      
    } // while

    return true;
  } // Maybe_shift_exp()

  bool Rest_of_maybe_shift_exp( Data& value ) {
    if ( !Rest_of_maybe_additive_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mType == LS || mTokenStr[i].mType == RS ) {
      if (  mTokenStr[i].mType == LS ) {
        // <<
        i++;
        Maybe_additive_exp( value2 );
        value.mVal = IntToStr( atoi( value.mVal.c_str() ) << atoi( value2.mVal.c_str() ) );
      } // if
      else {
        // >>
        i++;
        Maybe_additive_exp( value2 );
        value.mVal = IntToStr( atoi( value.mVal.c_str() ) >> atoi( value2.mVal.c_str() ) );
      } // else
    } // while

    return true;
  } // Rest_of_maybe_shift_exp()

  bool Maybe_additive_exp( Data& value ) {
    if ( !Maybe_mult_exp( value ) ) {
      return false;
    } // if

    Data value2;
    while ( mTokenStr[i].mValue == "+" || mTokenStr[i].mValue == "-" ) {
      if ( mTokenStr[i].mValue == "+" ) {
        i++;
        Maybe_mult_exp( value2 );
        value = value.Plus( value2 );
      } // if
      else {
        i++;
        Maybe_mult_exp( value2 );
        value = value.Minus( value2 );
      } // else
    } // while

    return true;
  } // Maybe_additive_exp()

  bool Rest_of_maybe_additive_exp( Data& value ) {
    if ( !Rest_of_maybe_mult_exp( value ) ) {
      return false;
    } // if

    Data value2;
    string optr = "";
    while ( mTokenStr[i].mValue == "+" || mTokenStr[i].mValue == "-" ) {
      optr = mTokenStr[i].mValue;
      i++;
      
      Maybe_mult_exp( value2 );

      if ( optr == "+" ) {
        value = value.Plus( value2 );
      } // if
      else {
        value = value.Minus( value2 );
      } // if      
    } // while

    return true;
  } // Rest_of_maybe_additive_exp()

  bool Maybe_mult_exp( Data& value ) {
    if ( Unary_exp( value ) ) {
      Rest_of_maybe_mult_exp( value ); 
      return true;
    } // if

    return false;
  } // Maybe_mult_exp()

  bool Rest_of_maybe_mult_exp( Data& value ) {
    string operatorToken = "";
    Data value2;

    while ( mTokenStr[i].mValue == "*" || mTokenStr[i].mValue == "/"
            || mTokenStr[i].mValue == "%" ) {
      operatorToken = mTokenStr[i].mValue;
      i++;

      Unary_exp( value2 ) ;

      // value must be numeric?
      if ( operatorToken == "*" ) {
        value = value.Mul( value2 );
      } // if
      else if ( operatorToken == "/" ) {
        value = value.Div( value2 );
      } // if
      else if ( operatorToken == "%" ) {
        value = value.Mod( value2 );
      } // if
    } // while

    return true;
  } // Rest_of_maybe_mult_exp()

  bool Unary_exp( Data& value ) {
    /*
    sign { sign } signed_unary_exp
    | unsigned_unary_exp
    | ( PP | MM ) Identifier [ '[' expression ']' ]
    */
    string signStr = "";

    if ( Sign( signStr ) ) {
      while ( Sign( signStr ) ) {

      } // while

      Signed_unary_exp( value ) ;

      if ( signStr == "!" ) {
        if ( value.mVal == "true" ) {
          value.mVal = "false";
        } // if
        else {
          value.mVal = "true";
        } // else
      } // if
      else if ( signStr == "-" ) {
        Data tmpD;
        tmpD.SetByConstantToken( "-1" );
        value = value.Mul( tmpD ); 
      } // if
      
      return true;
    } // if
    else if ( Unsigned_unary_exp( value ) ) {
      return true;
    } // if
    else if ( mTokenStr[i].mType == PP || mTokenStr[i].mType == MM ) {
      Token tmp = mTokenStr[i];
      i++;
      if ( mTokenStr[i].mType == ID ) {
        string idStr = mTokenStr[i].mValue;
        i++;
        if ( mTokenStr[i].mValue == "[" ) {
          i++;
          Data exprVal2;
          if ( Expression( exprVal2 ) ) {
            
            if ( mTokenStr[i].mValue == "]" ) {
              i++;
              return true;
            } // if
          } // if

        } // if ( mTokenStr[i].mValue == "[" )
        else {
          gCallStack->Get( idStr, value );
          Data tmpD;
          tmpD.SetByConstantToken( "1" );
          if ( tmp.mType == PP ) {
            value = value.Plus( tmpD );
            gCallStack->Set( idStr, value );
          } // if
          else {
            value = value.Minus( tmpD );
            gCallStack->Set( idStr, value );
          } // else

          return true;
        } // else
      } // if

    } // if

    return false;
  } // Unary_exp()

  bool Signed_unary_exp( Data& value ) {
    /*
      Identifier [ '(' [ actual_parameter_list ] ')'
                    |
                    '[' expression ']'
                  ]
      | Constant
      | '(' expression ')'
    */
    Data exprVal;
    if ( mTokenStr[i].mType == ID ) {
      string idStr = mTokenStr[i].mValue;
      i++;
      if ( mTokenStr[i].mValue == "[" ) {
        i++;
        Expression( exprVal ) ;   
        i++; // if ( mTokenStr[i].mValue == "]" )
      } // if
      
      gCallStack->Get( idStr, value );
      return true;
    } // if ( mTokenStr[i].mType == ID )
    else if ( mTokenStr[i].mType == CONSTANT ) {
      value.SetByConstantToken( mTokenStr[i].mValue );
      i++;
      return true;
    } // if
    else if ( mTokenStr[i].mValue == "(" ) {
      i++;

      if ( Expression( value ) ) {

        if ( mTokenStr[i].mValue == ")" ) {
          i++;
          return true;
        } // if
      } // if


    } // else if

    return false;
  } // Signed_unary_exp()

  bool Unsigned_unary_exp( Data& value ) {
    /*    
    Identifier [ '(' [ actual_parameter_list ] ')'
                 |
                 [ '[' expression ']' ] [ ( PP | MM ) ]
               ]
    | Constant
    | '(' expression ')'
    */
    Data exprVal2;
    if ( mTokenStr[i].mType == ID ) {
      string idStr = "";
      idStr = mTokenStr[i].mValue;
      i++;

      if ( mTokenStr[i].mValue == "[" ) {
        i++;

        Expression( exprVal2 ) ;

        i++; // if ( token.mValue != "]" ) 

      } // if ( token.mValue == "[" )

      gCallStack->Get( idStr, value );
      if ( mTokenStr[i].mType == PP || mTokenStr[i].mType == MM ) {
        Data tmpD;
        tmpD.SetByConstantToken( "1" );
        if ( mTokenStr[i].mType == PP ) {
          value = value.Plus( tmpD ); 
          gCallStack->Set( idStr, value );
          i++;
        } // if
        else if ( mTokenStr[i].mType == MM ) {
          value = value.Minus( tmpD );
          gCallStack->Set( idStr, value );
          i++;
        } // if
        
      } // if 
    
      return true;
    } // if
    else if ( mTokenStr[i].mType == CONSTANT ) {
      value.SetByConstantToken( mTokenStr[i].mValue );
      i++;
    
      return true;
    } // else if
    else if ( mTokenStr[i].mValue == "(" ) {
      i++;
      
      if ( Expression( value ) ) {
       
        if ( mTokenStr[i].mValue == ")" ) {
          i++;
          return true;
        } // if
      } // if 
    } // else if

    return false;
  } // Unsigned_unary_exp()
  
}; // class Evaler

class Parser {
public:
  TokenScanner mScanner;
  vector<Token>* mTokenString;
  int mInstructionNum;
  Evaler evaler;

  Parser() {
    mTokenString = new vector<Token>();
    mInstructionNum = 0;
    
  } // Parser()

  ~Parser() {
    delete mTokenString;
    mTokenString = NULL;
  } // ~Parser()

  void User_Input() {
    Token token;
    mTokenString->clear();
    mScanner.Init();
    mInstructionNum++;
    /*
    if ( gTestNum == 2 && mInstructionNum > 2 ) {
      ErrorMsg errorMsg( mScanner.mLine, QUIT_ERROR, "" );
      throw errorMsg;
    } // if
    */
    cout << "> ";
    if ( Definition() ) {

    } // if
    else if ( Statement() ) {
      evaler.mTokenStr = *mTokenString;
      evaler.Statement();
      cout << "Statement executed ...\n";
    } // if
    else {
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // else

    mScanner.CleanInputAfterCMD();

    mInstructionNum++;
    /*
    if ( gTestNum == 2 && mInstructionNum > 2 ) {
      ErrorMsg errorMsg( mScanner.mLine, QUIT_ERROR, "" );
      throw errorMsg;
    } // if
    */
    bool keepRun = true;
    while ( keepRun ) {
      mTokenString->clear();
      mScanner.Init();
      cout << "> ";
      if ( Definition() ) {
        mScanner.CleanInputAfterCMD();
      } // if
      else if ( Statement() ) {
        evaler.mTokenStr = *mTokenString;
        evaler.Statement();
        cout << "Statement executed ...\n";
        mScanner.CleanInputAfterCMD();
      } // if
      else {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // else

    } // while
  } // User_Input()

  bool Definition() {
    Token token, idToken;
    bool success = false;

    mScanner.PeekToken( token );
    if ( token.mType == VOID ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        idToken = token;
        
        if ( Function_Definition_Without_ID(  ) ) {
          if ( gCallStack->IsDefined( idToken.mValue ) ) {
            cout << "New definition of ";
          } // if
          else {
            cout << "Definition of ";
          } // else

          cout << idToken.mValue << "() entered ...\n";
          gCallStack->NewFunc( idToken.mValue, mTokenString );
          return true;
        } // if
      } // if
       
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if
    else if ( Type_specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        idToken = token;
        queue<string> idQueue;
        idQueue.push( idToken.mValue );
        if ( Function_Definition_or_Declarators( idQueue ) ) {
          return true;
        } // if
      } // if

      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // else if

    return false;
  } // Definition()

  bool Type_specifier() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mType == INT || token.mType == FLOAT || token.mType == CHAR
         || token.mType == STRING || token.mType == BOOL ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      return true;
    } // if

    return false;
  } // Type_specifier()

  bool Function_Definition_or_Declarators( queue<string>& idQueue ) {
    if ( Function_Definition_Without_ID() ) {
      // store func
      if ( gCallStack->IsDefined( idQueue.front() ) ) {
        cout << "New definition of ";
      } // if
      else {
        cout << "Definition of ";
      } // else

      cout << idQueue.front() << "() entered ...\n";
      gCallStack->NewFunc( idQueue.front(), mTokenString );
      idQueue.pop();
      return true;
    } // if
    else if ( Rest_of_Declarators( idQueue ) ) {
      while ( !idQueue.empty() ) {            
        if ( gCallStack->IsDefined( idQueue.front() ) ) {
          cout << "New definition of ";
        } // if
        else {
          cout << "Definition of ";
        } // else

        cout << idQueue.front() << " entered ...\n";
        gCallStack->NewID( idQueue.front(), mTokenString );
        idQueue.pop();
      } // while

      return true;
    } // if

    return false;
  } // Function_Definition_or_Declarators()

  bool Rest_of_Declarators( queue<string>& idQueue ) {
    Token token;
    bool error = false;

    // [ '[' Constant ']' ]
    mScanner.PeekToken( token );
    if ( token.mValue == "[" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      mScanner.PeekToken( token );
      if ( token.mType == CONSTANT ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        mScanner.PeekToken( token );
        if ( token.mValue == "]" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
        } // if
        else {
          error = true;
        } // else
      } // if
      else {
        error = true;
      } // else
    } // if

    if ( error ) {
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if

    // { ',' Identifier [ '[' Constant ']' ] }
    mScanner.PeekToken( token );
    while ( token.mValue == "," ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      mScanner.PeekToken( token );
      if ( token.mType != ID ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.GetToken( token );
      mTokenString->push_back( token );
      idQueue.push( token.mValue );

      // [ '[' Constant ']' ]
      mScanner.PeekToken( token );
      if ( token.mValue == "[" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        mScanner.PeekToken( token );
        if ( token.mType == CONSTANT ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );

          mScanner.PeekToken( token );
          if ( token.mValue == "]" ) {
            mScanner.GetToken( token );
            mTokenString->push_back( token );
          } // if
          else {
            error = true;
          } // else
        } // if
        else {
          error = true;
        } // else
      } // if

      if ( error ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while ( token.mValue == "," )

    mScanner.PeekToken( token );
    if ( token.mValue == ";" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      return true;
    } // if

    return false;
  } // Rest_of_Declarators()

  bool Function_Definition_Without_ID() {
    // '(' [ VOID | formal_parameter_list ] ')' compound_statement
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "(" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      gCallStack->NewRecord();

      mScanner.PeekToken( token );
      if ( token.mType == VOID ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
      } // if
      else if ( Formal_parameter_list() ) {

      } // if

      mScanner.GetToken( token );
      mTokenString->push_back( token );
      if ( token.mValue != ")" ) {
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      if ( !Compound_statement() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      gCallStack->PopRecord();

      return true;
    } // if ( token.mValue == "(" )

    return false;
  } // Function_Definition_Without_ID()

  bool Formal_parameter_list() {
    Token token, idToken;
    bool matchFirstToken = false, success = false;
    if ( Type_specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mValue == "&" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
      } // if

      mScanner.GetToken( token );
      mTokenString->push_back( token );
      if ( token.mType != ID ) {
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      idToken = token;

      mScanner.PeekToken( token );
      if ( token.mValue == "[" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( token.mType != CONSTANT ) {
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if

        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( token.mValue != "]" ) {
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if

      } // if

      gCallStack->NewID( idToken.mValue, mTokenString );

      mScanner.PeekToken( token );
      while ( token.mValue == "," ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( !Type_specifier() ) {
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if

        mScanner.PeekToken( token );
        if ( token.mValue == "&" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
        } // if

        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( token.mType != ID ) {
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if ( token.mType != ID ) 

        idToken = token;

        mScanner.PeekToken( token );
        if ( token.mValue == "[" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
          
          mScanner.GetToken( token );
          mTokenString->push_back( token );
          if ( token.mType != CONSTANT ) {
            ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
            throw errorMsg;
          } // if

          mScanner.GetToken( token );
          mTokenString->push_back( token );
          if ( token.mValue != "]" ) {
            ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
            throw errorMsg;
          } // if
        } // if

        gCallStack->NewID( idToken.mValue, mTokenString );

        mScanner.PeekToken( token );
      } // while

      return true;
    } // if

    return false;
  } // Formal_parameter_list()

  bool Compound_statement() {
    // '{' { declaration | statement } '}'
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "{" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      
      while ( Declaration() || Statement() ) {
       
      } // while

      mScanner.GetToken( token );
      mTokenString->push_back( token );
      if ( token.mValue != "}" ) {
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      return true;
    } // if

    return false;
  } // Compound_statement()

  bool Declaration() {
    Token token;
    // type_specifier Identifier rest_of_declarators
    if ( Type_specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        queue<string> idQueue;
        idQueue.push( token.mValue );
        if ( Rest_of_Declarators( idQueue ) ) {
          while ( !idQueue.empty() ) {
            gCallStack->NewID( idQueue.front(), mTokenString );
            idQueue.pop();
          } // while
          
          return true;
        } // if
      } // if ( token.mType == ID )

      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if ( Type_specifier() )

    return false;
  } // Declaration()

  bool Statement() {
    Token token;
    bool success = false;
    bool firstTokenMatch = false;
    Data value ;

    mScanner.PeekToken( token );
    if ( token.mValue == ";" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      firstTokenMatch = true;
      success = true;
    } // if
    else if ( Expression(  ) ) {
      firstTokenMatch = true;
      mScanner.PeekToken( token );
      if ( token.mValue == ";" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        success = true;
      } // if
    } // else if
    else if ( token.mType == RETURN ) {
      // RETURN [ expression ] ';'
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      firstTokenMatch = true;

      Expression(  );
      
      mScanner.PeekToken( token );
      if ( token.mValue == ";" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        success = true;
      } // if
    } // if
    else if ( token.mType == IF ) {
      // IF '(' expression ')' statement [ ELSE statement ]
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      firstTokenMatch = true;

      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        if ( Expression(  ) ) {
          mScanner.PeekToken( token );
          if ( token.mValue == ")" ) {
            mScanner.GetToken( token );
            mTokenString->push_back( token );

            if ( Statement() ) {
              mScanner.PeekToken( token );
              if ( token.mType == ELSE ) {
                mScanner.GetToken( token );
                mTokenString->push_back( token );

                if ( !Statement() ) {
                  mScanner.GetToken( token );
                  ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
                  throw errorMsg;
                } // if
              } // if ( token.mType == ELSE )
              
              success = true;
            } // if ( Statement() )
          } // if
        } // if
      } // if
    } // if
    else if ( token.mType == WHILE ) {
      // WHILE '(' expression ')' statement
      firstTokenMatch = true;
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        if ( Expression(  ) ) {
          mScanner.PeekToken( token );
          if ( token.mValue == ")" ) {
            mScanner.GetToken( token );
            mTokenString->push_back( token );

            if ( Statement() ) {
              success = true;
            } // if
          } // if
        } // if
      } // if
    } // if
    else if ( token.mType == DO ) {
      // DO statement WHILE '(' expression ')' ';'
      firstTokenMatch = true;
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( Statement() ) {
        mScanner.PeekToken( token );
        if ( token.mType == WHILE ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );

          mScanner.PeekToken( token );
          if ( token.mValue == "(" ) {
            mScanner.GetToken( token );
            mTokenString->push_back( token );

            if ( Expression(  ) ) {
              mScanner.PeekToken( token );
              if ( token.mValue == ")" ) {
                mScanner.GetToken( token );
                mTokenString->push_back( token );

                mScanner.PeekToken( token );
                if ( token.mValue == ";" ) {
                  mScanner.GetToken( token );
                  mTokenString->push_back( token );
                  success = true;
                } // if
              } // if
            } // if
          } // if
        } // if
      } // if
    } // if
    else {
      gCallStack->NewRecord();
      if ( Compound_statement() ) {
        firstTokenMatch = true;
        success = true;
      } // if

      gCallStack->PopRecord();
    } // else

    if ( firstTokenMatch && !success ) {
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if

    if ( mTokenString->size() == 4 && mTokenString->at( 0 ).mValue == "Done"
         && mTokenString->at( 1 ).mValue == "(" && mTokenString->at( 2 ).mValue == ")"
         && mTokenString->at( 3 ).mValue == ";" ) {
      ErrorMsg errorMsg( mScanner.mLine, QUIT_ERROR, "" );
      throw errorMsg;
    } // if
    else if ( mTokenString->size() == 4 && mTokenString->at( 0 ).mValue == "ListAllFunctions"
              && mTokenString->at( 1 ).mValue == "(" && mTokenString->at( 2 ).mValue == ")"
              && mTokenString->at( 3 ).mValue == ";" ) {
      gCallStack->ListAllFunctions();
    } // if
    else if ( mTokenString->size() == 4 && mTokenString->at( 0 ).mValue == "ListAllVariables"
              && mTokenString->at( 1 ).mValue == "(" && mTokenString->at( 2 ).mValue == ")"
              && mTokenString->at( 3 ).mValue == ";" ) {
      gCallStack->ListAllVariables();
    } // if
    else if ( mTokenString->size() == 5 && mTokenString->at( 0 ).mValue == "ListFunction"
              && mTokenString->at( 1 ).mValue == "(" && mTokenString->at( 2 ).mType == CONSTANT
              && mTokenString->at( 3 ).mValue == ")" && mTokenString->at( 4 ).mValue == ";" ) {
      string name = mTokenString->at( 2 ).mValue;
      name = name.substr( 1, name.size() - 2 );
      gCallStack->ListFunction( name );
    } // if
    else if ( mTokenString->size() == 5 && mTokenString->at( 0 ).mValue == "ListVariable"
              && mTokenString->at( 1 ).mValue == "(" && mTokenString->at( 2 ).mType == CONSTANT
              && mTokenString->at( 3 ).mValue == ")" && mTokenString->at( 4 ).mValue == ";" ) {
      string name = mTokenString->at( 2 ).mValue;
      name = name.substr( 1, name.size() - 2 );
      gCallStack->ListVariable( name );
    } // if
    
    return success;
    return false;
  } // Statement()

  bool Expression(  ) {
    // basic_expression { ',' basic_expression }
    if ( !Basic_expression() ) {
      return false;  
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "," ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Basic_expression() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Expression()

  bool Basic_expression(  ) {
    Token token, idToken;
    int idLine = 0;
    Data value;

    mScanner.PeekToken( token );
    if ( token.mType == ID ) {
      // Identifier rest_of_Identifier_started_basic_exp
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      idToken = token;
      idLine = mScanner.mLine;

      // because romce_and_romloe can be empty
      // don't need to check next token?
      if ( !gCallStack->IsDefined( idToken.mValue ) ) {
        ErrorMsg errorMsg( idLine, SEMANTIC_ERROR, idToken.mValue );
        throw errorMsg;
      } // if

      if ( Rest_of_Identifier_started_basic_exp() ) {  
        
        return true;
      } // if
      else {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // else
    } // if
    else if ( token.mType == PP || token.mType == MM ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        if ( !gCallStack->IsDefined( token.mValue ) ) {
          ErrorMsg errorMsg( mScanner.mLine, SEMANTIC_ERROR, token.mValue );
          throw errorMsg;
        } // if

        if ( Rest_of_PPMM_Identifier_started_basic_exp() ) {
          return true;
        } // if
      } // if
      
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if
    else if ( Sign() ) {
      while ( Sign() ) {

      } // while

      if ( !Signed_unary_exp() || !Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      return true;
    } // if
    else if ( token.mType == CONSTANT || token.mValue == "(" ) {
      // ( Constant | '(' expression ')' ) romce_and_romloe
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( token.mValue == "(" ) {
        if ( !Expression(  ) ) {
          mScanner.GetToken( token );
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if

        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( token.mValue != ")" ) {
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if

      } // if ( token.mValue == "(" )

      if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
        return true;
      } // if
      else {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // else
    } // else if
    
    return false;
  } // Basic_expression()

  bool Rest_of_Identifier_started_basic_exp() {
    Token token;
    Data value;

    mScanner.PeekToken( token );
    if ( token.mValue == "(" ) {
      // '(' [ actual_parameter_list ] ')' romce_and_romloe
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      
      Actual_parameter_list();

      mScanner.PeekToken( token );
      if ( token.mValue == ")" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
          return true;
        } // if
      } // if

      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if ( token.mValue == "(" )
    else {
      /*
      [ '[' expression ']' ]
      ( assignment_operator basic_expression
        |
        [ PP | MM ] romce_and_romloe
      )
      */
      mScanner.PeekToken( token );
      if ( token.mValue == "[" ) {
        // [ '[' expression ']' ]
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        if ( !Expression(  ) ) {
          mScanner.GetToken( token );
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if 

        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( token.mValue != "]" ) {
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if
      } // if

      if ( Assignment_Operator( ) ) {
        if ( Basic_expression(  ) ) {
          return true;
        } // if
        else {
          mScanner.GetToken( token );
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // else
      } // if
      else {
        mScanner.PeekToken( token );
        if ( token.mType == PP || token.mType == MM ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
        } // if

        if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
          return true;
        } // if
      } // else
    } // else

    return false;
  } // Rest_of_Identifier_started_basic_exp()

  bool Rest_of_PPMM_Identifier_started_basic_exp() {
    // [ '[' expression ']' ] romce_and_romloe
    Token token;
    Data value;

    mScanner.PeekToken( token );
    if ( token.mValue == "[" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Expression( ) ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.GetToken( token );
      mTokenString->push_back( token );
      if ( token.mValue != "]" ) {
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      if ( !Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      return true;
    } // if

    if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_PPMM_Identifier_started_basic_exp()

  bool Sign() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "+" || token.mValue == "-" || token.mValue == "!" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      return true;
    } // if

    return false;
  } // Sign()

  bool Actual_parameter_list() {
    if ( !Basic_expression() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "," ) {
      // { ',' basic_expression }
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Basic_expression() ) {
        // throw error
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Actual_parameter_list()

  bool Assignment_Operator() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "=" || token.mType == TE || token.mType == DE
         || token.mType == RE || token.mType == PE || token.mType == ME ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      return true;
    } // if

    return false;
  } // Assignment_Operator()

  bool Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() {
    //  rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
    if ( !Rest_of_maybe_logical_OR_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "?" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( Basic_expression() ) {
        mScanner.PeekToken( token );
        if ( token.mValue == ":" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );

          if ( Basic_expression() ) {
            return true;
          } // if
        } // if
      } // if

      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if ( token.mValue == "?" )

    return true;
  } // Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp()

  bool Rest_of_maybe_logical_OR_exp() {
    // rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
    if ( !Rest_of_maybe_logical_AND_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == OR ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_logical_AND_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_logical_OR_exp()

  bool Maybe_logical_AND_exp() {
    // maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    if ( !Maybe_bit_OR_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == AND ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      
      if ( !Maybe_bit_OR_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_logical_AND_exp()

  bool Rest_of_maybe_logical_AND_exp() {
    if ( !Rest_of_maybe_bit_OR_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == AND ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_bit_OR_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_logical_AND_exp()

  bool Maybe_bit_OR_exp() {
    if ( !Maybe_bit_ex_OR_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "|" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_bit_ex_OR_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_bit_OR_exp()

  bool Rest_of_maybe_bit_OR_exp() {
    if ( !Rest_of_maybe_bit_ex_OR_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "|" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_bit_ex_OR_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_bit_OR_exp()

  bool Maybe_bit_ex_OR_exp() {
    if ( !Maybe_bit_AND_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "^" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_bit_AND_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_bit_ex_OR_exp()

  bool Rest_of_maybe_bit_ex_OR_exp() {
    if ( !Rest_of_maybe_bit_AND_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "^" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_bit_AND_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_bit_ex_OR_exp()

  bool Maybe_bit_AND_exp() {
    if ( !Maybe_equality_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "&" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_equality_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_bit_AND_exp()

  bool Rest_of_maybe_bit_AND_exp() {
    if ( !Rest_of_maybe_equality_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "&" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_equality_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_bit_AND_exp()

  bool Maybe_equality_exp() {
    if ( !Maybe_relational_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == EQ || token.mType == NEQ ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      if ( !Maybe_relational_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_equality_exp()

  bool Rest_of_maybe_equality_exp() {
    if ( !Rest_of_maybe_relational_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == EQ || token.mType == NEQ ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_relational_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_equality_exp()

  bool Maybe_relational_exp() {
    if ( !Maybe_shift_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == LE || token.mType == GE
            || token.mValue == "<" || token.mValue == ">" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_shift_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_relational_exp()

  bool Rest_of_maybe_relational_exp() {
    if ( !Rest_of_maybe_shift_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == LE || token.mType == GE
            || token.mValue == "<" || token.mValue == ">" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_shift_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_relational_exp()

  bool Maybe_shift_exp() {
    if ( !Maybe_additive_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == LS || token.mType == RS ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_additive_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_shift_exp()

  bool Rest_of_maybe_shift_exp() {
    if ( !Rest_of_maybe_additive_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == LS || token.mType == RS ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_additive_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_shift_exp()

  bool Maybe_additive_exp() {
    if ( !Maybe_mult_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "+" || token.mValue == "-" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      if ( !Maybe_mult_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Maybe_additive_exp()

  bool Rest_of_maybe_additive_exp() {
    if ( !Rest_of_maybe_mult_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "+" || token.mValue == "-" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Maybe_mult_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_additive_exp()

  bool Maybe_mult_exp() {
    if ( Unary_exp() ) {
      if ( Rest_of_maybe_mult_exp() ) {
        return true;
      } // if

      Token token;
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if

    return false;
  } // Maybe_mult_exp()

  bool Rest_of_maybe_mult_exp() {
    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "*" || token.mValue == "/"
            || token.mValue == "%" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( !Unary_exp() ) {
        mScanner.GetToken( token );
        ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_mult_exp()

  bool Unary_exp(  ) {
    /*
    sign { sign } signed_unary_exp
    | unsigned_unary_exp
    | ( PP | MM ) Identifier [ '[' expression ']' ]
    */
    Token token, idToken;
    bool error = false;
    Data value;

    mScanner.PeekToken( token );
    if ( Sign() ) {
      while ( Sign() ) {

      } // while

      if ( Signed_unary_exp(  ) ) {

        return true;
      } // if
      else {
        error = true;
      } // else
    } // if
    else if ( Unsigned_unary_exp() ) {
      return true;
    } // if
    else if ( token.mType == PP || token.mType == MM ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        idToken = token;
        int idLine = mScanner.mLine;

        if ( !gCallStack->IsDefined( idToken.mValue ) ) {
          ErrorMsg errorMsg( idLine, SEMANTIC_ERROR, idToken.mValue );
          throw errorMsg;
        } // if

        mScanner.PeekToken( token );
        if ( token.mValue == "[" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );

          if ( Expression(  ) ) {
            mScanner.PeekToken( token );
            if ( token.mValue == "]" ) {
              mScanner.GetToken( token );
              mTokenString->push_back( token );
              return true;
            } // if
          } // if

          // not Expression() or not "]"
          error = true;
        } // if
        else {
          return true;
        } // else
      } // if

      error = true;
    } // if

    if ( error ) {
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if
    
    return false;
  } // Unary_exp()

  bool Signed_unary_exp(  ) {
  /*
     Identifier [ '(' [ actual_parameter_list ] ')'
                  |
                  '[' expression ']'
                ]
     | Constant
     | '(' expression ')'
  */
    Token token, idToken;
    int idLine = 0;
    bool error = false;
    bool success = false;
    Data value;

    mScanner.PeekToken( token );
    if ( token.mType == ID ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      idToken = token;
      idLine = mScanner.mLine;
      if ( !gCallStack->IsDefined( idToken.mValue ) ) {
        ErrorMsg errorMsg( idLine, SEMANTIC_ERROR, idToken.mValue );
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        Actual_parameter_list();
        
        mScanner.PeekToken( token );
        if ( token.mValue == ")" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
          return true;
        } // if
      } // if
      else if ( token.mValue == "[" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( Expression(  ) ) {
          mScanner.PeekToken( token );
          if ( token.mValue == "]" ) {
            mScanner.GetToken( token );
            mTokenString->push_back( token );
            return true;
          } // if
        } // if
      } // if
      else {
        return true;
      } // else

      // if encounter ID but didn't pass any correct path
      error = true;
    } // if
    else if ( token.mType == CONSTANT ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      return true;
    } // if
    else if ( token.mValue == "(" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );

      if ( Expression(  ) ) {
        mScanner.PeekToken( token );
        if ( token.mValue == ")" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
          return true;
        } // if
      } // if

      error = true;
    } // else if
    

    if ( error ) {
      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // if

    return false;
  } // Signed_unary_exp()

  bool Unsigned_unary_exp(  ) {
    /*    
    Identifier [ '(' [ actual_parameter_list ] ')'
                 |
                 [ '[' expression ']' ] [ ( PP | MM ) ]
               ]
    | Constant
    | '(' expression ')'
    */
    Token token, idToken;
    int idLine = 0;
    Data value;

    mScanner.PeekToken( token );
    if ( token.mType == ID ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      idToken = token;
      idLine = mScanner.mLine;

      if ( !gCallStack->IsDefined( idToken.mValue ) ) {
        ErrorMsg errorMsg( idLine, SEMANTIC_ERROR, idToken.mValue );
        throw errorMsg;
      } // if

      gCallStack->Get( idToken.mValue, value );

      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        mTokenString->push_back( token );

        Actual_parameter_list();
        
        mScanner.GetToken( token );
        mTokenString->push_back( token );
        if ( token.mValue != ")" ) {
          ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
          throw errorMsg;
        } // if
      } // if
      else {
        if ( token.mValue == "[" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );

          if ( !Expression(  ) ) {
            mScanner.GetToken( token );
            ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
            throw errorMsg;
          } // if

          mScanner.GetToken( token );
          mTokenString->push_back( token );
          if ( token.mValue != "]" ) {
            ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
            throw errorMsg;
          } // if 
        } // if ( token.mValue == "[" )

        mScanner.PeekToken( token );
        if ( token.mType == PP || token.mType == MM ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
        } // if 
      } // else

      return true;
    } // if
    else if ( token.mType == CONSTANT ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
    
      value.SetByConstantToken( token.mValue );

      return true;
    } // else if
    else if ( token.mValue == "(" ) {
      mScanner.GetToken( token );
      mTokenString->push_back( token );
      
      if ( Expression(  ) ) {
        mScanner.PeekToken( token );
        if ( token.mValue == ")" ) {
          mScanner.GetToken( token );
          mTokenString->push_back( token );
          return true;
        } // if
      } // if 

      mScanner.GetToken( token );
      ErrorMsg errorMsg( mScanner.mLine, SYNTACTICAL_ERROR, token.mValue );
      throw errorMsg;
    } // else if

    return false;
  } // Unsigned_unary_exp()

}; // class Parser

int main() {
  cin >> gTestNum ;
  ReadLine();
  if ( gTestNum == 1 ) {
    // return 0;
  } // if

  // cout << "after Init: " << gCallStack.mCallStack.size();
  gCallStack = new CallStack();
  // cout << gCallStack->mCallStack.size();
  cout << "Our-C running ...\n";
  bool quit = false;
  Parser parser;
  
  while ( !quit ) {
    try {
      parser.User_Input();
    } 
    catch ( ErrorMsg& errorMsg ) {
      if ( errorMsg.mType == QUIT_ERROR ) {
        quit = true;
      } // if
      else {
        cout << "Line " << errorMsg.mLine;
        if ( errorMsg.mType == LEXICAL_ERROR ) {
          cout << " : unrecognized token with first char : \'"; 
        } // if
        else if ( errorMsg.mType == SYNTACTICAL_ERROR ) {
          cout << " : unexpected token : \'";
        } // if
        else if ( errorMsg.mType == SEMANTIC_ERROR ) {
          cout << " : undefined identifier : \'";
          
          
        } // if

        cout << errorMsg.mToken + "\'\n";
        parser.mScanner.ReadLine();

        gCallStack->PopExceptBase();
        
      } // else
    } // catch
  } // while

  cout << "Our-C exited ...";
  delete gCallStack;
  return 0;
} // main()
