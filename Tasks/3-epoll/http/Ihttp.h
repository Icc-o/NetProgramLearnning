#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

const std::string HTTP_VERSION = "HTTP/1.1";
const std::string HTTP_STATUS_OK = "200";


typedef std::map<std::string, std::string> Http_Header_Map;

//分割子串 checked
inline std::vector<std::string> split_str(std::string str, char symbol);

class IHttpRequest
{
public:
    IHttpRequest(){}
    virtual ~IHttpRequest(){}

    //获取http请求方法
    virtual const std::string& method()=0;
    //获取http请求url
    virtual const std::string& url()=0;
    //获取http请求版本
    virtual const std::string& version()=0;
    //获取http请求首部字段（map<str,str>）
    virtual const Http_Header_Map& headers()=0;
};

class IHttpResponse
{
public:
    IHttpResponse(){}
    virtual ~IHttpResponse(){}
};

class CHttpRequest:public IHttpRequest
{
public:
    explicit CHttpRequest(){}
    ~CHttpRequest(){}
    virtual const std::string& method(){return m_method;}
    virtual const std::string& url(){return m_url;}
    virtual const std::string& version(){return m_version;}
    virtual const Http_Header_Map& headers(){return m_headers;}

    void load_content(std::string http_buffer){m_http_content=http_buffer;}
    void parse_content();

private:
    std::string m_method;
    std::string m_url;
    std::string m_version;
    Http_Header_Map m_headers;

    std::string m_http_content; //请求内容
};





