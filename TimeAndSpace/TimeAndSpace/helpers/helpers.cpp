//
//  helperMethods.cpp
//  TimeAndSpace
//
//  Created by Carry on 2018/7/2.
//  Copyright © 2018 Carry. All rights reserved.
//

#include "helpers.hpp"

bool ReadFile(const char* pFileName, string& outFile)
{
    ifstream f(pFileName);
    
    bool ret = false;
    
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }
        
        f.close();
        
        ret = true;
    }
    else {
        cout << "can't open file: " << pFileName << endl;
    }
    
    return ret;
}
