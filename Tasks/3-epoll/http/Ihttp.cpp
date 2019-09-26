#include "Ihttp.h"

// CHttpRequest::CHttpRequest()
// {

// }

// CHttpRequest::~CHttpRequest()
// {

// }

void CHttpRequest::parse_content()
{
    std::vector<std::string> vecContent = split_str(m_http_content, '\n');
    std::cout<<"Get http method after split:"<<std::endl;
    for(auto & str: vecContent)
        std::cout<<str<<std::endl;
    std::cout<<std::endl;
}

std::vector<std::string> split_str(std::string str, char symbol)
{
    int start_pos = 0, end_pos=0;
    std::vector<std::string> vecRet;
    while(start_pos<str.size())
    {
        std::cout<<"start: "<<start_pos<<" end: "<<end_pos<<std::endl;
        end_pos = str.find(symbol, start_pos);
        if(end_pos == std::string::npos)
        {
            vecRet.push_back(str.substr(start_pos, str.size()-start_pos));
            break;
        }
        vecRet.push_back(str.substr(start_pos, end_pos-start_pos));
        ++end_pos;
        start_pos = end_pos;
    }
    return vecRet;
}