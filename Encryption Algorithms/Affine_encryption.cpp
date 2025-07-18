#include <bits/stdc++.h>
using namespace std;


//Declaring Functions
string Normalize(string);
int String_Compare(string, string);
string String_Addition(string, string);
string String_Subtraction(string, string);
string String_Multiplication(string, string);
string String_Divide(string, string);
string String_Modulo(string, string);
string String_GCD(string, string);
string String_Modulo_Inverse(string, string);
string affine_encryption(string,string,string,string);
string affine_decryption(string,string,string,string);



//Function for making the number usable, removing leading zeros.
string Normalize(string s) {

    //Checking if we have to add a negative sign at the end.
    bool neg = false;
    if (s[0] == '-') {
        neg = true;
        s = s.substr(1);
    }

    //Removing the leading zeros.
    int i = 0;
    while (i < s.size() - 1 && s[i] == '0') i++;
    s = s.substr(i);

    //adding the negative sign back.
    if (neg && s != "0") s = '-' + s;
    return s;
}



//This function returns -1,1 and 0 when s1 is less than, greater than and equal to s2 respectively.
int String_Compare(string s1, string s2) {
    //Remove the leading zeros.
    s1 = Normalize(s1);
    s2 = Normalize(s2);
    
    //Checking if either of them is negative.
    bool neg1 = (s1[0] == '-');
    bool neg2 = (s2[0] == '-');

    //Basic sign check
    if (neg1 && !neg2) return -1; 
    if (!neg1 && neg2) return 1;  
    
    //If both are negative we invert the mod inequality
    if (neg1 && neg2) {
        s1 = s1.substr(1);
        s2 = s2.substr(1);
        if (s1.size() > s2.size()) return -1;
        if (s1.size() < s2.size()) return 1;
        if (s1 > s2) return -1;
        if (s1 < s2) return 1;
        return 0;
    }

    //If both positive we just compare them.
    if (s1.size() > s2.size()) return 1;
    if (s1.size() < s2.size()) return -1;
    if (s1 > s2) return 1;
    if (s1 < s2) return -1;
    return 0;
}



//Function for subtracting the strings.
string String_Subtraction(string s1, string s2) {
    //Making number usable
    s1 = Normalize(s1);
    s2 = Normalize(s2);
    
    //Checking the sign of each of the numbers
    bool neg1 = (s1[0] == '-');
    bool neg2 = (s2[0] == '-');
    
    //Checking whether the difference will be positive or negative and calling functions accordingly.
    if (neg1 && !neg2) return '-' + String_Addition(s1.substr(1), s2);
    if (!neg1 && neg2) return String_Addition(s1, s2.substr(1));
    if (neg1 && neg2)  return String_Subtraction(s2.substr(1), s1.substr(1));
    if (String_Compare(s1, s2) < 0) return '-' + String_Subtraction(s2, s1);

    // Actual subtraction
    reverse(s1.begin(), s1.end());
    reverse(s2.begin(), s2.end());
    
    //Making the length of both numbers same. (Note s1.size() >= s2.size())
    while (s2.size() < s1.size()) s2 += '0';
    
    //Initializing variables
    string result = "";
    int borrow = 0;
    
    //Implimenting the normal subtraction and borrow.
    for (int i = 0; i < s1.size(); i++) {

        //Storing digits.
        int digit1 = s1[i] - '0';
        int digit2 = (i < s2.size()) ? s2[i] - '0' : 0;
        
        //Borrow implimentation.
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result += (diff + '0');
    }
    
    // Remove leading zeros
    while (result.size() > 1 && result.back() == '0') {
        result.pop_back();
    }
    
    //Reverting the number back.
    reverse(result.begin(), result.end());
    return Normalize(result);
}



//Function for adding the strings.
string String_Addition(string s1, string s2) {
    //Making the numbers usable.
    s1 = Normalize(s1);
    s2 = Normalize(s2);
    
    //Checking the sign of the numbers.
    bool neg1 = (s1[0] == '-');
    bool neg2 = (s2[0] == '-');
    
    //Checking the sign of the sum and calling functions accordingly.
    if (neg1 && !neg2) return String_Subtraction(s2, s1.substr(1));
    if (!neg1 && neg2) return String_Subtraction(s1, s2.substr(1));
    if (neg1 && neg2) return '-' + String_Addition(s1.substr(1), s2.substr(1));

    // Actual addition algorithm.
    reverse(s1.begin(), s1.end());
    reverse(s2.begin(), s2.end());
    
    //Making the size of both the numbers equal.
    int n = max(s1.size(), s2.size());
    while (s1.size() < n) s1 += '0';
    while (s2.size() < n) s2 += '0';
    
    //Initializing variables
    string result = "";
    int carry = 0;
    
    //Implimentation of digit-wise sum and carry.
    for (int i = 0; i < n; i++) {
        int sum = (s1[i] - '0') + (s2[i] - '0') + carry;
        result += (sum % 10 + '0');
        carry = sum / 10;
    }
    if (carry) result += (carry + '0');
    
    //Reverting the number back.
    reverse(result.begin(), result.end());
    return Normalize(result);
}



