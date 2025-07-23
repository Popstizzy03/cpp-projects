#include <iostream>
#include <regex>
#include <string>
#include <vector>
using namespace std;

int main() {
    vector<string> testStrings = {
        "john.doe@email.com",
        "invalid-email",
        "user@domain.org",
        "123-45-6789",
        "invalid-phone",
        "(555) 123-4567",
        "192.168.1.1",
        "256.1.1.1",
        "Hello World 123"
    };
    
    regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    regex phonePattern(R"(\d{3}-\d{2}-\d{4}|\(\d{3}\)\s\d{3}-\d{4})");
    regex ipPattern(R"(^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
    regex numberPattern(R"(\d+)");
    
    cout << "Testing Regular Expressions:" << endl;
    cout << "=============================" << endl;
    
    for (const string& str : testStrings) {
        cout << "\nTesting: \"" << str << "\"" << endl;
        
        if (regex_match(str, emailPattern)) {
            cout << "  ✓ Valid email address" << endl;
        }
        
        if (regex_match(str, phonePattern)) {
            cout << "  ✓ Valid phone number" << endl;
        }
        
        if (regex_match(str, ipPattern)) {
            cout << "  ✓ Valid IP address" << endl;
        }
        
        smatch numberMatches;
        if (regex_search(str, numberMatches, numberPattern)) {
            cout << "  ✓ Contains numbers: ";
            for (const auto& match : numberMatches) {
                cout << match.str() << " ";
            }
            cout << endl;
        }
    }
    
    string text = "Contact us at support@company.com or call (555) 123-4567";
    cout << "\n\nExtracting information from: \"" << text << "\"" << endl;
    
    smatch emailMatch;
    if (regex_search(text, emailMatch, emailPattern)) {
        cout << "Found email: " << emailMatch.str() << endl;
    }
    
    smatch phoneMatch;
    if (regex_search(text, phoneMatch, phonePattern)) {
        cout << "Found phone: " << phoneMatch.str() << endl;
    }
    
    return 0;
}