#include "System.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <strstream>
#include <WS2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <unistd.h>
#include <stdio.h>
#include <string.h>

System::System(string n, string pwd)
{
    username = n;
    password = pwd;
    type = VISITOR;
    logged = false;
}

int System::exeCommand(const string &ip, const string &port)
{
    map<string, cmdVal> cmdList;

    cmdList["help"] = cmdVal::help;
    cmdList["sign_up"] = cmdVal::sign_up;
    cmdList["sign_in"] = cmdVal::sign_in;
    cmdList["list_user"] = cmdVal::list_user;
    cmdList["list_comm"] = cmdVal::list_comm;
    cmdList["search_comm"] = cmdVal::search_comm;
    cmdList["quit"] = cmdVal::quit;
    cmdList["sign_out"] = cmdVal::sign_out;
    cmdList["change_pass"] = cmdVal::change_pass;
    cmdList["charge"] = cmdVal::charge;
    cmdList["buy"] = cmdVal::buy;
    cmdList["list_cart"] = cmdVal::list_cart;
    cmdList["add_cart"] = cmdVal::add_cart;
    cmdList["delete_cart"] = cmdVal::delete_cart;
    cmdList["change_cart"] = cmdVal::change_cart;
    cmdList["list_order"] = cmdVal::list_order;
    cmdList["create_order"] = cmdVal::create_order;
    cmdList["delete_order"] = cmdVal::delete_order;
    cmdList["pay_order"] = cmdVal::pay_order;
    cmdList["add_comm"] = cmdVal::add_comm;
    cmdList["change_quantity"] = cmdVal::change_quantity;
    cmdList["change_price"] = cmdVal::change_price;
    cmdList["change_discount"] = cmdVal::change_discount;

    string cmd;
    cout << "> ";

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(atoi(port.c_str()));

    char buffRecv[MAXBUF];
    char buffSend[MAXBUF];

    //初始化 DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    while (cin >> cmd) //循环读入指令
    {
        // 建立套接字
        SOCKET clientFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        memset(buffRecv, 0, MAXBUF);
        memset(buffSend, 0, MAXBUF);

        string nm, pwd, commType, des;
        int num, t;
        double d;

        auto it = cmdList.find(cmd);
        if (it != cmdList.end())
        {
            switch (cmdList[cmd]) //选择执行指令
            {
            case help: // 0
            {
                cout << "Commands are as below :" << endl
                     << "--------------------System--------------------" << endl
                     << "help : helpful information" << endl
                     << "sign_up : user sign up" << endl
                     << "sign_in : user sign in" << endl
                     << "sign_out : user sign out" << endl
                     << "change_pass : user change password" << endl
                     << "list_user : list the info of your acount" << endl
                     << "list_comm : list all commodity" << endl
                     << "search_comm : search for commodity" << endl
                     << "quit : quit the eTrade system" << endl
                     << "-------------------Consumer-------------------" << endl
                     << "charge : consumer recharge balance" << endl
                     << "buy : consumer buy commodity" << endl
                     << "list_cart : list commodity in your cart" << endl
                     << "add_cart : add commodity to your cart" << endl
                     << "delete_cart : delete commodity from your cart" << endl
                     << "change_cart : change commodity in your cart" << endl
                     << "list_order : list info of your order" << endl
                     << "create_order : create an order from your cart" << endl
                     << "delete_order : delete your order" << endl
                     << "pay_order : pay your order" << endl
                     << "-------------------Merchant-------------------" << endl
                     << "add_comm : merchant add commodity" << endl
                     << "change_quantity : change quantity of commodity" << endl
                     << "change_price : change price of commodity" << endl
                     << "change_discout : change discount of commodity" << endl
                     << "----------------------------------------------" << endl;
            }
            break;

            case sign_up: // 1
            {
                cout << "Please enter <username> <password> <0(consumer)/1(merchant)>" << endl;
                cin >> nm >> pwd >> t;
                // 建立连接
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(sign_up)
                    << " " << nm
                    << " " << pwd
                    << " " << t;

                send(clientFd, buffSend, MAXBUF, 0);

                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '1')
                {
                    cout << "User [ " << nm << " ] added\n"
                         << "Please login again\n";
                }
                else
                    cout << "Failed\n";
            }
            break;

            case sign_in: // 2
            {
                if (logged)
                {
                    cout << "You have already logged in." << endl;
                    break;
                }
                cout << "Please enter <username> <password>" << endl;
                cin >> nm >> pwd;

                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(sign_in)
                    << " " << nm
                    << " " << pwd;
                send(clientFd, buffSend, MAXBUF, 0);

                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] != '1')
                {
                    cout << "Failed\n";
                }
                else
                {
                    t = buffRecv[2] - '0';
                    username = nm;
                    password = pwd;
                    logged = true;
                    type = t;
                    cout << "Logged in" << endl;
                }
            }
            break;

            case list_comm: // 3
            {
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(list_comm);
                send(clientFd, buffSend, MAXBUF, 0);

                int len = recv(clientFd, buffRecv, MAXBUF, 0);
                for (int i = 0; i < len; i++)
                {
                    cout << buffRecv[i];
                }
            }
            break;

            case search_comm: // 4
            {
                cout << "Please enter <commodity name>" << endl;
                cin >> nm;

                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(search_comm)
                    << " " << nm;
                send(clientFd, buffSend, MAXBUF, 0);

                int len = recv(clientFd, buffRecv, MAXBUF, 0);
                for (int i = 0; i < len; i++)
                {
                    cout << buffRecv[i];
                }
            }
            break;

            case list_user: // 5
            {
                if (logged == false)
                {
                    cout << "NOT Logged in." << endl;
                    break;
                }
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(list_user)
                    << " " << username;

                send(clientFd, buffSend, MAXBUF, 0);

                int len = recv(clientFd, buffRecv, MAXBUF, 0);
                for (int i = 0; i < len; i++)
                {
                    cout << buffRecv[i];
                }

                break;
            }
            break;

            case quit: // 6
            {
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(quit);
                send(clientFd, buffSend, MAXBUF, 0);

                cout << "Quit.\n";
                close(clientFd);
                return 0;
            }
            break;

            case sign_out: // 7
            {
                if (logged == false)
                {
                    cout << "NOT Logged in." << endl;
                    break;
                }
                logged = false;
                type = VISITOR;
                cout << "You have logged out" << endl;
            }
            break;

            case change_pass: // 8
            {
                if (logged == false)
                {
                    cout << "NOT Logged in." << endl;
                    break;
                }
                cout << "Please enter your new password: " << endl;
                cin >> pwd;

                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(change_pass)
                    << " " << username
                    << " " << pwd;
                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
            }
            break;

            case charge: // 9
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter how much you want to charge: " << endl;
                cin >> num;
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(charge)
                    << " " << username
                    << " " << num;
                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
            }
            break;

            case buy: // 10
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;

                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(buy)
                    << " " << username
                    << " " << nm
                    << " " << num;
                send(clientFd, buffSend, MAXBUF, 0);
                int len = recv(clientFd, buffRecv, MAXBUF, 0);
                for (int i = 0; i < len; i++)
                {
                    cout << buffRecv[i];
                }
            }
            break;

            case list_cart: // 11
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(list_cart)
                    << " " << username;
                send(clientFd, buffSend, MAXBUF, 0);

                int len = recv(clientFd, buffRecv, MAXBUF, 0);
                for (int i = 0; i < len; i++)
                {
                    cout << buffRecv[i];
                }
            }
            break;

            case add_cart: // 12
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(add_cart) // 锟斤拷锟斤拷buffSend
                    << " " << username
                    << " " << nm
                    << " " << num;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "add successfully\n";
            }
            break;

            case delete_cart: // 13
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(delete_cart) // 锟斤拷锟斤拷buffSend
                    << " " << username
                    << " " << nm
                    << " " << num;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "delete successfully\n";
            }
            break;

            case change_cart: // 14
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(change_cart) // 锟斤拷锟斤拷buffSend
                    << " " << username
                    << " " << nm
                    << " " << num;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "change successfully\n";
            }
            break;

            case list_order: // 15
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(list_order)
                    << " " << username;
                send(clientFd, buffSend, MAXBUF, 0);

                int len = recv(clientFd, buffRecv, MAXBUF, 0);
                for (int i = 0; i < len; i++)
                {
                    cout << buffRecv[i];
                }
            }
            break;

            case create_order: // 16
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }

                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(create_order)
                    << " " << username;
                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "create successfully\n";
            }
            break;

            case delete_order: // 17
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(delete_order)
                    << " " << username;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "delete successfully\n";
            }
            break;

            case pay_order: // 18
            {
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(pay_order)
                    << " " << username;
                send(clientFd, buffSend, MAXBUF, 0);
                int len = recv(clientFd, buffRecv, MAXBUF, 0);
                for (int i = 0; i < len; i++)
                {
                    cout << buffRecv[i];
                }
            }
            break;

            case add_comm: // 19
            {
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }

                cout << "Please enter <commodity name> <commodity type> <price> <quantity>" << endl;
                cin >> nm >> commType >> d >> num;
                cout << "Please enter [description](link with \"-\")" << endl;
                cin >> des;

                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(add_comm)
                    << " " << d
                    << " " << num
                    << " " << commType
                    << " " << nm
                    << " " << username
                    << " " << des;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "add successfully\n";
            }
            break;

            case change_quantity: // 20
            {
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;

                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(change_quantity)
                    << " " << nm
                    << " " << username
                    << " " << num;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "change successfully\n";
            }
            break;

            case change_price: // 21
            {
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <price>" << endl;
                cin >> nm >> d;

                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(change_price)
                    << " " << nm
                    << " " << username
                    << " " << d;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "change successfully\n";
            }
            break;

            case change_discount: // 22
            {
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }
                cout << "Please enter <commodity type> <discount>" << endl;
                cin >> commType >> d;
                if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                {
                    cout << "CANNOT connect server\n";
                    return 0;
                }
                
                ostrstream oss(buffSend, MAXBUF);
                oss << static_cast<char>(change_discount)
                    << " " << d
                    << " " << commType
                    << " " << username;

                send(clientFd, buffSend, MAXBUF, 0);
                recv(clientFd, buffRecv, MAXBUF, 0);
                if (buffRecv[0] == '0')
                {
                    cout << "Failed\n";
                }
                else
                    cout << "change successfully\n";
            }
            break;

            default: // default
            {
                cout << "illeagal command : " << cmd << endl
                     << "Type \"help\" for more information" << endl;
            }
            break;
            }
        }
        else
        {
            cout << "illeagal command : " << cmd << endl
                 << "Type \"help\" for more information" << endl;
        }

        cmd.clear();
        cout << "> ";
    }
    return 0;
}