//Function of multipying the strings.
string String_Multiplication(string s1, string s2) {
    //Making the numbers useful.
    s1 = Normalize(s1);
    s2 = Normalize(s2);
    
    //If either one is zero product will be zero.
    if (s1 == "0" || s2 == "0") return "0";
    
    //Checking sign of the numbers
    bool neg1 = (s1[0] == '-');
    bool neg2 = (s2[0] == '-');

    //Checking sign of the product.
    bool result_negative = (neg1 != neg2);
    
    //Taking the absolute value.
    if (neg1) s1 = s1.substr(1);
    if (neg2) s2 = s2.substr(1);
    
    //Initializing Variables.
    int n = s1.size(), m = s2.size();
    vector<int> result(n + m, 0);
    
    //Actual multiplication algorithm
    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int mul = (s1[i] - '0') * (s2[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }
    
    //Changing the answer back into a string.
    string ans = "";
    bool leading_zero = true;
    for (int digit : result) {
        if (digit == 0 && leading_zero) continue;
        leading_zero = false;
        ans += (digit + '0');
    }

    if (ans.empty()) ans = "0";
    if (result_negative && ans != "0") ans = '-' + ans;
    return Normalize(ans);
}



//Function for diving the strings. Note that this function does integer division, that is it will not return fractions or decimals.
string String_Divide(string dividend, string divisor) {
    //Making the numbers usable.
    dividend = Normalize(dividend);
    divisor = Normalize(divisor);
    
    //Division by zero is not possible but for simplicity here it just returns 0;
    if (divisor == "0") return "0"; 
    if (dividend == "0") return "0";
    
    //Checking the sign of numbers and the quotient.
    bool neg_dividend = (dividend[0] == '-');
    bool neg_divisor = (divisor[0] == '-');
    bool result_negative = (neg_dividend != neg_divisor);
    
    //Taking the absolute value of the number.
    if (neg_dividend) dividend = dividend.substr(1);
    if (neg_divisor) divisor = divisor.substr(1);
    
    //No fractions in integer division.
    if (String_Compare(dividend, divisor) < 0) return "0";
    
    //Initializing Variables
    string result = "";
    string current = "";
    
    //Actual Division algorithm.
    for (int i = 0; i < dividend.size(); i++) {
        current += dividend[i];
        current = Normalize(current);
        
        int quotient_digit = 0;
        while (String_Compare(current, divisor) >= 0) {
            current = String_Subtraction(current, divisor);
            quotient_digit++;
        }
        
        result += (quotient_digit + '0');
    }
    
    // Remove leading zeros
    int pos = 0;
    while (pos < result.size() - 1 && result[pos] == '0') pos++;
    result = result.substr(pos);
    
    if (result_negative && result != "0") result = '-' + result;
    
    return Normalize(result);
}



//Function for calculating the modulus residue of s1 with respect to s2, which is usually represented as s1%s2.
string String_Modulo(string dividend, string divisor) {
    dividend = Normalize(dividend);
    divisor = Normalize(divisor);
    
    if (divisor == "0") return "0"; 
    if (dividend == "0") return "0";
    
    bool neg_dividend = (dividend[0] == '-');
    bool neg_divisor = (divisor[0] == '-');
    
    if (neg_dividend) dividend = dividend.substr(1);
    if (neg_divisor) divisor = divisor.substr(1);
    
    if (String_Compare(dividend, divisor) < 0) {
        if (neg_dividend) {
            string rem = String_Subtraction(divisor, dividend);
            return rem == "0" ? "0" : String_Subtraction(divisor, rem);
        }
        return dividend;
    }
    
    string quotient = String_Divide(dividend, divisor);
    string product = String_Multiplication(quotient, divisor);
    string remainder = String_Subtraction(dividend, product);
    
    // Ensure remainder is non-negative
    if (neg_dividend && remainder != "0") {
        remainder = String_Subtraction(divisor, remainder);
    }
    
    return Normalize(remainder);
}



//Function for finding GCD of the strings
string String_GCD(string s1, string s2) {

    //Normal Euclidian GCD algorithm.
    if (Normalize(s2) == "0") return Normalize(s1);
    return String_GCD(s2, String_Modulo(s1, s2));
}



//Function returning the coeffeciets of the Bezout's Lemma. (ax + by = GCD(a,b))
pair<string, pair<string, string>> String_Extended_GCD(string a, string b) {

    //Making numbers usable.
    a = Normalize(a);
    b = Normalize(b);
    
    //Initializing variables.
    string old_r = a, r = b;
    string old_s = "1", s = "0";
    string old_t = "0", t = "1";
    
    //Using the common Bezout's recursive relation to solve the problem iteratively.
    while (r != "0") {
        string quotient = String_Divide(old_r, r);
        
        string new_r = String_Subtraction(old_r, String_Multiplication(quotient, r));
        old_r = r;
        r = new_r;
        
        string new_s = String_Subtraction(old_s, String_Multiplication(quotient, s));
        old_s = s;
        s = new_s;
        
        string new_t = String_Subtraction(old_t, String_Multiplication(quotient, t));
        old_t = t;
        t = new_t;
    }
    
    return make_pair(old_r, make_pair(old_s, old_t));
}



// Calculating the multiplicative inverse of a number in the set of all modular residues of another number.
string String_Modulo_Inverse(string a, string mod) {

    //Making numbers usable.
    a = Normalize(a);
    mod = Normalize(mod);
    
    //Intilializing variables.
    auto result = String_Extended_GCD(a, mod);
    string gcd = result.first;
    string x = result.second.first;
    
    // If they arent coprime, no inverse exists.
    if (gcd != "1") return "0";
    
    // Make sure x is positive and less than mod.
    x = Normalize(x);
    while (x[0] == '-') {
        x = String_Addition(x, mod);
    }
    x = String_Modulo(x, mod);
    
    return Normalize(x);
}



//Function for converting the message into a integer (which is stored as an string.)
string Convert_To_Int(string s) {

    //Initializing variables
    string result = "";

    //Using ASCII values.
    for (char c : s) {
        int ascii = (int)(c-'A');
        string padded = string(3 - to_string(ascii).size(), '0') + to_string(ascii);
        result += padded;
    }

    return result;
}



//Function to retrive the message from an integer (which is stored as an string.)
string Convert_To_String(string s) {
    string result = "";
    for (int i = 0; i < s.size(); i += 3) {
        int val = stoi(s.substr(i, 3));
        result += (char)(val+'A');
    }
    return result;
}


// Function for encrypting the text in Affine Cipher
string Affine_Encrypt( string message, pair <string , string> Key, string m){

            //Inititializing variables
            string cypher="";

            //Key
            string a = Key.first;
            string b = Key.second;

            for( char c : message ){

                //Encrypting each character differently
                string x= to_string(c-'A');

                //Calculating a*x
                string ax = String_Multiplication(a,x );

                //Calculating a*x+b
                string ax_plus_b = String_Addition(ax,b);

                //taking mod m
                string y= String_Modulo(ax_plus_b,m);
                int val= stoi(String_Modulo(y,m));

                 cypher += char(val+'A'); 
            }
            return cypher;
            
        }





//Funciton for generation of a and b for the Affine cypher
    pair <string,string> Affine_Key_Generation(string m){

        //Finding a
        string a;

        while(true){

            srand(time(0));
            int A = rand();
            a = to_string(A);
            a = String_Modulo(a,m);

            //a and m must be coprime
            if(String_GCD(a,m) == "1") break;
        }



        //Finding b
        srand(time(0));
        int B = rand();
        string b = to_string(B);
        b = String_Modulo(b,m);



        return make_pair(a,b);

    }



    //Function for decyrption in Affine Cypher
    string Affine_Decrypt(string cypher, pair <string,string> key, string m) {

        //Initializing variables
        string message = "";

        //Key
        string a = key.first;
        string b = key.second;


        for (char c : cypher) {

            //Decrypting each character indivisually
            string y = to_string(c - 'A');

            // Calculating y-b
            string y_minus_b = String_Subtraction(y, b);
            
            
            if (String_Compare(y_minus_b, "0") < 0) {
                y_minus_b = String_Addition(y_minus_b, m);
            }
            
            //Using stardard formula
            string a_inv = String_Modulo_Inverse(a, m);
            string D_y = String_Multiplication(a_inv, y_minus_b);
            string D_y_mod_m = String_Modulo(D_y, m);
            
            int val = stoi(D_y_mod_m);
            message += char(val + 'A');
        }

        return message;

    }



// Main 
int main(){
    string x ,m="26";

    // Randomly genetrating key using our key generator function
    pair<string,  string> key =Affine_Key_Generation(m);
    // Defining a and b
    string a=key.first,b=key.second; 
    
    cout<<"Please enter a message (Please use uppercase letters only)"<<endl;

    // Getting user input
    getline(cin, x);


// Check for Upperacse letters only
bool allUpper = all_of(x.begin(), x.end(), [](char c) { return isupper(c); }); 


// If all letters are not in upper case then user is dragged into a loop, which cannot be broken till user enters text in Uppercase
while (!allUpper) {
    cout << "Please use only uppercase letters" << endl;
     getline(cin, x);
      allUpper = all_of(x.begin(), x.end(), [](char c) { return isupper(c); });
}


    // Encryption
    string encrypted= Affine_Encrypt(x,key,m);
    cout<<"Encrypted text:"<< encrypted<<endl;


    // Decryption
    string gcd=String_GCD(a,m); 

    // For decryption to work a and m must be coprime so that we can find an inverse of a
    if (gcd == "1" ){         string decrypted= Affine_Decrypt(encrypted,key,m);
        cout<<"Decrypted text:"<< decrypted<<endl;
    }

    else{
        cout<<"Decyrption is not possible"<<endl;
    }
}