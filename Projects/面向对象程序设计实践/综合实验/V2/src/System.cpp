#include "System.h"

System::System(string n, string pwd)
{
    username = n;
    password = pwd;
    type = VISITOR;
    logged = false;
    interact = new Interact;
    interact->loadCommFile();
    interact->loadUserFile();
    interact->loadCartFile();
}

int System::exeCommand()
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
    while (cin >> cmd) //循环读入指令
    {
        string nm, pwd, commType, des;
        int num, t;
        double d;

        auto it = cmdList.find(cmd);
        if (it != cmdList.end())
        {
            switch (cmdList[cmd]) //选择执行指令
            {
            case help:
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

                break;

            case sign_up:
            {
                cout << "Please enter <username> <password> <0(consumer)/1(merchant)>" << endl;
                cin >> nm >> pwd >> t;

                if (interact->addUser(nm, pwd, t))
                    cout << "User [ " << nm << " ] added" << endl;
                else
                    cout << "Failed" << endl;
            }
            break;

            case sign_in:
                if (logged)
                {
                    cout << "You have already logged in." << endl;
                    break;
                }
                cout << "Please enter <username> <password>" << endl;
                cin >> nm >> pwd;
                if (interact->checkPass(nm, pwd, t))
                {
                    username = nm;
                    password = pwd;
                    logged = true;
                    type = t;
                    cout << "Logged in" << endl;
                }
                else
                    cout << "Failed" << endl;
                break;

            case sign_out:
                if (logged == false)
                {
                    cout << "NOT Logged in." << endl;
                    break;
                }
                logged = false;
                type = VISITOR;
                cout << "You have logged out" << endl;
                break;

            case search_comm:
                cout << "Please enter <commodity name>" << endl;
                cin >> nm;
                if (!interact->listComm(nm))
                    cout << "Sorry, there's no such commodity!" << endl;

                break;

            case list_comm:
                interact->listComm();
                break;

            case change_pass:
                if (logged == false)
                {
                    cout << "NOT Logged in." << endl;
                    break;
                }
                cout << "Please enter your new password: " << endl;
                cin >> pwd;
                if (!interact->changePass(username, pwd))
                    cout << "Failed" << endl;
                break;

            case quit:
                interact->saveCommFile();
                interact->saveUserFile();
                interact->saveCartFile();
                cout << "Quit the eTrade system." << endl;
                return 0;

            case list_user:
                if (logged == false)
                {
                    cout << "NOT Logged in." << endl;
                    break;
                }
                if (!interact->getUserInfo(username))
                    cout << "Failed" << endl;
                break;

            case charge:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter how much you want to charge: " << endl;
                cin >> num;
                if (!interact->addBal(username, num))
                    cout << "Failed" << endl;

                break;

            case buy:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (!interact->buy(username, nm, num))
                    cout << "Failed" << endl;
                break;

            case list_cart:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                interact->listCart(username);
                break;

            case add_cart:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (!interact->addCart(username, nm, num))
                    cout << "Failed" << endl;
                break;

            case delete_cart:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (!interact->delCart(username, nm, num))
                    cout << "Failed" << endl;
                break;

            case change_cart:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (!interact->changeCart(username, nm, num))
                    cout << "Failed" << endl;
                break;

            case list_order:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                interact->listOrder(username);
                break;

            case create_order:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (!interact->creatOrder(username))
                    cout << "Failed" << endl;
                break;

            case delete_order:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (!interact->delOrder(username))
                    cout << "Failed" << endl;
                break;

            case pay_order:
                if (logged == false || type != CONSUMER)
                {
                    cout << "NOT Logged in as a consumer." << endl;
                    break;
                }
                if (!interact->payOrder(username))
                    cout << "Failed" << endl;
                break;

            case add_comm:
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }

                cout << "Please enter <commodity name> <commodity type> <price> <quantity>" << endl;
                cin >> nm >> commType >> d >> num;
                cout << "Please enter [description](link with \"-\")" << endl;
                cin >> des;
                if (!interact->addComm(d, 1.0, num, commType, nm, username, des))
                    cout << "Failed" << endl;

                break;

            case change_quantity:
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <quantity>" << endl;
                cin >> nm >> num;
                if (!interact->setQuantity(nm, username, num))
                    cout << "Failed" << endl;

                break;

            case change_price:
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }
                cout << "Please enter <commodity name> <price>" << endl;
                cin >> nm >> d;
                if (!interact->setPrice(nm, username, d))
                    cout << "Failed" << endl;

                break;

            case change_discount:
                if (logged == false || type != MERCHANT)
                {
                    cout << "NOT Logged in as a merchant." << endl;
                    break;
                }
                cout << "Please enter <commodity type> <discount>" << endl;
                cin >> commType >> d;
                if (!interact->setDiscount(d, commType, username))
                    cout << "Failed" << endl;

                break;

            default:
                cout << "illeagal command : " << cmd << endl
                     << "Type \"help\" for more information" << endl;
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
